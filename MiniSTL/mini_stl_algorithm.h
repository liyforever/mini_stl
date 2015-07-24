#ifndef MINI_STL_ALGORITHM_H
#define MINI_STL_ALGORITHM_H
#include "mini_stl_algobase.h"
#include "mini_stl_debug.h"
MINI_STL_BEGIN

template<class ForwardIterator>
inline ForwardIterator
adjacent_find(ForwardIterator _First,
              ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "adjacent_find");

  if (_First == _Last)
    return _First;
  ForwardIterator next = _First;
  while (++next != _Last) {
    if (*_First == *next)
      return _First;
    ++_First;
  }
  return _Last;
}

template<class ForwardIterator , class BinaryPredicate>
inline ForwardIterator
adjacent_find(ForwardIterator _First,
              ForwardIterator _Last,
              BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "adjacent_find_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "adjacent_find_of_comp");
  if (_First == _Last)
    return _First;
  ForwardIterator next = _First;
  while (++next != _Last) {
    if (_Comp(*_First, *next))
      return _First;
    ++_First;
  }
  return _Last;
}

template<class InputIterator, class Predicate>
inline bool
all_of(InputIterator _First,
       InputIterator _Last,
       Predicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "all_of");
  MINI_STL_DEBUG_POINTER(_Comp, "all_of");
  for (; _First != _Last; ++_First)
    if (!_Comp(*_First))
      return false;
  return true;
}

template<class InputIterator, class UnaryPredicate>
inline bool
any_of(InputIterator _First,
       InputIterator _Last,
       UnaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "any_of");
  MINI_STL_DEBUG_POINTER(_Comp, "any_of");
  for (; _First != _Last; ++_First)
    if (_Comp(*_First))
      return true;
  return false;
}

template<class ForwardIterator, class Type>
bool binary_search(ForwardIterator _First,
                   ForwardIterator _Last,
                   const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "binary_search");

  _First = _MY_STL::lower_bound(_First, _Last, _Val);
  return (_First != _Last && !(Val < *_First));
}

template<class ForwardIterator, class Type, class BinaryPredicate>
bool binary_search(ForwardIterator _First,ForwardIterator _Last,
                   const Type& _Val,
                   BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "binary_search_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "binary_search_comp");
  _First = _MY_STL::lower_bound(_First, _Last, _Val, _Comp);
  return (_First != _Last && !(_Comp(_Val, *_First)));
}

template<class InputIterator, class OutputIterator, class Predicate>
inline OutputIterator
copy_if(InputIterator _First, InputIterator _Last,
        OutputIterator _Dest,
        Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "copy_if");
  MINI_STL_DEBUG_POINTER_FOR_N(_Dest, DISTANCE(_First, _Last), "copy_if");
  MINI_STL_DEBUG_POINTER(_Pred, "copy_if");
  for (; _First != _Last; ++_First)
    if (_Pred(*_First))
      *_Dest++ = *_First;
  return _Dest;
}

template<class InputIterator, class Type>
inline typename _MY_STL::iterator_traits<InputIterator>::difference_type
count(InputIterator _First,InputIterator _Last,
      const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "count");

  typename _MY_STL::iterator_traits<InputIterator>::difference_type count = 0;
  for (; _First != _Last; ++_First)
    if (*_First == _Val)
      ++count;
  return count;
}

template<class InputIterator, class Predicate>
inline typename _MY_STL::iterator_traits<InputIterator>::difference_type
count_if(InputIterator _First,InputIterator _Last,
         Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "count_if");
  MINI_STL_DEBUG_POINTER(_Pred, "count_if");
  typename _MY_STL::iterator_traits<InputIterator>::difference_type count = 0;
  for (; _First != _Last; ++_First)
    if (_Pred(*_First, _Val))
      ++count;
  return count;
}

template<class ForwardIterator, class Type, class Distance>
   _MY_STL::pair<ForwardIterator, ForwardIterator>
   _equal_range_aux(ForwardIterator _First,
                    ForwardIterator _Last,
                    const Type& _Val,
                    Distance*,
                    forward_iterator_tag
   )
{
  Distance len = DISTANCE(_First, _Last);
  Distance half;
  ForwardIterator middle;
  while (len > 0) {
    middle = _First;
    half = len / 2;
    advance(middle, half);
    if (*middle < _Val) {
      len = len - half - 1;
      _First = middle;
      ++_First;
    } else if (_Val < *middle) {
      len = half;
      _Last = middle;
    } else {
      ForwardIterator result1 = lower_bound(_First, middle, _Val);
      ForwardIterator result2 = upper_bound(++middle, _Last, _Val);
      return _MY_STL::pair<ForwardIterator,ForwardIterator>
             (result1, result2);
    }
  }
     return _MY_STL::pair<ForwardIterator,ForwardIterator>
                   (_First, _First);
}

