#ifndef MINI_STL_VECTOR_H
#define MINI_STL_VECTOR_H
#include "mini_stl_memory.h"
#include "mini_stl_algobase.h"

MINI_STL_BEGIN

template <class Type, class Alloc = _MY_STL::default_allocator>
class vector
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
  typedef value_type*       iterator;
  typedef const value_type* const_iterator;
  typedef _MY_STL::reverse_iterator<iterator> reverse_iterator;
  typedef _MY_STL::reverse_iterator<const_iterator> const_reverse_iterator;
protected:
  typedef simpleAlloc<value_type, Alloc> data_allocator_;
  iterator Myfirst_;
  iterator Mylast_;
  iterator Myend_;
public:
  vector(const allocator_type&/*Al*/=allocator_type())
    : Myfirst_(0), Mylast_(0), Myend_(0) {}

  explicit vector(size_type _Count)
  {
    _fill_init(_Count, Type());
  }

  vector(int _Count,
         const value_type& _Val,
         const allocator_type&/*Al*/=allocator_type())
  {
    _fill_init(_Count, _Val);
  }

  vector(long _Count,
         const value_type& _Val,
         const allocator_type&/*Al*/=allocator_type())
  {
    _fill_init(_Count, _Val);
  }

  vector(size_type _Count, const value_type& _Val,
         const allocator_type&/*Al*/=allocator_type())
  {
    _fill_init(_Count, _Val);
  }

  vector(const vector& _Right)
  {
    Myfirst_ = data_allocator_::allocate(_Right.size());
    Mylast_ = _MY_STL::uninitialized_copy(_Right.begin(), _Right.end(), Myfirst_);
    Myend_ = Mylast_;
  }

  template <class InputIterator>
    vector(InputIterator _First,
           InputIterator _Last,
           const allocator_type&/*Al*/=allocator_type(),
           typename is_iterator<InputIterator>::ID = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "vector");
    difference_type size = _Last - _First;
    Myfirst_ = data_allocator_::allocate(size);
    Mylast_ = _MY_STL::uninitialized_copy(_First, _Last, Myfirst_);
    Myend_ = Mylast_;
  }

#ifdef MINI_STL_RVALUE_REFS
  vector(vector&& _Right)
  {
    this->Myfirst_ = _Right.Myfirst_;
    this->Mylast_ = _Right.Mylast_;
    this->Myend_ = _Right.Myend_;
    _Right.Myfirst_ = nullptr;
    _Right.Mylast_ = nullptr;
    _Right.Myend_ = nullptr;
  }

  vector& operator=(vector&& _Right)
  {
    if (this != &_Right) {
      _MY_STL::swap(this->Myfirst_, _Right.Myfirst_);
      _MY_STL::swap(this->Mylast_, _Right.Mylast_);
      _MY_STL::swap(this->Myend_, _Right.Myend_);
    }
    return *this;
  }

#endif //MINI_STL_RVALUE_REFS

  vector& operator=(const vector& _Right)
  {
    if (this !=& _Right) {
      destroy(begin(), end());
      if (size() >= _Right.size()) {
        Mylast_ = _MY_STL::copy(_Right.begin(),
                                _Right.end(),
                                begin());
      } else {
        _deallocate();
        Myfirst_ = data_allocator_::allocate(_Right.size());
        Mylast_ = _MY_STL::uninitialized_copy(_Right.begin(),
                                              _Right.end(),
                                              begin());
        Myend_ = Mylast_;
      }
    }
    return *this;
  }

  ~vector()
  {
    destroy(Myfirst_, Mylast_);
    _deallocate();
  }

