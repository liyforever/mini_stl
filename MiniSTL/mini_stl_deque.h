#ifndef MINI_STL_DEQUE_H
#define MINI_STL_DEQUE_H
#include "mini_stl_memory.h"

MINI_STL_BEGIN

inline size_t __deque_buf_size(size_t _Num, size_t _Size)
{
  return _Num != 0 ? _Num : (_Size < 512 ? size_t(512 / _Size) : size_t(1));
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

  DequeIterator()
    : cur(0),first(0),
      last(0),map_node(0)
  {}

  DequeIterator(pointer _Ptr, map_pointer _Map_ptr)
  {
    this->cur = _Ptr;
    this->first = *_Map_ptr;
    this-> last = *_Map_ptr + buffer_size();
    this->map_node = _Map_ptr;
  }

  DequeIterator(const iterator& _Right)
  {
    this->cur = _Right.cur;
    this->first = _Right.first;
    this->last = _Right.last;
    this->map_node = _Right.map_node;
  }

  void reset()
  {
    this->cur = 0;
    this->first = 0;
    this->last = 0;
    this->map_node = 0;
  }

  static size_t buffer_size()
  {
    return __deque_buf_size(BuffSize, sizeof(Type));
  }

  void set_node(map_pointer _New_node)
  {
    this->map_node = _New_node;
    this->first = *_New_node;
    this->last = *_New_node + buffer_size();
  }

  reference operator*() const
  {
    return *cur;
  }

  pointer operator->() const
  {
    return &*cur;
  }

  difference_type operator-(const Self& _Right) const
  {
    if (!map_node)
      return 0;
    return difference_type(buffer_size()) * (map_node - _Right.map_node - 1)
                           + (cur - first) + (_Right.last - _Right.cur);
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

  Self& operator+=(difference_type _Num)
  {
    difference_type offset = _Num + (cur - first);

    if (offset>=0 && offset<difference_type(buffer_size())) {
        //offset on buffer
      cur += _Num;
    } else {
        difference_type map_node_offset =
            offset > 0 ? offset / difference_type(buffer_size())
                       : -difference_type((-offset - 1) / buffer_size()) - 1;
            set_node(map_node + map_node_offset);
            cur = first + (offset - map_node_offset * difference_type(buffer_size()));
    }
    return *this;
  }

  Self operator+(difference_type _Num) const
  {
    Self tmp = *this;
    return tmp += _Num;
  }

  Self& operator-=(difference_type _Num)
  {
    return *this += -_Num;
  }

  Self operator-(difference_type _Num) const
  {
    Self tmp = *this;
    return tmp -= _Num;
  }

  reference operator[](difference_type _Pos) const
  {
    return *(*this + _Pos);
  }

  bool operator==(const Self& _Right) const
  {
    return cur == _Right.cur;
  }

  bool operator!=(const Self& _Right) const
  {
    return cur != _Right.cur;
  }

  bool operator<(const Self& _Right) const
  {
    return (map_node == _Right.map_node) ?
          (cur < _Right.cur) : (map_node < _Right.map_node);
  }

  bool operator>(const Self& _Right) const
  {
    return _Right < *this;
  }

  pointer cur;
  pointer first;
  pointer last;
  map_pointer map_node;
};

template <class Type,
          class Alloc = _MY_STL::default_allocator,
          size_t BuffSize = 0>
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
  typedef _MY_STL::reverse_iterator<iterator>   reverse_iterator;
  typedef _MY_STL::reverse_iterator<const_iterator> const_reverse_iterator;
protected:
  typedef Type**    map_pointer;
  typedef simpleAlloc<value_type,Alloc> data_allocator_;
  typedef simpleAlloc<pointer,Alloc> map_allocator_;
  iterator MyfirstIter_;
  iterator MylastIter_;
  map_pointer Mymap_;
  size_type Mymap_size_;
  enum {k_init_map_size = 8,
        k_add_map_node = 1};
public:
  deque(const allocator_type&/*Al*/=allocator_type()) :
    MyfirstIter_(),MylastIter_(),Mymap_(0),Mymap_size_(0)
  {
    _init_map(0);
  }

  explicit deque(size_type _Count) :
    MyfirstIter_(),MylastIter_(),Mymap_(0),Mymap_size_(0)
  {
    _fill_init_aux(_Count, Type());
  }

  deque(size_type _Count, const Type& _Val,
        const allocator_type&/*Al*/=allocator_type()) :
    MyfirstIter_(),MylastIter_(),Mymap_(0),Mymap_size_(0)
  {
    _fill_init_aux(_Count, _Val);
  }

  deque(const deque& _Right)
  {
    size_type n = _Right.size();
    _init_map(n);
    _MY_STL::uninitialized_copy(_Right.begin(), _Right.end(), begin());
  }

  template<class InputIterator>
  deque(InputIterator _First,
        InputIterator _Last,
        const allocator_type&/*Al*/=allocator_type(),
        typename is_iterator<InputIterator>::ID = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "deque deque()");
    difference_type n = DISTANCE(_First, _Last);
    _init_map(n);
    _MY_STL::uninitialized_copy(_First, _Last, begin());
  }

