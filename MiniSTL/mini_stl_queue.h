#ifndef MINI_STL_QUEUE_H
#define MINI_STL_QUEUE_H
#include "mini_stl_deque.h"
MINI_STL_BEGIN

template <class Type, class Container = deque<Type> >
class queue
{
public:
  typedef typename Container::value_type value_type;
  typedef typename Container::size_type  size_type;
  typedef typename Container::reference  reference;
  typedef typename Container::const_reference const_reference;
  typedef typename Container container_type;
public:
  explicit queue() : c_() {}
  explicit queue(const container_type& rhs) : c_(rhs) {}
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

  reference front()
  {
    return c_.front();
  }

  const_reference front() const
  {
    return c_.front();
  }

  reference back()
  {
    return c_.back();
  }

  const_reference back() const
  {
    return c_.back();
  }

  bool operator!=(const queue& rhs)
  {
    return this->c_ != rhs.c_;
  }

  bool operator<(const queue& rhs)
  {
    return this->c_ < rhs.c_;
  }

  bool operator<=(const queue& rhs)
  {
    return this->c_ <= rhs.c_;
  }

  bool operator==(const queue& rhs)
  {
    return this->c_ == rhs.c_;
  }

  bool operator>(const queue& rhs)
  {
    return this->c_ > rhs.c_;
  }

  bool operator>=(const queue& rhs)
  {
    return this->c_ >= rhs.c_;
  }
private:
  Container c_;
};

template <class Type, class Container = vector<Type>,
          class Compare = less<typename Container::value_type> >
class priority_queue
{
public:
  typedef Container cotainer_type;
  typedef typename Container::value_type  value_type;
  typedef typename Container::size_type   size_type;
  typedef typename Container::reference   reference;
  typedef typename Container::const_reference const_reference;
private:
  Container c;
  Compare comp;
public:
  explicit priority_queue() : c() {}
  explicit priority_queue(const Compare& cmp)
    : c(), comp(cmp)
  {}
#ifdef MINI_STL_MEMBER_TEMPLATES
  template <class InputIterator>
  priority_queue(InputIterator first,
                 InputIterator last,
                 const Compare& cmp)
    : c(first,last), comp(cmp)
  {
    make_heap(c.begin(), c.end(), comp);
  }
#endif
  bool empty() const
  {
    return c.empty();
  }

  size_type size() const
  {
    return c.size();
  }

  reference top()
  {
    return c.front();
  }

  const_reference top() const
  {
    return c.front();
  }

  void push(const value_type& val)
  {
    c.push_back(val);
    push_heap(c.begin(), c.end(), comp);
  }

  void pop()
  {
    pop_heap(c.begin(), c.end(), comp);
    c.pop_back();
  }
};

MINI_STL_END
#endif // MINI_STL_QUEUE_H