public:
  iterator begin()
  {
    return Myfirst_;
  }

  const_iterator begin() const
  {
    return Myfirst_;
  }

  iterator end()
  {
    return Mylast_;
  }

  const_iterator end() const
  {
    return Mylast_;
  }

  reverse_iterator rbegin()
  {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(end());
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
    return (const_iterator)(Myfirst_);
  }

  const_iterator cend() const
  {
    return (const_iterator)(Mylast_);
  }

  const_reverse_iterator crbegin() const
  {
    return const_reverse_iterator(end());
  }

  const_reverse_iterator crend() const
  {
    return const_reverse_iterator(begin());
  }

  pointer data()
  {
    return Myfirst_;
  }

  const_pointer data() const
  {
    return Myfirst_;
  }

  reference front()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "vector front");
    return *Myfirst_;
  }

  const_reference front() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "vector front");
    return *Myfirst_;
  }

  reference back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "vector back");
    return *(Mylast_ - 1);
  }

  const_reference back() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "vector back");
    return *(Mylast_ - 1);
  }

  size_type size() const
  {
    return static_cast<size_type>(Mylast_ - Myfirst_);
  }

  size_type capacity() const
  {
    return static_cast<size_type>(Myend_ - Myfirst_);
  }

  bool empty() const
  {
    return Myfirst_ == Mylast_;
  }

  reference operator[](size_type _Pos)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "vector operator[]");
    MINI_STL_DEBUG_CHECK_SIZE(this->size(),"vector operator[]");
    return *(Myfirst_ + _Pos);
  }

  const_reference operator[](size_type _Pos) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "vector operator[]");
    MINI_STL_DEBUG_CHECK_SIZE(this->size(),"vector operator[]");
    return *(Myfirst_ + _Pos);
  }

  reference at(size_type _Pos)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "vector at");
    MINI_STL_DEBUG_CHECK_SIZE(this->size(),"vector at");
    return Myfirst_[_Pos];
  }

  const_reference at(size_type _Pos) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "vector at");
    MINI_STL_DEBUG_CHECK_SIZE(this->size(),"vector at");
    return Myfirst_[_Pos];
  }

  void swap(vector<Type, Alloc> &_Right);

  void push_back(const Type& _Val);

#ifdef MINI_STL_RVALUE_REFS
  void push_back(Type&& _Val);
#endif

  void pop_back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "vector pop_back");
    --Mylast_;
    _MY_STL::destroy(Mylast_);
  }

  iterator erase(iterator _First, iterator _Last);
  iterator erase(iterator _Position);

  void clear()
  {
    erase(Myfirst_, Mylast_);
  }

  void reserve(size_type _Count)
  {
    if (this->capacity() < _Count) {
      const size_type old_size = size();
      iterator tmp = data_allocator_::allocate(_Count);
      _MY_STL::uninitialized_copy(Myfirst_, Mylast_, tmp);
      _MY_STL::destroy(Myfirst_, Mylast_);
      data_allocator_::deallocate(Myfirst_, Myend_ - Myfirst_);
      Myfirst_ = tmp;
      Mylast_ = tmp + old_size;
      Myend_ = Myfirst_ + _Count;
    }
  }

  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  void insert(const_iterator _Position, size_type _Count, const Type& _Val);

  iterator insert(const_iterator _Position, const Type& _Val);

#ifdef MINI_STL_RVALUE_REFS
  iterator insert(const_iterator _Position, Type&& _Val);
#endif // MINI_STL_RVALUE_REFS

  template<class InputIterator>
  void insert(const_iterator _Position,
              InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID = Identity()
            );
  template<class InputIterator>
  inline void
  assign(InputIterator _First,
         InputIterator _Last,
         typename is_iterator<InputIterator>::ID = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "vector assign");
    this->clear();
    this->insert(this->begin(), _First, _Last);
  }

  void assign(size_type _Count, const Type& _Val)
  {
    this->clear();
    this->insert(begin(), _Count, _Val);
  }

  void resize(size_type _NewSize)
  {
    this->resize(_NewSize, Type());
  }

  void resize(size_type _NewSize, const Type& _Val);

  size_type max_size() const
  {
    return size_type(-1);
  }
protected:
  void _fill_init(size_type _Count, const Type& _Val);
  iterator _allocator_and_fill(size_type _Count, const Type& _Val);
  void _deallocate()
  {
    if (Myfirst_)
     data_allocator_::deallocate(Myfirst_, Myend_ - Myfirst_);
  }

  void _insert_aux(iterator _Position, const Type& _Val);
  void _insert_aux(iterator _Position);
};

