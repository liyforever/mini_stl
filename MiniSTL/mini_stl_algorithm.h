#ifndef MINI_STL_ALGORITHM_H
#define MINI_STL_ALGORITHM_H
#include "mini_stl_algobase.h"
#include "mini_stl_pair.h"
#include "mini_stl_memory.h"
#include "mini_stl_debug.h"
MINI_STL_BEGIN

const int _INSERTSORT_MAX = 40;
//sort use insertion_sort max elem

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
  return (_First != _Last && !(_Val < *_First));
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
    if (_Pred(*_First))
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
    if (MINI_STL_DEBUG_LESS(*middle, _Val)) {
      len = len - half - 1;
      _First = middle;
      ++_First;
    } else if (MINI_STL_DEBUG_LESS(_Val, *middle)) {
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
    if (MINI_STL_DEBUG_LESS(*middle, _Val)) {
      len = len - half - 1;
      _First = middle + 1;
    } else if (MINI_STL_DEBUG_LESS(_Val, *middle)) {
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
    if (MINI_STL_DEBUG_COMP(_Comp, *middle, _Val)) {
      len = len - half - 1;
      _First = middle;
      ++_First;
    } else if (MINI_STL_DEBUG_COMP(_Comp, _Val, *middle)) {
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
    if (MINI_STL_DEBUG_COMP(_Comp, *middle, _Val)) {
      len = len - half - 1;
      _First = middle + 1;
    } else if (MINI_STL_DEBUG_COMP(_Comp, _Val, *middle)) {
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

template<class InputIterator, class Predicate>
inline InputIterator
find_if(InputIterator _First, InputIterator _Last,
        Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "find_if");
  MINI_STL_DEBUG_POINTER(_Pred, "find_of_comp");
  for ( ; _First!=_Last && !_Pred(*_First);
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
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "find_of_not");
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
  MINI_STL_DEBUG_POINTER(_First, "generate_n");
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
    if (MINI_STL_DEBUG_LESS(*_First1, *_First2))
      ++_First1;
    else if (MINI_STL_DEBUG_LESS(*_First2, *_First1))
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
    if (MINI_STL_DEBUG_COMP(_Comp, *_First1, *_First2))
      ++_First1;
    else if (MINI_STL_DEBUG_COMP(_Comp, *_First2, *_First1))
      return false;
    else
      ++_First1, ++_First2;
  return _First2 == _Last2;
}

#ifdef MINI_STL_RVALUE_REFS
template<class BidirectionalIterator, class Type, class Distance>
void _inplace_merge_impl(BidirectionalIterator _First,
                         BidirectionalIterator _Middle,
                         BidirectionalIterator _Last,
                         Type*,
                         Distance*)
{
  Distance len1 = DISTANCE(_First, _Middle);
  Distance len2 = DISTANCE(_Middle, _Last);
  Distance count = len1 + len2;
  Type* buff_first = new Type[count];
  Type* buff_middle = buff_first + len1;
  Type* buff_last = buff_first + count;
  if (buff_first != 0) {
    uninitialized_copy(_First, _Last, buff_first);
    Type *tmpFirst = buff_first;
    Type *tmpMiddle = buff_middle;
    while (tmpFirst!=buff_middle && tmpMiddle!=buff_last) {
      if (MINI_STL_DEBUG_LESS(*tmpFirst, *tmpMiddle))
        *_First++ = *tmpFirst++;
      else
        *_First++ = *tmpMiddle++;
    }
    _First = _MY_STL::move(tmpFirst, buff_middle, _First);
    _MY_STL::move(tmpMiddle, buff_last, _First);
    delete []buff_first;
  }
}
#else
template<class BidirectionalIterator, class Type, class Distance>
void _inplace_merge_impl(BidirectionalIterator _First,
                         BidirectionalIterator _Middle,
                         BidirectionalIterator _Last,
                         Type*,
                         Distance*)
{
  Distance len1 = DISTANCE(_First, _Middle);
  Distance len2 = DISTANCE(_Middle, _Last);
  Distance count = len1 + len2;
  Type* buff_first = new Type[count];
  Type* buff_middle = buff_first + len1;
  Type* buff_last = buff_first + count;
  if (buff_first != 0) {
    uninitialized_copy(_First, _Last, buff_first);
    Type *tmpFirst = buff_first;
    Type *tmpMiddle = buff_middle;
    while (tmpFirst!=buff_middle && tmpMiddle!=buff_last) {
      if (MINI_STL_DEBUG_LESS(*tmpFirst, *tmpMiddle))
        *_First++ = *tmpFirst++;
      else
        *_First++ = *tmpMiddle++;
    }
    _First = copy(tmpFirst, buff_middle, _First);
    copy(tmpMiddle, buff_last, _First);
    delete []buff_first;
  }
}
#endif //MINI_STL_RVALUE_REFS
template<class BidirectionalIterator>
inline void
inplace_merge(BidirectionalIterator _First,
              BidirectionalIterator _Middle,
              BidirectionalIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Middle, "inplace_merge");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Middle, _Last, "inplace_merge");
  if (_First==_Middle || _Middle==_Last)
    return;
  _inplace_merge_impl(_First, _Middle, _Last,
                      VALUE_TYPE(_First),
                      DISTANCE_TYPE(_First));
}

#ifdef MINI_STL_RVALUE_REFS
template<class BidirectionalIterator, class Type,
         class Predicate, class Distance>
void _inplace_merge_impl(BidirectionalIterator _First,
                         BidirectionalIterator _Middle,
                         BidirectionalIterator _Last,
                         Predicate _Comp,
                         Type*,
                         Distance*)
{
  Distance len1 = DISTANCE(_First, _Middle);
  Distance len2 = DISTANCE(_Middle, _Last);
  Distance count = len1 + len2;
  Type* buff_first = new Type[count];
  Type* buff_middle = buff_first + len1;
  Type* buff_last = buff_first + count;
  if (buff_first != 0) {
    uninitialized_copy(_First, _Last, buff_first);
    Type *tmpFirst = buff_first;
    Type *tmpMiddle = buff_middle;
    while (tmpFirst!=buff_middle && tmpMiddle!=buff_last) {
      if (MINI_STL_DEBUG_COMP(_Comp, *tmpFirst, *tmpMiddle))
        *_First++ = *tmpFirst++;
      else
        *_First++ = *tmpMiddle++;
    }
    _First = _MY_STL::move(tmpFirst, buff_middle, _First);
    _MY_STL::move(tmpMiddle, buff_last, _First);
    delete []buff_first;
  }
}
#else
template<class BidirectionalIterator, class Type,
         class Predicate, class Distance>
void _inplace_merge_impl(BidirectionalIterator _First,
                         BidirectionalIterator _Middle,
                         BidirectionalIterator _Last,
                         Predicate _Comp,
                         Type*,
                         Distance*)
{
  Distance len1 = DISTANCE(_First, _Middle);
  Distance len2 = DISTANCE(_Middle, _Last);
  Distance count = len1 + len2;
  Type* buff_first = new Type[count];
  Type* buff_middle = buff_first + len1;
  Type* buff_last = buff_first + count;
  if (buff_first != 0) {
    uninitialized_copy(_First, _Last, buff_first);
    Type *tmpFirst = buff_first;
    Type *tmpMiddle = buff_middle;
    while (tmpFirst!=buff_middle && tmpMiddle!=buff_last) {
      if (MINI_STL_DEBUG_COMP(_Comp, *tmpFirst, *tmpMiddle))
        *_First++ = *tmpFirst++;
      else
        *_First++ = *tmpMiddle++;
    }
    _First = copy(tmpFirst, buff_middle, _First);
    copy(tmpMiddle, buff_last, _First);
    delete []buff_first;
  }
}
#endif //MINI_STL_RVALUE_REFS

template<class BidirectionalIterator, class Predicate>
void inplace_merge(BidirectionalIterator _First,
                   BidirectionalIterator _Middle,
                   BidirectionalIterator _Last,
                   Predicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Middle, "inplace_merge_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Middle, _Last, "inplace_merge_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "inplace_merge_of_comp");
  if (_First==_Middle || _Middle==_Last)
    return;
  _inplace_merge_impl(_First, _Middle, _Last,
                      _Comp,
                      VALUE_TYPE(_First),
                      DISTANCE_TYPE(_First));
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
      if (MINI_STL_DEBUG_LESS(*(_First+(off-1)/2), *(_First+off)))
        //parent < child
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
      if (MINI_STL_DEBUG_COMP(_Comp,*(_First+(off-1)/2), *(_First+off)))
        //comp(parent,child)
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
    if (MINI_STL_DEBUG_LESS(*_next, *_First))
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
    if (MINI_STL_DEBUG_COMP(_Comp, *_next, *_First))
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
  MINI_STL_DEBUG_POINTER(_Left, "iter_swap");
  MINI_STL_DEBUG_POINTER(_Right, "iter_swap");
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
      if (MINI_STL_DEBUG_LESS(*result, *_First))
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
      if (MINI_STL_DEBUG_COMP(_Comp, *result, *_First))
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
      if (MINI_STL_DEBUG_LESS(*_First, *result))
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
      if (MINI_STL_DEBUG_COMP(_Comp, *_First, *result))
        result = _First;
  return result;
}

