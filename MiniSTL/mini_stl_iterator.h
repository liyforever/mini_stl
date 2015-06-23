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
  typedef const Type*                pointer;
  typedef const Type&                reference;
};

template <class Iterator>
class reverse_iterator
{
public:
  typedef typename iterator_traits<Iterator>::iterator_category
          iterator_category;
  typedef typename iterator_traits<Iterator>::difference_type
          difference_type;
  typedef typename iterator_traits<Iterator>::pointer
          pointer;
  typedef typename iterator_traits<Iterator>::reference
          reference;
  typedef typename iterator_traits<Iterator>::value_type
          value_type;
  typedef reverse_iterator<Iterator>    Self;
  typedef Iterator  iterator_type;
private:
  Iterator baseIter_;
public:
  reverse_iterator() {}
  explicit reverse_iterator(iterator_type x) : baseIter_(x) {}
  reverse_iterator(const Self& x) : baseIter_(x.baseIter_) {}
  template <class Iter>
  reverse_iterator(const reverse_iterator<Iter>& x)
    : baseIter_(x.base()) {}
public:
  iterator_type base() const
  {
    return baseIter_;
  }

  reference operator*() const
  {
    Iterator tmp = baseIter_;
    return *--tmp;
  }

  Self& operator++()
  {
    --baseIter_;
    return *this;
  }

  Self operator++(int)
  {
    Self tmp = *this;
    --baseIter_;
    return tmp;
  }

  Self& operator--()
  {
    ++baseIter_;
    return *this;
  }

  Self operator--(int)
  {
    Self tmp = *this;
    ++baseIter_;
    return tmp;
  }

  Self operator+(difference_type n) const
  {
    return Self(baseIter_ - n);
  }

  Self& operator+=(difference_type n)
  {
    baseIter_ -= n;
    return *this;
  }

  Self operator-(difference_type n) const
  {
    return Self(baseIter_ + n);
  }

  Self& operator-=(difference_type n)
  {
    baseIter_ += n;
    return *this;
  }

  reference operator[](difference_type pos) const
  {
    return *(*this + pos);
  }
  pointer operator->() const
  {
    return &(operator*());
  }
};

template <class Iterator>
inline bool operator==(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
  return lhs.base() == rhs.base();
}

template <class Iterator>
inline bool operator!=(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
  return lhs.base() != rhs.base();
}

template <class Iterator>
inline bool operator<(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
  return lhs.base() < rhs.base();
}

template <class Iterator>
inline bool operator<=(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
  return lhs.base() <= rhs.base();
}

template <class Iterator>
inline bool operator>(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
  return rhs.base() < lhs.base();
}

template <class Iterator>
inline bool operator>=(const reverse_iterator<Iterator>& lhs,
                       const reverse_iterator<Iterator>& rhs)
{
  return rhs.base() <= lhs.base();
}

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