template<class RandomAccessIterator, class Type, class Distance>
   _MY_STL::pair<RandomAccessIterator, RandomAccessIterator>
   _equal_range_aux(RandomAccessIterator _First,
                    RandomAccessIterator _Last,
                    const Type& _Val,
                    Distance*,
                    random_access_iterator_tag
   )
{
  Distance len = _Last - _First;
  Distance half;
  RandomAccessIterator middle;
  while (len > 0) {
    middle = _First;
    half = len / 2;
    middle += half;
    if (*middle < _Val) {
      len = len - half - 1;
      _First = middle + 1;
    } else if (_Val < *middle) {
      len = half;
      _Last = middle;
    } else {
      RandomAccessIterator result1 = lower_bound(_First, middle, _Val);
      RandomAccessIterator result2 = upper_bound(++middle, _Last, _Val);
      return _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
          (result1, result2);
    }
  }
  return _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
                (_First, _First);
}

template<class ForwardIterator, class Type, class Distance, class Compare>
   _MY_STL::pair<ForwardIterator, ForwardIterator>
   _equal_range_aux(ForwardIterator _First,
                    ForwardIterator _Last,
                    const Type& _Val,
                    Distance*,
                    forward_iterator_tag,
                    Compare _Comp
   )
{
  Distance len = DISTANCE(_First, _Last);
  Distance half;
  ForwardIterator middle;
  while (len > 0) {
    middle = _First;
    half = len / 2;
    advance(middle, half);
    if (_Comp(*middle, _Val)) {
      len = len - half - 1;
      _First = middle;
      ++_First;
    } else if (_Comp(_Val, *middle)) {
      len = half;
      _Last = middle;
    } else {
      ForwardIterator result1 = lower_bound(_First, middle, _Val);
      ForwardIterator result2 = upper_bound(++middle, _Last, _Val);
      return _MY_STL::pair<ForwardIterator,ForwardIterator>
                (result1, result2);
    }
  }
  return _MY_STL::pair<ForwardIterator,ForwardIterator>
                      (_First, _First);
}

template<class RandomAccessIterator, class Type, class Distance, class Compare>
   _MY_STL::pair<RandomAccessIterator, RandomAccessIterator>
   _equal_range_aux(RandomAccessIterator _First,
                    RandomAccessIterator _Last,
                    const Type& _Val,
                    Distance*,
                    random_access_iterator_tag,
                    Compare _Comp
   )
{
  Distance len = _Last - _First;
  Distance half;
  RandomAccessIterator middle;
  while (len > 0) {
    middle = _First;
    half = len / 2;
    middle += half;
    if (_Comp(*middle, _Val)) {
      len = len - half - 1;
      _First = middle + 1;
    } else if (_Comp(_Val, *middle)) {
      len = half;
      _Last = middle;
    } else {
      RandomAccessIterator result1 = lower_bound(_First, middle, _Val);
      RandomAccessIterator result2 = upper_bound(++middle, _Last, _Val);
      return _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
             (result1, result2);
    }
  }
  return _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
                   (_First, _First);
}

template<class ForwardIterator, class Type>
inline _MY_STL::pair<ForwardIterator, ForwardIterator>
   equal_range(ForwardIterator _First,
               ForwardIterator _Last,
               const Type& _Val
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "equal_range");

  return _equal_range_aux(_First, _Last, _Val,
                          DISTANCE_TYPE(_First),
                          ITERATOR_CATEGORY(_First));
}

template<class ForwardIterator, class Type, class Compare>
inline _MY_STL::pair<ForwardIterator, ForwardIterator>
   equal_range(ForwardIterator _First,
               ForwardIterator _Last,
               const Type& _Val,
               Compare _Comp
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "equal_range_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "equal_range_of_comp");
  return _equal_range_aux(_First, _Last, _Val,
                          DISTANCE_TYPE(_First),
                          ITERATOR_CATEGORY(_First),
                          _Comp);
}

template<class InputIterator, class Type>
inline InputIterator
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
inline InputIterator
find(InputIterator _First, InputIterator _Last,
     const Type& _Val, Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "find_of_comp");
  MINI_STL_DEBUG_POINTER(_Pred, "find_of_comp");
  for ( ; _First!=_Last && !_Pred(*_First, _Val);
         ++_First)
    {}
  return _First;
}

