#ifndef VECTOR_H
#define VECTOR_H
#include <assert.h>
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "memory.h"
#include <iostream>
using std::cout;
using std::endl;
MINI_STL_BEGIN

template <class T, class Alloc = default_allocator>
class vector
{
public:
    typedef T                 value_type;
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
public:
    typedef simpleAlloc<value_type, Alloc> _M_data_allocator;
    iterator _M_first;
    iterator _M_last;
    iterator _M_end;
protected://aux_function
    void _M_fill_initialize(size_type count, const T &value);
    iterator _M_allocator_and_fill(size_type n, const T& val);
    void _M_deallocate()
    {
        if(_M_first)
        _M_data_allocator::deallocate(_M_first, _M_end - _M_first);
    }
    void _M_insert_aux(iterator position, const T& val);
    void _M_insert_aux(iterator position);
public://constructor and destory
    explicit vector() : _M_first(0), _M_last(0), _M_end(0){}
    explicit vector(size_type count)
    {
        _M_fill_initialize(count, T());
    }
    vector(int count, const value_type& val)
    {
        _M_fill_initialize(count, val);
    }
    vector(long count, const value_type& val)
    {
        _M_fill_initialize(count, val);
    }
    vector(size_type count, const value_type& val)
    {
        _M_fill_initialize(count, val);
    }
    vector(const vector& rightVec)
    {
        _M_first = _M_data_allocator::allocate(rightVec.size());
        _M_last = uninitialized_copy(rightVec._M_first, rightVec._M_end, _M_first);
        _M_end = _M_last;
    }
    template <class InuputIterator>
        vector(InuputIterator first,
               InuputIterator last)
    {
        difference_type size = last - first;
        _M_first = _M_data_allocator::allocate(size);
        _M_last = uninitialized_copy(first, last, _M_first);
        _M_end = _M_last;
    }
    vector& operator=(const vector& rightVec)
    {
        if(this != &rightVec)
        {
            destroy(begin(), end());
            if(size() >= rightVec.size()) {
                _M_last = uninitialized_copy(rightVec._M_first, rightVec._M_last,begin());
            }
            else{
                _M_deallocate();
                _M_first = _M_data_allocator::allocate(rightVec.size());
                _M_last = uninitialized_copy(rightVec._M_first, rightVec._M_last,begin());
                _M_end = _M_last;
            }
        }
        return *this;
    }


#ifdef MINI_STL_HAS_MOVE
    vector(vector&& rightVec)
    {
        this->_M_first = rightVec._M_first;
        this->_M_last = rightVec._M_last;
        this->_M_end = rightVec._M_end;
        rightVec._M_first = nullptr;
        rightVec._M_last = nullptr;
        rightVec._M_end = nullptr;
    }
    vector& operator=(vector&& rightVec)
    {
        destroy(begin(), end());
        _M_deallocate();
        this->_M_first = rightVec._M_first;
        this->_M_last = rightVec._M_last;
        this->_M_end = rightVec._M_end;
        rightVec._M_first = nullptr;
        rightVec._M_last = nullptr;
        rightVec._M_end = nullptr;
        return *this;
    }

#endif //MINI_STL_HAS_MOVE
    ~vector()
    {
        destroy(_M_first, _M_last);
        _M_deallocate();
    }
public:
    iterator begin() { return _M_first; }
    const_iterator begin() const { return _M_first; }

    iterator end() { return _M_last; }
    const_iterator end() const { return _M_last; }

    const_iterator cbegin() const { return (const_iterator)(_M_first); }
    const_iterator cend() const { return (const_iterator)(_M_last); }

    const_pointer data() const { return _M_first; }
    pointer data() { return _M_first; }

    size_type size() const { return static_cast<size_type>(_M_last - _M_first); }

    reference front() { return *_M_first; }
    const_reference front() const { return *_M_first; }

    reference back() { return *(_M_last - 1); }
    const_reference back() const { return *(_M_last - 1); }

    size_type capacity() const
    { return static_cast<size_type>(_M_end - _M_first); }
    bool empty() const
    { return _M_first == _M_last; }
    reference operator[](size_type pos) { return *(_M_first + pos); }
    const_reference operator[](size_type pos) const { return *(_M_first + pos); }

    reference at(size_type pos)
    {
        if(size()<=pos)
            throw std::out_of_range("invalid vector<T> at(size_type pos)");
        return _M_first[pos];
    }

    const_reference at(size_type pos) const
    {
        if(size()<=pos)
            throw std::out_of_range("invalid vector<T> at(size_type pos)");
        return _M_first[pos];
    }
    void swap(vector<T, Alloc> &rightVec);
    friend void swap(vector &leftVec, vector &rightVec)
    {
        leftVec.swap(rightVec);
    }
    void push_back(const T& val);
    void pop_back()
    {
        --_M_last;
        destroy(_M_last);
    }
    iterator erase(iterator first, iterator last);
    iterator erase(iterator position);

    void clear() { erase(_M_first, _M_last); }