template <class Type, class Alloc>
inline bool
operator==(const vector<Type, Alloc>& _Left,
           const vector<Type, Alloc>& _Right)
{
  return _Left.size() == _Right.size() &&
         _MY_STL::equal(_Left.begin(), _Left.end(),
                        _Right.begin());
}

template <class Type, class Alloc>
inline bool
operator<(const vector<Type, Alloc>& _Left,
          const vector<Type, Alloc>& _Right)
{
  return _MY_STL::lexicographical_compare(_Left.begin(),
                                          _Left.end(),
                                          _Right.begin(),
                                          _Right.end());
}

template <class Type, class Alloc>
inline bool
operator!=(const vector<Type, Alloc>& _Left,
           const vector<Type, Alloc>& _Right)
{
  return !(_Left == _Right);
}

template <class Type, class Alloc>
inline bool
operator>(const vector<Type, Alloc>& _Left,
          const vector<Type, Alloc>& _Right)
{
  return _Right < _Left;
}

template <class Type, class Alloc>
inline bool
operator<=(const vector<Type, Alloc>& _Left,
           const vector<Type, Alloc>& _Right)
{
  return !(_Right < _Left);
}

template <class Type, class Alloc>
inline bool
operator>=(const vector<Type, Alloc>& _Left,
           const vector<Type, Alloc>& _Right)
{
  return !(_Left < _Right);
}

template <class Type, class Alloc>
inline void
swap(const vector<Type, Alloc>& _Left,
     const vector<Type, Alloc>& _Right)
{
  _Left.swap(_Right);
}

template <class Type, class Alloc>
inline typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::
_allocator_and_fill(size_type _Count,
                    const Type& _Val)
{
  iterator result = data_allocator_::allocate(_Count);
  _MY_STL::uninitialized_fill_n(result, _Count, _Val);
  return result;
}

template <class Type, class Alloc>
void vector<Type, Alloc>::
_fill_init(size_type _Count,
           const Type& _Val)
{
  Myfirst_ = _allocator_and_fill(_Count, _Val);
  Mylast_ = Myfirst_ + _Count;
  Myend_ = Mylast_;
}

template <class Type, class Alloc>
void vector<Type, Alloc>::_insert_aux(iterator _Position,
                                      const Type& _Val)
{
  if (Mylast_ != Myend_) {
    _MY_STL::construct(Mylast_, *(Mylast_ - 1));
    ++Mylast_;
#ifdef MINI_STL_RVALUE_REFS
    _MY_STL::move_backward(_Position, Mylast_ - 2, Mylast_ - 1);
#else
    _MY_STL::copy_backward(_Position, Mylast_ - 2, Mylast_ - 1);
#endif
    *_Position = _Val;
  } else {
    const size_type oldSize = size();
    const size_type newSize = oldSize != 0 ? 2 * oldSize : 1;

    iterator newFirst = data_allocator_::allocate(newSize);
    iterator newLast = newFirst;
    MINI_STL_TRY {
      newLast = _MY_STL::uninitialized_copy(Myfirst_, _Position, newFirst);
      _MY_STL::construct(newLast, _Val);
      ++newLast;
      newLast = _MY_STL::uninitialized_copy(_Position, Mylast_, newLast);
    }
    MINI_STL_UNWIND((destroy(newFirst,newLast);
                    _deallocate(newFirst,newSize);))
    _MY_STL::destroy(Myfirst_, Mylast_);
    _deallocate();

    Myfirst_ = newFirst;
    Mylast_ = newLast;
    Myend_ = newFirst + newSize;
  }
}