template<class InputIterator, class Predicate>
inline InputIterator
find_if_not(InputIterator _First,
            InputIterator _Last,
            Predicate _Pred)
{
  MINI_STL_DEBUG_POINTER(_Pred, "find_if_not_comp");
  for (;_First != _Last && _Pred(*_First); ++_First)
    {}
  return _First;
}

template<class BidirectionalIterator1, class BidirectionalIterator2>
BidirectionalIterator1
_find_end_aux(BidirectionalIterator1 _First1, BidirectionalIterator1 _Last1,
              BidirectionalIterator2 _First2, BidirectionalIterator2 _Last2,
              bidirectional_iterator_tag, bidirectional_iterator_tag)
{
  typedef _MY_STL::reverse_iterator<BidirectionalIterator1> RI1;
  typedef _MY_STL::reverse_iterator<BidirectionalIterator2> RI2;
  RI1 RI1_first(_Last1);
  RI1 RI1_last(_First1);
  RI2 RI2_first(_Last2);
  RI2 RI2_last(_First2);

  RI1 RI1_result = _MY_STL::search(RI1_first, RI1_last,
                                   RI2_first, RI2_last);
  if (RI1_result == RI1_last)
    return _Last1; //NO find
  BidirectionalIterator1 result = RI1_result.base();
  advance(result, -DISTANCE(_First2, _Last2));
  return result;
}

template<class BidirectionalIterator1, class BidirectionalIterator2, class Compare>
BidirectionalIterator1
_find_end_aux(BidirectionalIterator1 _First1, BidirectionalIterator1 _Last1,
              BidirectionalIterator2 _First2, BidirectionalIterator2 _Last2,
              Compare _Comp,
              bidirectional_iterator_tag,
              bidirectional_iterator_tag)
{
  typedef _MY_STL::reverse_iterator<BidirectionalIterator1> RI1;
  typedef _MY_STL::reverse_iterator<BidirectionalIterator2> RI2;
  RI1 RI1_first(_Last1);
  RI1 RI1_last(_First1);
  RI2 RI2_first(_Last2);
  RI2 RI2_last(_First2);

  RI1 RI1_result = _MY_STL::search(RI1_first, RI1_last,
                                   RI2_first, RI2_last,
                                   _Comp);
  if (RI1_result == RI1_last)
    return _Last1; //NO find
  BidirectionalIterator1 result = RI1_result.base();
  advance(result, -DISTANCE(_First2, _Last2));
  return result;
}


template<class ForwardIterator1, class ForwardIterator2>
ForwardIterator1
_find_end_aux(ForwardIterator1 _First1, ForwardIterator1 _Last1,
              ForwardIterator2 _First2, ForwardIterator2 _Last2,
              forward_iterator_tag, forward_iterator_tag)
{
  ForwardIterator1 result = _Last1;
  _First1 = _MY_STL::search(_First1, _Last1,
                            _First2, _Last2);
  while (_First1 != _Last1) {
    result = _First1;
    ++_First1;
    _First1 = _MY_STL::search(_First1, _Last1,
                              _First2, _Last2);
  }
  return result;
}

template<class ForwardIterator1, class ForwardIterator2, class Compare>
ForwardIterator1
_find_end_aux(ForwardIterator1 _First1, ForwardIterator1 _Last1,
              ForwardIterator2 _First2, ForwardIterator2 _Last2,
              Compare _Comp,
              forward_iterator_tag,
              forward_iterator_tag)
{
  ForwardIterator1 result = _Last1;
  _First1 = _MY_STL::search(_First1, _Last1,
                            _First2, _Last2,
                            _Comp);
  while (_First1 != _Last1) {
    result = _First1;
    ++_First1;
    _First1 = _MY_STL::search(_First1, _Last1,
                              _First2, _Last2,
                              _Comp);
  }
  return result;
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
find_end(ForwardIterator1 _First1, ForwardIterator1 _Last1,
         ForwardIterator2 _First2, ForwardIterator2 _Last2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "find_end");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "find_end");
  return _find_end_aux(_First1, _Last1, _First2, _Last2,
                       ITERATOR_CATEGORY(_First1),
                       ITERATOR_CATEGORY(_First2));
}

