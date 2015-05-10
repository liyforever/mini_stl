#ifndef MINI_STL_ALLOCATOR_H
#define MINI_STL_ALLOCATOR_H
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>
#include "global.h"
MINI_STL_BEGIN
#include <winerror.h>
template <typename T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    pointer allocate(size_type n)
    {
        set_new_handler(0);
        pointer tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
        if (tmp == 0)
        {
            std::cerr << "out of memory" << std::endl;
            exit(1);
        }
        return tmp;
    }

    void deallocate(T* buffer)
    {
        ::operator delete(buffer);
    }

    pointer address(reference x)
    {
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x)
    {
        return (const_address)&x;
    }

    size_type init_page_size()
    {

    }

    size_type max_size() const
    {

    }
};

template<>
class allocator<void>
{
    typedef void* pointer;
};
MINI_STL_END
#endif // MINI_STL_ALLOCATOR_H