template <class Type, class Alloc>
void vector<Type, Alloc>::_insert_aux(iterator _Position)
{
  _insert_aux(_Position, Type());
}

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::swap(vector<Type, Alloc> &_Right)
{
  _MY_STL::swap(this->Myfirst_, _Right.Myfirst_);
  _MY_STL::swap(this->Mylast_, _Right.Mylast_);
  _MY_STL::swap(this->Myend_, _Right.Myend_);
}

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::push_back(const Type& _Val)
{
  if (Mylast_ != Myend_) {
    _MY_STL::construct(Mylast_, _Val);
    ++Mylast_;
  } else {
    _insert_aux(Mylast_, _Val);
  }
}
#ifdef MINI_STL_RVALUE_REFS
template <class Type, class Alloc>
inline void
vector<Type, Alloc>::push_back(Type&& _Val)
{
  if (Mylast_ != Myend_) {
    _MY_STL::construct(Mylast_, _MY_STL::move(_Val));
    ++Mylast_;
  } else {
    insert(end(), _MY_STL::move(_Val));
  }
}
#endif

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::resize(size_type _NewSize, const Type &_Val)
{
  if (_NewSize < size())
    erase(begin() + _NewSize, end());
  else
    insert(end(), _NewSize - size(), _Val);
}

template <class Type, class Alloc>
typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::erase(iterator _First, iterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "vector erase1");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->Myfirst_, _Last, "vector erase2");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->Myfirst_, _First, "vector erase3");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Last, this->Mylast_, "vector erase4");
#ifdef MINI_STL_RVALUE_REFS
  iterator i = _MY_STL::move(_Last, Mylast_, _First);
#else
  iterator i = _MY_STL::copy(_Last, Mylast_, _First);
#endif
  _MY_STL::destroy(i, Mylast_);
  Mylast_ -= (_Last - _First);
  return _First;
}

template <class Type, class Alloc>
typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::erase(iterator _Position)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->Myfirst_, _Position, "vector erase");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->Mylast_-1,  "vector erase");
  if (_Position + 1 != Mylast_)
#ifdef MINI_STL_RVALUE_REFS
    _MY_STL::move(_Position + 1, Mylast_, _Position);
#else
    _MY_STL::copy(_Position + 1, Mylast_, _Position);
#endif
  --Mylast_;
  destroy(Mylast_);
  return _Position;
}

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::insert(const_iterator _Position,
                            size_type _Count,
                            const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->cbegin(), _Position, "vector insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->cend(), "vector insert");
  if (_Count != 0) {
    if (size_type(Myend_ - Mylast_) >= _Count) {
      Type copyVal(_Val);
      const size_type posAfterNum = Mylast_ - _Position;
      iterator oldLast = Mylast_;
      if (posAfterNum > _Count) {
        _MY_STL::uninitialized_copy(Mylast_ - _Count, Mylast_, Mylast_);
        Mylast_ += _Count;
#ifdef MINI_STL_RVALUE_REFS
        _MY_STL::move_backward((iterator)_Position, oldLast - _Count, oldLast);
#else
        _MY_STL::copy_backward((iterator)_Position, oldLast - _Count, oldLast);
#endif
        fill((iterator)_Position, (iterator)_Position + _Count, copyVal);
      } else {
        _MY_STL::uninitialized_fill_n(Mylast_, _Count - posAfterNum, copyVal);
        Mylast_ += _Count - posAfterNum;
        _MY_STL::uninitialized_copy((iterator)_Position, oldLast, Mylast_);
        Mylast_ += posAfterNum;
        _MY_STL::fill((iterator)_Position, oldLast, copyVal);
      }
    } else {
        const size_type oldSize = size();
        const size_type newSize = oldSize + max((size_type)oldSize, _Count);
        iterator newFirst = data_allocator_::allocate(newSize);
        iterator newLast = newFirst;
        MINI_STL_TRY {
          newLast = _MY_STL::uninitialized_copy(Myfirst_, (iterator)_Position, newFirst);
          newLast = _MY_STL::uninitialized_fill_n(newLast, _Count, _Val);
          newLast = _MY_STL::uninitialized_copy((iterator)_Position, Mylast_, newLast);
        }
        MINI_STL_UNWIND (
          _MY_STL::destroy(newFirst, newLast);
          data_allocator_::deallocate(newFirst, newSize);)
        _MY_STL::destroy(Myfirst_, Mylast_);
        _deallocate();

        Myfirst_ = newFirst;
        Mylast_ = newLast;
        Myend_ = newFirst + newSize;
    }
  }
}