template<class ForwardIterator1, class ForwardIterator2, class Pred>
inline ForwardIterator1
find_end(ForwardIterator1 _First1, ForwardIterator1 _Last1,
         ForwardIterator2 _First2, ForwardIterator2 _Last2,
         Pred _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "find_end_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "find_end_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "find_end_of_comp");
  return _find_end_aux(_First1, _Last1, _First2, _Last2,
                       _Comp, ITERATOR_CATEGORY(_First1),
                       ITERATOR_CATEGORY(_First2));
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
find_first_of(ForwardIterator1 _First1, ForwardIterator1 _Last1,
              ForwardIterator2 _First2, ForwardIterator2 _Last2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "find_first_of");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "find_first_of");
  for (; _First1!=_Last1; ++_First1)
    for (ForwardIterator2 i=_First2; i!=_Last2; ++i)
      if (*_First1 == *i)
        return _First1;
  return _Last1;
}

template<class ForwardIterator1, class ForwardIterator2, class BinaryPredicate>
inline ForwardIterator1
find_first_of(ForwardIterator1 _First1, ForwardIterator1 _Last1,
              ForwardIterator2 _First2, ForwardIterator2 _Last2,
              BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "find_first_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "find_first_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "find_first_of_comp");
  for (; _First1!=_Last1; ++_First1)
    for (ForwardIterator2 i=_First2; i!=_Last2; ++i)
      if (_Comp(*_First1, *i))
        return _First1;
  return _Last1;
}

template<class InputIterator, class Function>
inline Function
for_each(InputIterator _First,
         InputIterator _Last,
         Function _Func)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "for_each");
  MINI_STL_DEBUG_POINTER(_Func, "for_each");
  for (; _First!=_Last; ++_First)
    _Func(*_First);
#ifdef MINI_STL_RVALUE_REFS
  return _MY_STL::move(_Func);
#else
  return _Func;
#endif
}

template<class ForwardIterator, class Generator>
inline void
generate(ForwardIterator _First,
         ForwardIterator _Last,
         Generator _Gen)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "generate");
  MINI_STL_DEBUG_POINTER(_Gen, "generate");
  for (; _First!=_Last; ++_First)
    *_First = _Gen();
}

template<class OutputIterator, class Distance, class Generator>
inline OutputIterator
generate_n(OutputIterator _First,
           Distance _Count,
           Generator _Gen)
{
  if (_Count <= 0)
    return _First;
  for (; --_Count>=0; ++_First)
    *_First = _Gen();
  return _First;
}

template<class InputIterator1, class InputIterator2>
bool includes(InputIterator1 _First1, InputIterator1 _Last1,
              InputIterator2 _First2, InputIterator2 _Last2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "includes");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "includes");
  while (_First1!=_Last1 && _First2!=_Last2)
    if (*_First1 < *_First2)
      ++_First1;
    else if (*_First2 < *_First1)
      return false;
    else
      ++_First1, ++_First2;
  return _First2 == _Last2;
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
bool includes(InputIterator1 _First1, InputIterator1 _Last1,
              InputIterator2 _First2, InputIterator2 _Last2,
              BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "includes_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "includes_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "includes_of_comp");
  while (_First1!=_Last1 && _First2!=_Last2)
    if (_Comp(*_First1, *_First2))
      ++_First1;
    else if (_Comp(*_First2, *_First1))
      return false;
    else
      ++_First1, ++_First2;
  return _First2 == _Last2;
}

template<class BidirectionalIterator>
void inplace_merge(BidirectionalIterator _First,
                   BidirectionalIterator _Middle,
                   BidirectionalIterator _Last)
{


}

template<class BidirectionalIterator, class Predicate>
void inplace_merge(BidirectionalIterator _First,
                   BidirectionalIterator _Middle,
                   BidirectionalIterator _Last,
                   Predicate _Comp)
{

}

template<class RandomAccessIterator, class Distance>
inline RandomAccessIterator
_is_heap_until_aux(RandomAccessIterator _First,
                   RandomAccessIterator _Last,
                   Distance*)
{
  Distance len = _Last - _First;
  if (len >= 2)
    for (Distance off=1; off<len; ++off)
      if (*(_First + (off-1) / 2) < *(_First + off))//parent < child
        return _First + off;
  return _Last;
}

template<class RandomAccessIterator, class BinaryPredicate, class Distance>
inline RandomAccessIterator
_is_heap_until_aux(RandomAccessIterator _First,
                   RandomAccessIterator _Last,
                   BinaryPredicate _Comp, Distance*)
{
  Distance len = _Last - _First;
  if (len >= 2)
    for (Distance off=1; off<len; ++off)
      if (_Comp(*(_First + (off-1) / 2), *(_First + off)))//comp(parent,child)
        return _First + off;
  return _Last;
}


