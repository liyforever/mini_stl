#ifndef MINI_STL_FUNCTION_H
#define MINI_STL_FUNCTION_H
#include "mini_stl_config.h"
#include <functional>
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

template <class Predicate>
class unary_negate :
    public unary_function<typename Predicate::argument_type, bool>
{
private:
  Predicate Mypred_;
public:
  explicit unary_negate(const Predicate& _Pred) : Mypred_(_Pred) {}
  bool operator()(const typename Predicate::argument_type& _Val) const
  {
    return !Mypred_(_Val);
  }
};

template <class Predicate>
inline unary_negate<Predicate> not1(const Predicate& _Pred)
{
  return unary_negate<Predicate>(_Pred);
}

template <class BinaryPredicate>
class binary_negate :
    public binary_function<typename BinaryPredicate::first_argument_type,
                           typename BinaryPredicate::second_argument_type,
                           bool>
{
private:
  BinaryPredicate Mypred_;
public:
  explicit binary_negate(const BinaryPredicate& _Pred) : Mypred_(_Pred) {}
  bool operator()(const typename BinaryPredicate::first_argument_type& _Left,
                  const typename BinaryPredicate::second_argument_type& _Right) const
  {
    return !Mypred_(_Left, _Right);
  }
};

template <class BinaryPredicate>
inline binary_negate< BinaryPredicate> not2(const BinaryPredicate& _Pred)
{
  return binary_negate<BinaryPredicate>(_Pred);
}

template <class Operation>
class binder1st
    : public unary_function<typename Operation::second_argument_type,
                            typename Operation::result_type>
{
private:
  Operation Myoper_;
  typename Operation::first_argument_type Myval_;
public:
  binder1st(const Operation& _Oper,
            const typename Operation::first_argument_type _Val)
    : Myoper_(_Oper), Myval_(_Val)
  {}

  typename Operation::result_type
  operator() (const typename Operation::second_argument_type _Val) const
  {
    return Myoper_(Myval_, _Val);
  }
};

template <class Operation, class Type>
inline binder1st<Operation>
bind1nd(const Operation& _Op, const Type& _Val)
{
  typedef typename Operation::first_argument arg1_type;
  return binder1st<Operation>(_Op, _Val);
}

template <class Operation>
class binder2st
    : public unary_function<typename Operation::first_argument_type,
                            typename Operation::result_type>
{
private:
  Operation Myoper_;
  typename Operation::second_argument_type Myval_;
public:
  binder2st(const Operation& _Oper,
            const typename Operation::second_argument_type _Val)
    : Myoper_(_Oper), Myval_(_Val)
  {}

  typename Operation::result_type
  operator() (const typename Operation::first_argument_type _Val) const
  {
    return Myoper_(_Val, Myval_);
  }
};

template <class Operation, class Type>
inline binder2st<Operation>
bind2nd(const Operation& _Op, const Type& _Val)
{
  typedef typename Operation::second_argument arg1_type;
  return binder2st<Operation>(_Op, _Val);
}
MINI_STL_END
#endif // MINI_STL_FUNCTION_H
