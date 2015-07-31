#ifndef MINI_STL_QUEUE_H
#define MINI_STL_QUEUE_H
#include "mini_stl_deque.h"
MINI_STL_BEGIN

template <class Type, class Container = _MY_STL::deque<Type> >
class queue
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
  explicit queue() : Myc_() {}
  explicit queue(const container_type& _Right) : Myc_(_Right) {}
public:
  void push(const Type& val)
  {
    Myc_.push_back(val);
  }

  void pop()
  {
    Myc_.pop_front();
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  size_type size() const
  {
    return Myc_.size();
  }

  reference front()
  {
    return Myc_.front();
  }

  const_reference front() const
  {
    return Myc_.front();
  }

  reference back()
  {
    return Myc_.back();
  }

  const_reference back() const
  {
    return Myc_.back();
  }

  bool operator!=(const queue& _Right)
  {
    return this->Myc_ != _Right.Myc_;
  }

  bool operator<(const queue& _Right)
  {
    return this->Myc_ < _Right.Myc_;
  }

  bool operator<=(const queue& _Right)
  {
    return this->Myc_ <= _Right.Myc_;
  }

  bool operator==(const queue& _Right)
  {
    return this->Myc_ == _Right.Myc_;
  }

  bool operator>(const queue& _Right)
  {
    return this->Myc_ > _Right.Myc_;
  }

  bool operator>=(const queue& _Right)
  {
    return this->Myc_ >= _Right.Myc_;
  }
};

template <class Type, class Container = _MY_STL::vector<Type>,
          class Compare = _MY_STL::less<typename Container::value_type> >
class priority_queue
{
public:
  typedef Container cotainer_type;
  typedef typename Container::value_type  value_type;
  typedef typename Container::size_type   size_type;
  typedef typename Container::reference   reference;
  typedef typename Container::const_reference const_reference;
private:
  Container Myc_;
  Compare Mycomp_;
public:
  explicit priority_queue() : Myc_() {}
  explicit priority_queue(const Compare& _Comp)
    : Myc_(), Mycomp_(_Comp)
  {}

  template <class InputIterator>
  priority_queue(InputIterator _First,
                 InputIterator _Last,
                 const Compare& _Comp)
    : Myc_(_First, _Last), Mycomp_(_Comp)
  {
    _MY_STL::make_heap(Myc_.begin(), Myc_.end(), Mycomp_);
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
    return Myc_.front();
  }

  const_reference top() const
  {
    return Myc_.front();
  }

  void push(const value_type& _Val)
  {
    Myc_.push_back(_Val);
    push_heap(Myc_.begin(), Myc_.end(), Mycomp_);
  }

  void push(value_type&& _Val)
  {
    Myc_.push_back(_MY_STL::move(_Val));
    _MY_STL::push_heap(Myc_.begin(), Myc_.end(), Mycomp_);
  }

  void pop()
  {
    _MY_STL::pop_heap(Myc_.begin(), Myc_.end(), Mycomp_);
    Myc_.pop_back();
  }
};

MINI_STL_END
#endif // MINI_STL_QUEUE_H
