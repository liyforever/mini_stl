#ifndef MINI_STL_CONSTRUCT_H
#define MINI_STL_CONSTRUCT_H
#include "mini_stl_config.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_iterator.h"
#include <new.h>

MINI_STL_BEGIN

template <class Type1, class Type2>
inline void construct(Type1* _Ptr, const Type2& _Val)
{
  ::new ((void*)_Ptr) Type1(_Val);
}

template <class Type>
inline void construct(Type* _Ptr)
{
  ::new ((void*)_Ptr) Type();
}

#ifdef MINI_STL_RVALUE_REFS
template <class Type1, class Type2>
inline void construct(Type1* _Ptr, Type2&& _Val)
{
  ::new ((void*)_Ptr) Type1(_MY_STL::move(_Val));
}
#endif

template <class Type>
inline void destroy(Type* _Ptr)
{
  _Ptr->~Type();
}


template <class ForwardIterator>
inline void
_destroy_aux(ForwardIterator _First,
             ForwardIterator _Last,
             __false_type)
{
  for (; _First != _Last; ++_First)
    destroy(&*_First);
}

template <class ForwardIterator>
inline void
_destroy_aux(ForwardIterator,
             ForwardIterator,
             __true_type)
{}


template <class ForwardIterator, class Type>
inline void
_destroy(ForwardIterator _First,
         ForwardIterator _Last,
         Type*)
{
  typedef typename _type_traits<Type>::has_trivial_destructor
      trivial_destructor;
  _destroy_aux(_First, _Last,
               trivial_destructor());
}


template <class ForwardIterator>
inline void
destroy(ForwardIterator _First,
        ForwardIterator _Last)
{
  _destroy(_First, _Last, VALUE_TYPE(_First));
}

inline void destroy(int*, int*) {}
inline void destroy(char*, char*) {}
inline void destroy(long*, long*) {}
inline void destroy(float*, float*) {}
inline void destroy(double*, double*) {}
#ifdef MINI_STL_HAS_WCHAR_T
inline void destroy(wchar_t*, wchar_t*) {}
#endif // MINI_STL_HAS_WCHAR_T

MINI_STL_END
#endif // MINI_STL_CONSTRUCT_H
