#ifndef MINI_STL_LIST_H
#define MINI_STL_LIST_H
#include "memory.h"
#include "mini_stl_iterator.h"
#ifdef MINI_STL_DEBUG
#include <iostream>
using std::cerr;
using std::endl;
#endif
MINI_STL_BEGIN

struct __list_node_base;

struct __list_node_base
{
  __list_node_base* prev;
  __list_node_base* next;
};

template <class Type>
struct _list_node : public __list_node_base
{
  Type data;
};

struct __list_iterator_base
{
  typedef size_t                      size_type;
  typedef ptrdiff_t                   difference_type;
  typedef forward_iterator_tag iterator_category;//bidirectional_iterator_tag  iterator_category;

  __list_node_base* node;

  __list_iterator_base() {}
  __list_iterator_base(__list_node_base* x) : node(x) {}

  void incr() { node = node->next; }
  void decr() { node = node->prev; }

  bool operator==(const __list_iterator_base& rhs) const
  {
    return node == rhs.node;
  }

  bool operator!=(const __list_iterator_base& rhs) const
  {
    return node != rhs.node;
  }
};

template <class Type, class Ref, class Ptr>
struct _list_iterator : public __list_iterator_base
{
  typedef _list_iterator<Type,Type&,Type*>             iterator;
  typedef _list_iterator<Type,const Type&,const Type*> const_iterator;
  typedef _list_iterator<Type,Ref,Ptr>                 Self;

  typedef Type             value_type;
  typedef Ptr              pointer;
  typedef Ref              reference;
  typedef _list_node<Type> node_type;

  _list_iterator() {}
  _list_iterator(node_type* x) : __list_iterator_base(x) {}
  _list_iterator(const iterator& x) : __list_iterator_base(x.node) {}

  reference operator*() const
  {
    return static_cast<node_type*>(node)->data;
  }

  pointer operator->() const
  {
    return &(operator *());
  }

  Self& operator++()
  {
    this->incr();
    return *this;
  }

  Self operator++(int)
  {
    Self tmp = *this;
    this->incr();
    return tmp;
  }

  Self& operator--()
  {
    this->decr();
    return *this;
  }

  Self operator--(int)
  {
    Self tmp = *this;
    this->decr();
    return tmp;
  }
};


template <class Type, class Alloc = default_allocator>
class list
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
  typedef _list_iterator<Type,Type&,Type*>             iterator;
  typedef _list_iterator<Type,const Type&,const Type*> const_iterator;
  typedef _MY_STL::reverse_iterator<iterator>          reverse_iterator;
  typedef _MY_STL::reverse_iterator<const_iterator>    const_reverse_iterator;
protected:
  typedef _list_node<Type>    node_type;
  typedef _list_node<Type>*   node_ptr;
  typedef simpleAlloc<node_type, default_allocator>   data_allocator_;
  node_ptr head_;
  size_type size_;
public:
  explicit list(const allocator_type&/*Al*/=allocator_type())
  {
    head_ = data_allocator_::allocate();
    _empty_init();
  }

  explicit list(size_type count)
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    head_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), count, Type());
  }

  list(size_type count, const Type& val,
       const allocator_type&/*Al*/=allocator_type())
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    head_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), count, val);
  }

  list(const list& rhsList)
  {
    head_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), rhsList.begin(), rhsList.end());
  }


  template <class InputIterator>
  list(InputIterator first,
       InputIterator last,
       const allocator_type&/*Al*/=allocator_type(),
       typename is_iterator<InputIterator>::ID = Identity()
      )
  {
#ifdef MINI_STL_DEBUG
    _check_range(first, last);
#endif
    head_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), first, last);
  }

#ifdef MINI_STL_RVALUE_REFS
  list(list&& rhsList)
  {
    head_ = rhsList.head_;
    size_ = rhsList.size_;
    rhsList.size_ = 0;
    rhsList.head_ = nullptr;
  }

  list& operator=(list&& rhsList)
  {
    clear();
    data_allocator_::deallocate(head_);
    head_ = rhsList.head_;
    size_ = rhsList.size_;
    rhsList.size_ = 0;
    rhsList.head_ = nullptr;
    return *this;
  }
#endif //MINI_STL_RVALUE_REFS
  list& operator=(const list& rhsList)
  {
    if (this != &rhsList)
      assign(rhsList.begin(), rhsList.end());
    return *this;
  }

  ~list()
  {
    clear();
    data_allocator_::deallocate(head_);
  }
