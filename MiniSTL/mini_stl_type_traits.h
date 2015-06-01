#ifndef MINI_STL_TYPE_TRAITS_H
#define MINI_STL_TYPE_TRAITS_H
#include "mini_stl_config.h"

MINI_STL_BEGIN
struct true_type {};
struct false_type{};

template <typename type>
struct type_traits {
    typedef true_type   this_dummy_member_must_be_first;
    typedef false_type  has_trivial_default_constructor;
    typedef false_type  has_trivial_copy_constructor;
    typedef false_type  has_trivial_assignment_operator;
    typedef false_type  has_trivial_destructor;
    typedef false_type  is_identity_type;
};

template <>
struct type_traits<char> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<signed char> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<unsigned char> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<short> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<unsigned short> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<int> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<unsigned int> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<long> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<unsigned long> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<float> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<double> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <>
struct type_traits<long double> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};

template <typename T>
struct type_traits<T*> {
    typedef true_type  has_trivial_default_constructor;
    typedef true_type  has_trivial_copy_constructor;
    typedef true_type  has_trivial_assignment_operator;
    typedef true_type  has_trivial_destructor;
    typedef true_type  is_identity_type;
};
MINI_STL_END
#endif // MINI_STL_TYPE_TRAITS_H
