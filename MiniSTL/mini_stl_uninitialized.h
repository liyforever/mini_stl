#ifndef MINI_STL_UNINITIALIZED_H
#define MINI_STL_UNINITIALIZED_H
#include <new.h>
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_construct.h"
#include "mini_std_pair.h"
MINI_STL_BEGIN

/***************************uninitalized_copy___begin***********************************/
//内置类型
template <class InputIter, class ForwardIter>
inline ForwardIter
uninitialized_copy_aux(InputIter first, InputIter last,
                        ForwardIter result,
                        __true_type)
{
    return copy(first, last, result);
}

//有构造函数类型
template <class InputIter, class ForwardIter>
ForwardIter
uninitialized_copy_aux(InputIter first, InputIter last,
                         ForwardIter result,
                         __false_type)
{
    ForwardIter cur = result;
    MINI_STL_TRY {
    for( ; first != last; ++first, ++cur)
        construct(&*cur, *first);
    return cur;
    }
    //吃掉异常,再抛出
    MINI_STL_UNWIND(destroy(result, cur));
}

//萃取型别
template <class InputIter, class ForwardIter, class Type>
inline ForwardIter
_uninitialized_copy(InputIter first, InputIter last,
                     ForwardIter result, Type)
{
    typedef typename _type_traits<Type>::is_POD_type Is_POD;
    return uninitialized_copy_aux(first, last, result, Is_POD());
}


template <class InputIter, class ForwardIter>
inline ForwardIter
uninitialized_copy(InputIter first, InputIter last,
                     ForwardIter result)
{
    return _uninitialized_copy(first, last, result,
                              VALUE_TYPE(result));
}

//特化版本
inline char* uninitialized_copy(const char* first, const char* last,
                                char* result)
{
    memmove(result, first, last - first);
    return result + (last - first);
}


#ifdef MINI_STL_HAS_WCHAR_T
inline wchar_t*
uninitialized_copy(const wchar_t* first, const wchar_t* last,
                   wchar_t* result)
{
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last - first);
}
#endif // MINI_STL_HAS_WCHAR_T
/***************************uninitalized_copy___end***********************************/




/***************************uninitalized_copy_n__begin***********************************/
template <class InputIter, class Size, class ForwardIter>
pair<InputIter, ForwardIter>
__uninitialized_copy_n(InputIter first, Size count,
                       ForwardIter result,
                       input_iterator_tag)
{
    ForwardIter cur = result;
    MINI_STL_TRY {
        for ( ; __count > 0 ; --count, ++first, ++cur)
            construct(&*cur, *first);
        return pair<InputIter, ForwardIter>(first, cur);
    }
    //吃掉异常,再抛出
    MINI_STL_UNWIND(destroy(result, cur));
}

template <class RandomAccessIter, class Size, class ForwardIter>
inline pair<RandomAccessIter, ForwardIter>
__uninitialized_copy_n(RandomAccessIter first, Size count,
                       ForwardIter result,
                       random_access_iterator_tag)
{
    RandomAccessIter last = first + count;
    return pair<RandomAccessIter, ForwardIter>(
                 last,
                 uninitialized_copy(first, last, result));
}

template <class InputIter, class Size, class ForwardIter>
inline pair<InputIter, ForwardIter>
__uninitialized_copy_n(InputIter first, Size count,
                     ForwardIter result) {
  return __uninitialized_copy_n(first, count, result,
                                ITERATOR_CATEGORY(first));
}

template <class InputIter, class Size, class ForwardIter>
inline pair<InputIter, ForwardIter>
uninitialized_copy_n(InputIter first, Size count,
                     ForwardIter result) {
  return __uninitialized_copy_n(first, count, result,
                                ITERATOR_CATEGORY(first));
}
//uninitialized_copy_n__end



template <class ForwardIter, class T>
inline void
__uninitialized_fill_aux(ForwardIter first, ForwardIter last,
                         const T& x, __true_type)
{
    fill(first, last, x);
}


template <class ForwardIter, class T>
void
__uninitialized_fill_aux(ForwardIter first, ForwardIter last,
                         const T& x, __false_type)
{
    ForwardIter cur = first;
    MINI_STL_TRY {
    for ( ; cur != last; ++cur)
        construct(&*cur, x);
    }
    //吃掉,再抛出异常
    MINI_STL_UNWIND(_Destroy(__first, __cur));
}

template <class ForwardIter, class T, class T1>
inline void __uninitialized_fill(ForwardIter first,
                                 ForwardIter last, const T& x, T1*)
{
    typedef typename _type_traits<T1>::is_POD_type Is_POD;
    __uninitialized_fill_aux(first, last, x, Is_POD());

}

template <class ForwardIter, class T>
inline void uninitialized_fill(ForwardIter first,
                               ForwardIter last,
                               const T& x)
{
    __uninitialized_fill(first, last, x, VALUE_TYPE(first));
}



template <class ForwardIter, class Size, class T>
inline ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n,
                           const T& x, __true_type)
{
  return fill_n(first, n, x);
}

template <class ForwardIter, class Size, class T>
ForwardIter
__uninitialized_fill_n_aux(ForwardIter first, Size n,
                           const T& x, __false_type)
{
    ForwardIter cur = first;
    MINI_STL_TRY {
    for ( ; n > 0; --n, ++cur)
        construct(&*cur, x);
    return cur;
    }
    //吃掉,再抛出
    MINI_STL_UNWIND(destroy(first, cur));
}

template <class ForwardIter, class Size, class T, class T1>
inline ForwardIter
__uninitialized_fill_n(ForwardIter first, Size n, const T& x, T1)
{
    typedef typename _type_traits<T1>::is_POD_type Is_POD;
    return __uninitialized_fill_n_aux(first, n, x, Is_POD());
}

template <class ForwardIter, class Size, class T>
inline ForwardIter
uninitialized_fill_n(ForwardIter first, Size n, const T& x)
{
    return __uninitialized_fill_n(first, n, x, VALUE_TYPE(first));
}

template <class InputIter1, class InputIter2, class ForwardIter>
inline ForwardIter
__uninitialized_copy_copy(InputIter1 first1, InputIter1 last1,
                          InputIter2 first2, InputIter2 last2,
                          ForwardIter result)
{
    ForwardIter mid = uninitialized_copy(first1, last1, result);
    MINI_STL_TRY {
        return uninitialized_copy(first2, last2, mid);
    }
    //吃掉,再抛出
    MINI_STL_UNWIND(destroy(result, mid));
}


template <class ForwardIter, class T, class InputIter>
inline ForwardIter
__uninitialized_fill_copy(ForwardIter result, ForwardIter mid,
                          const T& x,
                          InputIter first, InputIter last)
{
    uninitialized_fill(result, mid, x);
    MINI_STL_TRY {
        return uninitialized_copy(first, last, mid);
    }
    //吃掉,再抛出
    MINI_STL_UNWIND(destroy(result, mid));
}


template <class InputIter, class ForwardIter, class T>
inline void
__uninitialized_copy_fill(InputIter first1, InputIter last1,
                          ForwardIter first2, ForwardIter last2,
                          const T& x)
{
    ForwardIter mid2 = uninitialized_copy(first1, last1, first2);
    MINI_STL_TRY {
        uninitialized_fill(mid2, last2, x);
    }
    //吃掉,再抛出
    MINI_STL_UNWIND(destroy(first2, mid2));
}

MINI_STL_END
#endif // MINI_STL_UNINITIALIZED_H
