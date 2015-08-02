#ifndef MINI_STL_LIST_H
#define MINI_STL_LIST_H
#include "mini_stl_memory.h"
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
  typedef bidirectional_iterator_tag  iterator_category;

  __list_node_base* node;

  __list_iterator_base() {}
  __list_iterator_base(__list_node_base* _Right) : node(_Right) {}

  void incr() { node = node->next; }
  void decr() { node = node->prev; }

  bool operator==(const __list_iterator_base& _Right) const
  {
    return this->node == _Right.node;
  }

  bool operator!=(const __list_iterator_base& _Right) const
  {
    return this->node != _Right.node;
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
  _list_iterator(node_type* _Right) : __list_iterator_base(_Right) {}
  _list_iterator(const iterator& _Right) : __list_iterator_base(_Right.node) {}
  _list_iterator(const const_iterator& _Right) : __list_iterator_base(_Right.node) {}
  _list_iterator& operator=(const _list_iterator& _Right)
  {
    this->node = _Right.node;
    return *this;
  }

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


template <class Type, class Alloc = _MY_STL::default_allocator>
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
  node_ptr Myhead_;
  size_type Mysize_;
public:
  list(const allocator_type&/*Al*/=allocator_type())
  {
    Myhead_ = data_allocator_::allocate();
    _empty_init();
  }

  explicit list(size_type _Count)
  {
    Myhead_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), _Count, Type());
  }

  list(size_type _Count, const Type& _Val,
       const allocator_type&/*Al*/=allocator_type())
  {
    Myhead_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), _Count, _Val);
  }

  list(const list& _Right)
  {
    Myhead_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), _Right.begin(), _Right.end());
  }


  template <class InputIterator>
  list(InputIterator _First,
       InputIterator _Last,
       const allocator_type&/*Al*/=allocator_type(),
       typename is_iterator<InputIterator>::ID = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "list list");
    Myhead_ = data_allocator_::allocate();
    _empty_init();
    insert(begin(), _First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  list(list&& _Right)
  {
    this->Myhead_ = _Right.Myhead_;
    this->Mysize_ = _Right.Mysize_;
    _Right.Mysize_ = 0;
    _Right.Myhead_ = nullptr;
  }

  list& operator=(list&& _Right)
  {
    if (this != &_Right) {
      _MY_STL::swap(this->Myhead_, _Right.Myhead_);
      _MY_STL::swap(this->Mysize_, _Right.Mysize_);
    }
    return *this;
  }
#endif //MINI_STL_RVALUE_REFS
  list& operator=(const list& _Right)
  {
    if (this != &_Right) {
        clear();
        insert(this->begin(),_Right.begin(),_Right.end());
    }
    return *this;
  }

  ~list()
  {
    clear();
    data_allocator_::deallocate(Myhead_);
  }
public:
  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  iterator begin()
  {
    return static_cast<node_ptr>(Myhead_->next);
  }

  const_iterator begin() const
  {
    return static_cast<node_ptr>(Myhead_->next);
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
    return Myhead_;
  }

  const_iterator end() const
  {
    return Myhead_;
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
    return static_cast<node_ptr>(Myhead_->next);
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator(cend());
  }

  const_iterator cend() const
  {
    return Myhead_;
  }

  const_reverse_iterator crend() const
  {
    return const_reverse_iterator(cbegin());
  }

  reference front()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "list front");
    return *begin();
  }

  const_reference front() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "list front");
    return *begin();
  }

  reference back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "list back");
    return *(--end());
  }

  const_reference back() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "list back");
    return *(--end());
  }

  void pop_back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "pop_back");
    erase(--end());
  }

  void pop_front()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "pop_front");
    erase(begin());
  }

  size_type size() const
  {
    return Mysize_;
  }

  size_type max_size() const
  {
    return size_type(-1);
  }

  bool empty() const
  {
    return Mysize_ == 0;
  }

  void resize(size_type _NewSize)
  {
    resize(_NewSize, Type());
  }

  void resize(size_type _NewSize, const Type& _Val)
  {
    if (this->Mysize_ == _NewSize) {
      return;
    }
    if (this->Mysize_ > _NewSize) {
      size_type eraseNum = size() - _NewSize;
      for (int i=eraseNum; i > 0; --i)
        erase(--end());
    } else {
      size_type insertNum = _NewSize - size();
      insert(end(), insertNum, _Val);
    }
  }

  void reverse()
  {
    if (this->Mysize_ == 0 || this->Mysize_ == 1)
      return;
    size_type n = this->Mysize_;
    iterator first = begin();
    iterator oldFirst;
    ++first;
    while(--n) {
      oldFirst = first;
      ++first;
      _transfer(begin(), oldFirst, first);
    }
  }

  void swap(list& _Right)
  {
    _MY_STL::swap(this->Mysize_, _Right.Mysize_);
    _MY_STL::swap(this->Myhead_, _Right.Myhead_);
  }

  void clear();

  iterator insert(iterator _Position, const Type& _Val)
  {
    //MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list insert");
    node_ptr tmp = _create_node(_Val);
    tmp->prev = _Position.node->prev;
    tmp->next = _Position.node;
    _Position.node->prev->next = tmp;
    _Position.node->prev = tmp;
    ++Mysize_;
    return tmp;
  }

  void insert(iterator _Position, size_type _Count, const Type& _Val)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list insert");
    iterator tmp = _Position;
    for ( ; _Count; --_Count)
    {
      tmp = insert(tmp, _Val);
    }
}

  template <class InputIterator>
  void insert(iterator _Position,
              InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list insert");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "list insert");
    for (; _First != _Last; ++_First)
      insert(_Position, *_First);
  }

