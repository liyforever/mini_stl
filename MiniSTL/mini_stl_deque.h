#ifndef MINI_STL_DEQUE_H
#define MINI_STL_DEQUE_H
#include "memory.h"
#include <iostream>
using std::cout;
using std::endl;
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
  explicit deque() :
    firstIter_(),lastIter_(),map_(0),map_size_(0)
  {
    _init_map(0);
  }

  explicit deque(size_type n) :
    firstIter_(),lastIter_(),map_(0),map_size_(0)
  {
    _fill_init_aux(n, Type());
  }

  deque(size_type n, const Type& val) :
    firstIter_(),lastIter_(),map_(0),map_size_(0)
  {
    _fill_init_aux(n, val);
  }

  deque(const deque& rhs);

#ifdef MINI_STL_MEMBER_TEMPLATES
  template<class InputIterator>
  deque(InputIterator first,
        InputIterator last,
        typename is_iterator<InputIterator>::ID = Identity());
#endif //MINI_STL_MEMBER_TEMPLATES

#ifdef MINI_STL_HAS_MOVE
  deque(deque&& rhs)
  {
    firstIter_ = rhs.firstIter_;
    lastIter_ = rhs.lastIter_;
    map_size_ = rhs.map_size_;
    map_ = rhs.map_;
    rhs.map_ = 0;
  }
#endif //MINI_STL_HAS_MOVE

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
    return *firstIter_;
  }

  const_reference front() const
  {
    return *firstIter_;
  }

  reference back()
  {
    return *(lastIter_ - 1);
  }

  const_reference back() const
  {
    return *(lastIter_ - 1);
  }

  reference operator[](size_type pos)
  {
    return firstIter_[difference_type(pos)];
  }

  const_reference operator [](size_type pos) const
  {
    return firstIter_[difference_type(pos)];
  }

  reference at(size_type pos);

  const_reference at(size_type pos) const;

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
#ifdef MINI_STL_HAS_MOVE
  //iterator insert(const_iterator position, value_type&& val);
#endif //MINI_STL_HAS_MOVE
  void insert(
     iterator position,
     size_type count,
     const value_type& val
  );
#ifdef MINI_STL_MEMBER_TEMPLATES
  template<class InputIterator>
     void insert(
        iterator position,
        InputIterator first,
        InputIterator last,
        typename is_iterator<InputIterator>::ID = Identity()
     );
#endif //MINI_STL_MEMBER_TEMPLATES
  iterator erase(iterator position);

  iterator erase(iterator first, iterator last);
  void clear();

  void assign(size_type count, const value_type& val);
#ifdef MINI_STL_MEMBER_TEMPLATES
  template<class InputIterator>
     void assign(
        InputIterator first,
        InputIterator last,
        typename is_iterator<InputIterator>::ID = Identity()
     );
#endif //MINI_STL_MEMBER_TEMPLATES

  void resize(size_type newSize);

  void resize(size_type newSize, value_type val);

  void swap(deque& rhs);

  friend void swap(deque& lhs, deque& rhs);

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

  void _reserve_map_at_front();

  void _reserve_map_at_back();

  void _reallocate_map(bool frontOrBack);

  void _pop_back_aux();

  void _pop_front_aux();

  iterator _insert_aux(iterator position, const value_type& val);
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
    lastIter_.set_node(lastIter_.map_node + 1);
    lastIter_.cur = lastIter_.first;
    construct(lastIter_.cur, val);
    ++lastIter_.cur;
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
  if (lastIter_.cur!=lastIter_.last)
    destroy(--lastIter_.cur);
  else
    _pop_back_aux();
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::pop_front()
{
  if (firstIter_.cur!=lastIter_.last-1) {
    destroy(firstIter_.cur);
    ++firstIter_.cur;
  } else {
    _pop_front_aux();
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_pop_back_aux()
{
  _deallocate_node(lastIter_.map_node);
  lastIter_.set_node(lastIter_.map_node - 1);
  lastIter_.cur = lastIter_.last - 1;
  destroy(lastIter_.cur);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_pop_front_aux()
{
  destroy(firstIter_.cur);
  firstIter_.set_node(firstIter_.map_node + 1);
  firstIter_.cur = firstIter_.first;
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reserve_map_at_back()
{
  if (k_add_map_node + 1 > map_size_ - (lastIter_.map_node - map_))
    _reallocate_map(false);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reserve_map_at_front()
{
  if (k_add_map_node  > firstIter_.map_node - map_)
    _reallocate_map(true);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reallocate_map(bool frontOrback)
{
  size_type oldMapNum = lastIter_.map_node - firstIter_.map_node + 1;
  size_type newMapNum = oldMapNum + k_add_map_node;
  map_pointer newMapFirst;
  if (map_size_ > 2 * newMapNum) {
      newMapFirst = map_ + (map_size_ - newMapNum) / 2
          + (frontOrback ? k_add_map_node : 0);
      if (newMapFirst < firstIter_.map_node)
        copy(firstIter_.map_node, lastIter_.map_node + 1, newMapFirst);
      else
        copy_backward(firstIter_.map_node,
                      lastIter_.map_node + 1,
                      newMapFirst + oldMapNum);
  } else {
      size_type addNum = k_add_map_node;
      size_type newMapSize = map_size_ + max(map_size_, addNum) + 2;
      map_pointer newMap = _allocate_map(newMapSize);
      newMapFirst = newMap + (newMapSize - newMapNum) / 2
                    + (frontOrback ? k_add_map_node : 0);
      copy(firstIter_.map_node, lastIter_.map_node + 1, newMapFirst);

      _deallocate_map(map_, map_size_);
      map_ = newMap;
      map_size_ = newMapSize;
  }
  firstIter_.set_node(newMapFirst);
  lastIter_.set_node(newMapFirst + oldMapNum - 1);
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
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::erase(iterator position)
{
  iterator next = position;
  difference_type offset = position - position;
  if ((size_type)(offset)<size()/2) {
    ++next;
    copy_backward(firstIter_,position, next);
    front();
  } else {
    copy(next, lastIter_, position);
  }
  return firstIter_ + offset;
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::erase(iterator first, iterator last)
{

}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::clear()
{

}

MINI_STL_END
#endif // MINI_STL_DEQUE_H