template<class InputIterator1, class InputIterator2>
inline _MY_STL::pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 _First1,
         InputIterator1 _Last1,
         InputIterator2 _First2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "mismatch");
  MINI_STL_DEBUG_POINTER_FOR_N(_First2, DISTANCE(_First1, _Last1), "mismatch");
  for (; _First1!=_Last1 && *_First1 == *_First2; ++_First1, ++_First2)
    {}
  return _MY_STL::pair<InputIterator1, InputIterator2>
                      (_First1, _First2);
}

template<class InputIterator1, class InputIterator2, class BinaryPredicate>
inline _MY_STL::pair<InputIterator1, InputIterator2>
mismatch(InputIterator1 _First1,
         InputIterator1 _Last1,
         InputIterator2 _First2,
         BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "mismatch_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_First2, DISTANCE(_First1, _Last1), "mismatch_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "mismatch_of_comp");
  for (; _First1!=_Last1 &&
       _Comp(*_First1, *_First2);
       ++_First1, ++_First2)
    {}
  return _MY_STL::pair<InputIterator1, InputIterator2>
                      (_First1, _First2);
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
inline OutputIterator
merge(InputIterator1 _First1, InputIterator1 _Last1,
      InputIterator2 _First2, InputIterator2 _Last2,
      OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "merge");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "merge");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "merge");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "merge");
  MINI_STL_DEBUG_ORDER(_First1, _Last1, "merge");
  MINI_STL_DEBUG_ORDER(_First2, _Last2, "merge");
  while (_First1!=_Last1 && _First2!= _Last2) {
    if (MINI_STL_DEBUG_LESS(*_First1, *_First2))
      *_Result++ = *_First1++;
    else
      *_Result++ = *_First2++;
  }
  _Result = _MY_STL::copy(_First1, _Last1, _Result);
  return _MY_STL::copy(_First2, _Last2, _Result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
inline OutputIterator
merge(InputIterator1 _First1, InputIterator1 _Last1,
      InputIterator2 _First2, InputIterator2 _Last2,
      OutputIterator _Result,BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "merge_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "merge_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "merge_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "merge_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "merge_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First1, _Last1, _Comp, "merge_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First2, _Last2, _Comp, "merge_of_comp");
  while (_First1!=_Last1 && _First2!= _Last2) {
    if (MINI_STL_DEBUG_COMP(_Comp, *_First1, *_First2))
      *_Result++ = *_First1++;
    else
      *_Result++ = *_First2++;
  }
  _Result = _MY_STL::copy(_First1, _Last1, _Result);
  return _MY_STL::copy(_First2, _Last2, _Result);
}

template<class ForwardIterator>
_MY_STL::pair<ForwardIterator, ForwardIterator>
minmax_element(ForwardIterator _First,
               ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "minmax_element");
  _MY_STL::pair<ForwardIterator, ForwardIterator>
      result(_First, _First);
  if (_First == _Last)
    return result;
  ForwardIterator next;
  for (; ++_First != _Last;) {
    next = _First;
    if (++next == _Last) {//one elem
      if (*_First < *result.first)
        result.first = _First;
      else if (MINI_STL_DEBUG_LESS(*result.second, *_First))
        result.second = _First;
    } else { //two elem
      if (MINI_STL_DEBUG_LESS(*_First, *next)) {
        if (MINI_STL_DEBUG_LESS(*_First, *result.first))
          result.first = _First;
        if (MINI_STL_DEBUG_LESS(*result.second, *next))
          result.second = next;
      } else {
        if (MINI_STL_DEBUG_LESS(*next, *result.first))
          result.first = next;
        if (MINI_STL_DEBUG_LESS(*result.second, *_First))
          result.second = _First;
      }
    }
  }
  return result;
}

template<class ForwardIterator, class BinaryPredicate>
_MY_STL::pair<ForwardIterator,ForwardIterator>
minmax_element(ForwardIterator _First,
               ForwardIterator _Last,
               BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "minmax_element_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "minmax_element_of_comp");
  _MY_STL::pair<ForwardIterator, ForwardIterator>
      result(_First, _First);
  if (_First == _Last)
    return result;
  ForwardIterator next;
  for (; ++_First != _Last;) {
    next = _First;
    if (++next == _Last) {//one elem
      if (MINI_STL_DEBUG_COMP(_Comp, *_First, *result.first))
        result.first = _First;
      else if (MINI_STL_DEBUG_COMP(_Comp, *result.second, *_First))
        result.second = _First;
    } else { //two elem
      if (MINI_STL_DEBUG_COMP(_Comp, *_First, *next)) {
        if (_Comp(*_First, *result.first))
          result.first = _First;
        if (MINI_STL_DEBUG_COMP(_Comp, *result.second, *next))
          result.second = next;
      } else {
        if (MINI_STL_DEBUG_COMP(_Comp, *next, *result.first))
          result.first = next;
        if (MINI_STL_DEBUG_COMP(_Comp, *result.second, *_First))
          result.second = _First;
      }
    }
  }
  return result;
}

template<class Type>
_MY_STL::pair<const Type&, const Type&>
minmax(const Type& _Left,
       const Type& _Right)
{
  typedef _MY_STL::pair<const Type&, const Type&>
      PTT;
  return MINI_STL_DEBUG_LESS(_Left,_Right )?
        PTT(_Left, _Right)
      : PTT(_Right, _Left);
}

template<class Type, class BinaryPredicate>
_MY_STL::pair<const Type&, const Type&>
minmax(const Type& _Left,
       const Type& _Right,
       BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_POINTER(_Comp, "minmax_of_comp");
  typedef _MY_STL::pair<const Type&, const Type&>
      PTT;
  return MINI_STL_DEBUG_COMP(_Comp, _Left, _Right) ?
        PTT(_Left, _Right):
        PTT(_Right, _Left);
}

template<class BidirectionalIterator>
bool next_permutation(BidirectionalIterator _First,
                      BidirectionalIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "next_permutation");
  BidirectionalIterator next = _Last;
  if (_First == _Last || --next == _First)
    return false;
  BidirectionalIterator next_next;
  for (;;) {
    next_next = next;
    --next;
    if (MINI_STL_DEBUG_LESS(*next, *next_next)) {
      BidirectionalIterator FGN = _Last;
      //FGN = first_greate_next
      while(!(MINI_STL_DEBUG_LESS(*next, *--FGN)))
        {}
      _MY_STL::iter_swap(FGN, next);
      _MY_STL::reverse(next_next, _Last);
      return true;
    }
    if (next == _First) {
      _MY_STL::reverse(_First, _Last);
      return false;
    }
  }
}

template<class BidirectionalIterator, class BinaryPredicate>
bool next_permutation(BidirectionalIterator _First,
                      BidirectionalIterator _Last,
                      BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "next_permutation_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "next_permutation_of_comp");
  BidirectionalIterator next = _Last;
  if (_First == _Last || --next == _First)
    return false;
  BidirectionalIterator next_next;
  for (;;) {
    next_next = next;
    --next;
    if (MINI_STL_DEBUG_COMP(_Comp, *next,*next_next)) {
      BidirectionalIterator FGN = _Last;
      //FGN = first_greate_next
      while(!MINI_STL_DEBUG_COMP(_Comp, *next, *--FGN))
        {}
      _MY_STL::iter_swap(FGN, next);
      _MY_STL::reverse(next_next, _Last);
      return true;
    }
    if (next == _First) {
      _MY_STL::reverse(_First, _Last);
      return false;
    }
  }
}

template<class InputIterator, class Predicate>
inline bool
none_of(InputIterator _First,
        InputIterator _Last,
        Predicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "none_of");
  MINI_STL_DEBUG_POINTER(_Comp, "none_of");
  for (; _First!=_Last; ++_First)
    if (_Comp(*_First))
      return false;
  return true;
}

template<class RandomAccessIterator>
void nth_element(RandomAccessIterator _First,
                 RandomAccessIterator _Nth,
                 RandomAccessIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Nth, "nth_element");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "nth_element");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Nth, _Last, "nth_element");
  for (; _INSERTSORT_MAX < _Last - _First; ) {
    _MY_STL::pair<RandomAccessIterator, RandomAccessIterator>
        middle =
                 _MY_STL::_partition(_First, _Last);
    if (middle.second <= _Nth)
      _First = middle.second;
    else if (middle.first <= _Nth) //[middle.first,_Nth,middle.second)
      return;	//finish
    else
      _Last = middle.first;
  }

  _MY_STL::_insert_sort(_First, _Last);
}

