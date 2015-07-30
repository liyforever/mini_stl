#ifndef MINI_STL_UNORDERED_MULTIMAP_H
#define MINI_STL_UNORDERED_MULTIMAP_H
#include "mini_stl_hashtable.h"

MINI_STL_BEGIN

template<class Key,
    class Type,
    class Hash = _MY_STL::hash<Key>,
    class EqualKey = _MY_STL::equal_to<Key>,
    class Alloc = _MY_STL::default_allocator >
class unordered_multimap
{
private:
  typedef _MY_STL::hashtable<_MY_STL::pair<const Key, Type>, Key, Hash,
                             select1st<pair<const Key, Type> >, EqualKey, Alloc>
    HT_type;
  HT_type Myc_;
public:
  typedef typename HT_type::key_type        key_type;
  typedef typename HT_type::value_type      value_type;
  typedef typename HT_type::hasher          hasher;
  typedef typename HT_type::key_equal       key_equal;
  typedef typename HT_type::size_type       size_type;
  typedef typename HT_type::difference_type difference_type;
  typedef typename HT_type::pointer         pointer;
  typedef typename HT_type::const_pointer   const_pointer;
  typedef typename HT_type::reference       reference;
  typedef typename HT_type::const_reference const_reference;
  typedef typename HT_type::iterator        iterator;
  typedef typename HT_type::const_iterator  const_iterator;
  typedef typename HT_type::allocator_type  allocator_type;
  typedef typename HT_type::LI              local_iterator;
  typedef typename HT_type::LCI             const_local_iterator;
  typedef typename HT_type::reverse_iterator reverse_iterator;
  typedef typename HT_type::const_reverse_iterator const_reverse_iterator;
public:
  unordered_multimap()
    : Myc_(52, hasher(), key_equal())
  {}

  explicit unordered_multimap(
      size_type _Count,
      const hasher& _Hfn = hasher(),
      const EqualKey& _Comp = EqualKey(),
      const Alloc&/*AL*/ = Alloc())
    : Myc_(_Count, _Hfn, _Comp)
  {
    MINI_STL_DEBUG_POINTER(_Hfn, "unordered_multimap Invalid hasher");
    MINI_STL_DEBUG_POINTER(_Comp, "unordered_multimap Invalid EqualKey");
  }

  template<class InputIterator>
    unordered_multimap(
      InputIterator _First,
      InputIterator _Last,
      size_type _Nbuckets = 52,
      const Hash& _Hfn = Hash(),
      const EqualKey& _Comp = EqualKey(),
      const Alloc& /*AL*/ = Alloc())
    : Myc_(_Nbuckets, _Hfn, _Comp)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unordered_multimap unordered_multimap()");
    Myc_.insert_equal(_First, _Last);
  }

  unordered_multimap(const unordered_multimap& _Right)
    : Myc_(_Right.Myc_)
  {}

#ifdef MINI_STL_RVALUE_REFS
  unordered_multimap(unordered_multimap&& _Right)
    : Myc_(_MY_STL::move(_Right.Myc_))
  {}

  unordered_multimap& operator=(unordered_multimap&& _Right)
  {
    Myc_ = _MY_STL::move(_Right.Myc_);
    return *this;
  }
#endif
  unordered_multimap& operator=(const unordered_multimap& _Right)
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
    return Myc_.cbegin();
  }

  const_iterator cbegin() const
  {
    return Myc_.cbegin();
  }

  local_iterator begin(size_type _Nbucket)
  {
    return Myc_.begin(_Nbucket);
  }

  const_local_iterator begin(size_type _Nbucket) const
  {
    return Myc_.begin(_Nbucket);
  }

  iterator end()
  {
    return Myc_.end();
  }

  const_iterator end() const
  {
    return Myc_.end();
  }

  local_iterator end(size_type _Nbucket)
  {
    return Myc_.end(_Nbucket);
  }

  const_local_iterator end(size_type _Nbucket) const
  {
    return Myc_.end(_Nbucket);
  }

  const_iterator cend() const
  {
    return Myc_.cend();
  }

  size_type bucket(const Key& _KeyVal) const
  {
    return Myc_.bucket(_KeyVal);
  }

  size_type bucket_count() const
  {
    return Myc_.bucket_count();
  }

  size_type bucket_size(size_type _Nbucket) const
  {
    return Myc_.bucket_size(_Nbucket);
  }

  void clear()
  {
    Myc_.clear();
  }

  size_type count(const Key& _KeyVal) const
  {
    return Myc_.count(_KeyVal);
  }

  bool empty() const
  {
    return Myc_.empty();
  }

  _MY_STL::pair<iterator, iterator>
        equal_range(const Key& _KeyVal) const
  {
    return Myc_.equal_range(_KeyVal);
  }

  void erase(iterator _Position)
  {
    Myc_.erase(_Position);
  }

  void erase(iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unordered_multimap erase");
    Myc_.erase(_First, _Last);
  }

  size_type erase(const Key& _KeyVal)
  {
    return Myc_.erase(_KeyVal);
  }

  const_iterator find(const Key& _KeyVal) const
  {
    return Myc_.find(_KeyVal);
  }

  hasher hash_function() const
  {
    return Myc_.hash_function();
  }

  key_equal key_eq() const
  {
    return Myc_.key_eq();
  }

  float load_factor() const
  {
    return Myc_.load_factor();
  }

  size_type max_bucket_count() const
  {
    return Myc_.max_bucket_count();
  }

  float max_load_factor() const
  {
    return Myc_.max_load_factor();
  }

  void max_load_factor(float _Factor)
  {
    Myc_.max_load_factor(_Factor);
  }

  size_type max_size() const
  {
    return Myc_.max_size();
  }

  void rehash(size_type _Nbuckets)
  {
    return Myc_.rehash(_Nbuckets);
  }

  size_type size() const
  {
    return Myc_.size();
  }

  iterator insert(const value_type& _Val)
  {
    return Myc_.insert_equal(_Val);
  }

  template<class InputIterator>
    void insert(InputIterator _First, InputIterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "unordered_multimap insert");
    Myc_.insert_equal(_First, _Last);
  }

#ifdef MINI_STL_RVALUE_REFS
  iterator insert(value_type&& _Val)
  {
    return Myc_.insert_equal(_MY_STL::move(_Val));
  }
#endif

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

  Type& operator[](Key&& _Key)
  {
    return (*((insert(value_type(_MY_STL::move(_Key), Type()))).first)).second;
  }

  void swap(unordered_multimap& _Right)
  {
    Myc_.swap(_Right.Myc_);
  }

  bool operator==(const unordered_multimap& _Right)
  {
    return Myc_ == _Right.Myc_;
  }

  bool operator!=(const unordered_multimap& _Right)
  {
    return Myc_ != _Right.Myc_;
  }
};

template<class Key,
    class Type,
    class Hash,
    class EqualKey,
    class Alloc>
void swap(unordered_multimap<Key,Type,Hash,EqualKey,Alloc>& _Left,
          unordered_multimap<Key,Type,Hash,EqualKey,Alloc>& _Right)
{
  _Left.swap(_Right);
}
MINI_STL_END
#endif // MINI_STL_UNORDERED_MULTIMAP_H
