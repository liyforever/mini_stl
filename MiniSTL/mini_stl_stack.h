#ifndef MINI_STL_STACK_H
#define MINI_STL_STACK_H
#include "mini_stl_deque.h"
MINI_STL_BEGIN

template <class Type, class Container = deque<Type> >
class stack
{
public:
  typedef typename Container::value_type value_type;
  typedef typename Container::size_type  size_type;
  typedef typename Container::reference  reference;
  typedef typename Container::const_reference const_reference;
  typedef typename Container container_type;
public:
  explicit stack() : c_() {}
  explicit stack(const container_type& rhs) : c_(rhs) {}
public:
  void push(const Type& val)
  {
    c_.push_back(val);
  }

  void pop()
  {
    c_.pop_back();
  }

  bool empty() const
  {
    return c_.empty();
  }

  size_type size() const
  {
    return c_.size();
  }

  reference top()
  {
    return c_.back();
  }

  const_reference top() const
  {
    return c_.back();
  }

  bool operator!=(const stack& rhs)
  {
    return this->c_ != rhs.c_;
  }

  bool operator<(const stack& rhs)
  {
    return this->c_ < rhs.c_;
  }

  bool operator<=(const stack& rhs)
  {
    return this->c_ <= rhs.c_;
  }

  bool operator==(const stack& rhs)
  {
    return this->c_ == rhs.c_;
  }

  bool operator>(const stack& rhs)
  {
    return this->c_ > rhs.c_;
  }

  bool operator>=(const stack& rhs)
  {
    return this->c_ >= rhs.c_;
  }
private:
  Container c_;
};

MINI_STL_END
#endif // MINI_STL_STACK_H
