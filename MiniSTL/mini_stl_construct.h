#ifndef MINI_STL_CONSTRUCT_H
#define MINI_STL_CONSTRUCT_H

#include "mini_stl_config.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_iterator.h"
#include <new.h>
MINI_STL_BEGIN
template <class T1, class T2>
inline void construct(T1* p, const T2& value)
{
  new (p)T1(value);
}

template <class T>
inline void construct(T* p)
{
  new (p) T();
}

//true_type的destroy版本调用析构函数
template <class T>
inline void destroy(T* pointer)
{
  pointer->~T();
}

//一对迭代器版本
//false_type的一对迭代器的destroy版本调用析构函数
template <class ForwardIter>
inline void _destroy_aux(ForwardIter first, ForwardIter last, __false_type)
{
  for (; first!=last; ++first)
    destroy(&*first);
}
//内置类型,空函数
template <class ForwardIter>
inline void _destroy_aux(ForwardIter , ForwardIter , __true_type) {}

//判断迭代器型别
template <class ForwardIter, class T>
inline void _destroy(ForwardIter first, ForwardIter last, T*)
{
  typedef typename _type_traits<T>::has_trivial_destructor trivial_destructor;
  _destroy_aux(first, last, trivial_destructor());
}

//上层接口的析构函数,一对迭代器版本
template <class ForwardIter>
inline void destroy(ForwardIter first, ForwardIter last)
{
  _destroy(first, last, VALUE_TYPE(first));
}

inline void destroy(char*, char*) {}
inline void destroy(int*, int*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}
#ifdef MINI_STL_HAS_WCHAR_T
inline void destroy(wchar_t*, wchar_t*) {}
#endif // MINI_STL_HAS_WCHAR_T


MINI_STL_END
#endif // MINI_STL_CONSTRUCT_H
