#ifndef MINI_STL_ALGORITHM_H
#define MINI_STL_ALGORITHM_H
#include "mini_stl_algobase.h"
#include "mini_stl_debug.h"
MINI_STL_BEGIN

template<class ForwardIterator>
   ForwardIterator adjacent_find(
      ForwardIterator _First,
      ForwardIterator _Last
   )
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
   ForwardIterator adjacent_find(
      ForwardIterator _First,
      ForwardIterator _Last,
      BinaryPredicate _Comp
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "adjacent_find_of_comp");

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
   bool all_of(
       InputIterator _First,
       InputIterator _Last,
       Predicate _Comp
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "all_of");

  for (; _First != _Last; ++_First)
    if (!_Comp(*_First))
      return false;
  return true;
}

template<class InputIterator, class UnaryPredicate>
   bool any_of(
       InputIterator _First,
       InputIterator _Last,
       UnaryPredicate _Comp
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "any_of");

  for (; _First != _Last; ++_First)
    if (_Comp(*_First))
      return true;
  return false;
}

template<class ForwardIterator, class Type>
   bool binary_search(
       ForwardIterator _First,
       ForwardIterator _Last,
       const Type& _Val
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "binary_search");

  _First = _MY_STL::lower_bound(_First, _Last, _Val);
  return (_First != _Last && !(Val < *_First));
}

template<class ForwardIterator, class Type, class BinaryPredicate>
   bool binary_search(
       ForwardIterator _First,
       ForwardIterator _Last,
       const Type& _Val,
       BinaryPredicate _Comp
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "binary_search_comp");

  _First = _MY_STL::lower_bound(_First, _Last, _Val, _Comp);
  return (_First != _Last && !(_Comp(_Val, *_First)));
}

template<class InputIterator, class OutputIterator, class Predicate>
   OutputIterator copy_if(
       InputIterator _First,
       InputIterator _Last,
       OutputIterator _Dest,
       Predicate _Pred
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "copy_if");
  MINI_STL_DEBUG_POINTER(_Dest, "copy_if");

  for (; _First != _Last; ++_First)
    if (_Pred(*_First))
      *_Dest++ = *_First;
  return _Dest;
}

template<class InputIterator, class Type>
   typename _MY_STL::iterator_traits<InputIterator>::difference_type
   count(InputIterator _First,
         InputIterator _Last,
         const Type& _Val
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "count");

  typename _MY_STL::iterator_traits<InputIterator>::difference_type count = 0;
  for (; _First != _Last; ++_First)
    if (*_First == _Val)
      ++count;
  return count;
}

template<class InputIterator, class Predicate>
   typename _MY_STL::iterator_traits<InputIterator>::difference_type
   count_if(InputIterator _First,
            InputIterator _Last,
            Predicate _Pred
   )
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "count_if");

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
  ForwardIterator middle = _First;
  while (len > 0) {
    half = len / 2;
    advance(middle, half);
    if (*middle < _Val) {
      len /= 2;
      _First = middle;
    } else if (_Val < *middle) {
      len /= 2;
      middle = _First;
    } else {
      ForwardIterator result1 = lower_bound(_First, middle, _Val);
      ForwardIterator result2 = upper_bound(++middle, _Last, _Val);
      return _MY_STL::pair<ForwardIterator,ForwardIterator>
          (result1, result2);
    }
  }
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
  RandomAccessIterator middle = _First;
  while (len > 0) {
    half = len / 2;
    middle += half;
    if (*middle < _Val) {
        cout << "if" << endl;
      len /= 2;
      _First = middle;
    } else if (_Val < *middle) {
        cout << "else if" << endl;
      len = len - half - 1;
      middle = _First;
    } else {
        cout << "else" << endl;
      RandomAccessIterator result1 = lower_bound(_First, middle, _Val);
      //++middle;
      cout << "middle:" << *middle << endl;
      RandomAccessIterator result2 = upper_bound(middle, _Last, _Val);
      return _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
          (result1, result2);
    }
  }
  return _MY_STL::pair<RandomAccessIterator,RandomAccessIterator>
                (_First, _First);
}

template<class ForwardIterator, class Type>
   _MY_STL::pair<ForwardIterator, ForwardIterator>
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

template<class ForwardIterator, class Type, class Predicate>
   _MY_STL::pair<ForwardIterator, ForwardIterator>
   equal_range(ForwardIterator _First,
               ForwardIterator _Last,
               const Type& _Val,
               Predicate _Comp
   )
{

}

MINI_STL_END
#endif // MINI_STL_ALGORITHM_H
