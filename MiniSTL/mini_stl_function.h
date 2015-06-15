#ifndef MINI_STL_FUNCTION_H
#define MINI_STL_FUNCTION_H
#include "mini_stl_config.h"
MINI_STL_BEGIN
template <class Arg, class Result>
struct unary_function
{
  typedef Arg argument_type;
  typedef Result result_type;
};

template <class Type>
struct negete : public unary_function<Type, Type>
{
  Type operator()(const Type& val) const
  {
    return -val;
  }
};

template <class Arg1, class Arg2, class Result>
struct binary_function
{
  typedef Arg1  first_argument_type;
  typedef Arg2  second_argument_type;
  typedef Result result_type;
};

template <class Type>
struct plus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs + rhs;
  }
};

template <class Type>
struct minus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs - rhs;
  }
};

template <class Type>
struct multiplies : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs * rhs;
  }
};

template <class Type>
struct divides : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs / rhs;
  }
};

template <class Type>
struct modulus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs % rhs;
  }
};

template <class Type>
struct greater : public binary_function<Type,Type,bool>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return rhs < lhs;
  }
};

template <class Type>
struct less : public binary_function<Type,Type,bool>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs < rhs;
  }
};

template <class Type>
struct greater_equal : public binary_function<Type,Type,bool>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return rhs <= lhs;
  }
};

template <class Type>
struct less_equeal : public binary_function<Type,Type,bool>
{
  Type operator()(const Type& lhs, const Type& rhs)
  {
    return lhs <= rhs;
  }
};

MINI_STL_END
#endif // MINI_STL_FUNCTION_H