    void reserve(size_type count)
    {
        _M_first = _M_data_allocator::allocate(count);
        _M_last = _M_first;
        _M_end = _M_first + count;
    }
    allocator_type get_allocator() const { return allocator_type(); }
    void insert(const_iterator position, size_type n, const T& val);
    iterator insert(const_iterator position, const T& val);
#ifdef MINI_STL_HAS_MOVE
    iterator insert(const_iterator postion, T&& val);
#endif // MINI_STL_HAS_MOVE

    void assign(size_type n, const T& val)
    {
        clear();
        insert(begin(), n, val);
    }
#define MINI_STL_MEMBER_TEMPLATES
#ifdef MINI_STL_MEMBER_TEMPLATES
    template<class InputIterator>
       void assign(InputIterator first,
          InputIterator last)
    {
        clear();
        insert(begin(), first, last);
    }

#endif //MINI_STL_MEMBER_TEMPLATES

#ifdef MINI_STL_MEMBER_TEMPLATES

template<class InputIterator>
    void insert(const_iterator position,
              InputIterator first,
              InputIterator last
                );

#endif // MINI_STL_MEMBER_TEMPLATES*/
    void resize(size_type newSize) { resize(newSize, T()); }
    void resize(size_type newSize, const T &val);
    size_type max_size() const
    { return size_type(-1) / sizeof(T); }

// vector cmp
    bool operator == (const vector& rhsVec)
    {
        if(size() != rhsVec.size())
            return false;
        iterator lhsIter = this->_M_first;
        iterator rhsIter = (iterator)(rhsVec.begin());
        for( ; lhsIter!=this->_M_last; ++lhsIter,++rhsIter)
        {
            if(*lhsIter != *rhsIter)
                return false;
        }
        return true;
    }
    bool operator != (const vector& rhsVec)
    {
        if(size() != rhsVec.size())
            return true;
        return !(*this == rhsVec);
    }

