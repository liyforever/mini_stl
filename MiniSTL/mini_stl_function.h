
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
  Type operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left + _Right;
  }
};

template <class Type>
struct minus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left - _Right;
  }
};

template <class Type>
struct multiplies : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left * _Right;
  }
};

template <class Type>
struct divides : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left / _Right;
  }
};

template <class Type>
struct modulus : public binary_function<Type,Type,Type>
{
  Type operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left % _Right;
  }
};

template <class Type>
struct negete : public unary_function<Type, Type>
{
  Type operator()(const Type& _Val) const
  {
    return -_Val;
  }
};

template <class Type>
struct equal_to : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left == _Right;
  }
};

template <class Type>
struct not_equal_to : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left != _Right;
  }
};

template <class Type>
struct greater : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left > _Right;
  }
};

template <class Type>
struct less : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left < _Right;
  }
};

template <class Type>
struct greater_equal : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Right <= _Left;
  }
};

template <class Type>
struct less_equal : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left <= _Right;
  }
};

template <class Type>
struct logical_and : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left && _Right;
  }
};

template <class Type>
struct logical_or : public binary_function<Type,Type,bool>
{
  bool operator()(const Type& _Left, const Type& _Right) const
  {
    return _Left || _Right;
  }
};

template <class Type>
struct logical_not : public unary_function<Type,bool>
{
  bool operator()(const Type& _Val) const
  {
    return !_Val;
  }
};

template <class Type>
struct identity : public unary_function<Type,Type>
{
  const Type& operator()(const Type& _Val) const
  {
    return _Val;
  }
};

template <class Pair>
struct select1st : public unary_function<Pair, typename Pair::first_type>
{
  const typename Pair::first_type& operator()(const Pair& _Val) const
  {
    return _Val.first;
  }
};

template <class Pair>
struct select2st : public unary_function<Pair, typename Pair::second_type>
{
  const typename Pair::second_type& operator()(const Pair& _Val) const
  {
    return _Val.second;
  }
};
MINI_STL_END
#endif // MINI_STL_FUNCTION_H