template<class RandomAccessIterator, class BinaryPredicate>
void nth_element(RandomAccessIterator _First,
                 RandomAccessIterator _Nth,
                 RandomAccessIterator _Last,
                 BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Nth, "nth_element_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "nth_element_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Nth, _Last, "nth_element_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "nth_element_of_comp");
  for (; _INSERTSORT_MAX < _Last - _First; ) {
    _MY_STL::pair<RandomAccessIterator, RandomAccessIterator>
        middle =
                 _MY_STL::_partition(_First, _Last, _Comp);
    if (middle.second <= _Nth)
      _First = middle.second;
    else if (middle.first <= _Nth) //[middle.first,_Nth,middle.second)
      return;	//finish
    else
      _Last = middle.first;
  }

  _MY_STL::_insert_sort(_First, _Last, _Comp);
}

template<class BidirectionalIterator>
bool prev_permutation(BidirectionalIterator _First,
                      BidirectionalIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "prev_permutation");
  BidirectionalIterator next = _Last;
  if (_First == _Last || --next == _Last)
    return false;
  BidirectionalIterator next_next;
  for (;;) {
    next_next = next;
    --next;
    if (MINI_STL_DEBUG_LESS(*next_next, *next)) {
      BidirectionalIterator FGN = _Last;
      //FGN = first_greate_next
      while(!(MINI_STL_DEBUG_LESS(*--FGN, *next)))
        {}
      _MY_STL::iter_swap(FGN, next);
      _MY_STL::reverse(next_next, _Last);
      return true;
    }
    if (next == _First) {
      _MY_STL::reverse(_First, _Last);
      return false;
    }
  }
}

template<class BidirectionalIterator, class BinaryPredicate>
bool prev_permutation(BidirectionalIterator _First,
                      BidirectionalIterator _Last,
                      BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "prev_permutation_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "prev_permutation_of_comp");
  BidirectionalIterator next = _Last;
  if (_First == _Last || --next == _Last)
    return false;
  BidirectionalIterator next_next;
  for (;;) {
    next_next = next;
    --next;
    if (MINI_STL_DEBUG_COMP(_Comp, *next_next, *next)) {
      BidirectionalIterator FGN = _Last;
      //FGN = first_greate_next
      while(!MINI_STL_DEBUG_COMP(_Comp, *--FGN, *next))
        {}
      _MY_STL::iter_swap(FGN, next);
      _MY_STL::reverse(next_next, _Last);
      return true;
    }
    if (next == _First) {
      _MY_STL::reverse(_First, _Last);
      return false;
    }
  }
}

template<class RandomAccessIterator, class Type>
inline void
_partial_sort_impl(RandomAccessIterator _First,
                   RandomAccessIterator _SortEnd,
                   RandomAccessIterator _Last,
                   Type*)
{
  _MY_STL::make_heap(_First, _SortEnd);
  for (RandomAccessIterator i = _SortEnd;
       i <_Last; ++i)
    if (MINI_STL_DEBUG_LESS(*i, *_First))
      _MY_STL::_pop_heap(_First, _SortEnd, i,
                         *i, DISTANCE_TYPE(_First));
  sort_heap(_First, _SortEnd);
}

template<class RandomAccessIterator>
inline void
partial_sort(RandomAccessIterator _First,
             RandomAccessIterator _SortEnd,
             RandomAccessIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _SortEnd, "partial_sort");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_SortEnd, _Last, "partial_sort");
  _partial_sort_impl(_First, _SortEnd, _Last,
                     VALUE_TYPE(_First));
}

template<class RandomAccessIterator, class BinaryPredicate, class Type>
inline void
_partial_sort_impl(RandomAccessIterator _First,
                   RandomAccessIterator _SortEnd,
                   RandomAccessIterator _Last,
                   BinaryPredicate _Comp,
                   Type*)
{
  _MY_STL::make_heap(_First, _SortEnd);
  for (RandomAccessIterator i = _SortEnd;
       i <_Last; ++i)
    if (MINI_STL_DEBUG_COMP(_Comp, *i, *_First))
      _MY_STL::_pop_heap(_First, _SortEnd, i,
                         Type(*i), DISTANCE_TYPE(_First),
                         _Comp);
  _MY_STL::sort_heap(_First, _SortEnd, _Comp);
}

template<class RandomAccessIterator, class BinaryPredicate>
inline void
partial_sort(RandomAccessIterator _First,
             RandomAccessIterator _SortEnd,
             RandomAccessIterator _Last,
             BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _SortEnd, "partial_sort_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_SortEnd, _Last, "partial_sort_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "partial_sort_of_comp");
  _partial_sort_impl(_First, _SortEnd, _Last,
                     _Comp,
                     VALUE_TYPE(_First));
}

template<class InputIterator, class RandomAccessIterator>
RandomAccessIterator
partial_sort_copy(InputIterator _First1,
                  InputIterator _Last1,
                  RandomAccessIterator _First2,
                  RandomAccessIterator _Last2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "partial_sort_copy");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "partial_sort_copy");
  if (_First1 == _Last1 || _First2 == _Last2)
    return _Last2;
  RandomAccessIterator sortEnd = _First2;
  for (; _First1!=_Last1 && sortEnd!=_Last2;
       ++_First1, ++sortEnd)
    *sortEnd = *_First1;
  _MY_STL::partial_sort(_First2, sortEnd, _Last2);
  return sortEnd;
}

template<class InputIterator, class RandomAccessIterator, class BinaryPredicate>
RandomAccessIterator
partial_sort_copy(InputIterator _First1,
                  InputIterator _Last1,
                  RandomAccessIterator _First2,
                  RandomAccessIterator _Last2,
                  BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "partial_sort_copy_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "partial_sort_copy_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "partial_sort_copy_of_comp");
  if (_First1 == _Last1 || _First2 == _Last2)
    return _Last2;
  RandomAccessIterator sortEnd = _First2;
  for (; _First1!=_Last1 && sortEnd!=_Last2;
       ++_First1, ++sortEnd)
    *sortEnd = *_First1;
  _MY_STL::partial_sort(_First2, sortEnd, _Last2, _Comp);
  return sortEnd;
}

template<class InputIterator, class OutputIterator1, class OutputIterator2, class Predicate>
inline _MY_STL::pair<OutputIterator1, OutputIterator2>
partition_copy(InputIterator _First, InputIterator _Last,
               OutputIterator1 _Dest1, OutputIterator2 _Dest2,
               Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "partial_copy");
  MINI_STL_DEBUG_POINTER_FOR_N(_Dest1, DISTANCE(_First, _Last),"partial_copy");
  MINI_STL_DEBUG_POINTER_FOR_N(_Dest2, DISTANCE(_First, _Last),"partial_copy");
  MINI_STL_DEBUG_POINTER(_Pred, "partial_copy");
  for (; _First!=_Last; ++_First)
    if (_Pred(*_First))
      *_Dest1++ = *_First;
    else
      *_Dest2++ = *_First;
  return _MY_STL::pair<OutputIterator1, OutputIterator2>
      (_Dest1, _Dest2);
}

template<class ForwardIterator, class Predicate, class Distance>
inline ForwardIterator
_partition_point_impl(ForwardIterator _First,
                      ForwardIterator _Last,
                      Predicate _Pred,
                      Distance*)
{
  Distance count = DISTANCE(_First, _Last);
  Distance half;
  ForwardIterator middle;
  while (0 < count) {
    half = count / 2;
    middle = _First;
    _MY_STL::advance(middle, half);
    if (_Pred(*middle)) {
      _First = ++middle;
      count = count - half - 1;
    } else {
      count = half;
    }
  }
  return middle;
}

template<class ForwardIterator, class Predicate>
inline ForwardIterator
partition_point(ForwardIterator _First,
                ForwardIterator _Last,
                Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "partition_point");
  MINI_STL_DEBUG_POINTER(_Pred, "partition_point");
  return _partition_point_impl(_First, _Last,
                               _Pred,
                               DISTANCE_TYPE(_First));
}

template<class BidirectionalIterator, class Predicate>
inline BidirectionalIterator
_partition_impl(BidirectionalIterator _First,
                BidirectionalIterator _Last,
                Predicate _Pred,
                bidirectional_iterator_tag)
{
  for(;;++_First) {
      for (; _First != _Last; ++_First)
        if (_Pred(*_First))
          ;
        else
          break;
      if (_First == _Last)//finish
        return _First;
      for (; _First!= --_Last;)
        if (!_Pred(*_Last))
          ;
        else
          break;
      if (_First == _Last)//finish
        return _First;
      _MY_STL::iter_swap(_First, _Last);
  }
  return _First;
}

