#ifndef MINI_STL_CONSTRUCT_H
#define MINI_STL_CONSTRUCT_H

#include "global.h"
#include "mini_stl_type_traits.h"

MINI_STL_BEGIN
template <typename T1, typename T2>
inline void construct(T1* p, const T2& value)
{
    new (p)T1(value);
}
//true_type的destroy版本调用析构函数
template <typename T>
inline void destroy(T* pointer)
{
    pointer->~T();
}

//一对迭代器版本BEGIN
//true_type的一对迭代器的destroy版本调用析构函数
template <typename ForwardIterator>
inline void _destroy_aux(ForwardIterator first, ForwardIterator last, false_type)
{
    for(; first!=last; ++first)
        destroy(&*first);
}
//内置类型,空函数
template <typename ForwardIterator>
inline void _destroy_aux(ForwardIterator , ForwardIterator , true_type) {}
//判断迭代器型别
template <typename ForwardIterator, typename T>
inline void _destroy(ForwardIterator first, ForwardIterator last, T*)
{
    typedef typename type_traits<T>::has_trivial_destructor trivial_destructor;
    _destroy_aux(first, last, trivial_destructor());
}

//上层接口的析构函数,一对迭代器版本
template <typename ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last)
{
    _destroy(first, last, value_type(first));
}

inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}
//一对迭代器版本END
MINI_STL_END
#endif // MINI_STL_CONSTRUCT_H
