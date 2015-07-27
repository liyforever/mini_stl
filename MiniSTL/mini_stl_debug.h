#ifndef MINI_STL_DEBUG_H
#define MINI_STL_DEBUG_H
#include "mini_stl_config.h"
#include <stddef.h>
MINI_STL_BEGIN
template <class InputIterator>
void mini_stl_debug_range_of_iterator(
      InputIterator _First,
      InputIterator _Last,
      const char* _Msg
    )
{
  ptrdiff_t n = DISTANCE(_First, _Last);
  if (n < 0)
    MINI_STL_THROW_RANGE_ERROR(_Msg);
}

template <class Type>
inline void
mini_stl_debug_pointer(Type *_Ptr,
                       const char *_Msg)
{
  if (_Ptr == 0)
    MINI_STL_THROW_INVALID_ERROR(_Msg);
}

template <class Type>
inline void
mini_stl_debug_pointer(const Type *_Ptr,
                       const char *_Msg)
{
  if (_Ptr == 0)
    MINI_STL_THROW_INVALID_ERROR(_Msg);
}

template <class Iterator>
void mini_stl_debug_pointer(Iterator /*_Ptr*/, const char * /*_Msg*/)
{}

template <class Type, class Distance>
inline void
mini_stl_debug_pointer_for_n(Type *_Ptr,
                             Distance _Len,
                             const char *_Msg)
{
  if (_Len > 0)
    mini_stl_debug_pointer(_Ptr, _Msg);
}

template <class Type, class Distance>
inline void
mini_stl_debug_pointer_for_n(const Type *_Ptr,
                             Distance _Len,
                             const char *_Msg)
{
  if (_Len > 0)
    mini_stl_debug_pointer(_Ptr, _Msg);
}

template <class Iterator, class Distance>
void mini_stl_debug_pointer_for_n(Iterator /*Ptr*/, Distance /*_Len*/, const char* /*_Msg*/)
{}

template <class Type, class BinaryPredicate>
inline bool
mini_stl_debug_comp(BinaryPredicate _Comp,
                    const Type& _Left,
                    const Type& _Right,
                    const char* _Msg)
{
  if (_Comp(_Left, _Right) && _Comp(_Right, _Left))
    MINI_STL_THROW_LOGIC_ERROR(_Msg);
  return _Comp(_Left, _Right);
}

template <class Type>
inline bool
mini_stl_debug_less(const Type& _Left,
                    const Type& _Rihgt,
                    const char* _Msg)
{
  if (_Left < _Rihgt && _Rihgt < _Left)
    MINI_STL_THROW_LOGIC_ERROR(_Msg);
  return _Left < _Rihgt;
}

template <class InputIterator>
inline void
mini_stl_debug_order(InputIterator _First, InputIterator _Last,
                     const char* _Msg)
{
  if (_First == _Last)
    return;
  InputIterator next = _First;
  for (; ++next!=_Last; ++_First)
    if (*next < *_First)
      MINI_STL_THROW_LOGIC_ERROR(_Msg);
}

template <class InputIterator, class BinaryPredicate>
inline void
mini_stl_debug_order_comp(InputIterator _First, InputIterator _Last,
                          BinaryPredicate _Comp,
                          const char* _Msg)
{
  if (_First == _Last)
    return;
  InputIterator next = _First;
  for (; ++next!=_Last; ++_First)
    if (_Comp(*next, *_First))
      MINI_STL_THROW_LOGIC_ERROR(_Msg);
}

MINI_STL_END

#ifdef MINI_STL_DEBUG
  #define MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, _Msg) \
    Mini_STL::mini_stl_debug_range_of_iterator(_First, _Last, _Msg);
  #define MINI_STL_DEBUG_POINTER(_Ptr, _Msg) \
    Mini_STL::mini_stl_debug_pointer(_Ptr, _Msg)
  #define MINI_STL_DEBUG_POINTER_FOR_N(_Ptr, _Len, _Msg) \
    Mini_STL::mini_stl_debug_pointer_for_n(_Ptr, _Len, _Msg)
  #define MINI_STL_DEBUG_COMP(_Comp, _Left, _Right, _Msg) \
    Mini_STL::mini_stl_debug_comp(_Comp, _Left, _Right, _Msg)
  #define MINI_STL_DEBUG_LESS(_Left, _Right, _Msg) \
    Mini_STL::mini_stl_debug_less(_Left, _Right, _Msg)
  #define MINI_STL_DEBUG_ORDER(_First, _Last, _Msg) \
    Mini_STL::mini_stl_debug_order(_First, _Last, _Msg);
  #define MINI_STL_DEBUG_ORDER_COMP(_First, _Last, _Comp, _Msg) \
    Mini_STL::mini_stl_debug_order_comp(_First, _Last, _Comp, _Msg)
#else
  #define MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, _Msg)
  #define MINI_STL_DEBUG_POINTER(_Ptr, _Msg)
  #define MINI_STL_DEBUG_POINTER_FOR_N(_Ptr, _Len, _Msg)
  #define MINI_STL_DEBUG_COMP(_Comp, _Left, _Right, _Msg) \
    _Comp((_Left), (_Right))
  #define MINI_STL_DEBUG_LESS(_Left, _Right, _Msg) \
    ((_Left) < (_Right))
  #define MINI_STL_DEBUG_ORDER(_First, _Last, _Msg)
  #define MINI_STL_DEBUG_ORDER_COMP(_First, _Last, _Comp, _Msg)
#endif

//
#endif // MINI_STL_DEBUG_H