template<class BidirectionalIterator, class Predicate>
inline BidirectionalIterator
partition(BidirectionalIterator _First,
          BidirectionalIterator _Last,
          Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "partition");
  MINI_STL_DEBUG_POINTER(_Pred, "partition");

  return _partition_impl(_First, _Last,
                         _Pred,
                         ITERATOR_CATEGORY(_First));
}


template<class BidirectionalIterator>
inline void
_reverse_aux(BidirectionalIterator _First,
             BidirectionalIterator _Last,
             bidirectional_iterator_tag)
{
  for (;_First!=_Last && _First!= --_Last; ++_First)
    _MY_STL::iter_swap(_First, _Last);
}

template<class BidirectionalIterator>
inline void
reverse(BidirectionalIterator _First,
        BidirectionalIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "reverse");
  _reverse_aux(_First, _Last, ITERATOR_CATEGORY(_First));
}

template<class BidirectionalIterator, class OutputIterator>
inline void
_reverse_copy_aux(BidirectionalIterator _First,
             BidirectionalIterator _Last,
             OutputIterator _Result,
             bidirectional_iterator_tag)
{
  while (_First != _Last) {
    --_Last;
    *_Result++ = _Last;
  }
  return _Result;
}

template<class BidirectionalIterator, class OutputIterator>
inline OutputIterator
reverse_copy(BidirectionalIterator _First, BidirectionalIterator _Last,
             OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "reverse_copy");
  return _reverse_copy_aux(_First, _Last,
                           _Result,
                           ITERATOR_CATEGORY(_First));
}

template<class InputIterator, class OutputIterator, class Type>
inline OutputIterator
remove_copy(InputIterator _First, InputIterator _Last,
            OutputIterator _Result, const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "remove_copy");
  for (; _First!=_Last; ++_First)
    if (*_First != _Val)
      *_Result++ = *_First;
  return _Result;
}

template<class InputIterator, class OutputIterator, class Predicate>
inline OutputIterator
remove_copy_if(InputIterator _First, InputIterator _Last,
               OutputIterator _Result, Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "remove_copy_if");
  MINI_STL_DEBUG_POINTER(_Pred, "remove_copy_if");
  for (; _First!=_Last; ++_First)
    if (!_Pred(*_First))
      *_Result++ = *_First;
  return _Result;
}

template<class ForwardIterator, class Type>
inline ForwardIterator
remove(ForwardIterator _First,
       ForwardIterator _Last,
       const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "remove");
  _First = _MY_STL::find(_First, _Last, _Val);
  if (_First == _Last)
    return _First;
  ForwardIterator result = _First;
  for (++_First; _First!=_Last; ++_First)
    if (*_First != _Val)
#ifdef MINI_STL_RVALUE_REFS
      *result++ = _MY_STL::move(*_First);
#else
      *result++ = *_First;
#endif
  return result;
}

template<class ForwardIterator, class Predicate>
inline ForwardIterator
remove_if(ForwardIterator _First,
          ForwardIterator _Last,
          Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "remove_if");
  MINI_STL_DEBUG_POINTER(_Pred, "remove_if");
  _First = _MY_STL::find_if(_First, _Last, _Pred);
  if (_First == _Last)
    return _First;
  ForwardIterator result = _First;
  for (++_First; _First!=_Last; ++_First)
    if (!_Pred(*_First))
#ifdef MINI_STL_RVALUE_REFS
      *result++ = _MY_STL::move(*_First);
#else
      *result++ = *_First;
#endif
  return result;
}

template<class ForwardIterator, class Type>
inline void
replace(ForwardIterator _First, ForwardIterator _Last,
        const Type& _OldVal, const Type& _NewVal)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "replace");
  for (; _First != _Last; ++_First)
    if (*_First == _OldVal)
      *_First = _NewVal;
}

template<class ForwardIterator, class Predicate, class Type>
inline void
replace_if(ForwardIterator _First, ForwardIterator _Last,
           Predicate _Pred, const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "replace_if");
  MINI_STL_DEBUG_POINTER(_Pred, "replace_if");
  for (; _First != _Last; ++_First)
    if (_Pred(*_First))
      *_First = _Val;
}

template<class InputIterator, class OutputIterator, class Type>
inline OutputIterator
replace_copy(InputIterator _First, InputIterator _Last,
             OutputIterator _Result,
             const Type& _OldVal,
             const Type& _NewVal)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "replace_copy");
  for (; _First != _Last; ++_First)
    *_Result++ = *_First == _OldVal ? _NewVal : *_First;
  return _Result;
}

template<class InputIterator, class OutputIterator, class Predicate, class Type>
inline OutputIterator
replace_copy_if(InputIterator _First, InputIterator _Last,
                OutputIterator _Result,
                Predicate _Pred,
                const Type& _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "replace_copy");
  MINI_STL_DEBUG_POINTER(_Pred, "replace_copy_if");
  for (; _First != _Last; ++_First)
    *_Result++ = _Pred(*_First) ? _Val : *_First;
  return _Result;
}

template<class Distance>
inline Distance __gcd(Distance _Left, Distance _Right)
{
  Distance tmp = _Left;
  while (_Right != 0) {
    tmp =  _Left % _Right;
    _Left = _Right;
    _Right = tmp;
  }
  return _Left;
}

template<class RandomAccessIterator, class Distance,
         class Type>
inline void
_RI_rotate_aux(RandomAccessIterator _First,
               RandomAccessIterator _Middle,
               RandomAccessIterator _Last,
               Distance*,
               Type*,
               random_access_iterator_tag)
{
  Distance offset = _Middle - _First;
  Distance len = _Last - _First;
  Distance shiftCount = _MY_STL::__gcd(offset, len);

  for (;0 < shiftCount; --shiftCount) {
    Distance hole = shiftCount;
    Distance next = hole;
    Distance next1 = next + offset < len ? next + offset
                                         : (next + offset) % len;
    for (;;) {
      if (next1 == hole)
        break;
      _MY_STL::iter_swap(_First+next, _First+next1);
      next = next1;
      next1 = next1 + offset < len ? next1 + offset
                                   : (next1 + offset) % len;
    }
  }
}

template<class BidirectionIterator, class Distance>
inline void
_rotate_impl(BidirectionIterator _First,
             BidirectionIterator _Middle,
             BidirectionIterator _Last,
             Distance*,
             bidirectional_iterator_tag)
{
  _MY_STL::reverse(_First, _Middle);
  _MY_STL::reverse(_Middle, _Last);
  _MY_STL::reverse(_First, _Last);
}

template<class RandomAccessIterator, class Distance>
inline void
_rotate_impl(RandomAccessIterator _First,
             RandomAccessIterator _Middle,
             RandomAccessIterator _Last,
             Distance*,
             random_access_iterator_tag)
{
  _RI_rotate_aux(_First, _Middle, _Last,
                 DISTANCE_TYPE(_First),
                 VALUE_TYPE(_First),
                 ITERATOR_CATEGORY(_First));
}

template<class ForwardIterator, class Distance>
inline void
_rotate_impl(ForwardIterator _First,
             ForwardIterator _Middle,
             ForwardIterator _Last,
             Distance*,
             forward_iterator_tag)
{
  ForwardIterator next;
  for (next = _Middle; ;) {
    _MY_STL::iter_swap(_First, next);
    if (++_First == _Middle)
        if (++next == _Last)
          return; //rotate finish
        else
          _Middle = _Last;
    else if (++next == _Last)
        next = _Middle;
  }
}

template<class ForwardIterator>
inline void
rotate(ForwardIterator _First,
       ForwardIterator _Middle,
       ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Middle, "rotate");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Middle, _Last, "rotate");
  if (_First==_Middle || _Middle==_Last)
    return;
  _rotate_impl(_First, _Middle, _Last,
               DISTANCE_TYPE(_First),
               ITERATOR_CATEGORY(_First));
}

template<class ForwardIterator, class OutputIterator>
inline OutputIterator
rotate_copy(ForwardIterator _First,
            ForwardIterator _Middle,
            ForwardIterator _Last,
            OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Middle, "rotate_copy");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_Middle, _Last, "rotate_copy");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last), "rotate_copy");
  _Result = _MY_STL::copy(_Middle, _Last, _Result);
  return _MY_STL::copy(_First, _Middle, _Result);
}

struct _MINI_STL_DEFAULT_RAND
{
  size_t operator()() const
  {
    return ::rand();
  }
};

template<class RandomAccessIterator, class RandomNumberGenerator,
         class Distance>
inline void
_random_shuffle_impl(RandomAccessIterator _First,
                     RandomAccessIterator _Last,
                     RandomNumberGenerator& _Rand,
                     Distance*)
{
  Distance count = _Last - _First - 1;
  for (RandomAccessIterator i=_First;
       i!=_Last; ++i) {
    _MY_STL::iter_swap(i, _First + _Rand()%count);
  }
}