template<class RandomAccessIterator>
inline RandomAccessIterator
is_heap_until(RandomAccessIterator _First,
              RandomAccessIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_heap_until");
  return _is_heap_until_aux(_First, _Last,
                            DISTANCE_TYPE(_First));
}

template<class RandomAccessIterator, class BinaryPredicate>
inline RandomAccessIterator
is_heap_until(RandomAccessIterator _First,
              RandomAccessIterator _Last,
              BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_heap_until_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "is_heap_until_of_comp");
  return _is_heap_until_aux(_First, _Last,
                            _Comp,
                            DISTANCE_TYPE(_First));
}

template<class RandomAccessIterator>
inline bool
is_heap(RandomAccessIterator _First,
        RandomAccessIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_heap");
  return _is_heap_until_aux(_First, _Last, DISTANCE_TYPE(_First))
         == _Last;
}

template<class RandomAccessIterator, class BinaryPredicate>
inline bool
is_heap(RandomAccessIterator _First,
        RandomAccessIterator _Last,
        BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_heap_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "is_heap_of_comp");
  return _is_heap_until_aux(_First, _Last,
                            _Comp,
                            DISTANCE_TYPE(_First))
         == _Last;
}

template<class InputIterator, class Predicate>
inline bool
is_partitioned(InputIterator _First,
               InputIterator _Last,
               Predicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_partitioned");
  MINI_STL_DEBUG_POINTER(_Comp, "is_partitioned");
  //check[_First,middle) _Comp is true; [middle,_Last) _Comp is false
  for (; _First != _Last; ++_First)
    if (!_Comp(*_First))
      break;
  for (; _First != _Last; ++_First)
    if (_Comp(*_First))
      return false;
  return true;
}

template<class ForwardIterator>
inline ForwardIterator
is_sorted_until(ForwardIterator _First,
                ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_sorted_until");
  for (ForwardIterator next=_First; ++next != _Last; ++_First)
    if (*_next < *_First)
      return next;
  return _Last;
}

template<class ForwardIterator, class BinaryPredicate>
inline ForwardIterator
is_sorted_until(ForwardIterator _First,
                ForwardIterator _Last,
                BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_sorted_unitl_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "is_sorted_unitl_of_comp");
  for (ForwardIterator next=_First; ++next != _Last; ++_First)
    if (_Comp(*_next, *_First))
      return next;
  return _Last;
}

template<class ForwardIterator>
inline bool
is_sorted(ForwardIterator _First,
          ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_sorted");
  return is_sorted_until(_First, _Last) == _Last;
}

template<class ForwardIterator, class BinaryPredicate>
inline bool
is_sorted(ForwardIterator _First,
          ForwardIterator _Last,
          BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "is_sorted_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "is_sorted_of_comp");
  return is_sorted_until(_First, _Last, _Comp) == _Last;
}

template<class ForwardIterator1, class ForwardIterator2>
inline void
iter_swap(ForwardIterator1 _Left,
          ForwardIterator2 _Right)
{
  _MY_STL::swap(*_Left, *_Right);
}

template<class ForwardIterator>
inline ForwardIterator
max_element(ForwardIterator _First,
            ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "max_element");
  ForwardIterator result = _First;
  if (_First != _Last)
    while (++_First != _Last)
      if (*result < *_First)
        result = _First;
  return result;
}

template<class ForwardIterator, class BinaryPredicate>
inline ForwardIterator
max_element(ForwardIterator _First,
            ForwardIterator _Last,
            BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "max_element_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "max_element_of_comp");
  ForwardIterator result = _First;
  if (_First != _Last)
    while (++_First != _Last)
      if (_Comp(result, *_First))
        result = _First;
  return result;
}

template<class ForwardIterator>
inline ForwardIterator
min_element(ForwardIterator _First,
            ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "min_element");
  ForwardIterator result = _First;
  if (_First != _Last)
    while (++_First != _Last)
      if (*_First < *result)
        result = _First;
  return result;
}

template<class ForwardIterator, class BinaryPredicate>
inline ForwardIterator
min_element(ForwardIterator _First,
            ForwardIterator _Last,
            BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "min_element_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "min_element_of_comp");
  ForwardIterator result = _First;
  if (_First != _Last)
    while (++_First != _Last)
      if (_Comp(*_First, result))
        result = _First;
  return result;
}
template<class ForwardIterator1, class ForwardIterator2,
         class Distance1, class Distance2>
