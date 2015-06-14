#ifndef MINI_STL_VECTOR_H
#define MINI_STL_VECTOR_H
#include "mini_stl_iterator.h"
#include "memory.h"
#ifdef MINI_STL_DEBUG
#include <iostream>
using std::endl;
using std::cerr;
#endif
MINI_STL_BEGIN

template <class Type, class Alloc = default_allocator>
class vector
{
public:
  typedef Type              value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type*       iterator;
  typedef const value_type* const_iterator;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef Alloc             allocator_type;
protected:
  typedef simpleAlloc<value_type, Alloc> data_allocator_;
  iterator first_;
  iterator last_;
  iterator end_;
public:
  explicit vector(const allocator_type&/*Al*/=allocator_type()) :
    first_(0), last_(0), end_(0) {}

  explicit vector(size_type count)
  {
    _fill_init(count, Type());
  }

  vector(int count, const value_type& val,
         const allocator_type&/*Al*/=allocator_type()
    )
  {
    _fill_init(count, val);
  }

  vector(long count, const value_type& val,
         const allocator_type&/*Al*/=allocator_type()
    )
  {
    _fill_init(count, val);
  }

  vector(size_type count, const value_type& val,
         const allocator_type&/*Al*/=allocator_type())
  {
    _fill_init(count, val);
  }

  vector(const vector& rightVec)
  {
    first_ = data_allocator_::allocate(rightVec.size());
    last_ = uninitialized_copy(rightVec.first_, rightVec.end_, first_);
    end_ = last_;
  }

#ifdef MINI_STL_MEMBER_TEMPLATES
  template <class InputIterator>
    vector(InputIterator first,
           InputIterator last,
           const allocator_type&/*Al*/=allocator_type(),
           typename is_iterator<InputIterator>::ID = Identity()
    )
  {
    difference_type size = last - first;
    first_ = data_allocator_::allocate(size);
    last_ = uninitialized_copy(first, last, first_);
    end_ = last_;
  }
#endif //MINI_STL_MEMBER_TEMPLATES

#ifdef MINI_STL_HAS_MOVE
  vector(vector&& rightVec)
  {
    this->first_ = rightVec.first_;
    this->last_ = rightVec.last_;
    this->end_ = rightVec.end_;
    rightVec.first_ = nullptr;
    rightVec.last_ = nullptr;
    rightVec.end_ = nullptr;
  }

  vector& operator=(vector&& rightVec)
  {
    destroy(begin(), end());
    _deallocate();
    this->first_ = rightVec.first_;
    this->last_ = rightVec.last_;
    this->end_ = rightVec.end_;
    rightVec.first_ = nullptr;
    rightVec.last_ = nullptr;
    rightVec.end_ = nullptr;
    return *this;
  }

#endif //MINI_STL_HAS_MOVE

  vector& operator=(const vector& rightVec)
  {
    if (this!=&rightVec) {
      destroy(begin(), end());
      if(size() >= rightVec.size()) {
        last_ = uninitialized_copy(rightVec.first_, rightVec.last_,begin());
      } else {
        _deallocate();
        first_ = data_allocator_::allocate(rightVec.size());
        last_ = uninitialized_copy(rightVec.first_, rightVec.last_,begin());
        end_ = last_;
      }
    }
    return *this;
  }

  ~vector()
  {
    destroy(first_, last_);
    _deallocate();
  }

public:
  iterator begin()
  {
    return first_;
  }

  const_iterator begin() const
  {
    return first_;
  }

  iterator end()
  {
    return last_;
  }

  const_iterator end() const
  {
    return last_;
  }

  const_iterator cbegin() const
  {
    return (const_iterator)(first_);
  }

  const_iterator cend() const
  {
    return (const_iterator)(last_);
  }

  pointer data()
  {
    return first_;
  }

  const_pointer data() const
  {
    return first_;
  }