#ifdef MINI_STL_RVALUE_REFS
  iterator insert(iterator _Position, Type&& _Val)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list insert");
    node_ptr tmp = data_allocator_::allocate();
    _MY_STL::construct(&(tmp->data), _MY_STL::move(_Val));
    tmp->prev = _Position.node->prev;
    tmp->next = _Position.node;
    _Position.node->prev->next = tmp;
    _Position.node->prev = tmp;
    ++Mysize_;
    return tmp;
  }
#endif //MINI_STL_RVALUE_REFS

  iterator erase(iterator _Position);
  iterator erase(iterator _First, iterator _Last);

  void remove(const Type& _Val);

  template<class Predicate>
  void remove_if(Predicate _Comp);

  void push_front(const Type& _Val)
  {
    insert(begin(), _Val);
  }

  void push_front(Type&& _Val)
  {
    insert(begin(), _MY_STL::move(_Val));
  }

  void push_back(const Type& _Val)
  {
    insert(end(), _Val);
  }

#ifdef MINI_STL_RVALUE_REFS
  void push_back(Type&& _Val)
  {
    insert(end(), _MY_STL::move(_Val));
  }
#endif

  void assign(size_type _Count, const Type& _Val);

  template <class InputIterator>
  void assign(InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID
              = Identity());

  template<class BinaryPredicate>
  void unique(BinaryPredicate _Comp);
  void unique();


  template <class BinaryPredicate>
  void sort(BinaryPredicate _Comp);
  void sort();

  void merge(list& _Right);

  template<class BinaryPredicate>
  void merge(list& _Right,
             BinaryPredicate _Comp);

  void splice(iterator _Position, list& _Right)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list splice");
    if (this == &_Right || _Right.empty())
      return;
    this->Mysize_ += _Right.Mysize_;
    _Right.Mysize_ = 0;
    _transfer(_Position, _Right.begin(), _Right.end());
  }

  void splice(iterator _Position, list& _Right, iterator _Position2)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list splice");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Right.begin(), _Position2, "list splice");
    iterator next = _Position2;
    ++next;
    if (this == &_Right || _Position == _Position2 || _Position == next)
      return;
    --_Right.Mysize_;
    ++this->Mysize_;
    _transfer(_Position, _Position2, next);
  }

  void splice(iterator _Position, list& _Right,
              iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "list splice");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Right.begin(), _First, "list splice");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Right.begin(), _Last, "list splice");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "list splice");
    if (this == &_Right || _First != _Last) {
      size_type n = 0;
      iterator tmpFirst = _First;
      iterator tmpLast = _Last;
      while (tmpFirst++ != tmpLast)
        ++n;
      _Right.Mysize_ -= n;
      this->Mysize_ += n;
      _transfer(_Position, _First, _Last);
    }
  }
