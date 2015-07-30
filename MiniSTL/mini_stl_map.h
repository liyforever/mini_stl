#ifndef MINI_STL_MAP_H
#define MINI_STL_MAP_H
#include "mini_stl_tree.h"

MINI_STL_BEGIN

template <class Key,
          class Type,
          class Compare = less<Key>,
          class Alloc = default_allocator>
class map
{
public:
  typedef Key           key_type;
  typedef Type          mapped_type;
  typedef Compare       key_compare;
  typedef _MY_STL::pair<const Key,Type>  value_type;
private:
  typedef rb_tree<key_type,value_type,select1st<value_type>,
                        key_compare, Alloc> RB_tree_type;
  typedef typename RB_tree_type::iterator  RB_iterator;
  RB_tree_type Myc_;
public:
  typedef typename RB_tree_type::pointer          pointer;
  typedef typename RB_tree_type::const_pointer    const_pointer;
  typedef typename RB_tree_type::reference        reference;
  typedef typename RB_tree_type::const_reference  const_reference;
  typedef typename RB_tree_type::iterator         iterator;
  typedef typename RB_tree_type::const_iterator   const_iterator;
  typedef typename RB_tree_type::size_type        size_type;
  typedef typename RB_tree_type::difference_type  difference_type;
  typedef typename RB_tree_type::allocator_type   allocator_type;
  typedef typename RB_tree_type::reverse_iterator       reverse_iterator;
  typedef typename RB_tree_type::const_reverse_iterator const_reverse_iterator;
  class value_compare
    : public binary_function<value_type,value_type,bool>
  {
    friend class map<Key,Type,Compare,Alloc>;
  protected:
    Compare comp;
    value_compare(Compare cmp) : comp(cmp) {}
  public:
    bool operator()(const value_type& lhs, const value_type& rhs) const
    {
    return comp(lhs.first, rhs.first);
    }
  };
public:
  explicit map(const Compare& _Comp = Compare(),
                 const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
    {
      MINI_STL_DEBUG_POINTER(_Comp, "map Invalid comp");
    }

  map(const map& _Right)
    : Myc_(_Right.Myc_) {}

  template<class InputIterator>
    map(InputIterator _First,
        InputIterator _Last,
        const Compare& _Comp = Compare(),
        const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "map Invalid comp");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "map map");
    Myc_.insert_unique(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  map(map&& _Right)
    : Myc_(_MY_STL::move(_Right.Myc_)) {}

  map& operator=(map&& _Right)
  {
    Myc_ = _MY_STL::move(_Right.Myc_);
    return *this;
  }
#endif
  map& operator=(const map& _Right)
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

  const_iterator begin() const
  {
    return Myc_.begin();
  }

  iterator end()
  {
    return Myc_.end();
  }

  const_iterator end() const
  {
    return Myc_.end();
  }

  reverse_iterator rbegin()
  {
    return Myc_.rbegin();
  }

  const_reverse_iterator rbegin() const
  {
    return Myc_.rbegin();
  }

  reverse_iterator rend()
  {
    return Myc_.rend();
  }

  const_reverse_iterator rend() const
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

  Type& at(const Key& _Key)
  {
    return (*((insert(value_type(_Key,Type()))).first)).second;
  }

  const Type& at(const Key& _Key) const
  {
    return (*((insert(value_type(_Key,Type()))).first)).second;
  }

  Type& operator[](const Key& _Key)
  {
    return (*((insert(value_type(_Key,Type()))).first)).second;
  }

  size_type count(const key_type& _Key) const
  {
    return Myc_.find(_Key) == Myc_.end() ? 0 : 1;
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  void swap(map& _Right)
  {
    this->Myc_.swap(_Right.Myc_);
  }

  size_type max_size() const
  {
    return Myc_.max_size();
  }

  iterator lower_bound(const key_type& _Key)
  {
    return Myc_.lower_bound(_Key);
  }

  const_iterator lower_bound(const key_type& _Key) const
  {
    return Myc_.lower_bound(_Key);
  }

  iterator upper_bound(const key_type& _Key)
  {
    return Myc_.upper_bound(_Key);
  }

  iterator upper_bound(const key_type& _Key) const
  {
    return Myc_.upper_bound(_Key);
  }

  pair<iterator,iterator> equal_range(const key_type& _Key)
  {
    return Myc_.equal_range(_Key);
  }

  pair<const_iterator,const_iterator> equal_range(const key_type& _Key) const
  {
    return Myc_.equal_range(_Key);
  }

  iterator find(const key_type& _Key)
  {
    return Myc_.find(_Key);
  }

  const_iterator find(const key_type& _Key) const
  {
    return Myc_.find(_Key);
  }

  _MY_STL::pair<iterator, bool> insert(const value_type& _Val)
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
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "map insert");
    Myc_.insert_unique(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  _MY_STL::pair<iterator, bool>
    insert(value_type&& _Val)
  {
    _MY_STL::pair<RB_iterator, bool> p =
            Myc_.insert_unique(_MY_STL::move(_Val));
    return pair<iterator, bool>(p.first, p.second);
  }

  iterator insert(iterator _Position,
            value_type&& _Val)
  {
    RB_iterator p = Myc_.insert_unique((RB_iterator&)_Position,
                                       _MY_STL::move(_Val));
    return p;
  }
#endif

  void erase(iterator _Position)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(),
                             DISTANCE(this->begin(), _Position),
                             "map erase");
    Myc_.erase((RB_iterator&)_Position);
  }

  void erase(iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "map erase");
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
    return value_compare(Myc_.key_comp());
  }

  bool operator !=(const map &_Right)
  {
    return this->Myc_ != _Right.Myc_;
  }

  bool operator <(const map &_Right)
  {
    return this->Myc_ < _Right.Myc_;
  }

  bool operator <=(const map &_Right)
  {
    return this->Myc_ <= _Right.Myc_;
  }

  bool operator ==(const map &_Right)
  {
    return this->Myc_ == _Right.Myc_;
  }

  bool operator >(const map &_Right)
  {
    return this->Myc_ > _Right.Myc_;
  }

  bool operator >=(const map &_Right)
  {
    return this->Myc_ >= _Right.Myc_;
  }
};

template <class Key,class Type,class Compare,class Alloc>
inline void swap(map<Key,Type,Compare,Alloc>& _Left,
                 map<Key,Type,Compare,Alloc>& _Right)
{
  _Left.swap(_Right);
}

MINI_STL_END
#endif // MINI_STL_MAP_H