  reference front()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *first_;
  }

  const_reference front() const
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *first_;
  }

  reference back()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *(last_ - 1);
  }

  const_reference back() const
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    return *(last_ - 1);
  }

  size_type size() const
  {
    return static_cast<size_type>(last_ - first_);
  }

  size_type capacity() const
  {
    return static_cast<size_type>(end_ - first_);
  }

  bool empty() const
  {
    return first_ == last_;
  }

  reference operator[](size_type pos)
  {
#ifdef MINI_STL_DEBUG
    _check_range(pos);
#endif
    return *(first_ + pos);
  }

  const_reference operator[](size_type pos) const
  {
#ifdef MINI_STL_DEBUG
    _check_range(pos);
#endif
    return *(first_ + pos);
  }

  reference at(size_type pos)
  {
#ifdef MINI_STL_DEBUG
    _check_range(pos);
#endif
    return first_[pos];
  }

  const_reference at(size_type pos) const
  {
#ifdef MINI_STL_DEBUG
    _check_range(pos);
#endif
    return first_[pos];
  }

  void swap(vector<Type, Alloc> &rightVec);

  friend void swap(vector &leftVec, vector &rightVec)
  {
    leftVec.swap(rightVec);
  }

  void push_back(const Type& val);

  void pop_back()
  {
#ifdef MINI_STL_DEBUG
    _check_range();
#endif
    --last_;
    destroy(last_);
  }

  iterator erase(iterator first, iterator last);
  iterator erase(iterator position);

  void clear()
  {
    erase(first_, last_);
  }

  void reserve(size_type count)
  {
#ifdef MINI_STL_DEBUG
    _check_range(count,true);
#endif
    first_ = data_allocator_::allocate(count);
    last_ = first_;
    end_ = first_ + count;
  }

  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  void insert(const_iterator position, size_type n, const Type& val);
  iterator insert(const_iterator position, const Type& val);
#ifdef MINI_STL_HAS_MOVE
  iterator insert(const_iterator position, Type&& val);
#endif // MINI_STL_HAS_MOVE

#ifdef MINI_STL_MEMBER_TEMPLATES
  template<class InputIterator>
  void insert(const_iterator position,
              InputIterator first,
              InputIterator last,
              typename is_iterator<InputIterator>::ID = Identity()
            );
  template<class InputIterator>
  void assign(InputIterator first,
              InputIterator last,
              typename is_iterator<InputIterator>::ID = Identity()
            )
  {
#ifdef MINI_STL_DEBUG
    _check_range(first, last);
#endif
    clear();
    insert(begin(), first, last);
  }
