#ifndef MINI_STL_UNINITIALIZED_H
#define MINI_STL_UNINITIALIZED_H
#include <new>
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_construct.h"

MINI_STL_BEGIN

//内置类型
template <class InputIter, class ForwardIter>
inline ForwardIter
uninitialized_copy_aux(InputIter first, InputIter last,
                        ForwardIter result,
                        true_type)
{
    return copy(first, last, result);
}

//有构造函数类型
template <class InputIter, class ForwardIter>
ForwardIter
uninitialized_copy_aux(InputIter first, InputIter last,
                         ForwardIter result,
                         false_type)
{
    ForwardIter cur = result;
    for( ; first != last; ++first, ++cur)
        construct(&*cur, *first);
    return cur;
}


template <class InputIter, class ForwardIter, class Type>
inline ForwardIter
uninitialized_copy(InputIter first, InputIter last,
                     ForwardIter result, Type*)
{
    typedef typename type_traits<Type>::is_identity_type Is_POD;
    return uninitialized_copy_aux(first, last, result, Is_POD());
}


template <class InputIter, class ForwardIter>
inline ForwardIter
  uninitialized_copy(InputIter first, InputIter last,
                     ForwardIter result)
{
    return uninitialized_copy(first, last, result,
                              value_type(result));
}

//特化版本
inline char* uninitialized_copy(const char* first, const char* last,
                                char* result) {
    memmove(result, first, last - first);
    return result + (last - first);
}

inline wchar_t*
uninitialized_copy(const wchar_t* first, const wchar_t* last,
                   wchar_t* result)
{
  memmove(result, first, sizeof(wchar_t) * (last - first));
  return result + (last - first);
}

template <class ForwardIter, class T>
inline void
__uninitialized_fill_aux(ForwardIter first, ForwardIter last,
                         const T& x, true_type)
{
    fill(first, last, x);
}

template <class ForwardIter, class T>
void
__uninitialized_fill_aux(ForwardIter first, ForwardIter last,
                         const T& x, false_type)
{
    ForwardIter cur = first;
    for ( ; cur != last; ++cur)
        construct(&*cur, x);
}

template <class ForwardIter, class T, class T1>
inline void __uninitialized_fill(ForwardIter first,
                                 ForwardIter last, const T& x, T1*)
{
    typedef typename type_traits<T1>::is_identity_type Is_POD;
    __uninitialized_fill_aux(first, last, x, Is_POD());
}

template <class ForwardIter, class T>
inline void uninitialized_fill(ForwardIter first,
                               ForwardIter last,
                               const T& x)
{
    __uninitialized_fill(first, last, x, value_type(first));
}


template <class ForwardIter, class Size, class T>
inline ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n,
                           const T& x, true_type)
{
  return fill_n(first, n, x);
}

template <class ForwardIter, class Size, class T>
ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n,
                           const T& x, false_type)
{
    ForwardIter cur = first;
    for ( ; n > 0; --n, ++cur)
        construct(&*cur, x);
    return cur;
}

template <class ForwardIter, class Size, class T, class T1>
inline ForwardIter
__uninitialized_fill_n(ForwardIter first, Size n, const T& x, T1*)
{
    typedef typename type_traits<T1>::is_identity_type Is_POD;
    return __uninitialized_fill_n_aux(first, n, x, Is_POD());
}

template <class ForwardIter, class Size, class T>
inline ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
  return __uninitialized_fill_n(first, n, x, value_type(first));
}


template <class InputIter1, class InputIter2, class ForwardIter>
inline ForwardIter
__uninitialized_copy_copy(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          ForwardIter result)
{
    ForwardIter mid = uninitialized_copy(first1, last1, result);
    return uninitialized_copy(first2, last2, mid);

}


template <class ForwardIter, class T, class InputIter>
inline ForwardIter
__uninitialized_fill_copy(ForwardIter result, ForwardIter mid,
                          const T& x,
                          InputIter first, InputIter last)
{
    uninitialized_fill(result, mid, x);
    return uninitialized_copy(first, last, mid);
}


template <class InputIter, class ForwardIter, class T>
inline void
__uninitialized_copy_fill(InputIter first1, InputIter last1,
                          ForwardIter first2, ForwardIter last2,
                          const T& x)
{
    ForwardIter mid2 = uninitialized_copy(first1, last1, first2);
    uninitialized_fill(mid2, last2, x);
}
MINI_STL_END
#endif // MINI_STL_UNINITIALIZED_H
