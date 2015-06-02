#ifndef VECTOR_H
#define VECTOR_H
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_allocator.h"
#include "memory.h"
MINI_STL_BEGIN

template <class T, class Alloc = default_allocator>
class vector
{
public:
    typedef T           value_type;
    typedef value_type* pointer;
    typedef value_type* iterator;
    typedef value_type& reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
protected:
    typedef simpleAlloc<value_type, Alloc> data_allocator;
    iterator m_first;
    iterator m_last;
    iterator m_end;

    void insert_aux(iterator position, const T& x);
    void deallocate()
    {
        if(m_first)
            data_allocator::deallocate(m_first, m_end - m_first);
    }

    void fill_initialize(size_type n, const value_type& value)
    {
        m_first = data_allocator::allocate(n);
        uninitialized_fill_n(m_first, n, value);
        m_last = m_first + n;
        m_end = m_last;
    }

public:
//构造函数
    explicit vector() : m_first(0), m_last(0), m_end(0){}
    explicit vector(size_type count)
    {
        fill_initialize(count, T());
    }
    vector(int count, const value_type& val)
    {
        fill_initialize(count, val);
    }
    vector(long count, const value_type& val)
    {
        fill_initialize(count, val);
    }
    vector(size_type count, const value_type& val)
    {
        fill_initialize(count, val);
    }

    vector(const vector& rightVec)
    {
        m_first = data_allocator::allocate(rightVec.size());
        m_last = uninitialized_copy(rightVec.m_first, rightVec.m_end, this->m_first);
        m_end = m_last;
     }

    template <class InuputIter>
    vector(InuputIter first,
           InuputIter last);

#ifdef C_PLUS_PLUS_11
    vector(vector&& rightVec)
    {
        this->m_first = rightVec.m_first;
        this->m_last = rightVec.m_last;
        this->m_end = rightVec.m_end;
    }
#endif

    void swap(vector<T, Alloc> &rightVec);
    friend void swap(vector &leftVec, vector &rightVec)
    {
        leftVec.swap(rightVec);
    }

    size_type size() const
    {
        return static_cast<size_type>(m_last - m_first);
    }

    iterator begin() { return m_first; }
    iterator end() { return m_last; }
    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);
    void resize(size_type newSize, const T &x);
    void resize(size_type newSize);
};

template <class T, class Alloc>
void vector<T, Alloc>::swap(vector<T, Alloc> &rightVec)
{
    if(this == &rightVec)
        ;
    else {
        iterator tempIter = m_first;
        m_first = rightVec.m_first;
        rightVec.m_first = tempIter;

        tempIter = m_last;
        m_last = rightVec.m_last;
        rightVec.m_last = tempIter;

        tempIter = m_end;
        m_end = rightVec.m_end;
        rightVec.m_end = tempIter;
    }
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator position)
{
    //如果迭代器位置超过end 那么结果未定义
    if (position + 1 != m_end)
        copy(position + 1, m_last, position);
    --m_last;
    destroy(m_last);
    return position;
}

template <class T, class Alloc>
typename vector<T, Alloc>::iterator
vector<T, Alloc>::erase(iterator first, iterator last)
{
    iterator i = copy(last, m_last, first);
    destroy(i, m_last);
    m_last = m_last - (last - first);
    return first;
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type newSize, const T &x)
{
    if(newSize < size())
        erase(m_first + newSize, m_last);
    else
        insert(m_last, newSize - size(), x);
}

template <class T, class Alloc>
void vector<T, Alloc>::resize(size_type newSize)
{
    resize(newSize, T());
}

MINI_STL_END
#endif // VECTOR_H