template<class RandomAccessIterator>
inline void
random_shuffle(RandomAccessIterator _First,
               RandomAccessIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "random_shuffle");
  _random_shuffle_impl(_First, _Last,
                       _MINI_STL_DEFAULT_RAND(),
                       DISTANCE_TYPE(_First));
}

template<class RandomAccessIterator, class RandomNumberGenerator>
inline void
random_shuffle(RandomAccessIterator _First,
               RandomAccessIterator _Last,
               RandomNumberGenerator& _Rand)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "random_shuffle_of_func");
  MINI_STL_DEBUG_POINTER(_Rand, "random_shuffle_of_func");
  _random_shuffle_impl(_First, _Last,
                       _Radn,
                       DISTANCE_TYPE(_First));
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
      for (; cur2!=_Last2 && cur1!= _Last1 &&
           _Comp(*cur1, *cur2);
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
  for (; _First!=_Last && !_Comp(*_First, _Val); ++_First)
    {}
  Distance n;
  ForwardIterator cur;
  while (_First != _Last) {
    n = _Count;
    --n;
    cur = _First;
    ++cur;
    for (; n>0 && cur!=_Last && _Comp(*cur, _Val);
         ++cur)
      --n;
    if (n == 0)
      return _First;
    _First = cur;
    for (; _First!=_Last && !_Comp(*_First, _Val); ++_First)
      {}
  }
  return _Last;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator
set_intersection(InputIterator1 _First1, InputIterator1 _Last1,
                 InputIterator2 _First2, InputIterator2 _Last2,
                 OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_intersection");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_intersection");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_intersection");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_intersection");
  MINI_STL_DEBUG_ORDER(_First1, _Last1, "invalid order in set_intersection");
  MINI_STL_DEBUG_ORDER(_First2, _Last2, "invalid order in set_intersection");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_LESS(*_First1, *_First2)) {
      ++_First1;
    } else if (MINI_STL_DEBUG_LESS(*_First2, *_First1)) {
      ++_First2;
    } else {
      *_Result++ = *_First1;
      ++_First1;
      ++_First2;
    }
  }
  return _Result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
OutputIterator
set_intersection(InputIterator1 _First1, InputIterator1 _Last1,
                 InputIterator2 _First2, InputIterator2 _Last2,
                 OutputIterator _Result,
                 BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_intersection_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_intersection_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_intersection_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_intersection_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "set_intersection_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First1, _Last1, _Comp, "invalid order in set_intersection_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First2, _Last2, _Comp, "invalid order in set_intersection_of_comp");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_COMP(_Comp, *_First1, *_First2)) {
      ++_First1;
    } else if (MINI_STL_DEBUG_COMP(_Comp, *_First2, *_First1)) {
      ++_First2;
    } else {
      *_Result++ = *_First1;
      ++_First1;
      ++_First2;
    }
  }
  return _Result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator
set_symmetric_difference(InputIterator1 _First1, InputIterator1 _Last1,
                         InputIterator2 _First2, InputIterator2 _Last2,
                         OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_symmetric_difference");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_symmetric_difference");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_symmetric_difference");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_symmetric_difference");
  MINI_STL_DEBUG_ORDER(_First1, _Last1, "invalid order in set_symmetric_difference");
  MINI_STL_DEBUG_ORDER(_First2, _Last2, "invalid order in set_symmetric_difference");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_LESS(*_First1, *_First2)) {
      *_Result++ = *_First1;
      ++_First1;
    } else if (MINI_STL_DEBUG_LESS(*_First2, *_First1)) {
      *_Result++ = *_First2;
      ++_First2;
    } else {
      ++_First1;
      ++_First2;
    }
  }
  _Result =  _MY_STL::copy(_First1, _Last1, _Result);
  return _MY_STL::copy(_First2, _Last2, _Result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
OutputIterator
set_symmetric_difference(InputIterator1 _First1, InputIterator1 _Last1,
                         InputIterator2 _First2, InputIterator2 _Last2,
                         OutputIterator _Result,
                         BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_symmetric_difference_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_symmetric_difference_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_symmetric_difference_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_symmetric_difference_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "set_symmetric_difference_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First1, _Last1, _Comp, "invalid order in set_symmetric_difference_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First2, _Last2, _Comp, "invalid order in set_symmetric_difference_of_comp");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_COMP(_Comp, *_First1, *_First2)) {
      *_Result++ = *_First1;
      ++_First1;
    } else if (MINI_STL_DEBUG_COMP(_Comp, *_First2, *_First1)) {
      *_Result++ = *_First2;
      ++_First2;
    } else {
      ++_First1;
      ++_First2;
    }
  }
  _Result =  _MY_STL::copy(_First1, _Last1, _Result);
  return _MY_STL::copy(_First2, _Last2, _Result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator
set_difference(InputIterator1 _First1, InputIterator1 _Last1,
               InputIterator2 _First2, InputIterator2 _Last2,
               OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_difference");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_difference");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_difference");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_difference");
  MINI_STL_DEBUG_ORDER(_First1, _Last1, "invalid order in set_difference");
  MINI_STL_DEBUG_ORDER(_First2, _Last2, "invalid order in set_difference");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_LESS(*_First1, *_First2)) {
      *_Result++ = *_First1;
      ++_First1;
    } else if (MINI_STL_DEBUG_LESS(*_First2, *_First1)) {
      ++_First2;
    } else {
      ++_First1;
      ++_First2;
    }
  }
  return _MY_STL::copy(_First1, _Last1, _Result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
OutputIterator
set_difference(InputIterator1 _First1, InputIterator1 _Last1,
               InputIterator2 _First2, InputIterator2 _Last2,
               OutputIterator _Result,
               BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_difference_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_difference_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_difference_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_difference_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "set_difference_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First1, _Last1, _Comp, "invalid order in set_difference_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First2, _Last2, _Comp, "invalid order in set_difference_of_comp");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_COMP(_Comp, *_First1, *_First2)) {
      *_Result++ = *_First1;
      ++_First1;
    } else if (MINI_STL_DEBUG_COMP(_Comp, *_First2, *_First1)) {
      ++_First2;
    } else {
      ++_First1;
      ++_First2;
    }
  }
  return _MY_STL::copy(_First1, _Last1, _Result);
}


template<class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator
set_union(InputIterator1 _First1, InputIterator1 _Last1,
          InputIterator2 _First2, InputIterator2 _Last2,
          OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_union");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_union");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_union");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_union");
  MINI_STL_DEBUG_ORDER(_First1, _Last1, "invalid order in set_union");
  MINI_STL_DEBUG_ORDER(_First2, _Last2, "invalid order in set_union");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_LESS(*_First1, *_First2)) {
      *_Result++ = *_First1;
      ++_First1;
    } else if (MINI_STL_DEBUG_LESS(*_First2, *_First1)) {
      *_Result++ = *_First2;
      ++_First2;
    } else {
      *_Result++ = *_First1;
      ++_First1;
      ++_First2;
    }
  }
  _Result = _MY_STL::copy(_First1, _Last1, _Result);
  return _MY_STL::copy(_First2, _Last2, _Result);
}

template<class InputIterator1, class InputIterator2, class OutputIterator, class BinaryPredicate>
OutputIterator
set_union(InputIterator1 _First1, InputIterator1 _Last1,
          InputIterator2 _First2, InputIterator2 _Last2,
          OutputIterator _Result,
          BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "set_union_of_comp");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "set_union_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1, _Last1), "set_union_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First2, _Last2), "set_union_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "set_union_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First1, _Last1, _Comp, "invalid order in set_union_of_comp");
  MINI_STL_DEBUG_ORDER_COMP(_First2, _Last2, _Comp, "invalid order in set_union_of_comp");
  while (_First1!=_Last1 && _First2!=_Last2) {
    if (MINI_STL_DEBUG_COMP(_Comp, *_First1, *_First2)) {
      *_Result++ = *_First1;
      ++_First1;
    } else if (MINI_STL_DEBUG_COMP(_Comp, *_First2, *_First1)) {
      *_Result++ = *_First2;
      ++_First2;
    } else {
      *_Result++ = *_First1;
      ++_First1;
      ++_First2;
    }
  }
  _Result = _MY_STL::copy(_First1, _Last1, _Result);
  return _MY_STL::copy(_First2, _Last2, _Result);
}

template <class Size>
Size __lg(Size _Num)
{
  Size result = 0;
  for (; _Num > 1; _Num >>= 1)
    ++result;
  return result;
}