#ifdef MINI_STL_RVALUE_REFS
  deque(deque&& _Right)
  {
    MyfirstIter_ = _Right.MyfirstIter_;
    MylastIter_ = _Right.MylastIter_;
    Mymap_size_ = _Right.Mymap_size_;
    Mymap_ = _Right.Mymap_;
    _Right.MyfirstIter_.reset();
    _Right.MylastIter_.reset();
    _Right.Mymap_ = 0;
  }

  deque& operator=(deque&& _Right)
  {
    if (this != &_Right) {
      _MY_STL::swap(this->MyfirstIter_, _Right.MyfirstIter_);
      _MY_STL::swap(this->MylastIter_, _Right.MylastIter_);
      _MY_STL::swap(this->Mymap_, _Right.Mymap_);
      _MY_STL::swap(this->Mymap_, _Right.Mymap_);
    }
    return *this;
  }

#endif //MINI_STL_RVALUE_REFS
  deque& operator=(const deque& _Right)
  {
    if (this !=& _Right) {
      if (size() >= _Right.size()) {
        clear();
        MylastIter_ = _MY_STL::copy(_Right.begin(),
                                    _Right.end(),
                                    begin());
      } else {
        _destroy_node(MyfirstIter_.map_node, MylastIter_.map_node);
        _deallocate_map(Mymap_, Mymap_size_);
        _init_map(_Right.size());
        _MY_STL::uninitialized_copy(_Right.begin(),
                                    _Right.end(),
                                    begin());
      }
    }
    return *this;
  }

  ~deque()
  {
    if(Mymap_)
    {
      _destroy_node(MyfirstIter_.map_node, MylastIter_.map_node);
      _deallocate_map(Mymap_, Mymap_size_);
    }
  }

public:
  allocator_type get_allocator( ) const
  {
    return Alloc();
  }

  iterator begin()
  {
    return MyfirstIter_;
  }

  const_iterator begin() const
  {
    return MyfirstIter_;
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
    return MylastIter_;
  }

  const_iterator end() const
  {
    return MylastIter_;
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
    return MyfirstIter_;
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator(end());
  }

  const_iterator cend() const
  {
    return MylastIter_;
  }

  const_reverse_iterator crend() const
  {
    return const_reverse_iterator(begin());
  }

  reference front()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "deque front");
    return *MyfirstIter_;
  }

  const_reference front() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "deque front");
    return *MyfirstIter_;
  }

  reference back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "deque back");
    return *(MylastIter_ - 1);
  }

  const_reference back() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "deque back");
    return *(MylastIter_ - 1);
  }

  reference operator[](size_type _Pos)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "deque operator[]");
    return MyfirstIter_[difference_type(_Pos)];
  }

  const_reference operator [](size_type _Pos) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "deque operator[]");
    return MyfirstIter_[difference_type(_Pos)];
  }

  reference at(size_type _Pos)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "deque at");
    return MyfirstIter_[difference_type(_Pos)];
  }

  const_reference at(size_type _Pos) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "deque at");
    return MyfirstIter_[difference_type(_Pos)];
  }

  size_type max_size() const
  {
    return size_type(-1);
  }

  size_type size() const
  {
    return MylastIter_ - MyfirstIter_;
  }

  bool empty() const
  {
    return MylastIter_ == MyfirstIter_;
  }

  void push_back(const value_type& _Val);

  void push_front(const value_type& _Val);
#ifdef MINI_STL_RVALUE_REFS
  void push_back(value_type&& _Val);
  void push_front(value_type&& _Val);
#endif
  void pop_back();

  void pop_front();

  iterator insert(iterator _Position, const value_type& _Val);
#ifdef MINI_STL_RVALUE_REFS
  iterator insert(iterator _Position, value_type&& _Val);
