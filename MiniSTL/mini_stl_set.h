#ifndef MINI_STL_SET_H
#define MINI_STL_SET_H
#include "mini_stl_tree.h"

MINI_STL_BEGIN

template <class Key,
          class Compare = less<Key>,
          class Alloc = default_allocator>
class set
{
public:
  typedef Key           key_type;
  typedef Key           value_type;
  typedef Compare       key_compare;
  typedef Compare       value_compare;
private:
  typedef rb_tree<key_type,value_type,identity<value_type>,
                      key_compare, Alloc> RB_tree_type;
  typedef typename RB_tree_type::iterator  RB_iterator;
  RB_tree_type c_;
public:
  typedef typename RB_tree_type::const_pointer    pointer;
  typedef typename RB_tree_type::const_pointer    const_pointer;
  typedef typename RB_tree_type::const_reference  reference;
  typedef typename RB_tree_type::const_reference  const_reference;
  typedef typename RB_tree_type::const_iterator   iterator;
  typedef typename RB_tree_type::const_iterator   const_iterator;
  typedef typename RB_tree_type::size_type        size_type;
  typedef typename RB_tree_type::difference_type  difference_type;
  typedef typename RB_tree_type::allocator_type   allocator_type;
  typedef typename RB_tree_type::const_reverse_iterator reverse_iterator;
  typedef typename RB_tree_type::const_reverse_iterator const_reverse_iterator;
public:
  explicit set(const Compare& comp = Compare(),
               const allocator_type&/*Al*/=allocator_type())
    : c_(comp) {}

  set(const set& right)
    : c_(right.c_) {}

  template<class InputIterator>
    set(InputIterator first,
        InputIterator last,
        const Compare& comp = Compare(),
        const allocator_type&/*Al*/=allocator_type())
    : c_(comp)
  {
    c_.insert_unique(first, last);
  }

#ifdef MINI_STL_RVALUE_REFS
  set(set&& right)
    : c_(move(right.c_)) {}

  set& operator=(set&& right)
  {
    c_ = move(right.c_);
    return *this;
  }

#endif
  set& operator=(const set& right)
  {
    c_ = right.c_;
    return *this;
  }

public:
  allocator_type get_allocator() const
  {
    return c_.get_allocator();
  }

  iterator begin()
  {
    return c_.begin();
  }

  iterator end()
  {
    return c_.end();
  }

  const_reverse_iterator rbegin()
  {
    return c_.end();
  }

  const_reverse_iterator rend()
  {
    return c_.begin();
  }

  const_iterator cbegin() const
  {
    return c_.begin();
  }

  const_iterator cend() const
  {
    return c_.end();
  }

  const_reverse_iterator crbegin() const
  {
    return c_.end();
  }

  const_reverse_iterator crend() const
  {
    return c_.begin();
  }

  void clear()
  {
    c_.clear();
  }

  size_type size() const
  {
    return c_.size();
  }

  size_type count(const key_type& key) const
  {
    return c_.find(key) == c_.end() ? 0 : 1;
  }

  bool empty() const
  {
    return c_.empty();
  }

  void swap(set& rhs)
  {
    this->c_.swap(rhs.c_);
  }

  size_type max_size() const
  {
    return c_.max_size();
  }

  iterator lower_bound(const key_type& k) const
  {
    return c_.lower_bound(k);
  }

  iterator upper_bound(const key_type& k) const
  {
    return c_.upper_bound(k);
  }

  pair<iterator,iterator> equal_range(const key_type& k) const
  {
    return c_.equal_range(k);
  }

  iterator find(const key_type& k) const
  {
    return c_.find(k);
  }

  pair<iterator, bool> insert(const value_type& val)
  {
    pair<RB_iterator, bool> p = c_.insert_unique(val);
    return pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator position, const value_type& val)
  {
    RB_iterator p = c_.insert_unique((RB_iterator&)position, val);
    return p;
  }
  template<class InputIterator>
    void insert(
          InputIterator first,
          InputIterator last
      )
  {
    c_.insert_unique(first, last);
  }
#ifdef MINI_STL_RVALUE_REFS
  pair<iterator, bool>
    insert(value_type&& val)
  {
    pair<RB_iterator, bool> p = c_.insert_unique(Mini_STL::move(val));
    return pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator position,
            value_type&& val)
  {
    RB_iterator p = c_.insert_unique((RB_iterator&)position, Mini_STL::move(val));
    return p;
  }

#endif
  void erase(iterator position)
  {
    c_.erase((RB_iterator&)position);
  }

  void erase(iterator first, iterator last)
  {
    c_.erase((RB_iterator&)first,
             (RB_iterator&)last);
  }

  size_type erase(const key_type& k)
  {
    return c_.erase(k);
  }

  key_compare key_comp() const
  {
    return c_.key_comp();
  }

  value_compare value_comp() const
  {
    return c_.key_comp();
  }

  bool operator !=(const set &rhs)
  {
    return this->c_ != rhs.c_;
  }

  bool operator <(const set &rhs)
  {
    return this->c_ < rhs.c_;
  }

  bool operator <=(const set &rhs)
  {
    return this->c_ <= rhs.c_;
  }

  bool operator ==(const set &rhs)
  {
    return this->c_ == rhs.c_;
  }

  bool operator >(const set &rhs)
  {
    return this->c_ > rhs.c_;
  }

  bool operator >=(const set &rhs)
  {
    return this->c_ >= rhs.c_;
  }
};

template <class Key,class Compare,class Alloc>
inline void swap(set<Key,Compare,Alloc>& lhs,
                 set<Key,Compare,Alloc>& rhs)
{
  lhs.swap(rhs);
}

MINI_STL_END
#endif // MINI_STL_SET_H