template<class BidirectionalIterator, class Type>
void
_insert_sort_aux(BidirectionalIterator _First,
                 BidirectionalIterator _Last,
                 Type*)
{
#ifdef MINI_STL_RVALUE_REFS
  if (_First != _Last) {
    for (BidirectionalIterator next = _First;
         ++next != _Last; ) {
      BidirectionalIterator next1 = next;
      Type val = _MY_STL::move(*next);
      if (MINI_STL_DEBUG_LESS(*next, *_First)) {
        _MY_STL::copy_backward(_First, next, ++next1);
        *_First = _MY_STL::move(val);
      } else {
        for (BidirectionalIterator prev = next1;
             val <*--prev; next1 = prev)
          *next1 = _MY_STL::move(*prev);
        *next1 = _MY_STL::move(val);
      }
    }
  }
#else
  if (_First != _Last) {
    for (BidirectionalIterator next = _First;
         ++next != _Last; ) {
      BidirectionalIterator next1 = next;
      Type val = *next;
      if (MINI_STL_DEBUG_LESS(*next, *_First)) {
        _MY_STL::copy_backward(_First, next, ++next1);
        *_First = val;
      } else {
        for (BidirectionalIterator prev = next1;
             val <*--prev; next1 = prev)
          *next1 = *prev;
        *next1 = val;
      }
    }
  }
#endif
}

template<class BidirectionalIterator>
inline void
_insert_sort(BidirectionalIterator _First,
             BidirectionalIterator _Last)
{
  _insert_sort_aux(_First, _Last, VALUE_TYPE(_First));
}

template<class RandomAccessIterator>
inline void
_Med3(RandomAccessIterator _First,
      RandomAccessIterator _Middle,
      RandomAccessIterator _Last)
{
  if (MINI_STL_DEBUG_LESS(*_Middle, *_First))
    _MY_STL::iter_swap(_Middle, _First);
  if (MINI_STL_DEBUG_LESS(*_Last, *_Middle)) {
    _MY_STL::iter_swap(_Middle, _Last);
    if (MINI_STL_DEBUG_LESS(*_Middle, *_First))
      _MY_STL::iter_swap(_Middle, _First);
  }
}

template<class RandomAccessIterator>
_MY_STL::pair<RandomAccessIterator, RandomAccessIterator>
_partition(RandomAccessIterator _First,
           RandomAccessIterator _Last)
{

  RandomAccessIterator middle = _First + (_Last - _First) / 2;
  _Med3(_First, middle, _Last - 1);
  RandomAccessIterator pFirst = middle;
  RandomAccessIterator pLast = middle + 1;
  //return pair first = pFirst, return pair second = plast

  while (_First < pFirst
         &&!(MINI_STL_DEBUG_LESS(*pFirst, *(pFirst-1)))
         &&!(MINI_STL_DEBUG_LESS(*(pFirst-1), *pFirst)))
    --pFirst;

  while (pLast < _Last
         &&!(MINI_STL_DEBUG_LESS(*pFirst, *pLast))
         &&!(MINI_STL_DEBUG_LESS(*pLast, *pFirst)))
    ++pLast;
  //[pFirst,pLast) has same elem

  RandomAccessIterator lFirst = pLast;
  RandomAccessIterator lLast = pFirst;
  //loop first, loop last

  for (;;) {
    for (; lFirst < _Last; ++lFirst)
      if (MINI_STL_DEBUG_LESS(*pFirst, *lFirst))
        ;
      else if (MINI_STL_DEBUG_LESS(*lFirst, *pFirst))
        break;
      else
        _MY_STL::iter_swap(pLast++, lFirst);

    for (; _First < lLast; --lLast)
      if (MINI_STL_DEBUG_LESS(*(lLast-1), *pFirst))
        ;
      else if (MINI_STL_DEBUG_LESS(*pFirst, *(lLast-1)))
        break;
      else
        _MY_STL::iter_swap(--pFirst, lLast-1);

    if (lFirst==_Last && _First==lLast)
      return pair<RandomAccessIterator,RandomAccessIterator>
          (pFirst, pLast);
    if (_First == lLast) {
       //[_First, pFirst) less pivot
       //[pFirst, pLast) equal pivot
      if (pLast != lFirst)
        //[pLast,lFirst) greate pivot
        _MY_STL::iter_swap(pFirst, pLast);
      ++pLast;
      _MY_STL::iter_swap(pFirst++, lFirst++);
    } else if (lFirst == _Last) {
        //[pLast,_Last) greate pivot
        //[pFirst, pLast) equal pivot
      if (--lLast != --pFirst)
        _MY_STL::iter_swap(lLast, pFirst);
      _MY_STL::iter_swap(pFirst, --pLast);
    } else
      _MY_STL::iter_swap(lFirst++, --lLast);
  }
}

template<class RandomAccessIterator, class Distance>
inline void
_sort_aux(RandomAccessIterator _First,
          RandomAccessIterator _Last,
          Distance _Dept)
{
  Distance count;
  while ((count=_Last-_First) > 0
         && _Dept > 0) {
    _Dept = 2*__lg(_Dept);
    _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
        mid = _partition(_First, _Last);
    if (_First - mid.first < _Last - mid.second) {
      _sort_aux(_First, mid.first, _Dept);
      _First = mid.second;
    } else {
      _sort_aux(mid.second, _Last, _Dept);
      _Last = mid.first;
    }
  }
  if (_INSERTSORT_MAX < count)
    _MY_STL::sort_heap(_First, _Last);
  else if (count < _INSERTSORT_MAX)
    _MY_STL::_insert_sort(_First, _Last);
}

template<class RandomAccessIterator>
inline void
sort(RandomAccessIterator _First,
     RandomAccessIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "sort");
  _sort_aux(_First, _Last, _Last - _First);
}

template<class RandomAccessIterator, class BinaryPredicate>
inline void
_Med3(RandomAccessIterator _First,
      RandomAccessIterator _Middle,
      RandomAccessIterator _Last,
      BinaryPredicate _Comp)
{
  if (MINI_STL_DEBUG_COMP(_Comp, *_Middle,*_First))
    _MY_STL::iter_swap(_Middle, _First);
  if (MINI_STL_DEBUG_COMP(_Comp, *_Last, *_Middle)) {
    _MY_STL::iter_swap(_Middle, _Last);
    if (MINI_STL_DEBUG_COMP(_Comp, *_Middle, *_First))
      _MY_STL::iter_swap(_Middle, _First);
  }
}

template<class BidirectionalIterator, class Type, class BinaryPredicate>
void
_insert_sort_aux(BidirectionalIterator _First,
                 BidirectionalIterator _Last,
                 BinaryPredicate _Comp,
                 Type*)
{
#ifdef MINI_STL_RVALUE_REFS
  if (_First != _Last) {
    for (BidirectionalIterator next = _First;
         ++next != _Last; ) {
      BidirectionalIterator next1 = next;
      Type val = _MY_STL::move(*next);
      if (MINI_STL_DEBUG_COMP(_Comp, *next, *_First)) {
        _MY_STL::copy_backward(_First, next, ++next1);
        *_First = _MY_STL::move(val);
      } else {
        for (BidirectionalIterator prev = next1;
             MINI_STL_DEBUG_COMP(_Comp, val,*--prev);
             next1 = prev)
          *next1 = _MY_STL::move(*prev);
        *next1 = _MY_STL::move(val);
      }
    }
  }
#else
  if (_First != _Last) {
    for (BidirectionalIterator next = _First;
         ++next != _Last; ) {
      BidirectionalIterator next1 = next;
      Type val = *next;
      if (MINI_STL_DEBUG_COMP(_Comp, *next, *_First)) {
        _MY_STL::copy_backward(_First, next, ++next1);
        *_First = val;
      } else {
        for (BidirectionalIterator prev = next1;
             MINI_STL_DEBUG_COMP(_Comp, val, *--prev);
             next1 = prev)
          *next1 = *prev;
        *next1 = val;
      }
    }
  }
#endif
}

template<class BidirectionalIterator, class BinaryPredicate>
inline void
_insert_sort(BidirectionalIterator _First,
             BidirectionalIterator _Last,
             BinaryPredicate _Comp)
{
  _insert_sort_aux(_First, _Last, _Comp, VALUE_TYPE(_First));
}