#endif
  void insert(iterator _Position, size_type _Count,
              const value_type& _Val)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "deque insert");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->end(), "deque insert");
    _insert_fill_aux(_Position, _Count, _Val);
  }

  template<class InputIterator>
  void insert(iterator _Position,
              InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID
              = Identity());

  iterator erase(iterator _Position);

  iterator erase(iterator _First, iterator _Last);
  void clear();

  void assign(size_type _Count, const value_type& _Val);

  template<class InputIterator>
  void assign(InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID
              = Identity());


  void resize(size_type _NewSize);

  void resize(size_type _NewSize, const Type& _Val);

  void swap(deque& _Right)
  {
    if (this == &_Right)
      return;
    _MY_STL::swap(this->MyfirstIter_, _Right.MyfirstIter_);
    _MY_STL::swap(this->MylastIter_, _Right.MylastIter_);
    _MY_STL::swap(this->Mymap_, _Right.Mymap_);
    _MY_STL::swap(this->Mymap_size_, _Right.Mymap_size_);
  }
protected:
  pointer _allocate_node()
  {
    return data_allocator_::allocate(_buffer_size());
  }

  void _deallocate_node(pointer _Ptr)
  {
      data_allocator_::deallocate(_Ptr, _buffer_size());
  }

  map_pointer _allocate_map(size_t _Count)
  {
    return map_allocator_::allocate(_Count);
  }

  void _deallocate_map(map_pointer _Ptr, size_t _Count)
  {
    map_allocator_::deallocate(_Ptr, _Count);
  }

  void _create_node(map_pointer _First, map_pointer _Last)
  {
    map_pointer cur;
    MINI_STL_TRY {
      for (cur = _First; cur != _Last; ++cur)
        *cur = _allocate_node();
    }
    MINI_STL_UNWIND(_destroy_node(_First, cur));
  }

  void _destroy_node(map_pointer _First, map_pointer _Last)
  {
    for ( ;_First != _Last; ++_First)
      _deallocate_node(*_First);
  }

  void _fill_init_aux(size_type _Count, const Type& _Val);

  void _init_map(size_type _Count);

  size_t _buffer_size()
  {
    return __deque_buf_size(BuffSize, sizeof(Type));
  }

  void _push_back_aux(const value_type& _Val);

  void _push_front_aux(const value_type& _Val);
#ifdef MINI_STL_RVALUE_REFS
  void _push_back_aux(value_type&& _Val);

  void _push_front_aux(value_type&& _Val);
#endif
  void _reserve_map_at_front(size_type _Nodes_add_num=1);

  void _reserve_map_at_back(size_type _Nodes_add_num=1);

  void _reallocate_map(size_type _Nodes_add_num,bool _FrontOrBack);

  void _new_elem_at_front(size_type _NewElem);

  void _new_elem_at_back(size_type _NewElem);

  iterator _reserve_elem_at_front(size_type _Count)
  {
    size_type freeNum = MyfirstIter_.cur - MyfirstIter_.first;
    if (_Count > freeNum)
      _new_elem_at_front(_Count - freeNum);
    return MyfirstIter_ - difference_type(_Count);
  }

  iterator _reserve_elem_at_back(size_type _Count)
  {
    size_type freeNum = (MylastIter_.last - MylastIter_.cur) - 1;
    if (_Count > freeNum)
      _new_elem_at_back(_Count - freeNum);
    return MylastIter_ + difference_type(_Count);
  }

  void _pop_back_aux();

  void _pop_front_aux();

  iterator _insert_aux(iterator _Position, const value_type& _Val);
#ifdef MINI_STL_RVALUE_REFS
  iterator _insert_aux(iterator _Position, value_type&& _Val);
