#ifndef MINI_STL___true_type_H
#define MINI_STL___true_type_H
#include "mini_stl_config.h"

MINI_STL_BEGIN
struct __true_type {};
//has_trivaial constructor and destructor
struct __false_type {};
//has notrivaial constructor and destructor
template <class type>
struct _type_traits
{
  typedef __false_type  has_trivial_default_constructor;
  typedef __false_type  has_trivial_copy_constructor;
  typedef __false_type  has_trivial_assignment_operator;
  typedef __false_type  has_trivial_destructor;
  typedef __false_type  is_POD_type;
};

template <>
struct _type_traits<char>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<signed char>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<unsigned char>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<short>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<unsigned short>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<int>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<unsigned int>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<long>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<unsigned long>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<float>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<double>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

template <>
struct _type_traits<long double>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};

#ifdef MINI_STL_CLASS_PARTIAL_SPECIALIZATION
template <class T>
struct _type_traits<T*>
{
  typedef __true_type  has_trivial_default_constructor;
  typedef __true_type  has_trivial_copy_constructor;
  typedef __true_type  has_trivial_assignment_operator;
  typedef __true_type  has_trivial_destructor;
  typedef __true_type  is_POD_type;
};
#else //MINI_STL_CLASS_PARTIAL_SPECIALIZATION
template <>
struct _type_traits<int*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<const int*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<signed char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<unsigned char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<const char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<const signed char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};

template <>
struct _type_traits<const unsigned char*>
{
  typedef __true_type    has_trivial_default_constructor;
  typedef __true_type    has_trivial_copy_constructor;
  typedef __true_type    has_trivial_assignment_operator;
  typedef __true_type    has_trivial_destructor;
  typedef __true_type    is_POD_type;
};
#endif

struct Identity {};
//aux class distinguish Iterator or built-in type

template <class Iterator>
struct is_iterator
{
  typedef Identity ID;
};

template <>
struct is_iterator<char> {};

template <>
struct is_iterator<unsigned char> {};

template <>
struct is_iterator<signed char> {};

template <>
struct is_iterator<short> {};

template <>
struct is_iterator<unsigned short> {};

template <>
struct is_iterator<int> {};

template <>
struct is_iterator<unsigned int> {};

template <>
struct is_iterator<long> {};

template <>
struct is_iterator<unsigned long> {};

template <>
struct is_iterator<float> {};

template <>
struct is_iterator<double> {};

template <>
struct is_iterator<long double> {};

#ifdef MINI_STL_RVALUE_REFS
template<class Type>
struct remove_reference
{
  typedef Type type;
};

template<class Type>
struct remove_reference<Type&>
{
  typedef Type type;
};

template<class Type>
struct remove_reference<Type&&>
{
  typedef Type type;
};

template<class Type> inline
typename remove_reference<Type>::type&&
  move(Type&& _Arg)
{
  return (typename remove_reference<Type>::type&&)
      (_Arg);
}
#endif //MINI_STL_RVALUE_REFS

MINI_STL_END
#endif // MINI_STL___true_type_H
