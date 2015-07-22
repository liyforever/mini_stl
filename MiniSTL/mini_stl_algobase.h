#ifndef MINI_STL_ALGOBASE_H
#define MINI_STL_ALGOBASE_H
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_function.h"
#include "mini_stl_debug.h"
#include <string.h>
#include <wchar.h>
#include <iostream>
using std::cout;
using std::endl;
MINI_STL_BEGIN

template <class Type>
inline const Type& max(const Type &_Left, const Type &_Right)
{
  return _Right < _Left ? _Left : _Right;
}

template <class Type, class Compare>
inline const Type& max(const Type &_Left, const Type &_Right, Compare _Comp)
{
  return _Comp(_Right, _Left) ? _Left : _Right;
}

template <class Type>
inline const Type& min(const Type &_Left, const Type &_Right)
{
  return _Left < _Right ? _Left : _Right;
}

template <class Type, class Compare>
inline const Type& min(const Type &_Left, const Type &_Right, Compare _Comp)
{
  return _Comp(_Left, _Right) ? _Left : _Right;
}


template <class Type>
inline void
swap(Type& _Left, Type& _Right)
{
  Type tmp(_Left);
  _Left = _Right;
  _Right = tmp;
}

template<class InputIterator, class Type>
InputIterator
find(InputIterator _First,
     InputIterator _Last,
     const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "find");
  for ( ; _First!=_Last && *_First!=_Val; ++_First)
    {}
  return _First;
}

template<class InputIterator, class Type, class Predicate>
InputIterator
find(InputIterator _First,
     InputIterator _Last,
     const Type& _Val,
     Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "find_of_comp");
  for ( ; _First!=_Last && !_Pred(*_First, _Val);
        ++_First)
    {}
  return _First;
}

template <class ForwardIterator, class Type>
inline void
fill(ForwardIterator _First, ForwardIterator _Last, const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "fill");
  for(; _First != _Last; ++_First)
    *_First = _Val;
}


template <class OutputIterator, class Size, class Type>
inline OutputIterator
fill_n(OutputIterator _First, Size _Count, const Type& _Val)
{
  MINI_STL_DEBUG_POINTER(_First, "fill_n");
  for(; _Count > 0; --_Count, ++_First)
    *_First = _Val;
  return _First;
}
/////////////////////////////////////////////////////////////
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
         __true_type)
{
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}

template <class T>
inline T*
__copy_t(const T* first, const T* last, T* result,
         __false_type)
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
  return __copy_d(first, last, result, DISTANCE_TYPE(first));
}
template <class InputIter, class OutputIter>
struct __copy_dispatch
{
  OutputIter operator()(InputIter first, InputIter last,
                          OutputIter result)
  {
    return __copy(first, last, result, ITERATOR_CATEGORY(first));
  }
};

template <class T>
struct __copy_dispatch<T*, T*>
{
  T* operator ()(T* first, T* last, T* result)
  {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return __copy_t(first, last, result, t());
  }
};

template <class T>
struct __copy_dispatch<const T*, T*>
{
  T* operator ()(const T* first, const T* last, T* result)
  {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return __copy_t(first, last, result, t());
  }
};

template <class InputIterator, class OutputIterator>
inline OutputIterator
copy(InputIterator _First, InputIterator _Last, OutputIterator _Result)
{
  return __copy_dispatch<InputIterator, OutputIterator>()
            (_First, _Last, _Result);
}

inline char* copy(const char *_First, const char *_Last, char *_Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "copy");
  MINI_STL_DEBUG_POINTER(_Result, "copy");

  memmove(_Result, _First, _Last - _First);
  return _Result + (_Last - _First);
}

inline wchar_t* copy(const wchar_t *_First, const wchar_t *_Last, wchar_t *_Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "fill");
  MINI_STL_DEBUG_POINTER(_Result, "copy");

  wmemmove(_Result, _First, _Last - _First);
  return _Result + (_Last - _First);
}


/************************copy_backward__begin**************************/

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 _First,
                                        BidirectionalIter1 _Last,
                                        BidirectionalIter2 _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "copy_backward");
  MINI_STL_DEBUG_POINTER(_Result, "copy_backward");

  while (_First != _Last)
    *--_Result = *--_Last;
  return _Result;
}

template <class RandomAccessIter, class BidirectionalIter, class Distance>
inline BidirectionalIter copy_backward(RandomAccessIter _First,
                                       RandomAccessIter _Last,
                                       BidirectionalIter _Result,
                                       Distance*)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "copy_backward");
  MINI_STL_DEBUG_POINTER(_Result, "copy_backward");

  for (Distance n = _Last - _First; n > 0; --n)
    *--_Result = *--_Last;
  return _Result;
}
/************************copy_backward__end**************************/

/************************heap_begin**********************************/
template <class RandomAccessIterator, class Distance, class Type>
void _push_heap(RandomAccessIterator first, Distance holeIndex,
                Distance topIndex, Type val)
{
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex>topIndex && *(first + parent)<val) {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = val;
}