public:
  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  iterator begin()
  {
    return (node_ptr)(head_->next);
  }

  const_iterator begin() const
  {
    return (node_ptr)(head_->next);
  }

  reverse_iterator rbegin()
  {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(end());
  }

  iterator end()
  {
    return head_;
  }

  const_iterator end() const
  {
    return head_;
  }

  reverse_iterator rend()
  {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(begin());
  }

  const_iterator cbegin() const
  {
    return (node_ptr)(head_->next);
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator(cend());
  }

  const_iterator cend() const
  {
    return head_;
  }

  const_reverse_iterator crend() const
  {
    return const_reverse_iterator(cbegin());
  }

  reference front()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *begin();
  }

  const_reference front() const
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *begin();
  }

  reference back()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *(--end());
  }

  const_reference back() const
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *(--end());
  }

  void pop_back()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    erase(--end());
  }

  void pop_front()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    erase(begin());
  }

  size_type size() const
  {
    return size_;
  }

  size_type max_size() const
  {
    return size_type(-1);
  }

  bool empty() const
  {
    return size_ == 0;
  }

  void resize(size_type newSize)
  {
    resize(newSize, Type());
  }

  void resize(size_type newSize, const Type& val)
  {
#ifdef MINI_STL_DEBUG
    _check_range(newSize);
#endif
    if (size()==newSize) {
      return;
    }
    if (size()>newSize) {
      size_type eraseNum = size() - newSize;
      for (int i=eraseNum; i>0; --i)
        erase(--end());
    } else {
      size_type insertNum = newSize - size();
      insert(end(), insertNum, val);
    }
  }

  void reverse()
  {
    if (size()==0 || size()==1)
      return;
    size_type n = size();
    iterator first = begin();
    iterator oldFirst;
    ++first;
    while(--n) {
      oldFirst = first;
      ++first;
      _transfer(begin(), oldFirst, first);
    }
  }

  void swap(list& rhs)
  {
    _MY_STL::swap(this->size_, rhs.size_);
    _MY_STL::swap(this->head_, rhs.head_);
  }

  void clear();

  iterator insert(iterator position, const Type& val)
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
#endif
    node_ptr tmp = _create_node(val);
    tmp->prev = position.node->prev;
    tmp->next = position.node;
    position.node->prev->next = tmp;
    position.node->prev = tmp;
    ++size_;
    return tmp;
  }

  void insert(iterator position, size_type count, const Type &val)
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
    _check_range(count);
#endif
    iterator tmp = position;
    for ( ; count; --count)
    {
      tmp = insert(tmp, val);
    }
}

  template <class InputIterator>
  void insert(iterator position,
              InputIterator first,
              InputIterator last,
              typename is_iterator<InputIterator>::ID = Identity()
      )
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
    _check_range(first, last);
#endif
    while(first != last)
      insert(position,*first++);
  }

#ifdef MINI_STL_RVALUE_REFS
  iterator insert(iterator position, Type&& val)
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
#endif
    node_ptr tmp = data_allocator_::allocate();
    construct(&(tmp->data));
    tmp->data = _MY_STL::move(val);
    tmp->prev = position.node->prev;
    tmp->next = position.node;
    position.node->prev->next = tmp;
    position.node->prev = tmp;
    ++size_;
    return tmp;
  }

#endif //MINI_STL_RVALUE_REFS

  iterator erase(iterator position);
  iterator erase(iterator first, iterator last);

  void remove(const Type& val);

  template <class Predicate>
  void remove_if(Predicate pre);

  void push_front(const Type& val)
  {
    insert(begin(), val);
  }

  void push_front(Type&& val)
  {
    insert(begin(), _MY_STL::move(val));
  }

  void push_back(const Type& val)
  {
    insert(end(), val);
  }

#ifdef MINI_STL_RVALUE_REFS
  void push_back(Type&& val)
  {
    insert(end(), _MY_STL::move(val));
  }

#endif
  void assign(size_type count, const Type& val);

  template <class InputIterator>
  void assign(
      InputIterator first,
      InputIterator last,
      typename is_iterator<InputIterator>::ID = Identity()
      );

  template<class BinaryPredicate>
  void unique(BinaryPredicate Pred);


  void unique();


  template <class BinaryPredicate>
  void sort(BinaryPredicate pred);


  void sort();

  void merge(list& rhs);

  template<class BinaryPredicate>
     void merge(
        list& rhs,
        BinaryPredicate Comp
     );

  void splice(iterator position, list& rhs)
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
#endif
    if (this==&rhs || rhs.empty()) {
           //不支持自我splice
      ;
    } else {
      size_ += rhs.size_;
      rhs.size_ = 0;
      _transfer(position, rhs.begin(), rhs.end());
    }
  }

  void splice(iterator position, list& rhs, iterator i)
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
    rhs._check_range(i);
