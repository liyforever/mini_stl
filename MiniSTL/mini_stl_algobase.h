#ifndef MINI_STL_ALGOBASE_H
#define MINI_STL_ALGOBASE_H
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"
#include <string.h>
#include <wchar.h>
MINI_STL_BEGIN
template <class ForwardIter, class T>
inline void
fill(ForwardIter first, ForwardIter last, const T& value)
{
    for( ; first!=last; ++first)
        *first = value;
}

template <class OutputIter, class Size, class T>
inline OutputIter
fill_n(OutputIter first, Size n, const T& value)
{
    for( ; n>0; --n, ++first)
        *first = value;
    return first;
}

template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter
__copy_d(RandomAccessIter first, RandomAccessIter last,
         OutputIter result, Distance*)
{
    for(Distance n = last - first; n>0; --n, ++result, ++first)
        *result = *first;
    return result;
}

template <class T>
inline T*
__copy_t(const T* first, const T* last, T* result,
         true_type)
{
    memmove(result, first, sizeof(T) * (last - first));
    return result + (last - first);
}

template <class T>
inline T*
__copy_t(const T* first, const T* last, T* result,
         false_type)
{
    return __copy_d(first, last, result, (ptrdiff_t*)0);
}
//InputIter 版本
template <class InputIter, class OutputIter>
inline OutputIter
__copy(InputIter first, InputIter last,
       OutputIter result, input_iterator_tag)
{
    for( ; first!=last; ++result, ++first)
        *result = *first;
    return result;
}

//RandomAccessIter 版本
template <class RandomAccessIter, class OutputIter>
inline OutputIter
__copy(RandomAccessIter first, RandomAccessIter last,
       OutputIter result, random_access_iterator_tag)
{
    return __copy_d(first, last, result, distance_type(first));
}
template <class InputIter, class OutputIter>
struct __copy_dispatch
{
    OutputIter operator()(InputIter first, InputIter last,
                          OutputIter result)
    {
        return __copy(first, last, result, iterator_category(first));
    }
};

template <class T>
struct __copy_dispatch<T*, T*>
{
    T* operator ()(T* first, T* last, T* result)
    {
        typedef typename type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template <class T>
struct __copy_dispatch<const T*, T*>
{
    T* operator ()(const T* first, const T* last, T* result)
    {
        typedef typename type_traits<T>::has_trivial_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};
//copy对外接口
template <class InputIter, class OutputIter>
inline OutputIter
copy(InputIter first, InputIter last, OutputIter result)
{
    return __copy_dispatch<InputIter, OutputIter>()
            (first, last, result);
}
//char*特化版本
inline char* copy(const char* first, const char* last, char *result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}
//wchar_t*特化版
inline wchar_t* copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
    wmemmove(result, first, last - first);
    return result + (last - first);
}

MINI_STL_END
#endif // MINI_STL_ALGOBASE_H
