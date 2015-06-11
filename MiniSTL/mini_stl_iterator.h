#ifndef MINI_STL_ITERATOR_H
#define MINI_STL_ITERATOR_H
#include "mini_stl_config.h"
#include "mini_stl_type_traits.h"
#include <stddef.h>
MINI_STL_BEGIN

struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};


template <class Category, class Type, class Distance = ptrdiff_t,
          class Pinter = Type*, class Reference = Type&>
struct my_iterator_base
{
  typedef Category                iterator_category;
  typedef Type                    value_type;
  typedef ptrdiff_t               difference_type;
  typedef Type*                   pointer;
  typedef const Type*             const_pointer;
  typedef Type&                   reference;
  typedef const Type&             const_reference;
};
#ifdef MINI_STL_CLASS_PARTIAL_SPECIALIZATION

template <class Iterator>
struct iterator_traits
{
  typedef typename Iterator::iterator_category iterator_category;
  typedef typename Iterator::value_type        value_type;
  typedef typename Iterator::difference_type   difference_type;
  typedef typename Iterator::pointer           pointer;
  typedef typename Iterator::reference         reference;
};

template <class Type>
struct iterator_traits<Type*>
{
  typedef random_access_iterator_tag iterator_category;
  typedef Type                       value_type;
  typedef ptrdiff_t                  difference_type;
  typedef Type*                      pointer;
  typedef Type&                      reference;
};

template <class Type>
struct iterator_traits<const Type*>
{
  typedef random_access_iterator_tag iterator_category;
  typedef Type                       value_type;
  typedef ptrdiff_t                  difference_type;
  typedef Type*                      pointer;
  typedef Type&                      reference;
};


template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
ITERATOR_CATEGORY(const Iterator&)
{
  typedef typename iterator_traits<Iterator>::iterator_category Category;
  return Category();
}

template <class _Iter>
inline typename iterator_traits<_Iter>::value_type*
VALUE_TYPE(const _Iter&)
{
  return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
}

template <class Iter>
inline typename iterator_traits<Iter>::difference_type*
DISTANCE_TYPE(const Iter&)
{
  return static_cast<typename iterator_traits<Iter>::difference_type*>(0);
}

#else // MINI_STL_CLASS_PARTIAL_SPECIALIZATION


#endif

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance_aux(InputIterator first, InputIterator last,
             input_iterator_tag)
{
  typename iterator_traits<InputIterator>::difference_type n = 0;
  while (first++ != last)
    ++n;
  return n;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
distance_aux(InputIterator first, InputIterator last,
             random_access_iterator_tag)
{
  return last - first;
}

template <class InputIterator>
inline typename iterator_traits<InputIterator>::difference_type
DISTANCE(InputIterator first, InputIterator last)
{
  typedef typename iterator_traits<InputIterator>::iterator_category
      category;
  return distance_aux(first, last, category());
}
MINI_STL_END
#endif // MINI_STL_ITERATOR_H
