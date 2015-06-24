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
  Type operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs + rhs;
  }
};

template <class Type>
struct minus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs - rhs;
  }
};

template <class Type>
struct multiplies : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs * rhs;
  }
};

template <class Type>
struct divides : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs / rhs;
  }
};

template <class Type>
struct modulus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs % rhs;
  }
};

template <class Type>
struct negete : public unary_function<Type, Type>
{
  Type operator()(const Type& val) const
  {
    return -val;
  }
};

template <class Type>
struct equal_to : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs == rhs;
  }
};

template <class Type>
struct not_equal_to : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs != rhs;
  }
};

template <class Type>
struct greater : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return rhs < lhs;
  }
};

template <class Type>
struct less : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs < rhs;
  }
};

template <class Type>
struct greater_equal : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return rhs <= lhs;
  }
};

template <class Type>
struct less_equal : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs <= rhs;
  }
};

template <class Type>
struct logical_and : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs && rhs;
  }
};

template <class Type>
struct logical_or : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& lhs, const Type& rhs) const
  {
    return lhs || rhs;
  }
};

template <class Type>
struct logical_not : public unary_function<Type,bool>
{
  bool operator()(const Type& x) const
  {
    return !x;
  }
};

template <class Type>
struct identity : public unary_function<Type,Type>
{
  const Type& operator()(const Type& x) const
  {
    return x;
  }
};

template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type>
{
  const typename Pair::first_type& operator()(const Pair& x) const
  {
    return x.first;
  }
};

MINI_STL_END
#endif // MINI_STL_FUNCTION_H
