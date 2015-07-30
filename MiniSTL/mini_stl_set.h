#ifndef MINI_STL_SET_H
#define MINI_STL_SET_H
#include "mini_stl_tree.h"

MINI_STL_BEGIN

template <class Key,
          class Compare = _MY_STL::less<Key>,
          class Alloc = _MY_STL::default_allocator>
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
  RB_tree_type Myc_;
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
  explicit set(const Compare& _Comp = Compare(),
               const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "set Invalid comp");
  }

  set(const set& _Right)
    : Myc_(_Right.Myc_) {}

  template<class InputIterator>
    set(InputIterator _First,
        InputIterator _Last,
        const Compare& _Comp = Compare(),
        const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "set Invalid comp");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "set set(F,L)");
    Myc_.insert_unique(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  set(set&& _Right)
    : Myc_(_MY_STL::move(_Right.Myc_))
  {}

  set& operator=(set&& _Right)
  {
    Myc_ = _MY_STL::move(_Right.Myc_);
    return *this;
  }
#endif
  set& operator=(const set& _Right)
  {
    Myc_ = _Right.Myc_;
    return *this;
  }

public:
  allocator_type get_allocator() const
  {
    return Myc_.get_allocator();
  }

  iterator begin()
  {
    return Myc_.cbegin();
  }

  iterator end()
  {
    return Myc_.cend();
  }

  const_reverse_iterator rbegin()
  {
    return Myc_.crbegin();
  }

  const_reverse_iterator rend()
  {
    return Myc_.crend();
  }

  const_iterator cbegin() const
  {
    return Myc_.cbegin();
  }

  const_iterator cend() const
  {
    return Myc_.cend();
  }

  const_reverse_iterator crbegin() const
  {
    return Myc_.crbegin();
  }

  const_reverse_iterator crend() const
  {
    return Myc_.crend();
  }

  void clear()
  {
    Myc_.clear();
  }

  size_type size() const
  {
    return Myc_.size();
  }

  size_type count(const key_type& _Key) const
  {
    return Myc_.find(_Key) == Myc_.end() ? 0 : 1;
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  void swap(set& _Right)
  {
    this->Myc_.swap(_Right.Myc_);
  }

  size_type max_size() const
  {
    return Myc_.max_size();
  }

  iterator lower_bound(const key_type& _Key) const
  {
    return Myc_.lower_bound(_Key);
  }

  iterator upper_bound(const key_type& _Key) const
  {
    return Myc_.upper_bound(_Key);
  }

  pair<iterator,iterator> equal_range(const key_type& _Key) const
  {
    return Myc_.equal_range(_Key);
  }

  iterator find(const key_type& _Key) const
  {
    return Myc_.find(_Key);
  }

  _MY_STL::pair<iterator, bool>
  insert(const value_type& _Val)
  {
    _MY_STL::pair<RB_iterator, bool> p = Myc_.insert_unique(_Val);
    return pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator _Position, const value_type& _Val)
  {
    RB_iterator p = Myc_.insert_unique((RB_iterator&)_Position, _Val);
    return p;
  }

  template<class InputIterator>
  void insert(InputIterator _First,
              InputIterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "set insert");
    Myc_.insert_unique(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  _MY_STL::pair<iterator, bool>
    insert(value_type&& _Val)
  {
    _MY_STL::pair<RB_iterator, bool> p = Myc_.insert_unique(_MY_STL::move(_Val));
    return _MY_STL::pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator _position, value_type&& _Val)
  {
    RB_iterator p = Myc_.insert_unique((RB_iterator&)_position,
                                       _MY_STL::move(_Val));
    return p;
  }
#endif

  void erase(iterator _Position)
  {
    MINI_STL_DEBUG_CHECK_POS(Myc_.size(), DISTANCE(this->begin(), _Position), "set erase");
    Myc_.erase((RB_iterator&)_Position);
  }

  void erase(iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "set erase");
    Myc_.erase((RB_iterator&)_First,
             (RB_iterator&)_Last);
  }

  size_type erase(const key_type& _Key)
  {
    return Myc_.erase(_Key);
  }

  key_compare key_comp() const
  {
    return Myc_.key_comp();
  }

  value_compare value_comp() const
  {
    return Myc_.key_comp();
  }

  bool operator !=(const set &_Right)
  {
    return this->Myc_ != _Right.Myc_;
  }

  bool operator <(const set &_Right)
  {
    return this->Myc_ < _Right.Myc_;
  }

  bool operator <=(const set &_Right)
  {
    return this->Myc_ <= _Right.Myc_;
  }

  bool operator ==(const set &_Right)
  {
    return this->Myc_ == _Right.Myc_;
  }

  bool operator >(const set &_Right)
  {
    return this->Myc_ > _Right.Myc_;
  }

  bool operator >=(const set &_Right)
  {
    return this->Myc_ >= _Right.Myc_;
  }
};

template <class Key,class Compare,class Alloc>
inline void swap(set<Key,Compare,Alloc>& _Left,
                 set<Key,Compare,Alloc>& _Right)
{
  _Left.swap(_Right);
}

MINI_STL_END
#endif // MINI_STL_SET_H
