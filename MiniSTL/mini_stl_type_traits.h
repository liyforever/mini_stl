#ifndef MINI_STL___true_type_H
#define MINI_STL___true_type_H
#include "mini_stl_config.h"

MINI_STL_BEGIN
struct __true_type
{

};

struct __false_type
{

};

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
MINI_STL_END
#endif // MINI_STL___true_type_H