template<class RandomAccessIterator, class BinaryPredicate>
_MY_STL::pair<RandomAccessIterator, RandomAccessIterator>
_partition(RandomAccessIterator _First,
           RandomAccessIterator _Last,
           BinaryPredicate _Comp)
{

  RandomAccessIterator middle = _First + (_Last - _First) / 2;
  _Med3(_First, middle, _Last - 1, _Comp);
  RandomAccessIterator pFirst = middle;
  RandomAccessIterator pLast = middle + 1;
  //return pair first = pFirst, return pair second = plast

  while (_First < pFirst
         &&!MINI_STL_DEBUG_COMP(_Comp, *pFirst, *(pFirst-1))
         &&!MINI_STL_DEBUG_COMP(_Comp, *(pFirst-1), *pFirst))
    --pFirst;

  while (pLast < _Last
         &&!MINI_STL_DEBUG_COMP(_Comp, *pFirst, *pLast)
         &&!MINI_STL_DEBUG_COMP(_Comp, *pLast, *pFirst))
    ++pLast;
  //[pFirst,pLast) has same elem

  RandomAccessIterator lFirst = pLast;
  RandomAccessIterator lLast = pFirst;
  //loop first, loop last

  for (;;) {
    for (; lFirst < _Last; ++lFirst)
      if (MINI_STL_DEBUG_COMP(_Comp, *pFirst, *lFirst))
        ;
      else if (MINI_STL_DEBUG_COMP(_Comp, *lFirst, *pFirst))
        break;
      else
        _MY_STL::iter_swap(pLast++, lFirst);

    for (; _First < lLast; --lLast)
      if (MINI_STL_DEBUG_COMP(_Comp, *(lLast-1), *pFirst))
        ;
      else if (MINI_STL_DEBUG_COMP(_Comp, *pFirst, *(lLast-1)))
        break;
      else
        _MY_STL::iter_swap(--pFirst, lLast-1);

    if (lFirst==_Last && _First==lLast)
      return pair<RandomAccessIterator,RandomAccessIterator>
          (pFirst, pLast);
    if (_First == lLast) {
       //[_First, pFirst) less pivot
       //[pFirst, pLast) equal pivot
      if (pLast != lFirst)
        //[pLast,lFirst) greate pivot
        _MY_STL::iter_swap(pFirst, pLast);
      ++pLast;
      _MY_STL::iter_swap(pFirst++, lFirst++);
    } else if (lFirst == _Last) {
        //[pLast,_Last) greate pivot
        //[pFirst, pLast) equal pivot
      if (--lLast != --pFirst)
        _MY_STL::iter_swap(lLast, pFirst);
      _MY_STL::iter_swap(pFirst, --pLast);
    } else
      _MY_STL::iter_swap(lFirst++, --lLast);
  }
}

template<class RandomAccessIterator, class BinaryPredicate, class Distance>
inline void
_sort_aux(RandomAccessIterator _First,
          RandomAccessIterator _Last,
          BinaryPredicate _Comp,
          Distance _Dept)
{
  Distance count;
  while ((count=_Last-_First) > 0
         && _Dept > 0) {
    _Dept = 2*__lg(_Dept);
    _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
        mid = _partition(_First, _Last, _Comp);
    if (_First - mid.first < _Last - mid.second) {
      _sort_aux(_First, mid.first, _Comp, _Dept);
      _First = mid.second;
    } else {
      _sort_aux(mid.second, _Last, _Comp, _Dept);
      _Last = mid.first;
    }
  }
  if (_INSERTSORT_MAX < count)
    _MY_STL::sort_heap(_First, _Last, _Comp);
  else if (count < _INSERTSORT_MAX)
    _MY_STL::_insert_sort(_First, _Last, _Comp);
}

template<class RandomAccessIterator, class BinaryPredicate>
inline void
sort(RandomAccessIterator _First,
     RandomAccessIterator _Last,
     BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "sort_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "sort_of_comp");
  _sort_aux(_First, _Last, _Comp, _Last - _First);
}

template<class BidirectionalIterator, class Type,
         class Predicate, class Distance>
BidirectionalIterator
_stable_partition_impl(BidirectionalIterator _First,
                       BidirectionalIterator _Last,
                       Predicate _Comp,
                       Type*,
                       Distance*)
{
  Distance count = DISTANCE(_First, _Last);
  Distance falseOffset = 0;
  Distance resultOffset = 0;
  Distance trueOffset = 0;
  Type* buff_first = new Type[count];
  if (buff_first != 0) {
    for (BidirectionalIterator i = _First;
         i!=_Last; ++i)
      if (_Comp(*i))
        ++falseOffset;
    resultOffset = falseOffset;
    for (BidirectionalIterator i = _First;
         i!=_Last; ++i)
      if (_Comp(*i))
        buff_first[trueOffset++] = _MY_STL::move(*i);
      else
        buff_first[falseOffset++] = _MY_STL::move(*i);
    _MY_STL::move(buff_first, buff_first+count, _First);
    delete []buff_first;
  }
  _MY_STL::advance(_First, resultOffset);
  return _First;
}

template<class BidirectionalIterator, class Predicate>
BidirectionalIterator
stable_partition(BidirectionalIterator _First,
                 BidirectionalIterator _Last,
                 Predicate _Pred)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "stable_partition");
  MINI_STL_DEBUG_POINTER(_Pred, "stable_partition");
  return _stable_partition_impl(_First, _Last,
                                _Pred,
                                DISTANCE_TYPE(_First),
                                VALUE_TYPE(_First));
}

#ifdef MINI_STL_RVALUE_REFS
template <class RandomAccessIterator, class Type, class Distance>
void _RI_merge(RandomAccessIterator _First,
               Type* _BuffPtr,
               Distance _Left,
               Distance _Middle,
               Distance _Right)
{
  Distance leftEnd = _Middle - 1;
  Distance tmpPos = _Left;
  Distance count = _Right - _Left + 1;

  while (_Left<=leftEnd && _Middle<=_Right)
    if (MINI_STL_DEBUG_LESS(*(_First+_Left), *(_First+_Middle)))
      _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Left++));
    else
      _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Middle++));

  while (_Left<=leftEnd)
    _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Left++));
  while (_Middle<=_Right)
    _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Middle++));

  for (Distance i=0; i<count; ++i, --_Right)
    *(_First+_Right) = _MY_STL::move(_BuffPtr[_Right]);
}
#else
template <class RandomAccessIterator, class Type, class Distance>
void _RI_merge(RandomAccessIterator _First,
               Type* _BuffPtr,
               Distance _Left,
               Distance _Middle,
               Distance _Right)
{
  Distance leftEnd = _Middle - 1;
  Distance tmpPos = _Left;
  Distance count = _Right - _Left + 1;

  while (_Left<=leftEnd && _Middle<=_Right)
    if (MINI_STL_DEBUG_LESS(*(_First+_Left), *(_First+_Middle)))
      _BuffPtr[tmpPos++] = *(_First+_Left++);
    else
      _BuffPtr[tmpPos++] = *(_First+_Middle++);

  while (_Left<=leftEnd)
    _BuffPtr[tmpPos++] = *(_First+_Left++);
  while (_Middle<=_Right)
    _BuffPtr[tmpPos++] = *(_First+_Middle++);

  for (Distance i=0; i<count; ++i, --_Right)
    *(_First+_Right) = _BuffPtr[_Right];
}
#endif //MINI_STL_RVALUE_REFS

template <class RandomAccessIterator, class Type, class Distance>
void _RI_mergeSort(RandomAccessIterator _First,
                   Type* _BuffPtr,
                   Distance _Left, Distance _Right)
{
  Distance half;
  if (_Left < _Right) {
    half = (_Left + _Right) / 2;
    _RI_mergeSort(_First, _BuffPtr, _Left, half);
    _RI_mergeSort(_First, _BuffPtr, half+1, _Right);
    _RI_merge(_First, _BuffPtr, _Left, half+1, _Right);
  }
}

template <class RandomAccessIterator, class Type, class Distance>
inline void _RI_merge_aux(RandomAccessIterator _First,
                          RandomAccessIterator _Last,
                          Type*, Distance*)
{
  typedef typename _MY_STL::iterator_traits<RandomAccessIterator>
      ::difference_type diff;
  Distance len = _Last - _First;
  Type* buff_ptr = new Type[len];
  if (buff_ptr != 0) {
    _RI_mergeSort(_First, buff_ptr, (diff)(0), len-1);
    delete[] buff_ptr;
  }
}

template<class RandomAccessIterator>
inline void
_stable_sort_dispatch(RandomAccessIterator _First,
                      RandomAccessIterator _Last,
                      random_access_iterator_tag)
{
  _RI_merge_aux(_First, _Last,
                VALUE_TYPE(_First),
                DISTANCE_TYPE(_First));
}

template <class BidirectionalIterator>
void _BI_merge(BidirectionalIterator _First,
               BidirectionalIterator _Last)
{
  typename _MY_STL::iterator_traits<BidirectionalIterator>::
      difference_type len = DISTANCE(_First, _Last);
  if (len==0 || len==1)
    return;
  BidirectionalIterator middle = _First;
  _MY_STL::advance(middle, len/2);
  _BI_merge(_First, middle);
  _BI_merge(middle, _Last);
  _MY_STL::inplace_merge(_First, middle, _Last);
}

template<class BidirectionalIterator>
inline void
_stable_sort_dispatch(BidirectionalIterator _First,
                      BidirectionalIterator _Last,
                      bidirectional_iterator_tag)
{
  _BI_merge(_First, _Last);
}