protected:
  void _empty_init()
  {
    Myhead_->next = Myhead_;
    Myhead_->prev = Myhead_;
    Mysize_ = 0;
  }

  node_ptr _create_node(const Type& _Val)
  {
    node_ptr tmp = data_allocator_::allocate(1);
    MINI_STL_TRY {
      _MY_STL::construct(&tmp->data, _Val);
    }
    MINI_STL_UNWIND(data_allocator_::deallocate(tmp));
    return tmp;
  }

  void _destroy_node(node_ptr _Ptr)
  {
    destroy(&_Ptr->data);
    data_allocator_::deallocate(_Ptr);
  }

  void _transfer(iterator _Position, iterator _First, iterator _Last)
  {
    if (_Position != _Last) {
      _Last.node->prev->next = _Position.node;
      _First.node->prev->next = _Last.node;
      _Position.node->prev->next = _First.node;

      __list_node_base* tmp = _Position.node->prev;
      _Position.node->prev = _Last.node->prev;
      _Last.node->prev = _First.node->prev;
      _First.node->prev = tmp;
    }
  }
};


template <class Type, class Alloc>
inline bool
operator==(const list<Type, Alloc>& _Left,
           const list<Type, Alloc>& _Right)
{
  return _Left.size() == _Right.size() &&
         _MY_STL::equal(_Left.begin(), _Left.end(),
                        _Right.begin());
}

template <class Type, class Alloc>
inline bool
operator<(const list<Type, Alloc>& _Left,
          const list<Type, Alloc>& _Right)
{
  return _MY_STL::lexicographical_compare(_Left.begin(),
                                          _Left.end(),
                                          _Right.begin(),
                                          _Right.end());
}

template <class Type, class Alloc>
inline bool
operator!=(const list<Type, Alloc>& _Left,
           const list<Type, Alloc>& _Right)
{
  return !(_Left == _Right);
}

template <class Type, class Alloc>
inline bool
operator>(const list<Type, Alloc>& _Left,
          const list<Type, Alloc>& _Right)
{
  return _Right < _Left;
}

template <class Type, class Alloc>
inline bool
operator<=(const list<Type, Alloc>& _Left,
           const list<Type, Alloc>& _Right)
{
  return !(_Right < _Left);
}

template <class Type, class Alloc>
inline bool
operator>=(const list<Type, Alloc>& _Left,
           const list<Type, Alloc>& _Right)
{
  return !(_Left < _Right);
}

template <class Type, class Alloc>
inline void
swap(const list<Type, Alloc>& _Left,
     const list<Type, Alloc>& _Right)
{
  _Left.swap(_Right);
}

template <class Type, class Alloc>
typename list<Type,Alloc>::iterator
list<Type,Alloc>::erase(iterator _Position)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, --this->end(),"list erase");
  __list_node_base* prev_node = _Position.node->prev;
  __list_node_base* next_node = _Position.node->next;
  prev_node->next = next_node;
  next_node->prev = prev_node;
  _MY_STL::destroy(&*_Position);
  data_allocator_::deallocate((node_ptr)(_Position.node));
  --Mysize_;
  return (node_ptr)(next_node);
}

template <class Type, class Alloc>
typename list<Type,Alloc>::iterator
list<Type,Alloc>::erase(iterator _First, iterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "list erase");
  for ( ; _First!=_Last; )
    _First = erase(_First);
  return _First;
}

template <class Type, class Alloc>
void list<Type,Alloc>::clear()
{
  if (!Myhead_)
    return;
  node_ptr cur = (node_ptr)(Myhead_->next);
  node_ptr tmp;
  while (cur != Myhead_) {
    tmp = cur;
    cur = (node_ptr)(tmp->next);
    destroy(&tmp);
    data_allocator_::deallocate(tmp);
  }
  _empty_init();
}

