#ifndef MINI_STL_UNINITIALIZED_H
#define MINI_STL_UNINITIALIZED_H
#include <new>
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_construct.h"
#include "mini_stl_pair.h"

MINI_STL_BEGIN


template<class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy_aux(InputIterator _First, InputIterator _Last,
                       ForwardIterator _Result,
                        __true_type)
{
  return _MY_STL::copy(_First, _Last, _Result);
}


template<class InputIterator, class ForwardIterator>
ForwardIterator
uninitialized_copy_aux(InputIterator _First, InputIterator _Last,
                       ForwardIterator _Result,
                       __false_type)
{
  ForwardIterator cur = _Result;
  MINI_STL_TRY {
    for( ; _First != _Last; ++_First, ++cur)
       _MY_STL::construct(&*cur, *_First);
    return cur;
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_Result, cur));
}


template<class InputIterator, class ForwardIterator, class Type>
inline ForwardIterator
_uninitialized_copy(InputIterator _First, InputIterator _Last,
                    ForwardIterator _Result,
                    Type*)
{
  typedef typename _type_traits<Type>::is_POD_type Is_POD;
  return uninitialized_copy_aux(_First, _Last, _Result, Is_POD());
}


template<class InputIterator, class ForwardIterator>
inline ForwardIterator
uninitialized_copy(InputIterator _First, InputIterator _Last,
                   ForwardIterator _Result)
{
  return _uninitialized_copy(_First, _Last, _Result,
                             VALUE_TYPE(_Result));
}

inline char*
uninitialized_copy(const char* _First, const char* _Last,
                   char* _Result)
{
  memmove(_Result, _First, sizeof(char)*(_Last - _First));
  return _Result + (_Last - _First);
}


#ifdef MINI_STL_HAS_WCHAR_T
inline wchar_t*
uninitialized_copy(const wchar_t* _First, const wchar_t* _Last,
                   wchar_t* _Result)
{
  memmove(_Result, _First, sizeof(wchar_t) * (_Last - _First));
  return _Result + (_Last - _First);
}
#endif // MINI_STL_HAS_WCHAR_T


template<class InputIterator, class Size, class ForwardIterator>
inline _MY_STL::pair<InputIterator, ForwardIterator>
__uninitialized_copy_n(InputIterator _First,
                       Size _Count,
                       ForwardIterator _Result,
                       input_iterator_tag)
{
  ForwardIterator cur = _Result;
  MINI_STL_TRY {
    for ( ; _Count > 0 ; --_Count, ++_First, ++cur)
      _MY_STL::construct(&*cur, *_First);
    return pair<InputIterator, ForwardIterator>(_First, cur);
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_Result, cur));
}

template <class RandomAccessIterator, class Size, class ForwardIterator>
inline _MY_STL::pair<RandomAccessIterator, ForwardIterator>
__uninitialized_copy_n(RandomAccessIterator _First,
                       Size _Count,
                       ForwardIterator _Result,
                       random_access_iterator_tag)
{
  RandomAccessIterator last = _First + _Count;
  return pair<RandomAccessIterator, ForwardIterator>
      (last,
      uninitialized_copy(_First, last, _Result));
}

template <class InputIterator, class Size, class ForwardIterator>
inline pair<InputIterator, ForwardIterator>
__uninitialized_copy_n(InputIterator _First,
                       Size _Count,
                       ForwardIterator _Result) {
  return __uninitialized_copy_n(_First, _Count, _Result,
                                ITERATOR_CATEGORY(_First));
}

template <class InputIterator, class Size, class ForwardIterator>
inline _MY_STL::pair<InputIterator, ForwardIterator>
uninitialized_copy_n(InputIterator _First,
                     Size _Count,
                     ForwardIterator _Result) {
  return __uninitialized_copy_n(_First, _Count, _Result,
                                ITERATOR_CATEGORY(_First));
}


template <class ForwardIterator, class Type>
inline void
__uninitialized_fill_aux(ForwardIterator _First,
                         ForwardIterator _Last,
                         const Type& _Val,
                         __true_type)
{
  _MY_STL::fill(_First, _Last, _Val);
}


