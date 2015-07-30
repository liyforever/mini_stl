#ifndef MINI_STL_MULTIMAP_H
#define MINI_STL_MULTIMAP_H
#include "mini_stl_tree.h"

MINI_STL_BEGIN

template <class Key,
          class Type,
          class Compare = _MY_STL::less<Key>,
          class Alloc = _MY_STL::default_allocator>
class multimap
{
public:
  typedef Key           key_type;
  typedef Type          mapped_type;
  typedef Compare       key_compare;
  typedef pair<const Key,Type>  value_type;
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
    friend class multimap<Key,Type,Compare,Alloc>;
  protected:
    Compare comp;
    value_compare(Compare _Cmp) : comp(_Cmp) {}
  public:
    bool operator()(const value_type& _Left, const value_type& _Right) const
    {
    return comp(_Left.first, _Right.first);
    }
  };
public:
  explicit multimap(const Compare& _Comp = Compare(),
                 const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "multimap Invalid comp");
  }

  multimap(const multimap& _Right)
    : Myc_(_Right.Myc_) {}

  template<class InputIterator>
    multimap(InputIterator _First,
        InputIterator _Last,
        const Compare& _Comp = Compare(),
        const allocator_type&/*Al*/=allocator_type())
    : Myc_(_Comp)
  {
    MINI_STL_DEBUG_POINTER(_Comp, "multimap Invalid comp");
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "multimap map");
    Myc_.insert_equal(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  multimap(multimap&& _Right)
    : Myc_(_MY_STL::move(_Right.Myc_))
  {}

  multimap& operator=(multimap&& _Right)
  {
    Myc_ = _MY_STL::move(_Right.Myc_);
    return *this;
  }
#endif
  multimap& operator=(const multimap& _Right)
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
    return Myc_.count(_Key);
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  void swap(multimap& _Right)
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

  iterator insert(const value_type& _Val)
  {
    RB_iterator  p = Myc_.insert_equal(_Val);
    return p;
  }

  iterator insert(iterator _Position, const value_type& _Val)
  {
    RB_iterator p = Myc_.insert_equal((RB_iterator&)_Position, _Val);
    return p;
  }

  template<class InputIterator>
    void insert(InputIterator _First,
                InputIterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "multimap insert");
    Myc_.insert_equal(_First, _Last);
  }
#ifdef MINI_STL_RVALUE_REFS
    iterator insert(value_type&& _Val)
  {
    RB_iterator p = Myc_.insert_equal(_MY_STL::move(_Val));
    return p;
  }

  iterator insert(iterator _Position,
                  value_type&& _Val)
  {
    RB_iterator p = Myc_.insert_equal((RB_iterator&)_Position, _MY_STL::move(_Val));
    return p;
  }
#endif
  void erase(iterator _Position)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(),
                             DISTANCE(this->begin(), _Position),
                             "multimap erase");
    Myc_.erase((RB_iterator&)_Position);
  }

  void erase(iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "multmap erase");
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

  bool operator !=(const multimap &_Right)
  {
    return this->Myc_ != _Right.Myc_;
  }

  bool operator <(const multimap &_Right)
  {
    return this->Myc_ < _Right.Myc_;
  }

  bool operator <=(const multimap &_Right)
  {
    return this->Myc_ <= _Right.Myc_;
  }

  bool operator ==(const multimap &_Right)
  {
    return this->Myc_ == _Right.Myc_;
  }

  bool operator >(const multimap &_Right)
  {
    return this->Myc_ > _Right.Myc_;
  }

  bool operator >=(const multimap &_Right)
  {
    return this->Myc_ >= _Right.Myc_;
  }
};

template <class Key,class Type,class Compare,class Alloc>
inline void swap(multimap<Key,Type,Compare,Alloc>& _Left,
                 multimap<Key,Type,Compare,Alloc>& _Right)
{
  _Left.swap(_Right);
}

MINI_STL_END
#endif // MINI_STL_MULTIMAP_H