    bool operator > (const vector& rhsVec)
    {
        iterator lhsIter = this->_M_first;
        iterator rhsIter = (iterator)(rhsVec.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
        {
            if(*rhsIter < *lhsIter)
                return true;
            else
                return false;
        }
    }

    bool operator <= (const vector& rhsVec)
    {
        iterator lhsIter = this->_M_first;
        iterator rhsIter = (iterator)(rhsVec.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
        {
            if(*lhsIter < *rhsIter)
                return true;
            else if(*rhsIter < *lhsIter)
                return false;
        }
        return size() <= rhsVec.size() ? true : false;
    }

    bool operator < (const vector& rhsVec)
    {
        iterator lhsIter = this->_M_first;
        iterator rhsIter = (iterator)(rhsVec.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
        {
            if(*lhsIter < *rhsIter)
                return true;
            else if(*rhsIter < *lhsIter)
                return false;
        }
    }

    bool operator >= (const vector& rhsVec)
    {
        iterator lhsIter = this->_M_first;
        iterator rhsIter = (iterator)(rhsVec.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsVec.end(); ++lhsIter,++rhsIter)
        {
            if(*rhsIter < *lhsIter)
                return true;
            else if(*lhsIter < *rhsIter)
                return false;
        }
        return size() >= rhsVec.size() ? true : false;
    }
};


/************************protected aux_function__begin**************************/
template <class T, class Alloc>
inline typename vector<T, Alloc>::iterator
vector<T, Alloc>::_M_allocator_and_fill(size_type n, const T &val)
{
    iterator result = _M_data_allocator::allocate(n);
    uninitialized_fill_n(result, n, val);
    return result;
}

template <class T, class Alloc>
void vector<T, Alloc>::_M_fill_initialize(size_type count, const T& value)
{
    _M_first = _M_allocator_and_fill(count, value);
    _M_last = _M_first + count;
     _M_end = _M_last;
}

template <class T, class Alloc>
void vector<T, Alloc>::_M_insert_aux(iterator position, const T& val)
{
    if (_M_last != _M_end) {
        //copy last elem
        construct(_M_last, *(_M_last - 1));
        ++_M_last;
        copy_backward(position, _M_last - 2, _M_last - 1);
        *position = val;
    }
    else {
        const size_type oldSize = size();
        const size_type newSize = oldSize != 0 ? 2 * oldSize : 1;

        iterator newFirst = _M_data_allocator::allocate(newSize);
        iterator newLast = newFirst;
        MINI_STL_TRY {
            //copy old [first,position) to [newFirst,newFirst + (position - oldFirst) )
            newLast = uninitialized_copy(_M_first, position, newFirst);
            construct(newLast, val);
            ++newLast;
            newLast = uninitialized_copy(position, _M_last, newLast);
        }
        //rollback
        MINI_STL_UNWIND(
                    (destroy(newFirst,newLast);
                    _M_deallocate(newFirst,newSize);))
        destroy(_M_first, _M_last);
        _M_deallocate();

        _M_first = newFirst;
        _M_last = newLast;
        _M_end = newFirst + newSize;
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::_M_insert_aux(iterator position)
{
    _M_insert_aux(position, T());
}

/************************protected aux_function__end**************************/


/************************public function__begin******************************/
template <class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc> &rightVec)
{
    if(this == &rightVec)
        ;
    else {
        iterator tempIter = _M_first;
        _M_first = rightVec._M_first;
        rightVec._M_first = tempIter;

        tempIter = _M_last;
        _M_last = rightVec._M_last;
        rightVec._M_last = tempIter;

        tempIter = _M_end;
        _M_end = rightVec._M_end;
        rightVec._M_end = tempIter;
    }
}

template <class T, class Alloc>
inline void
vector<T, Alloc>::push_back(const T& val)
{
    if(_M_last != _M_end) {
        construct(_M_last, val);
        ++_M_last;
    } else {
        _M_insert_aux(_M_last, val);
    }
}

template <class T, class Alloc>
inline void
vector<T, Alloc>::resize(size_type newSize, const T &val)
{
    if(newSize < size())
        erase(begin() + newSize, end());
    else
        insert(end(), newSize - size(), val);
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator first, iterator last)
{
    iterator i = copy(last, _M_last, first);
    destroy(i, _M_last);
    _M_last -= (last - first);
    return first;
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator position)
{
    if(position + 1 != _M_last) {
        copy(position + 1, _M_last, position);
    }
    --_M_last;
    destroy(_M_last);
    return position;
}

template <class T, class Alloc>
inline void
vector<T, Alloc>::insert(const_iterator position, size_type n, const T& val)
{
#ifdef MINI_STL_DEBUG
    assert(n > 0||"vector::insert");
#endif //MINI_STL_DEBUG
    if(n!=0) {
        if(size_type(_M_end - _M_last) >= n) {
            T copyVal(val);
            const size_type posAfterNum = _M_last - position;
            iterator oldLast = _M_last;
            if(posAfterNum > n) {
                uninitialized_copy(_M_last - n, _M_last, _M_last);
                _M_last += n;
                copy_backward((iterator)position, oldLast -n, oldLast);
                fill((iterator)position, (iterator)position + n, copyVal);
            }
            else {
                uninitialized_fill_n(_M_last, n - posAfterNum, copyVal);
                _M_last += n - posAfterNum;
                uninitialized_copy((iterator)position, oldLast, _M_last);
                _M_last += posAfterNum;
                fill((iterator)position, oldLast, copyVal);
            }
        }
        else {
            const size_type oldSize = size();
            const size_type newSize = oldSize + max((size_type)oldSize, n);

            iterator newFirst = _M_data_allocator::allocate(newSize);
            iterator newLast = newFirst;
            MINI_STL_TRY {
                newLast = uninitialized_copy(_M_first, (iterator)position, newFirst);
                newLast = uninitialized_fill_n(newLast, n, val);
                newLast = uninitialized_copy((iterator)position, _M_last, newLast);
            }
            MINI_STL_UNWIND (
                     destroy(newFirst, newLast);
                    _M_data_allocator::deallocate(newFirst, newSize);)
            destroy(_M_first, _M_last);
            _M_deallocate();

            _M_first = newFirst;
            _M_last = newLast;
            _M_end = newFirst + newSize;
        }
    }
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::insert(const_iterator position, const T& val)
{
    difference_type off = position - begin();
    _M_insert_aux((iterator)position, val);
    return begin() + off;
}

#ifdef MINI_STL_MEMBER_TEMPLATES

template <class T, class Alloc>
template <class InputIterator>
void vector<T, Alloc>::
insert(const_iterator position,
       InputIterator first,
       InputIterator last)
{
    if (first != last) {//范围不空
        size_type n = last - first;
        if ((size_type)(_M_end - _M_last) >= n) {//还有空间
            iterator oldLast = _M_last;
            copy_backward((iterator)position, oldLast, oldLast + n);
            copy(first, last, (iterator)position);
            _M_last += n;
        }
        else {//空间不够
            const size_type oldSize = size();
            const size_type newSize = oldSize + max((size_type)oldSize, (size_type)n);
            iterator newFirst = _M_data_allocator::allocate(newSize);
            iterator newLast = newFirst;
            MINI_STL_TRY {
                newLast = uninitialized_copy(_M_first, (iterator)position, newFirst);
                //copy oldSpace[begin,pos)->newSpace[newFirst,newFist + (pos-begin))
                newLast = uninitialized_copy(first, last, newLast);
                //copy [first,last) ->
                newLast = uninitialized_copy((iterator)position, _M_last, newLast);
         }
         //rollback 再抛出异常
         MINI_STL_UNWIND((destroy(newFirst,newLast),
                          _M_data_allocator::deallocate(newFirst, newSize)));
         destroy(_M_first, _M_last);
         _M_deallocate();
         _M_first = newFirst;
         _M_last = newLast;
         _M_end = newFirst + newSize;
       }
     }
}

#endif //MINI_STL_MEMBER_TEMPLATES

#ifdef MINI_STL_HAS_MOVE
//iterator insert(const_iterator postion, T&& val);
#endif // MINI_STL_HAS_MOVE

/************************public function__end********************************/

MINI_STL_END
#endif // VECTOR_H