template <class ForwardIterator, class Type>
void
__uninitialized_fill_aux(ForwardIterator _First,
                         ForwardIterator _Last,
                         const Type& _Val,
                         __false_type)
{
  ForwardIterator cur = _First;
  MINI_STL_TRY {
    for ( ; cur != _Last; ++cur)
      _MY_STL::construct(&*cur, _Val);
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_First, cur));
}

template <class ForwardIterator, class Type, class Type1>
inline void
__uninitialized_fill(ForwardIterator _First,
                     ForwardIterator _Last,
                     const Type& _Val,
                     Type1*)
{
  typedef typename _type_traits<Type1>::is_POD_type Is_POD;
   __uninitialized_fill_aux(_First, _Last, _Val, Is_POD());
}

template <class ForwardIterator, class Type>
inline void
uninitialized_fill(ForwardIterator _First,
                   ForwardIterator _Last,
                   const Type& _Val)
{
  __uninitialized_fill(_First, _Last,
                       _Val,
                       VALUE_TYPE(_First));
}



template <class ForwardIterator, class Size, class Type>
inline ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator _First,
                           Size _Count,
                           const Type& _Val,
                           __true_type)
{
  return fill_n(_First, _Count, _Val);
}

template <class ForwardIterator, class Size, class Type>
ForwardIterator
__uninitialized_fill_n_aux(ForwardIterator _First,
                           Size _Count,
                           const Type& _Val,
                           __false_type)
{
  ForwardIterator cur = _First;
  MINI_STL_TRY {
    for ( ; _Count > 0; --_Count, ++cur)
      _MY_STL::construct(&*cur, _Val);
    return cur;
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_First, cur));
}

template <class ForwardIterator, class Size, class Type, class Type1>
inline ForwardIterator
__uninitialized_fill_n(ForwardIterator _First,
                       Size _Count,
                       const Type& _Val,
                       Type1*)
{
  typedef typename _type_traits<Type1>::is_POD_type Is_POD;
  return __uninitialized_fill_n_aux(_First, _Count, _Val, Is_POD());
}

template <class ForwardIterator, class Size, class Type>
inline ForwardIterator
uninitialized_fill_n(ForwardIterator _First,
                     Size _Count,
                     const Type& _Val)
{
  return __uninitialized_fill_n(_First, _Count,
                                _Val,
                                VALUE_TYPE(_First));
}

template <class InputIterator1, class InputIterator2, class ForwardIterator>
inline ForwardIterator
__uninitialized_copy_copy(InputIterator1 _First1, InputIterator1 _Last1,
                          InputIterator2 _First2, InputIterator2 _Last2,
                          ForwardIterator _Result)
{
  ForwardIterator mid = uninitialized_copy(_First1, _Last1, _Result);
  MINI_STL_TRY {
    return uninitialized_copy(_First2, _Last2, mid);
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_Result, mid));
}


template <class ForwardIterator, class Type, class InputIterator>
inline ForwardIterator
__uninitialized_fill_copy(ForwardIterator _Result,
                          ForwardIterator _Middle,
                          const Type& _Val,
                          InputIterator _First,
                          InputIterator _Last)
{
  uninitialized_fill(_Result, _Middle, _Val);
  MINI_STL_TRY {
    return uninitialized_copy(_First, _Last, _Middle);
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_Result, _Middle));
}


template <class InputIterator, class ForwardIterator, class Type>
inline void
__uninitialized_copy_fill(InputIterator _First1, InputIterator _Last1,
                          ForwardIterator _First2, ForwardIterator _Last2,
                          const Type& _Val)
{
  ForwardIterator mid2 = uninitialized_copy(_First1, _Last1, _First2);
  MINI_STL_TRY {
    uninitialized_fill(mid2, _Last2, _Val);
  }
  MINI_STL_UNWIND(_MY_STL::destroy(_First2, mid2));
}

MINI_STL_END
#endif // MINI_STL_UNINITIALIZED_H
