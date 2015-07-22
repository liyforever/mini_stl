#ifndef MINI_STL_DEBUG_H
#define MINI_STL_DEBUG_H
#include "memory.h"
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
void mini_stl_debug_pointer(Type *_Ptr, const char *_Msg)
{
  if (_Ptr == 0)
    MINI_STL_THROW_INVALID_ERROR(_Msg);
}

template <class Type>
void mini_stl_debug_pointer(const Type *_Ptr, const char *_Msg)
{
  if (_Ptr == 0)
    MINI_STL_THROW_INVALID_ERROR(_Msg);
}

template <class Iterator>
void mini_stl_debug_pointer(Iterator /*_Ptr*/, const char */*_Msg*/)
{}

#ifdef MINI_STL_DEBUG
  #define MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, _Msg) \
    mini_stl_debug_range_of_iterator(_First, _Last, _Msg);
  #define MINI_STL_DEBUG_POINTER(_Ptr, _Msg) \
    mini_stl_debug_pointer(_Ptr, _Msg)
#else
  #define MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, _Msg)
  #define MINI_STL_DEBUG_POINTER(_Ptr, _Msg)
#endif
MINI_STL_END
#endif // MINI_STL_DEBUG_H
