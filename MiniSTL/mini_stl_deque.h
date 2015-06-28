#ifndef MINI_STL_DEQUE_H
#define MINI_STL_DEQUE_H
#include "memory.h"
#ifdef MINI_STL_DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#endif
MINI_STL_BEGIN

inline size_t __deque_buf_size(size_t n, size_t sz)
{
  return n != 0? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <class Type, class Ref, class Ptr, size_t BuffSize>
struct DequeIterator
{
  typedef DequeIterator<Type,Type&,Type*,BuffSize>    iterator;
  typedef DequeIterator<Type,const Type&,const Type*,BuffSize> const_iterator;
  typedef random_access_iterator_tag  iterator_category;
  typedef Type                value_type;
  typedef Ptr                 pointer;
  typedef Ref                 reference;
  typedef size_t              size_type;
  typedef ptrdiff_t           difference_type;
  typedef Type**              map_pointer;
  typedef DequeIterator       Self;

  DequeIterator():
      cur(0),first(0),last(0),map_node(0) {}

  DequeIterator(pointer x, map_pointer map_ptr)
  {
    cur = x;
    first = *map_ptr;
    last = *map_ptr + buffer_size();
    map_node = map_ptr;
  }

  DequeIterator(const iterator& x)
  {
    cur = x.cur;
    first = x.first;
    last = x.last;
    map_node = x.map_node;
  }

  static size_t buffer_size()
  {
    return __deque_buf_size(BuffSize, sizeof(Type));
  }

  void set_node(map_pointer new_node)
  {
    map_node = new_node;
    first = *new_node;
    last = *new_node + buffer_size();
  }

  reference operator*() const
  {
    return *cur;
  }

  pointer operator->() const
  {
    return cur;
  }

  difference_type operator-(const Self& x) const
  {
    return difference_type(buffer_size()) * (map_node - x.map_node - 1)
                           + (cur - first) + (x.last - x.cur);
  }

  Self& operator++()
  {
    ++cur;
    if (cur==last) {
        set_node(map_node + 1);
        cur = first;
    }
    return *this;
  }

  Self operator++(int)
  {
    Self tmp = *this;
    ++*this;
    return tmp;
  }

  Self& operator--()
  {
    if (cur==first) {
        set_node(map_node - 1);
        cur = last;
    }
    --cur;
    return *this;
  }

  Self operator--(int)
  {
    Self tmp = *this;
    --*this;
    return tmp;
  }

  Self& operator+=(difference_type n)
  {
    //需要移动的距离
    difference_type offset = n + (cur - first);
    //如果移动范围在本缓冲区
    if(offset>=0 && offset<difference_type(buffer_size())) {
      cur += n;
    } else {
        difference_type map_node_offset =
            offset > 0 ? offset / difference_type(buffer_size())
                       : -difference_type((-offset - 1) / buffer_size()) - 1;
            set_node(map_node + map_node_offset);
            cur = first + (offset - map_node_offset * difference_type(buffer_size()));
    }
    return *this;
  }

  Self operator+(difference_type n) const
  {
    Self tmp = *this;
    return tmp += n;
  }

  Self& operator-=(difference_type n)
  {
    return *this += -n;
  }

  Self operator-(difference_type n) const
  {
    Self tmp = *this;
    return tmp -= n;
  }

  reference operator[](difference_type pos) const
  {
    return *(*this + pos);
  }

  bool operator==(const Self& x) const
  {
    return cur == x.cur;
  }

  bool operator!=(const Self& x) const
  {
    return cur != x.cur;
  }

  bool operator<(const Self& x) const
  {
    return (map_node == x.map_node) ?
          (cur < x.cur) : (map_node < x.map_node);
  }

  bool operator>(const Self& x) const
  {
    return x < *this;
  }

  pointer cur;
  pointer first;
  pointer last;
  map_pointer map_node;
};

template <class Type, class Alloc = default_allocator, size_t BuffSize = 0>
class deque
{
public:
  typedef Type              value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef Alloc             allocator_type;
  typedef DequeIterator<Type,Type&,Type*,BuffSize>  iterator;
  typedef DequeIterator<Type,const Type&, const Type*,BuffSize> const_iterator;
protected:
  typedef Type**    map_pointer;
  typedef simpleAlloc<value_type,Alloc> data_allocator_;
  typedef simpleAlloc<pointer,Alloc> map_allocator_;
  iterator firstIter_;
  iterator lastIter_;
  map_pointer map_;
  size_type map_size_;
  enum {k_init_map_size = 8,
        k_add_map_node = 1};
public:
  explicit deque(const allocator_type&/*Al*/=allocator_type()) :
    firstIter_(),lastIter_(),map_(0),map_size_(0)
  {
    _init_map(0);
  }

  explicit deque(size_type n) :
    firstIter_(),lastIter_(),map_(0),map_size_(0)
  {
    _fill_init_aux(n, Type());
  }

  deque(size_type n, const Type& val,
        const allocator_type&/*Al*/=allocator_type()) :
    firstIter_(),lastIter_(),map_(0),map_size_(0)
  {
    _fill_init_aux(n, val);
  }

  deque(const deque& rhs)
  {
    size_type n = rhs.size();
    _init_map(n);
    copy(rhs.begin(), rhs.end(), begin());
  }

  template<class InputIterator>
  deque(InputIterator first,
        InputIterator last,
        const allocator_type&/*Al*/=allocator_type(),
        typename is_iterator<InputIterator>::ID = Identity())
  {
    difference_type n = DISTANCE(first, last);
    _init_map(n);
    copy(first, last, begin());
  }

#ifdef MINI_STL_RVALUE_REFS
  deque(deque&& rhs)
  {
    firstIter_ = rhs.firstIter_;
    lastIter_ = rhs.lastIter_;
    map_size_ = rhs.map_size_;
    map_ = rhs.map_;
    rhs.map_ = 0;
  }
#endif //MINI_STL_RVALUE_REFS

  ~deque()
  {
    if(map_)
    {
      _destroy_node(firstIter_.map_node, lastIter_.map_node);
      _deallocate_map(map_, map_size_);
    }
  }

public:
  allocator_type get_allocator( ) const
  {
    return Alloc();
  }

  iterator begin()
  {
    return firstIter_;
  }

  const_iterator begin() const
  {
    return firstIter_;
  }

  iterator end()
  {
    return lastIter_;
  }

  const_iterator end() const
  {
    return lastIter_;
  }

  const_iterator cbegin() const
  {
    return firstIter_;
  }

  const_iterator cend() const
  {
    return lastIter_;
  }

  reference front()
  {
#ifdef MINI_STL_DEBUG
  _check_range();
#endif
    return *firstIter_;
  }

  const_reference front() const
  {
#ifdef MINI_STL_DEBUG
  _check_range();
#endif
    return *firstIter_;
  }

  reference back()
  {
#ifdef MINI_STL_DEBUG
  _check_range();
#endif
    return *(lastIter_ - 1);
  }

  const_reference back() const
  {
#ifdef MINI_STL_DEBUG
  _check_range();
#endif
    return *(lastIter_ - 1);
  }

  reference operator[](size_type pos)
  {
#ifdef MINI_STL_DEBUG
  _check_range(pos);
#endif
    return firstIter_[difference_type(pos)];
  }

  const_reference operator [](size_type pos) const
  {
#ifdef MINI_STL_DEBUG
  _check_range(pos);
#endif
    return firstIter_[difference_type(pos)];
  }

  reference at(size_type pos)
  {
#ifdef MINI_STL_DEBUG
  _check_range(pos);
#endif
    return firstIter_[difference_type(pos)];
  }

  const_reference at(size_type pos) const
  {
#ifdef MINI_STL_DEBUG
  _check_range(pos);
#endif
    return firstIter_[difference_type(pos)];
  }

  size_type max_size() const
  {
    return size_type(-1);
  }

  size_type size() const
  {
    return lastIter_ - firstIter_;
  }

  bool empty() const
  {
    return lastIter_ == firstIter_;
  }

  void push_back(const value_type& val);

  void push_front(const value_type& val);

  void pop_back();

  void pop_front();

  iterator insert(iterator position, const value_type& val);

  void insert(
     iterator position,
     size_type count,
     const value_type& val
  )
  {
    _insert_fill_aux(position, count, val);
  }

  template<class InputIterator>
     void insert(
        iterator position,
        InputIterator first,
        InputIterator last,
        typename is_iterator<InputIterator>::ID = Identity()
     );

  iterator erase(iterator position);

  iterator erase(iterator first, iterator last);
  void clear();

  void assign(size_type count, const value_type& val);

  template<class InputIterator>
     void assign(
        InputIterator first,
        InputIterator last,
        typename is_iterator<InputIterator>::ID = Identity()
     );


  void resize(size_type newSize);

  void resize(size_type newSize, const Type& val);

  void swap(deque& rhs)
  {
    if (this==&rhs)
      return;
    iterator tmp = firstIter_;
    firstIter_ = rhs.firstIter_;
    rhs.firstIter_ = tmp;

    tmp = lastIter_;
    lastIter_ = rhs.lastIter_;
    rhs.lastIter_ = tmp;

    map_pointer tt = map_;
    map_ = rhs.map_;
    rhs.map_ = tt;

    size_type num = map_size_;
    map_size_ = rhs.map_size_;
    rhs.map_size_ = num;
  }

  friend void swap(deque& lhs, deque& rhs)
  {
    lhs.swap(rhs);
  }

  bool operator!=(const deque& rhs);

  bool operator<(const deque& rhs);

  bool operator<=(const deque& rhs);

  bool operator==(const deque& rhs);

  bool operator>(const deque& rhs);

  bool operator>=(const deque& rhs);
protected:
  pointer _allocate_node()
  {
    return data_allocator_::allocate(_buffer_size());
  }

  void _deallocate_node(pointer p)
  {
      data_allocator_::deallocate(p, _buffer_size());
  }

  map_pointer _allocate_map(size_t n)
  {
    return map_allocator_::allocate(n);
  }

  void _deallocate_map(map_pointer p, size_t n)
  {
    map_allocator_::deallocate(p, n);
  }

  void _create_node(map_pointer first, map_pointer last)
  {
    map_pointer cur;
    MINI_STL_TRY {
      for(cur=first; cur!=last; ++cur)
        *cur = _allocate_node();
    }
    MINI_STL_UNWIND(_destroy_node(first, cur));
  }

  void _destroy_node(map_pointer first, map_pointer last)
  {
    for( ;first!=last;++first)
      _deallocate_node(*first);
  }

  void _fill_init_aux(size_type n, const Type& val);

  void _init_map(size_type n);

  size_t _buffer_size()
  {
    return __deque_buf_size(BuffSize, sizeof(Type));
  }

  void _push_back_aux(const value_type& val);

  void _push_front_aux(const value_type& val);

  void _reserve_map_at_front(size_type nodes_add_num=1);

  void _reserve_map_at_back(size_type nodes_add_num=1);

  void _reallocate_map(size_type nodes_add_num,bool frontOrBack);

  void _new_elem_at_front(size_type newElem);

  void _new_elem_at_back(size_type newElem);

  iterator _reserve_elem_at_front(size_type n)
  {
    size_type freeNum = firstIter_.cur - firstIter_.first;
    if (n > freeNum)
      _new_elem_at_front(n - freeNum);
    return firstIter_ - difference_type(n);
  }

  iterator _reserve_elem_at_back(size_type n)
  {
    size_type freeNum = (lastIter_.last - lastIter_.cur) - 1;
    if (n > freeNum)
      _new_elem_at_back(n - freeNum);
    return lastIter_ + difference_type(n);
  }

  void _pop_back_aux();

  void _pop_front_aux();

  iterator _insert_aux(iterator position, const value_type& val);

  void _insert_fill_aux(iterator position, size_type n, const Type& val);

  void _insert_aux(iterator position, size_type n, const value_type& val);
#ifdef MINI_STL_DEBUG
  void _check_range(size_t pos)
  {
    if (pos<0 || pos>=size()) {
      cerr << "deque:pos<0 || pos>=size()" << endl;
      MINI_STL_THROW_RANGE_ERROR("deque");
    }
  }

  void _check_range(size_t n, bool)
  {
    if (n<0 || n>=max_size()) {
      cerr << "deque:n<0" << endl;
      MINI_STL_THROW_RANGE_ERROR("deque");
    }
  }

  void _check_range(const_iterator position)
  {
    if (position>last_ ||
        position<first_) {
      cerr << "deque:postion>=end() || position < begin()" << endl;
      MINI_STL_THROW_RANGE_ERROR("deque");
    }
  }

  template <class InputIterator>
  void _check_range(InputIterator first,
                   InputIterator last)
  {
    difference_type n = DISTANCE(first, last);
    if (n<0) {
      cerr << "deque:InputIterator last - first < 0" << endl;
      MINI_STL_THROW_RANGE_ERROR("deque");
    }
  }

  void _check_range()
  {
    if (empty()) {
      cerr << "deque:is empty" << endl;
      MINI_STL_THROW_RANGE_ERROR("deque");
    }
  }
#endif //MINI_STL_DEBUG
};

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_init_map(size_type n)
{
  size_type mapNodeSize = n / _buffer_size() + 1;
  map_size_ = k_init_map_size > mapNodeSize +  2 ? k_init_map_size :
                                                   mapNodeSize + 2;
  //max(k_init_map_size, (mapNodeSize + 2));
  map_ = _allocate_map(map_size_);
  //使first last保持在中间
  map_pointer newFirst = map_ + (map_size_ - mapNodeSize) / 2;
  map_pointer newLast = newFirst + mapNodeSize - 1;
  MINI_STL_TRY{
    _create_node(newFirst, newLast + 1);
  }
  MINI_STL_UNWIND((_deallocate_map(newFirst,map_size_),
                    map_ = 0, map_size_ = 0));
  //调整迭代器
  firstIter_.set_node(newFirst);
  lastIter_.set_node(newLast);
  firstIter_.cur = firstIter_.first;
  lastIter_.cur = lastIter_.first + n % _buffer_size();
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_fill_init_aux(size_type n, const Type &val)
{
  _init_map(n);
  map_pointer cur;
  MINI_STL_TRY {
    for(cur=firstIter_.map_node; cur<lastIter_.map_node; ++cur)
      uninitialized_fill(*cur, *cur + _buffer_size(), val);
      uninitialized_fill(lastIter_.first, lastIter_.cur, val);
    }
  MINI_STL_UNWIND(destroy(firstIter_,iterator(*cur, cur)));
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_push_front_aux(const value_type& val)
{
  _reserve_map_at_front();
  *(firstIter_.map_node - 1) = _allocate_node();
  MINI_STL_TRY {
    firstIter_.set_node(firstIter_.map_node - 1);
    firstIter_.cur = firstIter_.last - 1;
    construct(firstIter_.cur, val);
  }
  MINI_STL_UNWIND(firstIter_.set_node(firstIter_.map_node + 1);
      firstIter_.cur = firstIter_.first;
      _deallocate_node(*(firstIter_.map_node - 1));)
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_push_back_aux(const value_type& val)
{
  _reserve_map_at_back();
  *(lastIter_.map_node + 1) = _allocate_node();
  MINI_STL_TRY {
    construct(lastIter_.cur, val);
    lastIter_.set_node(lastIter_.map_node + 1);
    lastIter_.cur = lastIter_.first;
  }
  MINI_STL_UNWIND(_deallocate_node(*(lastIter_.map_node + 1));)
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::push_back(const value_type& val)
{
  if (lastIter_.cur != lastIter_.last - 1) {
      construct(lastIter_.cur, val);
      ++lastIter_.cur;
  } else {
    _push_back_aux(val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::push_front(const value_type& val)
{
  if (firstIter_.cur != firstIter_.first) {
      construct(firstIter_.cur - 1, val);
      --firstIter_.cur;
  } else {
      _push_front_aux(val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::pop_back()
{
#ifdef MINI_STL_DEBUG
  _check_range();
#endif
  if (lastIter_.cur!=lastIter_.first)
    destroy(--lastIter_.cur);
  else
    _pop_back_aux();
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::pop_front()
{
#ifdef MINI_STL_DEBUG
  _check_range();
#endif
  if (firstIter_.cur!=firstIter_.last-1) {
    destroy(firstIter_.cur);
    ++firstIter_.cur;
  } else {
    _pop_front_aux();
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::assign(size_type count, const value_type& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(count,true);
#endif
  if (count > size()) {
    fill(begin(), end(), val);
    insert(end(), count - size(), val);
  } else {
    erase(begin()+count, end());
    fill(begin(), end(), val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
template<class InputIterator>
void deque<Type,Alloc,BuffSize>::assign(
      InputIterator first,
      InputIterator last,
      typename is_iterator<InputIterator>::ID
   )
{
#ifdef MINI_STL_DEBUG
  _check_range(first, last);
#endif
  iterator cur;
  for (cur=begin(); cur!=end() && first!=last;
       ++cur,++first)
      *cur = *first;
  if (cur==end())
    insert(cur, first, last);
  else
    erase(cur, end());
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_pop_back_aux()
{
  _deallocate_node(*lastIter_.map_node);
  lastIter_.set_node(lastIter_.map_node - 1);
  lastIter_.cur = lastIter_.last - 1;
  destroy(lastIter_.cur);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_pop_front_aux()
{
  destroy(firstIter_.cur);
  _deallocate_node(*firstIter_.map_node);
  firstIter_.set_node(firstIter_.map_node + 1);
  firstIter_.cur = firstIter_.first;
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reserve_map_at_back(size_type nodes_add_num)
{
  if (nodes_add_num + 1 > map_size_ - (lastIter_.map_node - map_))
    _reallocate_map(nodes_add_num, false);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reserve_map_at_front(size_type nodes_add_num)
{
  if (nodes_add_num  > (size_type)(firstIter_.map_node - map_))
    _reallocate_map(nodes_add_num, true);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reallocate_map(size_type nodes_add_num,bool frontOrback)
{
  size_type oldMapNum = lastIter_.map_node - firstIter_.map_node + 1;
  size_type newMapNum = oldMapNum + nodes_add_num;
  map_pointer newMapFirst;
  if (map_size_ > 2 * newMapNum) {
      newMapFirst = map_ + (map_size_ - newMapNum) / 2
          + (frontOrback ? nodes_add_num : 0);
      if (newMapFirst < firstIter_.map_node)
        copy(firstIter_.map_node, lastIter_.map_node + 1, newMapFirst);
      else
        copy_backward(firstIter_.map_node,
                      lastIter_.map_node + 1,
                      newMapFirst + oldMapNum);
  } else {
      size_type addNum = nodes_add_num;
      size_type newMapSize = map_size_ + max(map_size_, addNum) + 2;
      map_pointer newMap = _allocate_map(newMapSize);
      newMapFirst = newMap + (newMapSize - newMapNum) / 2
                    + (frontOrback ? nodes_add_num : 0);
      copy(firstIter_.map_node, lastIter_.map_node + 1, newMapFirst);

      _deallocate_map(map_, map_size_);
      map_ = newMap;
      map_size_ = newMapSize;
  }
  firstIter_.set_node(newMapFirst);
  lastIter_.set_node(newMapFirst + oldMapNum - 1);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_new_elem_at_back(size_type newElem)
{
  size_type new_map_node = (newElem + _buffer_size() - 1)
      / _buffer_size();
  _reserve_map_at_back(new_map_node);
  size_type i;
  MINI_STL_TRY {
    for(i=1; i<=new_map_node; ++i)
      *(lastIter_.map_node + i) = _allocate_node();
  }
  MINI_STL_UNWIND(
    for(size_type j=1; j<=i; ++j)
          _deallocate_node(*(lastIter_.map_node + j));
  )
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_new_elem_at_front(size_type newElem)
{
  size_type new_map_node = (newElem + _buffer_size() - 1)
      / _buffer_size();
  _reserve_map_at_front(new_map_node);
  size_type i;
  MINI_STL_TRY {
    for(i=1; i<=new_map_node; ++i)
      *(firstIter_.map_node - i) = _allocate_node();
  }
  MINI_STL_UNWIND(
    for(size_type j=1; j<=i; ++j)
      _deallocate_node(*(firstIter_.map_node + j));
  )
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::_insert_aux(iterator position, const value_type& val)
{
  difference_type offset = position - firstIter_;
  if ((size_type)(offset)<size()/2) {
    push_front(front());
    iterator frontTo = firstIter_;
    ++frontTo;
    iterator frontS = frontTo;
    ++frontS;
    position = firstIter_ + offset;
    copy(frontS, position, frontTo);
  } else {
    push_back(back());
    iterator back1 = lastIter_;
    --back1;
    iterator back2 = back1;
    --back2;
    position = firstIter_ + offset;
    copy_backward(position, back2, back1);
  }
  *position = val;
  return position;
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::insert(iterator position, const value_type& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
#endif
  if (position.cur==firstIter_.cur) {
      push_front(val);
      return firstIter_;
  } else if (position.cur==lastIter_.cur) {
      push_back(val);
      return lastIter_;
  } else {
      return _insert_aux(position, val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::resize(size_type newSize)
{
  resize(newSize, Type());
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::resize(
    size_type newSize,
    const Type& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(newSize,true);
#endif
  if (newSize>size())
    insert(end(), newSize-size(), val);
  else
    erase(begin()+newSize, end());
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_insert_aux(
    iterator position,
    size_type n,
    const value_type& val)
{  
  const difference_type frontNum = position - firstIter_;
  size_type length = size();
  if (frontNum < difference_type(length / 2)) {
    iterator newFirst = _reserve_elem_at_front(n);
    iterator oldFirst = firstIter_;
    position = firstIter_ + frontNum;
    MINI_STL_TRY {
        oldFirst =uninitialized_copy(firstIter_, position, newFirst);
        firstIter_ = newFirst;
        uninitialized_fill_n(oldFirst, n, val);
    }
    MINI_STL_UNWIND(
       _destroy_node(newFirst.map_node, firstIter_.map_node))
  } else {
    iterator newLast = _reserve_elem_at_back(n);
    iterator oldLast = lastIter_;
    const difference_type afterNum = length - frontNum;
    position = lastIter_ - afterNum;
    MINI_STL_TRY {
      copy_backward(position, oldLast, newLast);
      fill_n(position, n, val);
      lastIter_ = newLast;
    }
    MINI_STL_UNWIND(_destroy_node(lastIter_.map_node + 1,
                                  newLast.map_node + 1));
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_insert_fill_aux(
    iterator position,
    size_type n,
    const Type &val
    )
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
  _check_range(n, true);
#endif
  if (position.cur==firstIter_.cur) {
    iterator newFirst = _reserve_elem_at_front(n);
    MINI_STL_TRY {
      uninitialized_fill(newFirst, firstIter_, val);
      firstIter_ = newFirst;
    }
    MINI_STL_UNWIND(_destroy_node(newFirst.map_node,firstIter_.map_node));
  } else if (position.cur==lastIter_.cur) {
    iterator newLast = _reserve_elem_at_back(n);
    MINI_STL_TRY {
      uninitialized_fill(lastIter_, newLast, val);
      lastIter_ = newLast;
    }
    MINI_STL_UNWIND(_destroy_node(firstIter_.map_node + 1,
                                  newLast.map_node + 1));
  } else {
    _insert_aux(position, n, val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
template<class InputIterator>
   void deque<Type,Alloc,BuffSize>::insert(
      iterator position,
      InputIterator first,
      InputIterator last,
      typename is_iterator<InputIterator>::ID
   )
  {
#ifdef MINI_STL_DEBUG
  _check_range(position);
  _check_range(first, last);
#endif
  difference_type n = DISTANCE(first, last);
  if (position.cur==firstIter_.cur) {
    iterator newFirst = _reserve_elem_at_front(n);
    MINI_STL_TRY {
      uninitialized_copy(first, last, newFirst);
      firstIter_ = newFirst;
    }
    MINI_STL_UNWIND(_destroy_node(newFirst.map_node,firstIter_.map_node));
  } else if (position.cur==lastIter_.cur) {
    iterator newLast = _reserve_elem_at_back(n);
    MINI_STL_TRY {
      uninitialized_copy(first, last, lastIter_);//(lastIter_, newLast, val);
      lastIter_ = newLast;
    }
    MINI_STL_UNWIND(_destroy_node(firstIter_.map_node + 1,
                                     newLast.map_node + 1));
  } else {
    const difference_type frontNum = position - firstIter_;
    size_type length = size();
    if (frontNum < difference_type(length / 2)) {
      iterator newFirst = _reserve_elem_at_front(n);
      iterator oldFirst = firstIter_;
      position = firstIter_ + frontNum;
      MINI_STL_TRY {
        oldFirst =uninitialized_copy(firstIter_, position, newFirst);
        firstIter_ = newFirst;
        uninitialized_copy(first, last, oldFirst);
      }
      MINI_STL_UNWIND(
        _destroy_node(newFirst.map_node, firstIter_.map_node))
    } else {
      iterator newLast = _reserve_elem_at_back(n);
      iterator oldLast = lastIter_;
      const difference_type afterNum = length - frontNum;
      position = lastIter_ - afterNum;
      MINI_STL_TRY {
        copy_backward(position, oldLast, newLast);
        uninitialized_copy(first, last, position);
        lastIter_ = newLast;
      }
      MINI_STL_UNWIND(_destroy_node(lastIter_.map_node + 1,
                                      newLast.map_node + 1));
    }
  }
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::erase(iterator position)
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
  _check_range(position - firstIter_);
#endif
  iterator next = position;
  ++next;
  difference_type offset = position - firstIter_;
  if ((size_type)(offset)<size()/2) {
    copy_backward(firstIter_,position, next);
    pop_front();
  } else {
    copy(next, lastIter_, position);
    pop_back();
  }
  return firstIter_ + offset;
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::erase(iterator first, iterator last)
{
#ifdef MINI_STL_DEBUG
  _check_range(first);
  _check_range(last);
  _check_range(first, last);
#endif
  if (first==begin() && last==end()) {
    clear();
    return firstIter_;
  } else {
    difference_type n = last - first;
    difference_type beforeNum = first - begin();
    if ((size_type)(beforeNum) < (size()-n)/2){
      copy_backward(begin(), first, last);
      iterator newFirst = begin() + n;
      destroy(begin(), newFirst);
      _destroy_node(firstIter_.map_node, newFirst.map_node);
      firstIter_ = newFirst;
    } else {
      copy(last, end(), first);
      iterator newLast = end() - n;
      destroy(newLast, end());
      _destroy_node(newLast.map_node+1, lastIter_.map_node+1);
      lastIter_ = newLast;
    }
    return firstIter_ + beforeNum;
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::clear()
{
  for (map_pointer node = firstIter_.map_node + 1;
       node < lastIter_.map_node; ++ node) {
    destroy(*node, *node + _buffer_size());
    _deallocate_node(*node);
  }

  if (firstIter_.map_node!=lastIter_.map_node) {
    destroy(firstIter_.cur, firstIter_.last);
    destroy(lastIter_.first, lastIter_.cur);
    _deallocate_node(*lastIter_.map_node);
  } else {
    destroy(firstIter_.cur, firstIter_.last);
  }
  lastIter_ = firstIter_;
}

MINI_STL_END
#endif // MINI_STL_DEQUE_H