template <class RandomAccessIterator, class Distance, class Type>
inline void push_heap_aux(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Distance*, Type*)
{
  _push_heap(first, Distance((last - first) - 1), Distance(0),
             Type(*(last - 1)));
}

template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first,
                      RandomAccessIterator last)
{
  push_heap_aux(first, last, DISTANCE_TYPE(first),
                VALUE_TYPE(first));
}

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,
                     RandomAccessIterator last)
{
  pop_heap_aux(first, last, VALUE_TYPE(first));
}

template <class RandomAccessIterator, class Type>
inline void pop_heap_aux(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Type*)
{
  _pop_heap(first, last - 1, last - 1, Type(*(last-1)),
            DISTANCE_TYPE(first));
}

template <class RandomAccessIterator, class Type,
          class Distance>
inline void _pop_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      RandomAccessIterator result,
                      Type val, Distance*)
{
  *result = *first;
  _adjust_heap(first, Distance(0), Distance(last - first), val);
}

template <class RandomAccessIterator, class Distance, class Type>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex,
                  Distance len, Type val)
{
  Distance topIndex = holeIndex;
  Distance rChild = 2 * holeIndex + 2;
  while (rChild<len) {
    if (*(first+rChild) < *(first+(rChild-1)))
      --rChild;
    *(first + holeIndex) = *(first + rChild);
    holeIndex = rChild;
    rChild = 2 * (rChild + 1);
  }
  if (rChild==len) {
    *(first + holeIndex) = *(first + (rChild - 1));
    holeIndex = rChild - 1;
  }
  _push_heap(first, holeIndex, topIndex, val);
}

template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last
               )
{
  while (last-first>1)
    pop_heap(first, last--);
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last)
{
  make_heap_aux(first, last, VALUE_TYPE(first),
                DISTANCE_TYPE(first));
}

template <class RandomAccessIterator, class Type, class Distance>
void make_heap_aux(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Type*, Distance*)
{
  if (last-first<2)
    return;
  Distance len = last - first;

  Distance parent = (len-2)/2;
  for (;;)
  {
    _adjust_heap(first, parent, len, Type(*(first+parent)));
    if (parent==0)
      return;
    --parent;
  }
}


template <class RandomAccessIterator, class Distance, class Type,
          class Compare>
void _push_heap(RandomAccessIterator first, Distance holeIndex,
                Distance topIndex, Type val, Compare comp)
{
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex>topIndex && comp(*(first + parent),val)) {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = val;
}

template <class RandomAccessIterator, class Distance, class Type,
          class Compare>
inline void push_heap_aux(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Distance*, Type*,
                          Compare comp)
{
  _push_heap(first, Distance((last - first) - 1), Distance(0),
             Type(*(last - 1)), comp);
}

template <class RandomAccessIterator, class Compare>
inline void push_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      Compare comp)
{
  push_heap_aux(first, last, DISTANCE_TYPE(first),
                VALUE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first,
                     RandomAccessIterator last,
                     Compare comp)
{
  pop_heap_aux(first, last, VALUE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Type, class Compare>
inline void pop_heap_aux(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Type*, Compare comp)
{
  _pop_heap(first, last - 1, last - 1, Type(*(last-1)),
            DISTANCE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Type,
          class Distance, class Compare>
inline void _pop_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      RandomAccessIterator result,
                      Type val, Distance*,
                      Compare comp)
{
  *result = *first;
  _adjust_heap(first, Distance(0), Distance(last - first), val,
               comp);
}


template <class RandomAccessIterator, class Distance, class Type,
          class Compare>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex,
                  Distance len, Type val, Compare comp)
{
  Distance topIndex = holeIndex;
  Distance rChild = 2 * holeIndex + 2;
  while (rChild<len) {
    if (comp(*(first+rChild),*(first+(rChild-1))))
      --rChild;
    *(first + holeIndex) = *(first + rChild);
    holeIndex = rChild;
    rChild = 2 * (rChild + 1);
  }
  if (rChild==len) {
    *(first + holeIndex) = *(first + (rChild - 1));
    holeIndex = rChild - 1;
  }
  _push_heap(first, holeIndex, topIndex, val, comp);
}

template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last,
               Compare comp)
{
  while (last-first>1)
    pop_heap(first, last--, comp);
}

template <class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      Compare comp)
{
  make_heap_aux(first, last, VALUE_TYPE(first),
                DISTANCE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Type, class Distance,
          class Compare>
void make_heap_aux(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Type*, Distance*, Compare comp)
{
  if (last-first<2)
    return;
  Distance len = last - first;

  Distance parent = (len-2)/2;
  for (;;)
  {
    _adjust_heap(first, parent, len, Type(*(first+parent)),comp);
    if (parent==0)
      return;
    --parent;
  }
}


/************************heap_end**********************************/

template <class InputIterator1, class InputIterator2>
inline bool
 equal(InputIterator1 _First1, InputIterator1 _Last1,
       InputIterator2 _First2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "equal");
  MINI_STL_DEBUG_POINTER(_First2, "equal");

  for (; _First1!=_Last1; ++_First1,++_First2)
    if (*_First1 != *_First2)
      return false;
  return true;
}

template <class InputIterator1, class InputIterator2,
          class BinaryPredicate>
inline bool
 equal(InputIterator1 _First1, InputIterator1 _Last1,
       InputIterator2 _First2, BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "equal_of_comp");
  MINI_STL_DEBUG_POINTER(_First2, "equal_of_comp");

  for (; _First1!=_Last1; ++_First1,++_First2)
    if (!_Comp(*_First1, *_First2))
      return false;
  return true;
}

template <class InputIterator1, class InputIterator2>
inline bool
lexicographical_compare(InputIterator1 _First1, InputIterator1 _Last1,
                        InputIterator2 _First2, InputIterator2 _Last2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "lexicographical_compare");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "lexicographical_compare");

  for (; _First1!=_Last1 && _First2!=_Last2; ++_First1,++_First2) {
    if (*_First1<*_First2)
      return true;
    if (*_First2<*_First1)
      return false;
  }
  return _First1 == _Last1 && _First2 != _Last2;
}