#endif
    iterator j = i;
    ++j;
    if(this==&rhs || position==i || position==j)
      return;
    --rhs.size_;
    ++size_;
    _transfer(position, i, j);
  }

  void splice(iterator position, list& rhs,
               iterator first, iterator last)
  {
#ifdef MINI_STL_DEBUG
    _check_range(position);
    _check_range(first, last);
    rhs._check_range(first);
    rhs._check_range(last);
#endif
    if (this==&rhs || first!=last) {
      size_type n = 0;
      iterator tmpFirst = first;
      iterator tmpLast = last;
      while (tmpFirst++!=tmpLast)
        ++n;
      rhs.size_ -= n;
      this->size_ += n;
      _transfer(position, first, last);
    }
  }
protected:
  void _empty_init()
  {
    head_->next = head_;
    head_->prev = head_;
    size_ = 0;
  }

  node_ptr _create_node(const Type& val)
  {
    node_ptr tmp = data_allocator_::allocate(1);
    MINI_STL_TRY {
      construct(&tmp->data, val);
    }
    MINI_STL_UNWIND(data_allocator_::deallocate(tmp));
    return tmp;
  }

  void _destroy_node(node_ptr p)
  {
    destroy(&p->data);
    data_allocator_::deallocate(p);
  }

  void _transfer(iterator position, iterator first, iterator last)
  {
    if (position != last) {
      last.node->prev->next = position.node;
      first.node->prev->next = last.node;
      position.node->prev->next = first.node;

      __list_node_base* tmp = position.node->prev;
      position.node->prev = last.node->prev;
      last.node->prev = first.node->prev;
      first.node->prev = tmp;
    }
  }
#ifdef MINI_STL_DEBUG
  void _check_range(size_t n) const
  {
    if (n<0 || n>=max_size()) {
      std::cerr << "list:n<0" << std::endl;
      MINI_STL_THROW_RANGE_ERROR("list");
    }
  }

  void _check_range(const_iterator position) const
  {
    difference_type n = DISTANCE(begin(), position);
    if (n<0 || (size_type)(n)>size()) {
      std::cerr << "position < begin() || position > end()" << std::endl;
      MINI_STL_THROW_RANGE_ERROR("list");
    }
  }

  template <class InputIterator>
  void _check_range(InputIterator first,
                   InputIterator last) const
  {
    difference_type n = DISTANCE(first, last);
    if (n<0) {
      cerr << "last:InputIterator last - first < 0" << endl;
      MINI_STL_THROW_RANGE_ERROR("last");
    }
  }

  void _check_range() const
  {
    if (empty()) {
      cerr << "list:is empty" << endl;
      MINI_STL_THROW_RANGE_ERROR("list");
    }
  }
#endif //MINI_STL_DEBUG
};


template <class Type, class Alloc>
inline bool
operator==(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  return lhs.size() == rhs.size() &&
         equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Type, class Alloc>
inline bool
operator<(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  return lexicographical_compare(lhs.begin(), lhs.end(),
                                 rhs.begin(), rhs.end());
}

template <class Type, class Alloc>
inline bool
operator!=(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  return !(lhs == rhs);
}

template <class Type, class Alloc>
inline bool
operator>(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  return rhs < lhs;
}

template <class Type, class Alloc>
inline bool
operator<=(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  return !(rhs < lhs);
}

template <class Type, class Alloc>
inline bool
operator>=(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  return !(lhs < rhs);
}

template <class Type, class Alloc>
inline void
swap(const list<Type, Alloc>& lhs, const list<Type, Alloc>& rhs)
{
  lhs.swap(rhs);
}

template <class Type, class Alloc>
typename list<Type,Alloc>::iterator
list<Type,Alloc>::erase(iterator position)
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
#endif
  __list_node_base* prev_node = position.node->prev;
  __list_node_base* next_node = position.node->next;
  prev_node->next = next_node;
  next_node->prev = prev_node;
  destroy(&*position);
  data_allocator_::deallocate((node_ptr)(position.node));
  --size_;
  return (node_ptr)(next_node);
}

template <class Type, class Alloc>
typename list<Type,Alloc>::iterator
list<Type,Alloc>::erase(iterator first, iterator last)
{
#ifdef MINI_STL_DEBUG
  _check_range(first, last);
  _check_range(first);
  _check_range(last);
#endif
  for ( ; first!=last; )
    first = erase(first);
  return first;
}

template <class Type, class Alloc>
void list<Type,Alloc>::clear()
{
  if (!head_)
    return;
  node_ptr cur = (node_ptr)(head_->next);
  node_ptr tmp;
  while (cur != head_) {
    tmp = cur;
    cur = (node_ptr)(tmp->next);
    destroy(&tmp);
    data_allocator_::deallocate(tmp);
  }
  _empty_init();
}

