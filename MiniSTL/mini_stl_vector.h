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
    void deallocate();

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

    //void swap(vector<value_type,allocator> rightVec);
    size_type size() const
    {
        return m_last - m_first;
    }
};

MINI_STL_END
#endif // VECTOR_H
