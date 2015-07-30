#ifndef MINI_STL_NUMERIC_H
#define MINI_STL_NUMERIC_H
#include "mini_stl_config.h"
#include "mini_stl_debug.h"
MINI_STL_BEGIN

template<class InputIterator, class Type>
inline Type
accumulate(InputIterator _First, InputIterator _Last,
           Type _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "accumulate");
  for (; _First != _Last; ++_First)
    _Val = _Val + *_First;
  return _Val;
}

template<class InputIterator, class Type, class BinaryOperation>
inline Type
accumulate(InputIterator _First, InputIterator _Last,
           Type _Val,
           BinaryOperation _Binary_op)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "accumulate_of_func");
  MINI_STL_DEBUG_POINTER(_Binary_op, "accumulate_of_func");
  for (; _First != _Last; ++_First)
    _Val = _Binary_op(_Val, _Binary_op);
  return _Val;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator
adjacent_difference(InputIterator _First,
                    InputIterator _Last,
                    OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "adjacent_difference");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last),"adjacent_difference");
  if (_First == _Last)
    return _Result;
  *_Result++ = *_First;
  for (InputIterator next = _First; ++next!=_Last; ++_First)
    *_Result++ = *next - *_First;
  return _Result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
inline OutputIterator
adjacent_difference(InputIterator _First,
                    InputIterator _Last,
                    OutputIterator _Result,
                    BinaryOperation _Binary_op)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "adjacent_difference_of_func");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last), "adjacent_difference_of_func");
  MINI_STL_DEBUG_POINTER(_Binary_op, "adjacent_difference_of_func");
  if (_First == _Last)
    return _Result;
  *_Result++ = *_First;
  for (InputIterator next = _First; ++next!=_Last; ++_First)
    *_Result++ = _Binary_op(*next, *_First);
  return _Result;
}

template<class InputIterator1, class InputIterator2, class Type>
inline Type
inner_product(InputIterator1 _First1, InputIterator1 _Last1,
              InputIterator2 _First2,
              Type _Val)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "inner_product");
  MINI_STL_DEBUG_POINTER_FOR_N(_First2, DISTANCE(_First1, _Last1),"inner_product");
  for (; _First1 != _Last1; ++_First1, ++_First2)
    _Val = _Val + *_First1 * *_First2;
  return _Val;
}

template<class InputIterator1, class InputIterator2, class Type,
   class BinaryOperation1, class BinaryOperation2>
inline Type
inner_product(InputIterator1 _First1, InputIterator1 _Last1,
              InputIterator2 _First2,
              Type _Val,
              BinaryOperation1 _Binary_op1,
              BinaryOperation2 _Binary_op2)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "inner_product_of_func");
  MINI_STL_DEBUG_POINTER_FOR_N(_First2, DISTANCE(_First1, _Last1),"inner_product_of_func");
  MINI_STL_DEBUG_POINTER(_Binary_op1, "inner_product_of_func");
  MINI_STL_DEBUG_POINTER(_Binary_op2, "inner_product_of_func");
  for (; _First1 != _Last1; ++_First1, ++_First2)
    _Val = _Binary_op1(_Val,_Binary_op2(*_First1, *_First2));
  return _Val;
}

template<class InputIterator, class OutputIterator>
inline OutputIterator
partial_sum(InputIterator _First,
            InputIterator _Last,
            OutputIterator _Result)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "partial_sum");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last),"partial_sum");
  if (_First == _Last)
    return _Result;
  *_Result = *_First;
  for (OutputIterator front = _Result; ++_First!=_Last; ++front)
    *++_Result = *front + *_First;
  return ++_Result;
}

template<class InputIterator, class OutputIterator, class BinaryOperation>
inline OutputIterator
partial_sum(InputIterator _First,
            InputIterator _Last,
            OutputIterator _Result,
            BinaryOperation _Binary_op)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "partial_sum_of_func");
  MINI_STL_DEBUG_POINTER_FOR_N(_Result, DISTANCE(_First, _Last),"partial_sum_of_func");
  MINI_STL_DEBUG_POINTER(_Binary_op, "adjacent_difference_of_func");
  if (_First == _Last)
    return _Result;
  *_Result = *_First;
  for (OutputIterator front = _Result; ++_First!=_Last; ++front)
    *++_Result = _Binary_op(*front, *_First);
  return ++_Result;
}

MINI_STL_END
#endif // MINI_STL_NUMERIC_H