template <class Type, class Alloc>
void list<Type,Alloc>::assign(size_type count, const Type& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(count);
#endif
  iterator i = begin();
  for ( ; i!=end() && count > 0; ++i, --count)
    *i = val;
  if(count > 0)
    insert(end(), count, val);
  else
    erase(i, end());
}

template <class Type, class Alloc>
template <class InputIterator>
void list<Type,Alloc>::assign(
        InputIterator first,
        InputIterator last,
        typename is_iterator<InputIterator>::ID = Identity()
      )
{
#ifdef MINI_STL_DEBUG
  _check_range(first, last);
#endif
  size_type newSize = 0;
  iterator cur = begin();
  for ( ; cur!=end() && first != last;
        ++cur, ++first) {
    *cur = *first;
    ++newSize;
  }
  if (cur==end()) {
    for( ; first != last; ++first)
      insert(cur, *first);
  } else {
    erase(cur, end());
  }
  size_ = newSize;
}

template <class Type, class Alloc>
template <class Predicate>
void list<Type,Alloc>::remove_if(Predicate pre)
{
  for(iterator beg = begin(); beg != end(); ++beg)
    if(pre(*beg))
      beg = erase(beg);
}


template <class Type, class Alloc>
void list<Type,Alloc>::remove(const Type& val)
{
  for(iterator beg = begin(); beg != end(); ++beg)
    if(*beg == val)
      beg = erase(beg);
}

template <class Type, class Alloc>
template<class BinaryPredicate>
   void list<Type,Alloc>::unique(
           BinaryPredicate Pred
           )
  {
    iterator first = begin();
    iterator last = end();
    if(first == last)
      return;
    iterator next = first;
    while (++next != last) {
      if(Pred(*next,*first))
        erase(next);
      else
        first = next;
      next = first;
    }
  }

template <class Type, class Alloc>
void list<Type,Alloc>::unique()
{
  iterator first = begin();
  iterator last = end();
  if(first == last)
    return;
  iterator next = first;
  while (++next != last) {
    if(*next == *first)
      erase(next);
    else
      first = next;
    next = first;
  }
}
template <class Type, class Alloc>
void list<Type,Alloc>::merge(list &rhs)
{
  size_ += rhs.size_;
  rhs.size_ = 0;
  iterator first1 = this->begin();
  iterator last1 = this->end();
  iterator first2 = rhs.begin();
  iterator last2 = rhs.end();

  while (first1!=last1 && first2!=last2)
    if (*first2 < *first1) {
      iterator next = first2;
      _transfer(first1, first2, ++next);
      first2 = next;
    } else {
      ++first1;
    }
  if (first2!=last2)
    _transfer(first1,first2,last2);
}

template <class Type, class Alloc>
template <class BinaryPredicate>
void list<Type,Alloc>::sort(BinaryPredicate pred)
{
  if(size()==0 || size()==1)
    return;
  list<Type,Alloc> carry;
  list<Type,Alloc> counter[20];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
  int i = 0;
    while (i<fill && !counter[i].empty()) {
      counter[i].merge(carry, pred);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i==fill)
      ++fill;
  }
  for (int i=1; i<fill; ++i)
    counter[i].merge(counter[i-1], pred);
  swap(counter[fill-1]);
}

template <class Type, class Alloc>
template <class BinaryPredicate>
void list<Type,Alloc>::merge(list &rhs, BinaryPredicate Comp)
{
  size_ += rhs.size_;
  rhs.size_ = 0;
  iterator first1 = this->begin();
  iterator last1 = this->end();
  iterator first2 = rhs.begin();
  iterator last2 = rhs.end();

  while (first1!=last1 && first2!=last2)
    if (Comp(*first2,*first1)) {
      iterator next = first2;
      _transfer(first1, first2, ++next);
      first2 = next;
    } else {
      ++first1;
    }
  if (first2!=last2)
    _transfer(first1,first2,last2);
}

template <class Type, class Alloc>
void list<Type,Alloc>::sort()
{
  if(size()==0 || size()==1)
    return;
  list<Type,Alloc> carry;
  list<Type,Alloc> counter[20];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
    int i = 0;
    while (i<fill && !counter[i].empty()) {
      counter[i].merge(carry);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i==fill)
      ++fill;
  }
  for (int i=1; i<fill; ++i)
    counter[i].merge(counter[i-1]);
  swap(counter[fill-1]);
}


MINI_STL_END
#endif // MINI_STL_LIST_H