ForwardIterator1
_search_aux(ForwardIterator1 _First1, ForwardIterator1 _Last1,
            ForwardIterator2 _First2, ForwardIterator2 _Last2,
            Distance1*, Distance2*)
{
  Distance1 len1 = DISTANCE(_First1, _Last1);
  Distance2 len2 = DISTANCE(_First2, _Last2);
  if (len1 < len2)
    return _Last1;
  ForwardIterator1 cur1 = _First1;
  ForwardIterator2 cur2 = _First2;
  while (true) {
    if (*cur1 == *cur2) {
      for (; cur2!=_Last2 && cur1!= _Last1 && *cur1==*cur2;
           ++cur1, ++cur2)
        {}
      if (cur2 == _Last2)
        return _First1; //find
    } else {
      if (len1 == len2)
        return _Last1;
      ++_First1; //find[_First1,_Last1)[_First2,_Last2) loop
      --len1;
      cur1 = _First1;
      cur2 = _First2;
    }
  }
}

template<class ForwardIterator1, class ForwardIterator2,
         class Distance1, class Distance2, class Compare>
ForwardIterator1
_search_aux(ForwardIterator1 _First1, ForwardIterator1 _Last1,
            ForwardIterator2 _First2, ForwardIterator2 _Last2,
            Compare _Comp,
            Distance1*, Distance2*)
{
  Distance1 len1 = DISTANCE(_First1, _Last1);
  Distance2 len2 = DISTANCE(_First2, _Last2);
  if (len1 < len2)
    return _Last1;
  ForwardIterator1 cur1 = _First1;
  ForwardIterator2 cur2 = _First2;
  while (true) {
    if (_Comp(*cur1, *cur2)) {
      for (; cur2!=_Last2 && cur1!= _Last1 && _Comp(*cur1, *cur2);
           ++cur1, ++cur2)
        {}
      if (cur2 == _Last2)
        return _First1; //find
    } else {
      if (len1 == len2)
        return _Last1;
      ++_First1; //find[_First1,_Last1)[_First2,_Last2) loop
      --len1;
      cur1 = _First1;
      cur2 = _First2;
    }
  }
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1
search(ForwardIterator1 _First1, ForwardIterator1 _Last1,
       ForwardIterator2 _First2, ForwardIterator2 _Last2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "search");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "search");

  return _search_aux(_First1, _Last1, _First2, _Last2,
                     DISTANCE_TYPE(_First1),
                     DISTANCE_TYPE(_First2));
}

template<class ForwardIterator1, class ForwardIterator2, class Compare>
inline ForwardIterator1
search(ForwardIterator1 _First1, ForwardIterator1 _Last1,
       ForwardIterator2 _First2, ForwardIterator2 _Last2,
       Compare _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "search_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "search_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "search_of_comp");

  return _search_aux(_First1, _Last1, _First2, _Last2,
                     _Comp, DISTANCE_TYPE(_First1),
                     DISTANCE_TYPE(_First2));
}

template<class ForwardIterator, class Distance, class Type>
ForwardIterator
search_n(ForwardIterator _First, ForwardIterator _Last,
         Distance _Count, const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "search_n");
  if (_Count <= 0)
    return _Last;
  _First = _MY_STL::find(_First, _Last, _Val);
  Distance n;
  ForwardIterator cur;
  while (_First != _Last) {
    n = _Count;
    --n;
    cur = _First;
    ++cur;
    for (; n>0 && cur!=_Last && _Val==*cur; ++cur)
      --n;
    if (n == 0)
      return _First;
    _First = _MY_STL::find(cur, _Last, _Val);
  }
  return _Last;
}

template<class ForwardIterator, class Distance,
         class Type, class BinaryPredicate>
ForwardIterator
search_n(ForwardIterator _First, ForwardIterator _Last,
         Distance _Count, const Type& _Val,
         BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "search_n_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "search_n_of_comp");
  if (_Count <= 0)
    return _Last;
  _First = _MY_STL::find(_First, _Last, _Val);
  Distance n;
  ForwardIterator cur;
  while (_First != _Last) {
    n = _Count;
    --n;
    cur = _First;
    ++cur;
    for (; n>0 && cur!=_Last && _Comp(_Val, *cur); ++cur)
      --n;
    if (n == 0)
      return _First;
    _First = _MY_STL::find(cur, _Last, _Val);
  }
  return _Last;
}

MINI_STL_END
#endif // MINI_STL_ALGORITHM_H