template <class Type, class Alloc>
void list<Type,Alloc>::assign(size_type _Count, const Type& _Val)
{
  iterator i = begin();
  for ( ; i!=end() && _Count > 0; ++i, --_Count)
    *i = _Val;
  if (_Count > 0)
    insert(end(), _Count, _Val);
  else
    erase(i, end());
}

template <class Type, class Alloc>
template <class InputIterator>
void list<Type,Alloc>::
assign(InputIterator _First,
       InputIterator _Last,
       typename is_iterator<InputIterator>::ID
       = Identity())
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "list assign");
  size_type newSize = 0;
  iterator cur = begin();
  for ( ; cur != end() && _First != _Last;
        ++cur, ++_First) {
    *cur = *_First;
    ++newSize;
  }
  if (cur == end()) {
    for( ; _First != _Last; ++_First)
      insert(cur, *_First);
  } else {
    erase(cur, end());
  }
  this->Mysize_ = newSize;
}

template <class Type, class Alloc>
template <class Predicate>
void list<Type,Alloc>::remove_if(Predicate _Comp)
{
  MINI_STL_DEBUG_POINTER(_Comp, "list remove_if");
  for (iterator iter = begin(); beg != end(); ++iter)
    if (_Comp(*iter))
      iter = erase(iter);
}


template <class Type, class Alloc>
void list<Type,Alloc>::remove(const Type& _Val)
{
  for (iterator iter = begin(); iter != end(); ++iter)
    if(*iter == _Val)
      iter = erase(iter);
}

template <class Type, class Alloc>
template<class BinaryPredicate>
void list<Type,Alloc>::
unique(BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_POINTER(_Comp, "list unique");
  iterator first = begin();
  iterator last = end();
  if (first == last)
    return;
  iterator next = first;
  while (++next != last) {
    if(_Comp(*next,*first))
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
void list<Type,Alloc>::merge(list &_Right)
{
  this->Mysize_ += _Right.Mysize_;
  _Right.Mysize_ = 0;
  iterator first1 = this->begin();
  iterator last1 = this->end();
  iterator first2 = _Right.begin();
  iterator last2 = _Right.end();

  while (first1!=last1 && first2!=last2)
    if (*first2 < *first1) {
      iterator next = first2;
      _transfer(first1, first2, ++next);
      first2 = next;
    } else {
      ++first1;
    }
  if (first2 != last2)
    _transfer(first1, first2, last2);
}

template <class Type, class Alloc>
template <class BinaryPredicate>
void list<Type,Alloc>::sort(BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_POINTER(_Comp, "list sort");
  if (size()==0 || size()==1)
    return;
  list<Type,Alloc> carry;
  list<Type,Alloc> counter[20];
  int fill = 0;
  while (!empty()) {
    carry.splice(carry.begin(), *this, begin());
  int i = 0;
    while (i<fill && !counter[i].empty()) {
      counter[i].merge(carry, _Comp);
      carry.swap(counter[i++]);
    }
    carry.swap(counter[i]);
    if (i == fill)
      ++fill;
  }
  for (int i = 1; i < fill; ++i)
    counter[i].merge(counter[i-1], _Comp);
  swap(counter[fill-1]);
}

template <class Type, class Alloc>
template <class BinaryPredicate>
void list<Type,Alloc>::merge(list &_Right, BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_POINTER(_Comp, "list merge");
  this->Mysize_ += _Right.Mysize_;
  _Right.Mysize_ = 0;
  iterator first1 = this->begin();
  iterator last1 = this->end();
  iterator first2 = _Right.begin();
  iterator last2 = _Right.end();

  while (first1!=last1 && first2!=last2)
    if (MINI_STL_DEBUG_COMP(_Comp, *first2, *first1)) {
      iterator next = first2;
      _transfer(first1, first2, ++next);
      first2 = next;
    } else {
      ++first1;
    }
  if (first2 != last2)
    _transfer(first1, first2, last2);
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
    if (i == fill)
      ++fill;
  }
  for (int i=1; i < fill; ++i)
    counter[i].merge(counter[i-1]);
  swap(counter[fill-1]);
}

MINI_STL_END
#endif // MINI_STL_LIST_H
