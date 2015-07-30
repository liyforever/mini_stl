#ifndef MINI_STL_MULTISET_H
#define MINI_STL_MULTISET_H
#include "mini_stl_tree.h"

MINI_STL_BEGIN

template <class Key,
          class Compare = _MY_STL::less<Key>,
          class Alloc = _MY_STL::default_allocator>
class multiset
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
  explicit multiset(const Compare& _Comp = Compare(),
               const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "multset Invalid comp");
  }

  multiset(const multiset& _Right)
    : Myc_(_Right.Myc_)
  {}

  template<class InputIterator>
    multiset(InputIterator _First,
        InputIterator _Last,
        const Compare& _Comp = Compare(),
        const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "multset Invalid comp");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "multset set(F,L)");
    Myc_.insert_equal(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  multiset(multiset&& _Right)
    : Myc_(_MY_STL::move(_Right.Myc_)) {}

  multiset& operator=(multiset&& _Right)
  {
    Myc_ = _MY_STL::move(_Right.Myc_);
    return *this;
  }

#endif
  multiset& operator=(const multiset& _Right)
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
    return Myc_.begin();
  }

  iterator end()
  {
    return Myc_.end();
  }

  const_reverse_iterator rbegin()
  {
    return Myc_.rbegin();
  }

  const_reverse_iterator rend()
  {
    return Myc_.rend();
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
    return Myc_.count(_Key);
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  void swap(multiset& _Right)
  {
    Myc_.swap(_Right.Myc_);
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

  _MY_STL::pair<iterator,iterator>
    equal_range(const key_type& _Key) const
  {
    return Myc_.equal_range(_Key);
  }

  iterator find(const key_type& _Key) const
  {
    return Myc_.find(_Key);
  }

  iterator insert(const value_type& _Val)
  {
    return Myc_.insert_equal(_Val);
  }

  iterator insert(iterator _Position, const value_type& _Val)
  {
    RB_iterator p = Myc_.insert_equal((RB_iterator&)_Position, _Val);
    return p;
  }
  template<class InputIterator>
    void insert(
          InputIterator _First,
          InputIterator _Last
      )
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "multiset insert");
    Myc_.insert_equal(_First, _Last);
  }
#ifdef MINI_STL_RVALUE_REFS
  iterator insert(value_type&& _Val)
  {
    return Myc_.insert_equal(_MY_STL::move(_Val));
  }

  iterator insert(iterator _Position,
            value_type&& _Val)
  {
    RB_iterator p = Myc_.insert_equal((RB_iterator&)_Position,
                                      _MY_STL::move(_Val));
    return p;
  }

#endif
  void erase(iterator _Position)
  {
    MINI_STL_DEBUG_CHECK_POS(Myc_.size(), DISTANCE(this->begin(), _Position),
                             "multiset erase");
    Myc_.erase((RB_iterator&)_Position);
  }

  void erase(iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "multiset erase");
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

  bool operator !=(const multiset &_Right)
  {
    return this->Myc_ != _Right.Myc_;
  }

  bool operator <(const multiset &_Right)
  {
    return this->Myc_ < _Right.Myc_;
  }

  bool operator <=(const multiset &_Right)
  {
    return this->Myc_ <= _Right.Myc_;
  }

  bool operator ==(const multiset &_Right)
  {
    return this->Myc_ == _Right.Myc_;
  }

  bool operator >(const multiset &_Right)
  {
    return this->Myc_ > _Right.Myc_;
  }

  bool operator >=(const multiset &_Right)
  {
    return this->Myc_ >= _Right.Myc_;
  }
};

template <class Key,class Compare,class Alloc>
inline void swap(multiset<Key,Compare,Alloc>& _Left,
                 multiset<Key,Compare,Alloc>& _Right)
{
  _Left.swap(_Right);
}

MINI_STL_END
#endif // MINI_STL_MULTISET_H