template <class Type, class Alloc>
typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::insert(const_iterator _Position, const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _Position, "vector insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->end(),  "vector insert");
  difference_type off = _Position - begin();
  _insert_aux((iterator)_Position, _Val);
  return begin() + off;
}


template <class Type, class Alloc>
template <class InputIterator>
void vector<Type, Alloc>::
insert(const_iterator _Position,
       InputIterator _First,
       InputIterator _Last,
       typename is_iterator<InputIterator>::ID)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->cbegin(), _Position, "vector insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->cend(),  "vector insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "vector insert");
  if (_First != _Last) {
    size_type n = _MY_STL::DISTANCE(_First, _Last);
    if ((size_type)(Myend_ - Mylast_) >= n) {//has space
      iterator oldLast = Mylast_;
#ifdef MINI_STL_RVALUE_REFS
      _MY_STL::move_backward((iterator)_Position, oldLast, oldLast + n);
#else
      _MY_STL::copy_backward((iterator)_Position, oldLast, oldLast + n);
#endif
      _MY_STL::copy(_First, _Last, (iterator)_Position);
      Mylast_ += n;
    } else {//no space
      const size_type oldSize = size();
      const size_type newSize = oldSize + max((size_type)oldSize, (size_type)n);
      iterator newFirst = data_allocator_::allocate(newSize);
      iterator newLast = newFirst;
      MINI_STL_TRY {
        newLast = _MY_STL::uninitialized_copy(Myfirst_, (iterator)_Position, newFirst);
        newLast = _MY_STL::uninitialized_copy(_First, _Last, newLast);
        newLast = _MY_STL::uninitialized_copy((iterator)_Position, Mylast_, newLast);
      }
      MINI_STL_UNWIND((_MY_STL::destroy(newFirst,newLast),
                          data_allocator_::deallocate(newFirst, newSize)));
      _MY_STL::destroy(Myfirst_, Mylast_);
      _deallocate();
      Myfirst_ = newFirst;
      Mylast_ = newLast;
      Myend_ = newFirst + newSize;
    }
  }
}

#ifdef MINI_STL_RVALUE_REFS
template <class Type, class Alloc>
typename vector<Type,Alloc>::iterator
vector<Type, Alloc>::insert(const_iterator _Position, Type&& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->cbegin(), _Position, "vector insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Position, this->cend(),  "vector insert");
  iterator myIter = (iterator)_Position;
  difference_type offset = myIter - Myfirst_;
  if (Mylast_ != Myend_) {
    _MY_STL::construct(Mylast_, *(Mylast_ - 1));
    ++Mylast_;
    _MY_STL::move_backward((iterator)(myIter), Mylast_ - 2, Mylast_ - 1);
    _MY_STL::construct(myIter, _MY_STL::move(_Val));
  } else {
    const size_type oldSize = size();
    const size_type newSize = oldSize != 0 ? 2 * oldSize : 1;

    iterator newFirst = data_allocator_::allocate(newSize);
    iterator newLast = newFirst;
    MINI_STL_TRY {
      newLast = _MY_STL::uninitialized_copy(Myfirst_, myIter, newFirst);
      _MY_STL::construct(newLast, _MY_STL::move(_Val));
      ++newLast;
      newLast = _MY_STL::uninitialized_copy(myIter, Mylast_, newLast);
    }
    MINI_STL_UNWIND((_MY_STL::destroy(newFirst,newLast);
                    _deallocate(newFirst,newSize);))
    destroy(Myfirst_, Mylast_);
    _deallocate();

    Myfirst_ = newFirst;
    Mylast_ = newLast;
    Myend_ = newFirst + newSize;
  }
  return Myfirst_ + offset;
}
#endif // MINI_STL_RVALUE_REFS

MINI_STL_END
#endif // MINI_STL_VECTOR_H
