#ifndef MINI_STL_UNORDERED_MAP_H
#define MINI_STL_UNORDERED_MAP_H
#include "mini_stl_hashtable.h"

MINI_STL_BEGIN

template<class Key,
    class Type,
    class Hash = _MY_STL::hash<Key>,
    class EqualKey = _MY_STL::equal_to<Key>,
    class Alloc = _MY_STL::default_allocator >
class unordered_map
{
private:
  typedef _MY_STL::hashtable<_MY_STL::pair<const Key, Type>, Key, Hash,
                             select1st<pair<const Key, Type>, EqualKey, Alloc>
    HT_type;
  HT_type c_;
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
  unordered_map()
    : c_(52, hasher(), key_equal())
  {}

  explicit unordered_map(
      size_type n = 52,
      const hasher& hfn = hasher(),
      const EqualKey& comp = EqualKey(),
      const Alloc&/*AL*/ = Alloc())
    : c_(n, hfn, comp)
  {}

  template<class InputIterator>
    unordered_map(
      InputIterator first,
      InputIterator last,
      size_type nbuckets = 52,
      const Hash& hfn = Hash(),
      const EqualKey& comp = EqualKey(),
      const Alloc& /*AL*/ = Alloc())
    : c_(nbuckets, hfn, comp)
  {
    c_.insert_unique(first, last);
  }

  unordered_map(const unordered_map& right)
    : c_(right.c_)
  {}
#ifdef MINI_STL_RVALUE_REFS
  unordered_map(unordered_map&& right)
    : c_(_MY_STL::move(right))
  {}

  unordered_map& operator=(unordered_map&& right)
  {
    c_ = _MY_STL::move(right.c_);
    return *this;
  }
#endif
  unordered_map& operator=(const unordered_map& right)
  {
    c_ = right.c_;
    return *this;
  }
public:
  allocator_type get_allocator() const
  {
    return c_.get_allocator();
  }

  iterator begin() const
  {
    return c_.begin();
  }

  const_iterator cbegin() const
  {
    return c_.cbegin();
  }

  local_iterator begin(size_type nbucket) const
  {
    return c_.begin(nbucket);
  }

  iterator end() const
  {
    return c_.end();
  }

  local_iterator end(size_type nbucket) const
  {
    return c_.end(nbucket);
  }

  const_iterator cend() const
  {
    return c_.cend();
  }

  size_type bucket(const Key& keyval) const
  {
    return c_.bucket(keyval);
  }

  size_type bucket_count() const
  {
    return c_.bucket_count();
  }

  size_type bucket_size(size_type nbucket) const
  {
    return c_.bucket_size(nbucket);
  }

  void clear()
  {
    c_.clear();
  }

  size_type count(const Key& keyval) const
  {
    return c_.count(keyval);
  }

  bool empty() const
  {
    return c_.empty();
  }

  _MY_STL::pair<iterator, iterator>
        equal_range(const Key& keyval) const
  {
    return c_.equal_range(keyval);
  }

  void erase(iterator position)
  {
    c_.erase(position);
  }

  void erase(iterator first, iterator last)
  {
    c_.erase(first, last);
  }

  size_type erase(const Key& keyval)
  {
    return c_.erase(keyval);
  }

  const_iterator find(const Key& keyval) const
  {
    return c_.find(keyval);
  }

  hasher hash_function() const
  {
    return c_.hash_function();
  }

  key_equal key_eq() const
  {
    return c_.key_eq();
  }

  float load_factor() const
  {
    return c_.load_factor();
  }

  size_type max_bucket_count() const
  {
    return c_.max_bucket_count();
  }

  float max_load_factor() const
  {
    return c_.max_load_factor();
  }

  void max_load_factor(float factor)
  {
    c_.max_load_factor(factor);
  }

  size_type max_size() const
  {
    return c_.max_size();
  }

  void rehash(size_type nbuckets)
  {
    return c_.rehash(nbuckets);
  }

  size_type size() const
  {
    return c_.size();
  }

  _MY_STL::pair<iterator, bool> insert(const value_type& val)
  {
    return c_.insert_unique(val);
  }

  template<class InputIterator>
    void insert(InputIterator first, InputIterator last)
  {
    c_.insert_unique(first, last);
  }

#ifdef MINI_STL_RVALUE_REFS
  _MY_STL::pair<iterator, bool> insert(value_type&& val)
  {
    return c_.insert_unique(_MY_STL::move(val));
  }
#endif

  void swap(unordered_map& right)
  {
    c_.swap(right.c_);
  }

  bool operator==(const unordered_map& rhs)
  {
    return c_ == rhs.c_;
  }

  bool operator!=(const unordered_map& rhs)
  {
    return c_ != rhs.c_;
  }
};

template<class Key,
    class Type,
    class Hash,
    class EqualKey,
    class Alloc>
void swap(unordered_map<Key,Type,Hash,EqualKey,Alloc>& lhs,
          unordered_map<Key,Type,Hash,EqualKey,Alloc>& rhs)
{
  lhs.swap(rhs);
}
MINI_STL_END
#endif // MINI_STL_UNORDERED_MAP_H