template <class InputIterator1, class InputIterator2,
          class BinaryPredicate>
inline bool
lexicographical_compare(InputIterator1 _First1, InputIterator1 _Last1,
                        InputIterator2 _First2, InputIterator2 _Last2,
                        BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "lexicographical_compare_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "lexicographical_compare_of_comp");

  for (; _First1!=_Last1 && _First2!=&&_Last2; ++_First1,++_First2) {
    if (_Comp(*_First1,*_First2))
      return true;
    if (_Comp(*_First2,*_First1))
      return false;
  }
  return _First1 == _Last1 && _First2 != _Last2;
}

template <class ForwardIterator, class Type>
inline ForwardIterator
lower_bound(ForwardIterator first,
            ForwardIterator last,
            const Type& value)
{
  return _lower_bound_aux(first, last, value, DISTANCE_TYPE(first),
                          ITERATOR_CATEGORY(first));
}

template <class ForwardIterator, class Type, class Compare>
inline ForwardIterator
lower_bound(ForwardIterator first,
            ForwardIterator last,
            const Type& value,
            Compare comp)
{
  return _lower_bound_aux(first, last, value, DISTANCE_TYPE(first),
                          ITERATOR_CATEGORY(first), comp);
}

template <class ForwardIterator, class Type, class Distance>
ForwardIterator _lower_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 forward_iterator_tag)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (*midIter < value) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Type, class Distance>
ForwardIterator _lower_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 random_access_iterator_tag)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (*midIter < value) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Type, class Distance, class Compare>
ForwardIterator _lower_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 forward_iterator_tag,
                                 Compare comp)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (comp(*midIter, value)) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Type, class Distance, class Compare>
ForwardIterator _lower_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 random_access_iterator_tag,
                                 Compare comp)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (comp(*midIter, value)) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Type>
inline ForwardIterator
upper_bound(ForwardIterator first,
            ForwardIterator last,
            const Type& value)
{
  return _upper_bound_aux(first, last, value, DISTANCE_TYPE(first),
                          ITERATOR_CATEGORY(first));
}

template <class ForwardIterator, class Type, class Compare>
inline ForwardIterator
upper_bound(ForwardIterator first,
            ForwardIterator last,
            const Type& value,
            Compare comp)
{
  return _upper_bound_aux(first, last, value, DISTANCE_TYPE(first),
                          ITERATOR_CATEGORY(first), comp);
}

template <class ForwardIterator, class Type, class Distance>
ForwardIterator _upper_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 forward_iterator_tag)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (value < *midIter) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Type, class Distance>
ForwardIterator _upper_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 random_access_iterator_tag)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    midIter += half;
    if (value < *midIter) {
        cout << "if:" << *midIter << endl;
      //first = midIter;
      //++first;
      len = half;
    } else {
      cout << "else:" << *midIter << endl;
      len = len - half - 1;
      first = midIter;
    }
  }
  return first;
}

template <class ForwardIterator, class Type, class Distance, class Compare>
ForwardIterator _upper_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 forward_iterator_tag,
                                 Compare comp)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (comp(value,*midIter)) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}

template <class ForwardIterator, class Type, class Distance, class Compare>
ForwardIterator _upper_bound_aux(ForwardIterator first,
                                 ForwardIterator last,
                                 const Type& value,
                                 Distance*,
                                 random_access_iterator_tag,
                                 Compare comp)
{
  Distance len = DISTANCE(first, last);
  Distance half;
  ForwardIterator midIter;
  while (len > 0){
    half = len / 2;
    midIter = first;
    advance(midIter, half);
    if (comp(value,*midIter)) {
      first = midIter;
      ++first;
      len = len - half - 1;
    } else
      len = half;
  }
  return first;
}
MINI_STL_END
#endif // MINI_STL_ALGOBASE_H