template<class BidirectionalIterator>
inline void
stable_sort(BidirectionalIterator _First,
            BidirectionalIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "stable_sort");
  _stable_sort_dispatch(_First, _Last,
                        ITERATOR_CATEGORY(_First));
}

#ifdef MINI_STL_RVALUE_REFS
template <class RandomAccessIterator, class Type,
          class BinaryPredicate, class Distance>
void _RI_merge(RandomAccessIterator _First,
               Type* _BuffPtr,
               Distance _Left,
               Distance _Middle,
               Distance _Right,
               BinaryPredicate _Comp)
{
  Distance leftEnd = _Middle - 1;
  Distance tmpPos = _Left;
  Distance count = _Right - _Left + 1;

  while (_Left<=leftEnd && _Middle<=_Right)
    if (MINI_STL_DEBUG_COMP(_Comp, *(_First+_Left), *(_First+_Middle)))
      _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Left++));
    else
      _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Middle++));

  while (_Left<=leftEnd)
    _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Left++));
  while (_Middle<=_Right)
    _BuffPtr[tmpPos++] = _MY_STL::move(*(_First+_Middle++));

  for (Distance i=0; i<count; ++i, --_Right)
    *(_First+_Right) = _MY_STL::move(_BuffPtr[_Right]);
}
#else
template <class RandomAccessIterator, class Type,
          class BinaryPredicate, class Distance>
void _RI_merge(RandomAccessIterator _First,
               Type* _BuffPtr,
               Distance _Left,
               Distance _Middle,
               Distance _Right,
               BinaryPredicate _Comp)
{
  Distance leftEnd = _Middle - 1;
  Distance tmpPos = _Left;
  Distance count = _Right - _Left + 1;

  while (_Left<=leftEnd && _Middle<=_Right)
    if (MINI_STL_DEBUG_COMP(_Comp, *(_First+_Left), *(_First+_Middle)))
      _BuffPtr[tmpPos++] = *(_First+_Left++);
    else
      _BuffPtr[tmpPos++] = *(_First+_Middle++);

  while (_Left<=leftEnd)
    _BuffPtr[tmpPos++] = *(_First+_Left++);
  while (_Middle<=_Right)
    _BuffPtr[tmpPos++] = *(_First+_Middle++);

  for (Distance i=0; i<count; ++i, --_Right)
    *(_First+_Right) = _BuffPtr[_Right];
}
#endif //MINI_STL_RVALUE_REFS

template <class RandomAccessIterator, class Type,
          class BinaryPredicate, class Distance>
void _RI_mergeSort(RandomAccessIterator _First,
                   Type* _BuffPtr,
                   Distance _Left, Distance _Right,
                   BinaryPredicate _Comp)
{
  Distance half;
  if (_Left < _Right) {
    half = (_Left + _Right) / 2;
    _RI_mergeSort(_First, _BuffPtr, _Left, half, _Comp);
    _RI_mergeSort(_First, _BuffPtr, half+1, _Right, _Comp);
    _RI_merge(_First, _BuffPtr, _Left, half+1, _Right, _Comp);
  }
}

template <class RandomAccessIterator, class Type,
          class BinaryPredicate, class Distance>
inline void _RI_merge_aux(RandomAccessIterator _First,
                          RandomAccessIterator _Last,
                          BinaryPredicate _Comp,
                          Type*, Distance*)
{
  typedef typename _MY_STL::iterator_traits<RandomAccessIterator>
      ::difference_type diff;
  Distance len = _Last - _First;
  Type* buff_ptr = new Type[len];
  if (buff_ptr != 0) {
    _RI_mergeSort(_First, buff_ptr, (diff)(0), len-1, _Comp);
    delete[] buff_ptr;
  }
}

template<class RandomAccessIterator, class BinaryPredicate>
inline void
_stable_sort_dispatch(RandomAccessIterator _First,
                      RandomAccessIterator _Last,
                      BinaryPredicate _Comp,
                      random_access_iterator_tag)
{
  _RI_merge_aux(_First, _Last,
                _Comp,
                VALUE_TYPE(_First),
                DISTANCE_TYPE(_First));
}

template <class BidirectionalIterator, class BinaryPredicate>
void _BI_merge(BidirectionalIterator _First,
               BidirectionalIterator _Last,
               BinaryPredicate _Comp)
{
  typename _MY_STL::iterator_traits<BidirectionalIterator>::
      difference_type len = DISTANCE(_First, _Last);
  if (len==0 || len==1)
    return;
  BidirectionalIterator middle = _First;
  _MY_STL::advance(middle, len/2);
  _BI_merge(_First, middle, _Comp);
  _BI_merge(middle, _Last, _Comp);
  _MY_STL::inplace_merge(_First, middle, _Last, _Comp);
}

template<class BidirectionalIterator, class BinaryPredicate>
inline void
_stable_sort_dispatch(BidirectionalIterator _First,
                      BidirectionalIterator _Last,
                      BinaryPredicate _Comp,
                      bidirectional_iterator_tag)
{
  _BI_merge(_First, _Last, _Comp);
}

template<class BidirectionalIterator, class BinaryPredicate>
inline void
stable_sort(BidirectionalIterator _First,
            BidirectionalIterator _Last,
            BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "stable_sort_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "stable_sort_of_comp");
  _stable_sort_dispatch(_First, _Last,
                        _Comp,
                        ITERATOR_CATEGORY(_First));
}

template<class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator2
swap_ranges(ForwardIterator1 _First1, ForwardIterator1 _Last1,
            ForwardIterator2 _First2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "swap_ranges");
  MINI_STL_DEBUG_POINTER_FOR_N(_First2, DISTANCE(_First1, _Last1), "swap_ranges");
  for (; _First1!=_Last1; ++_First1, ++_First2)
    _MY_STL::iter_swap(_First1, _First2);
  return _First2;
}


template<class InputIterator, class OutputIterator, class UnaryFunction>
inline OutputIterator
transform(InputIterator _First1, InputIterator _Last1,
          OutputIterator _Result,
          UnaryFunction _Func)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "transform");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1,_Last1),"transform");
  MINI_STL_DEBUG_POINTER(_Func, "transform");
  for (; _First1!=_Last1; ++_First1)
    *_Result++ = _Func(*_First1);
  return _Result;
}

template<class InputIterator1, class InputIterator2, class OutputIterator,
   class BinaryFunction>
inline OutputIterator
transform(InputIterator1 _First1, InputIterator1 _Last1,
          InputIterator2 _First2,
          OutputIterator _Result,
          BinaryFunction _Func)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "transform");
  MINI_STL_DEBUG_POINTER_FOR_N(_First2, DISTANCE(_First1,_Last1),"transform");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First1,_Last1),"transform");
  MINI_STL_DEBUG_POINTER(_Func, "transform");
  for (; _First1!=_Last1; ++_First1,++_First2)
    *_Result++ = _Func(*_First1, *_First2);
  return _Result;
}

template<class ForwardIterator>
inline ForwardIterator
unique(ForwardIterator _First,
       ForwardIterator _Last)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unique");
  if (_First == _Last)
    return _First;
  ForwardIterator result;
  for (result = _First; ++_First!=_Last;)
    if (!(*_First == *result))
#ifdef MINI_STL_RVALUE_REFS
      *++result = _MY_STL::move(*_First);
#else
      *++result = *_First;
#endif
  return ++result;
}

template<class ForwardIterator, class Predicate>
inline ForwardIterator
unique(ForwardIterator _First, ForwardIterator _Last,
       Predicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unique_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "unique_of_comp");
  if (_First == _Last)
    return _First;
  ForwardIterator result;
  for (result = _First; ++_First!=_Last;)
    if (!_Comp(*_First, *result))
#ifdef MINI_STL_RVALUE_REFS
      *++result = _MY_STL::move(*_First);
#else
      *++result = *_First;
#endif
  return ++result;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator
unique_copy(InputIterator _First, InputIterator _Last,
            OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unique_copy");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last),"unique_copy");
  if (_First == _Last)
    return _Result;
  for (*_Result=*_First; ++_First!=_Last; ) {
    if (!(*_Result == *_First))
      *++_Result = *_First;
  }
  return ++_Result;
}

template<class InputIterator, class OutputIterator, class BinaryPredicate>
inline OutputIterator
unique_copy(InputIterator _First, InputIterator _Last,
            OutputIterator _Result,
            BinaryPredicate _Comp)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unique_copy_of_comp");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last),"unique_copy_of_comp");
  MINI_STL_DEBUG_POINTER(_Comp, "unique_copy_of_comp");
  if (_First == _Last)
    return _Result;
  for (*_Result=*_First; ++_First!=_Last; ) {
    if (!_Comp(*_Result, *_First))
      *++_Result = *_First;
  }
  return ++_Result;
}


MINI_STL_END
#endif // MINI_STL_ALGORITHM_H