#endif // MINI_STL_MEMBER_TEMPLATES*/
  void assign(size_type n, const Type& val)
  {
#ifdef MINI_STL_DEBUG
    _check_range(n);
#endif
    clear();
    insert(begin(), n, val);
  }

  void resize(size_type newSize)
  {
    resize(newSize, Type());
  }

  void resize(size_type newSize, const Type& val);

  size_type max_size() const
  {
    return size_type(-1);
  }

  bool operator == (const vector& rhsVec)
  {
    if (size() != rhsVec.size())
      return false;
    iterator lhsIter = this->first_;
    iterator rhsIter = (iterator)(rhsVec.begin());
    for ( ; lhsIter!=this->last_; ++lhsIter,++rhsIter)
    {
      if (*lhsIter != *rhsIter)
        return false;
    }
    return true;
  }

  bool operator != (const vector& rhsVec)
  {
    if (size() != rhsVec.size())
      return true;
    return !(*this == rhsVec);
  }

  bool operator > (const vector& rhsVec)
  {
    iterator lhsIter = this->first_;
    iterator rhsIter = (iterator)(rhsVec.begin());
    for ( ; lhsIter!=this->last_ &&
          rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
    {
      if (*rhsIter < *lhsIter)
        return true;
      else
        return false;
    }
  }

  bool operator <= (const vector& rhsVec)
  {
    iterator lhsIter = this->first_;
    iterator rhsIter = (iterator)(rhsVec.begin());
    for ( ; lhsIter!=this->last_ &&
      rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
    {
      if (*lhsIter < *rhsIter)
        return true;
      else if (*rhsIter < *lhsIter)
        return false;
    }
    return size() <= rhsVec.size() ? true : false;
  }

  bool operator < (const vector& rhsVec)
  {
    iterator lhsIter = this->first_;
    iterator rhsIter = (iterator)(rhsVec.begin());
    for ( ; lhsIter!=this->last_ &&
      rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
    {
      if(*lhsIter < *rhsIter)
        return true;
      else if (*rhsIter < *lhsIter)
        return false;
    }
  }

  bool operator >= (const vector& rhsVec)
  {
    iterator lhsIter = this->first_;
    iterator rhsIter = (iterator)(rhsVec.begin());
    for ( ; lhsIter!=this->last_ &&
          rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
    {
      if (*rhsIter < *lhsIter)
        return true;
      else if (*lhsIter < *rhsIter)
        return false;
    }
    return size() >= rhsVec.size() ? true : false;
  }
protected:
  void _fill_init(size_type count, const Type& value);
  iterator _allocator_and_fill(size_type n, const Type& val);
  void _deallocate()
  {
    if (first_)
     data_allocator_::deallocate(first_, end_ - first_);
  }

  void _insert_aux(iterator position, const Type& val);
  void _insert_aux(iterator position);

#ifdef MINI_STL_DEBUG
  void _check_range(size_t pos)
  {
    if (pos<0 || pos>=size()) {
      cerr << "vector:pos<0 || pos>=size()" << endl;
      MINI_STL_THROW_RANGE_ERROR("vector");
    }
  }

  void _check_range(size_t n, bool)
  {
    if (n<0 || n>=max_size()) {
      cerr << "vector:n<0" << endl;
      MINI_STL_THROW_RANGE_ERROR("vector");
    }
  }

  void _check_range(const_iterator position)
  {
    if (position>last_ ||
        position<first_) {
      cerr << "vector:postion>=end() || position < begin()" << endl;
      MINI_STL_THROW_RANGE_ERROR("vector");
    }
  }

  template <class InputIterator>
  void _check_range(InputIterator first,
                   InputIterator last)
  {
    difference_type n = DISTANCE(first, last);
    if (n<0) {
      cerr << "vector:InputIterator last - first < 0" << endl;
      MINI_STL_THROW_RANGE_ERROR("vector");
    }
  }

  void _check_range()
  {
    if (empty()) {
      cerr << "vector:is empty" << endl;
      MINI_STL_THROW_RANGE_ERROR("vector");
    }
  }
#endif //MINI_STL_DEBUG
};




template <class Type, class Alloc>
inline typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::_allocator_and_fill(size_type n, const Type& val)
{
  iterator result = data_allocator_::allocate(n);
  uninitialized_fill_n(result, n, val);
  return result;
}

template <class Type, class Alloc>
void vector<Type, Alloc>::_fill_init(size_type count, const Type& value)
{
  first_ = _allocator_and_fill(count, value);
  last_ = first_ + count;
  end_ = last_;
}

template <class Type, class Alloc>
void vector<Type, Alloc>::_insert_aux(iterator position, const Type& val)
{
  if (last_ != end_) {
    construct(last_, *(last_ - 1));
    ++last_;
    copy_backward(position, last_ - 2, last_ - 1);
    *position = val;
  } else {
    const size_type oldSize = size();
    const size_type newSize = oldSize != 0 ? 2 * oldSize : 1;

    iterator newFirst = data_allocator_::allocate(newSize);
    iterator newLast = newFirst;
    MINI_STL_TRY {
      newLast = uninitialized_copy(first_, position, newFirst);
      construct(newLast, val);
      ++newLast;
      newLast = uninitialized_copy(position, last_, newLast);
    }
    MINI_STL_UNWIND((destroy(newFirst,newLast);
                    _deallocate(newFirst,newSize);))
    destroy(first_, last_);
    _deallocate();

    first_ = newFirst;
    last_ = newLast;
    end_ = newFirst + newSize;
  }
}

template <class Type, class Alloc>
void vector<Type, Alloc>::_insert_aux(iterator position)
{
  _insert_aux(position, Type());
}

template <class Type, class Alloc>
void vector<Type, Alloc>::swap(vector<Type, Alloc> &rightVec)
{
  if (this == &rightVec)
    ;
  else {
    iterator tempIter = first_;
    first_ = rightVec.first_;
    rightVec.first_ = tempIter;

    tempIter = last_;
    last_ = rightVec.last_;
    rightVec.last_ = tempIter;

    tempIter = end_;
    end_ = rightVec.end_;
    rightVec.end_ = tempIter;
  }
}

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::push_back(const Type& val)
{
  if (last_ != end_) {
    construct(last_, val);
    ++last_;
  } else {
    _insert_aux(last_, val);
  }
}

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::resize(size_type newSize, const Type &val)
{
#ifdef MINI_STL_DEBUG
  _check_range(newSize,true);
#endif
  if (newSize < size())
    erase(begin() + newSize, end());
  else
    insert(end(), newSize - size(), val);
}

template <class Type, class Alloc>
typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::erase(iterator first, iterator last)
{
#ifdef MINI_STL_DEBUG
  _check_range(first,last);
  _check_range(first);
  _check_range(last);
#endif
  iterator i = copy(last, last_, first);
  destroy(i, last_);
  last_ -= (last - first);
  return first;
}

template <class Type, class Alloc>
typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::erase(iterator position)
{
#ifdef MINI_STL_DEBUG
  _check_range(position - first_);
#endif
  if (position + 1 != last_) {
    copy(position + 1, last_, position);
  }
  --last_;
  destroy(last_);
  return position;
}

template <class Type, class Alloc>
inline void
vector<Type, Alloc>::insert(const_iterator position, size_type n, const Type& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
  _check_range(n,true);
#endif
  if (n!=0) {
    if (size_type(end_ - last_) >= n) {
      Type copyVal(val);
      const size_type posAfterNum = last_ - position;
      iterator oldLast = last_;
      if (posAfterNum > n) {
        uninitialized_copy(last_ - n, last_, last_);
        last_ += n;
        copy_backward((iterator)position, oldLast -n, oldLast);
        fill((iterator)position, (iterator)position + n, copyVal);
      } else {
        uninitialized_fill_n(last_, n - posAfterNum, copyVal);
        last_ += n - posAfterNum;
        uninitialized_copy((iterator)position, oldLast, last_);
        last_ += posAfterNum;
        fill((iterator)position, oldLast, copyVal);
      }
    } else {
        const size_type oldSize = size();
        const size_type newSize = oldSize + max((size_type)oldSize, n);

        iterator newFirst = data_allocator_::allocate(newSize);
        iterator newLast = newFirst;
        MINI_STL_TRY {
          newLast = uninitialized_copy(first_, (iterator)position, newFirst);
          newLast = uninitialized_fill_n(newLast, n, val);
          newLast = uninitialized_copy((iterator)position, last_, newLast);
        }
        MINI_STL_UNWIND (
          destroy(newFirst, newLast);
          data_allocator_::deallocate(newFirst, newSize);)
        destroy(first_, last_);
        _deallocate();

        first_ = newFirst;
        last_ = newLast;
        end_ = newFirst + newSize;
    }
  }
}

template <class Type, class Alloc>
typename vector<Type, Alloc>::iterator
vector<Type, Alloc>::insert(const_iterator position, const Type& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
#endif
  difference_type off = position - begin();
  _insert_aux((iterator)position, val);
  return begin() + off;
}

#ifdef MINI_STL_MEMBER_TEMPLATES

template <class Type, class Alloc>
template <class InputIterator>
void vector<Type, Alloc>::
insert(const_iterator position,
       InputIterator first,
       InputIterator last,
       typename is_iterator<InputIterator>::ID
      )
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
  _check_range(first, last);
#endif
  if (first != last) {//范围不空
    size_type n = last - first;
    if ((size_type)(end_ - last_) >= n) {//还有空间
      iterator oldLast = last_;
      copy_backward((iterator)position, oldLast, oldLast + n);
      copy(first, last, (iterator)position);
      last_ += n;
    } else {//空间不够
      const size_type oldSize = size();
      const size_type newSize = oldSize + max((size_type)oldSize, (size_type)n);
      iterator newFirst = data_allocator_::allocate(newSize);
      iterator newLast = newFirst;
      MINI_STL_TRY {
        newLast = uninitialized_copy(first_, (iterator)position, newFirst);
        newLast = uninitialized_copy(first, last, newLast);
        newLast = uninitialized_copy((iterator)position, last_, newLast);
      }
      MINI_STL_UNWIND((destroy(newFirst,newLast),
                          data_allocator_::deallocate(newFirst, newSize)));
      destroy(first_, last_);
      _deallocate();
      first_ = newFirst;
      last_ = newLast;
      end_ = newFirst + newSize;
    }
  }
}

#endif //MINI_STL_MEMBER_TEMPLATES

#ifdef MINI_STL_HAS_MOVE
template <class Type, class Alloc>
typename vector<Type,Alloc>::iterator
vector<Type, Alloc>::insert(const_iterator position, Type&& val)
{
#ifdef MINI_STL_DEBUG
  _check_range(position);
#endif
  iterator myIter = (iterator)position;
  difference_type offset = myIter - first_;
  if (last_ != end_) {
    construct(last_, *(last_ - 1));
    ++last_;
    copy_backward((iterator)(myIter), last_ - 2, last_ - 1);
    *myIter = val;
  } else {
    const size_type oldSize = size();
    const size_type newSize = oldSize != 0 ? 2 * oldSize : 1;

    iterator newFirst = data_allocator_::allocate(newSize);
    iterator newLast = newFirst;
    MINI_STL_TRY {
      newLast = uninitialized_copy(first_, myIter, newFirst);
      *newLast = val;
      ++newLast;
      newLast = uninitialized_copy(myIter, last_, newLast);
    }
    MINI_STL_UNWIND((destroy(newFirst,newLast);
                    _deallocate(newFirst,newSize);))
    destroy(first_, last_);
    _deallocate();

    first_ = newFirst;
    last_ = newLast;
    end_ = newFirst + newSize;
  }
  return first_ + offset;
}

#endif // MINI_STL_HAS_MOVE

MINI_STL_END
#endif // MINI_STL_VECTOR_H