#endif
  void _insert_fill_aux(iterator _Position, size_type _Count, const Type& _Val);

  void _insert_aux(iterator _Position, size_type _Count, const value_type& _Val);
};

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_init_map(size_type _Count)
{
  size_type mapNodeSize = _Count / _buffer_size() + 1;
  Mymap_size_ = k_init_map_size > mapNodeSize +  2 ? k_init_map_size :
                                                   mapNodeSize + 2;
  //max(k_init_map_size, (mapNodeSize + 2));
  Mymap_ = _allocate_map(Mymap_size_);
  //adjuest first last in middle
  map_pointer newFirst = Mymap_ + (Mymap_size_ - mapNodeSize) / 2;
  map_pointer newLast = newFirst + mapNodeSize - 1;
  MINI_STL_TRY{
    _create_node(newFirst, newLast + 1);
  }
  MINI_STL_UNWIND((_deallocate_map(newFirst,Mymap_size_),
                    Mymap_ = 0, Mymap_size_ = 0));
  //adjust iterator
  MyfirstIter_.set_node(newFirst);
  MylastIter_.set_node(newLast);
  MyfirstIter_.cur = MyfirstIter_.first;
  MylastIter_.cur = MylastIter_.first + _Count % _buffer_size();
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_fill_init_aux(size_type _Count, const Type& _Val)
{
  _init_map(_Count);
  map_pointer cur;
  MINI_STL_TRY {
    for(cur = MyfirstIter_.map_node; cur < MylastIter_.map_node; ++cur)
      _MY_STL::uninitialized_fill(*cur, *cur + _buffer_size(), _Val);
      _MY_STL::uninitialized_fill(MylastIter_.first, MylastIter_.cur, _Val);
    }
  MINI_STL_UNWIND(_MY_STL::destroy(MyfirstIter_,iterator(*cur, cur)));
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_push_front_aux(const value_type& _Val)
{
  _reserve_map_at_front();
  *(MyfirstIter_.map_node - 1) = _allocate_node();
  MINI_STL_TRY {
    MyfirstIter_.set_node(MyfirstIter_.map_node - 1);
    MyfirstIter_.cur = MyfirstIter_.last - 1;
    _MY_STL::construct(MyfirstIter_.cur, _Val);
  }
  MINI_STL_UNWIND(MyfirstIter_.set_node(MyfirstIter_.map_node + 1);
      MyfirstIter_.cur = MyfirstIter_.first;
      _deallocate_node(*(MyfirstIter_.map_node - 1));)
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_push_back_aux(const value_type& _Val)
{
  _reserve_map_at_back();
  *(MylastIter_.map_node + 1) = _allocate_node();
  MINI_STL_TRY {
    _MY_STL::construct(MylastIter_.cur, _Val);
    MylastIter_.set_node(MylastIter_.map_node + 1);
    MylastIter_.cur = MylastIter_.first;
  }
  MINI_STL_UNWIND(_deallocate_node(*(MylastIter_.map_node + 1));)
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::push_back(const value_type& _Val)
{
  if (MylastIter_.cur != MylastIter_.last - 1) {
      _MY_STL::construct(MylastIter_.cur, _Val);
      ++MylastIter_.cur;
  } else {
    _push_back_aux(_Val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::push_front(const value_type& _Val)
{
  if (MyfirstIter_.cur != MyfirstIter_.first) {
      _MY_STL::construct(MyfirstIter_.cur - 1, _Val);
      --MyfirstIter_.cur;
  } else {
      _push_front_aux(_Val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::pop_back()
{
  MINI_STL_DEBUG_CHECK_SIZE(this->size(), "deque pop_back");
  if (MylastIter_.cur != MylastIter_.first)
    _MY_STL::destroy(--MylastIter_.cur);
  else
    _pop_back_aux();
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::pop_front()
{
  MINI_STL_DEBUG_CHECK_SIZE(this->size(), "deque pop_front");
  if (MyfirstIter_.cur != MyfirstIter_.last-1) {
    _MY_STL::destroy(MyfirstIter_.cur);
    ++MyfirstIter_.cur;
  } else {
    _pop_front_aux();
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::assign(size_type _Count, const value_type& _Val)
{
  if (_Count > size()) {
    fill(begin(), end(), _Val);
    insert(end(), _Count - size(), _Val);
  } else {
    erase(begin() + _Count, end());
    fill(begin(), end(), _Val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
template<class InputIterator>
void deque<Type,Alloc,BuffSize>::
assign(InputIterator _First, InputIterator _Last,
      typename is_iterator<InputIterator>::ID)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "deque assign");
  iterator cur;
  for (cur = begin(); cur!=end() && _First!=_Last;
       ++cur, ++_First)
      *cur = *_First;
  if (cur == end())
    insert(cur, _First, _Last);
  else
    erase(cur, end());
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_pop_back_aux()
{
  _deallocate_node(*MylastIter_.map_node);
  MylastIter_.set_node(MylastIter_.map_node - 1);
  MylastIter_.cur = MylastIter_.last - 1;
  destroy(MylastIter_.cur);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_pop_front_aux()
{
  _MY_STL::destroy(MyfirstIter_.cur);
  _deallocate_node(*MyfirstIter_.map_node);
  MyfirstIter_.set_node(MyfirstIter_.map_node + 1);
  MyfirstIter_.cur = MyfirstIter_.first;
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reserve_map_at_back(size_type _Nodes_add_num)
{
  if (_Nodes_add_num + 1 > Mymap_size_ - (MylastIter_.map_node - Mymap_))
    _reallocate_map(_Nodes_add_num, false);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reserve_map_at_front(size_type _Nodes_add_num)
{
  if (_Nodes_add_num  > (size_type)(MyfirstIter_.map_node - Mymap_))
    _reallocate_map(_Nodes_add_num, true);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_reallocate_map(size_type _Nodes_add_num, bool _FrontOrback)
{
  size_type oldMapNum = MylastIter_.map_node - MyfirstIter_.map_node + 1;
  size_type newMapNum = oldMapNum + _Nodes_add_num;
  map_pointer newMapFirst;
  if (Mymap_size_ > 2 * newMapNum) {
      newMapFirst = Mymap_ + (Mymap_size_ - newMapNum) / 2
          + (_FrontOrback ? _Nodes_add_num : 0);
      if (newMapFirst < MyfirstIter_.map_node)
#ifdef MINI_STL_RVALUE_REFS
        _MY_STL::move(MyfirstIter_.map_node, MylastIter_.map_node + 1, newMapFirst);
      else
        _MY_STL::move_backward(MyfirstIter_.map_node,
                               MylastIter_.map_node + 1,
                               newMapFirst + oldMapNum);
#else
        copy(MyfirstIter_.map_node, MylastIter_.map_node + 1, newMapFirst);
      else
        copy_backward(MyfirstIter_.map_node,
                      MylastIter_.map_node + 1,
                      newMapFirst + oldMapNum);
#endif
  } else {
      size_type addNum = _Nodes_add_num;
      size_type newMapSize = Mymap_size_ + _MY_STL::max(Mymap_size_, addNum) + 2;
      map_pointer newMap = _allocate_map(newMapSize);
      newMapFirst = newMap + (newMapSize - newMapNum) / 2
                    + (_FrontOrback ? _Nodes_add_num : 0);
      _MY_STL::uninitialized_copy(MyfirstIter_.map_node,
                                  MylastIter_.map_node + 1,
                                  newMapFirst);

      _deallocate_map(Mymap_, Mymap_size_);
      Mymap_ = newMap;
      Mymap_size_ = newMapSize;
  }
  MyfirstIter_.set_node(newMapFirst);
  MylastIter_.set_node(newMapFirst + oldMapNum - 1);
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_new_elem_at_back(size_type _NewElem)
{
  size_type new_map_node = (_NewElem + _buffer_size() - 1)
      / _buffer_size();
  _reserve_map_at_back(new_map_node);
  size_type i;
  MINI_STL_TRY {
    for (i = 1; i <= new_map_node; ++i)
      *(MylastIter_.map_node + i) = _allocate_node();
  }
  MINI_STL_UNWIND(
    for (size_type j = 1; j <= i; ++j)
          _deallocate_node(*(MylastIter_.map_node + j));
  )
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_new_elem_at_front(size_type _NewElem)
{
  size_type new_map_node = (_NewElem + _buffer_size() - 1)
      / _buffer_size();
  _reserve_map_at_front(new_map_node);
  size_type i;
  MINI_STL_TRY {
    for (i =1; i <= new_map_node; ++i)
      *(MyfirstIter_.map_node - i) = _allocate_node();
  }
  MINI_STL_UNWIND(
    for (size_type j = 1; j <= i; ++j)
      _deallocate_node(*(MyfirstIter_.map_node + j));
  )
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::_insert_aux(iterator _Position, const value_type& _Val)
{
  difference_type offset = _Position - MyfirstIter_;
  if ((size_type)(offset)<size()/2) {
    push_front(front());
    iterator frontTo = MyfirstIter_;
    ++frontTo;
    iterator frontS = frontTo;
    ++frontS;
    position = MyfirstIter_ + offset;
#ifdef MINI_STL_RVALUE_REFS
    _MY_STL::move(frontS, _Position, frontTo);
#else
    _MY_STL::copy(frontS, _Position, frontTo);
#endif
  } else {
    push_back(back());
    iterator back1 = MylastIter_;
    --back1;
    iterator back2 = back1;
    --back2;
    _Position = MyfirstIter_ + offset;
#ifdef MINI_STL_RVALUE_REFS
    _MY_STL::move_backward(_Position, back2, back1);
#else
    _MY_STL::copy_backward(_Position, back2, back1);
#endif
  }
  *_Position = _Val;
  return _Position;
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::insert(iterator _Position, const value_type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "deque insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->end(), "deque insert");
  if (_Position.cur == MyfirstIter_.cur) {
      push_front(_Val);
      return MyfirstIter_;
  } else if (_Position.cur == MylastIter_.cur) {
      push_back(_Val);
      return MylastIter_;
  } else {
      return _insert_aux(_Position, _Val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::resize(size_type _NewSize)
{
  resize(_NewSize, Type());
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::
resize(size_type _NewSize, const Type& _Val)
{
  if (_NewSize > size())
    insert(end(), _NewSize-size(), _Val);
  else
    erase(begin()+_NewSize, end());
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::
_insert_aux(iterator _Position,
            size_type _Count,
            const value_type& _Val)
{  
  const difference_type frontNum = _Position - MyfirstIter_;
  size_type length = size();
  if (frontNum < difference_type(length / 2)) {
    iterator newFirst = _reserve_elem_at_front(_Count);
    iterator oldFirst = MyfirstIter_;
    _Position = MyfirstIter_ + frontNum;
    MINI_STL_TRY {
        oldFirst = _MY_STL::uninitialized_copy(MyfirstIter_,
                                               _Position,
                                               newFirst);
        MyfirstIter_ = newFirst;
        _MY_STL::uninitialized_fill_n(oldFirst, _Count, _Val);
    }
    MINI_STL_UNWIND(
       _destroy_node(newFirst.map_node, MyfirstIter_.map_node))
  } else {
    iterator newLast = _reserve_elem_at_back(_Count);
    iterator oldLast = MylastIter_;
    const difference_type afterNum = length - frontNum;
    _Position = MylastIter_ - afterNum;
    MINI_STL_TRY {
#ifdef MINI_STL_RVALUE_REFS
      _MY_STL::move_backward(_Position, oldLast, newLast);
#else
      _MY_STL::copy_backward(_Position, oldLast, newLast);
#endif
      _MY_STL::fill_n(_Position, _Count, _Val);
      MylastIter_ = newLast;
    }
    MINI_STL_UNWIND(_destroy_node(MylastIter_.map_node + 1,
                                  newLast.map_node + 1));
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::
_insert_fill_aux(iterator _Position,
                 size_type _Count,
                 const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "deque insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->end(), "deque insert");
  if (_Position.cur == MyfirstIter_.cur) {
    iterator newFirst = _reserve_elem_at_front(_Count);
    MINI_STL_TRY {
      _MY_STL::uninitialized_fill(newFirst, MyfirstIter_, _Val);
      MyfirstIter_ = newFirst;
    }
    MINI_STL_UNWIND(_destroy_node(newFirst.map_node,MyfirstIter_.map_node));
  } else if (_Position.cur == MylastIter_.cur) {
    iterator newLast = _reserve_elem_at_back(_Count);
    MINI_STL_TRY {
      uninitialized_fill(MylastIter_, newLast, _Val);
      MylastIter_ = newLast;
    }
    MINI_STL_UNWIND(_destroy_node(MyfirstIter_.map_node + 1,
                                  newLast.map_node + 1));
  } else {
    _insert_aux(_Position, _Count, _Val);
  }
}

template <class Type, class Alloc, size_t BuffSize>
template<class InputIterator>
void deque<Type,Alloc,BuffSize>::
insert(iterator _Position,
       InputIterator _First,
       InputIterator _Last,
       typename is_iterator<InputIterator>::ID)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "deque insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->end(), "deque insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "deque insert");
  difference_type n = DISTANCE(_First, _Last);
  if (_Position.cur == MyfirstIter_.cur) {
    iterator newFirst = _reserve_elem_at_front(n);
    MINI_STL_TRY {
      _MY_STL::uninitialized_copy(_First, _Last, newFirst);
      MyfirstIter_ = newFirst;
    }
    MINI_STL_UNWIND(_destroy_node(newFirst.map_node,MyfirstIter_.map_node));
  } else if (_Position.cur == MylastIter_.cur) {
    iterator newLast = _reserve_elem_at_back(n);
    MINI_STL_TRY {
      _MY_STL::uninitialized_copy(_First, _Last, MylastIter_);
      MylastIter_ = newLast;
    }
    MINI_STL_UNWIND(_destroy_node(MyfirstIter_.map_node + 1,
                                     newLast.map_node + 1));
  } else {
    const difference_type frontNum = _Position - MyfirstIter_;
    size_type length = size();
    if (frontNum < difference_type(length / 2)) {
      iterator newFirst = _reserve_elem_at_front(n);
      iterator oldFirst = MyfirstIter_;
      _Position = MyfirstIter_ + frontNum;
      MINI_STL_TRY {
        oldFirst = _MY_STL::uninitialized_copy(MyfirstIter_, _Position, newFirst);
        MyfirstIter_ = newFirst;
        _MY_STL::uninitialized_copy(_First, _Last, oldFirst);
      }
      MINI_STL_UNWIND(
        _destroy_node(newFirst.map_node, MyfirstIter_.map_node))
    } else {
      iterator newLast = _reserve_elem_at_back(n);
      iterator oldLast = MylastIter_;
      const difference_type afterNum = length - frontNum;
      _Position = MylastIter_ - afterNum;
      MINI_STL_TRY {
#ifdef MINI_STL_RVALUE_REFS
        _MY_STL::move_backward(_Position, oldLast, newLast);
#else
        _MY_STL::copy_backward(_Position, oldLast, newLast);
#endif
        _MY_STL::uninitialized_copy(_First, _Last, _Position);
        MylastIter_ = newLast;
      }
      MINI_STL_UNWIND(_destroy_node(MylastIter_.map_node + 1,
                                      newLast.map_node + 1));
    }
  }
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::erase(iterator _Position)
{
  MINI_STL_DEBUG_CHECK_POS(this->size(), DISTANCE(this->begin(), _Position), "deque erase");
  iterator next = _Position;
  ++next;
  difference_type offset = _Position - MyfirstIter_;
  if ((size_type)(offset)<size()/2) {
#ifdef MINI_STL_RVALUE_REFS
    _MY_STL::move_backward(MyfirstIter_, _Position, next);
#else
    _MY_STL::copy_backward(MyfirstIter_, _Position, next);
#endif
    pop_front();
  } else {
#ifdef MINI_STL_RVALUE_REFS
    _MY_STL::move(next, MylastIter_, _Position);
#else
    _MY_STL::copy(next, MylastIter_, _Position);
#endif
    pop_back();
  }
  return MyfirstIter_ + offset;
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::erase(iterator _First, iterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "deque erase");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _First, "deque erase");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Last, this->end(), "deque erase");
  MINI_STL_DEBUG_CHECK_POS(this->size(), DISTANCE(this->begin(), _First), "deque erase");
  MINI_STL_DEBUG_CHECK_POS(this->size()+1, DISTANCE(this->begin(), _Last), "deque erase");
  if (_First==begin() && _Last==end()) {
    clear();
    return MyfirstIter_;
  } else {
    difference_type n = _Last - _First;
    difference_type beforeNum = _First - begin();
    if ((size_type)(beforeNum) < (size()-n)/2){
#ifdef MINI_STL_RVALUE_REFS
      _MY_STL::move_backward(begin(), _First, _Last);
#else
      _MY_STL::copy_backward(begin(), _First, _Last);
#endif
      iterator newFirst = begin() + n;
      _MY_STL::destroy(begin(), newFirst);
      _destroy_node(MyfirstIter_.map_node, newFirst.map_node);
      MyfirstIter_ = newFirst;
    } else {
#ifdef MINI_STL_RVALUE_REFS
      _MY_STL::move(_Last, end(), _First);
#else
      _MY_STL::copy(_Last, end(), _First);
#endif
      iterator newLast = end() - n;
      _MY_STL::destroy(newLast, end());
      _destroy_node(newLast.map_node+1, MylastIter_.map_node+1);
      MylastIter_ = newLast;
    }
    return MyfirstIter_ + beforeNum;
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::clear()
{
  for (map_pointer node = MyfirstIter_.map_node + 1;
       node < MylastIter_.map_node; ++ node) {
    _MY_STL::destroy(*node, *node + _buffer_size());
    _deallocate_node(*node);
  }

  if (MyfirstIter_.map_node!=MylastIter_.map_node) {
    _MY_STL::destroy(MyfirstIter_.cur, MyfirstIter_.last);
    _MY_STL::destroy(MylastIter_.first, MylastIter_.cur);
    _deallocate_node(*MylastIter_.map_node);
  } else {
    _MY_STL::destroy(MyfirstIter_.cur, MyfirstIter_.last);
  }
  MylastIter_ = MyfirstIter_;
}
#ifdef MINI_STL_RVALUE_REFS
template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_push_front_aux(value_type&& _Val)
{
  _reserve_map_at_front();
  *(MyfirstIter_.map_node - 1) = _allocate_node();
  MINI_STL_TRY {
    MyfirstIter_.set_node(MyfirstIter_.map_node - 1);
    MyfirstIter_.cur = MyfirstIter_.last - 1;
    _MY_STL::construct(MyfirstIter_.cur, _MY_STL::move(_Val));
  }
  MINI_STL_UNWIND(MyfirstIter_.set_node(MyfirstIter_.map_node + 1);
      MyfirstIter_.cur = MyfirstIter_.first;
      _deallocate_node(*(MyfirstIter_.map_node - 1));)
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_push_back_aux(value_type&& _Val)
{
  _reserve_map_at_back();
  *(MylastIter_.map_node + 1) = _allocate_node();
  MINI_STL_TRY {
    _MY_STL::construct(MylastIter_.cur, _MY_STL::move(_Val));
    MylastIter_.set_node(MylastIter_.map_node + 1);
    MylastIter_.cur = MylastIter_.first;
  }
  MINI_STL_UNWIND(_deallocate_node(*(MylastIter_.map_node + 1));)
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::push_back(value_type&& _Val)
{
  if (MylastIter_.cur != MylastIter_.last - 1) {
      _MY_STL::construct(MylastIter_.cur, _MY_STL::move(_Val));
      ++MylastIter_.cur;
  } else {
    _push_back_aux(_MY_STL::move(_Val));
  }
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::push_front(value_type&& _Val)
{
  if (MyfirstIter_.cur != MyfirstIter_.first) {
      _MY_STL::construct(MyfirstIter_.cur - 1, _MY_STL::move(_Val));
      --MyfirstIter_.cur;
  } else {
      _push_front_aux(_MY_STL::move(_Val));
  }
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::_insert_aux(iterator _Position, value_type&& _Val)
{
  difference_type offset = _Position - MyfirstIter_;
  if ((size_type)(offset)<size()/2) {
    push_front(front());
    iterator frontTo = MyfirstIter_;
    ++frontTo;
    iterator frontS = frontTo;
    ++frontS;
    _Position = MyfirstIter_ + offset;
    _MY_STL::move(frontS, _Position, frontTo);
  } else {
    push_back(back());
    iterator back1 = MylastIter_;
    --back1;
    iterator back2 = back1;
    --back2;
    _Position = MyfirstIter_ + offset;
    _MY_STL::move_backward(_Position, back2, back1);
  }
  *_Position = _MY_STL::move(_Val);
  return _Position;
}

template <class Type, class Alloc, size_t BuffSize>
typename deque<Type,Alloc,BuffSize>::iterator
deque<Type,Alloc,BuffSize>::insert(iterator _Position, value_type&& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "deque insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->end(), "deque insert");
  if (_Position.cur == MyfirstIter_.cur) {
      push_front(_MY_STL::move(_Val));
      return MyfirstIter_;
  } else if (_Position.cur == MylastIter_.cur) {
      push_back(_MY_STL::move(_Val));
      return MylastIter_;
  } else {
      return _insert_aux(_Position, _MY_STL::move(_Val));
  }
}
#endif //MINI_STL_RVALUE_REFS

template <class Type, class Alloc, size_t BuffSize>
inline bool
operator==(const deque<Type,Alloc,BuffSize>& _Left,
           const deque<Type,Alloc,BuffSize>& _Right)
{
  return _Left.size() == _Right.size() &&
         _MY_STL::equal(_Left.begin(), _Left.end(),
                        _Right.begin());
}

template <class Type, class Alloc, size_t BuffSize>
inline bool
operator<(const deque<Type,Alloc,BuffSize>& _Left,
          const deque<Type,Alloc,BuffSize>& _Right)
{
  return _MY_STL::lexicographical_compare(_Left.begin(),
                                          _Left.end(),
                                          _Right.begin(),
                                          _Right.end());
}

template <class Type, class Alloc, size_t BuffSize>
inline bool
operator!=(const deque<Type,Alloc,BuffSize>& _Left,
           const deque<Type,Alloc,BuffSize>& _Right)
{
  return !(_Left == _Right);
}

template <class Type, class Alloc, size_t BuffSize>
inline bool
operator>(const deque<Type,Alloc,BuffSize>& _Left,
          const deque<Type,Alloc,BuffSize>& _Right)
{
  return _Right < _Left;
}

template <class Type, class Alloc, size_t BuffSize>
inline bool
operator<=(const deque<Type,Alloc,BuffSize>& _Left,
           const deque<Type,Alloc,BuffSize>& _Right)
{
  return !(_Right < _Left);
}

template <class Type, class Alloc, size_t BuffSize>
inline bool
operator>=(const deque<Type,Alloc,BuffSize>& _Left,
           const deque<Type,Alloc,BuffSize>& _Right)
{
  return !(_Left < _Right);
}

template <class Type, class Alloc, size_t BuffSize>
inline void
swap(const deque<Type,Alloc,BuffSize>& _Left,
     const deque<Type,Alloc,BuffSize>& _Right)
{
  _Left.swap(_Right);
}

MINI_STL_END
#endif // MINI_STL_DEQUE_H
