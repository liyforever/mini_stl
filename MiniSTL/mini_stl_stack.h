#ifndef MINI_STL_STACK_H
#define MINI_STL_STACK_H
#include "mini_stl_deque.h"
MINI_STL_BEGIN

template <class Type, class Container = _MY_STL::deque<Type> >
class stack
{
public:
  typedef typename Container::value_type value_type;
  typedef typename Container::size_type  size_type;
  typedef typename Container::reference  reference;
  typedef typename Container::const_reference const_reference;
  typedef typename Container container_type;
private:
  Container Myc_;
public:
  explicit stack() : Myc_() {}
  explicit stack(const container_type& _Right) : Myc_(_Right) {}
public:
  void push(const Type& _Val)
  {
    Myc_.push_back(_Val);
  }

  void push(Type&& _Val)
  {
    Myc_.push_back(_MY_STL::move(_Val));
  }

  void pop()
  {
    Myc_.pop_back();
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  size_type size() const
  {
    return Myc_.size();
  }

  reference top()
  {
    return Myc_.back();
  }

  const_reference top() const
  {
    return Myc_.back();
  }

  bool operator!=(const stack& _Right)
  {
    return this->Myc_ != _Right.Myc_;
  }

  bool operator<(const stack& _Right)
  {
    return this->Myc_ < _Right.Myc_;
  }

  bool operator<=(const stack& _Right)
  {
    return this->Myc_ <= _Right.Myc_;
  }

  bool operator==(const stack& _Right)
  {
    return this->Myc_ == _Right.Myc_;
  }

  bool operator>(const stack& _Right)
  {
    return this->Myc_ > _Right.Myc_;
  }

  bool operator>=(const stack& _Right)
  {
    return this->Myc_ >= _Right.Myc_;
  }
};

MINI_STL_END
#endif // MINI_STL_STACK_H
