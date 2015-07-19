#ifndef MINI_STL_HASHTABLE_H
#define MINI_STL_HASHTABLE_H
#include "memory.h"
#include "mini_stl_list.h"
#include "mini_stl_vector.h"
#include "mini_stl_hash_fun.h"
#include <unordered_map>
#define private public
#define protected public
MINI_STL_BEGIN

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
class hashtable;

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey,
          class Alloc, class ListIterator>
struct HT_iterator;

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
struct HT_iterator
{
  typedef hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>
         hashtable;
  typedef HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>
         iterator;
  typedef _list_node<Val> node;
  typedef bidirectional_iterator_tag iterator_category;
  typedef Val value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef Val& reference;
  typedef Val* pointer;

  ListIterator cur;
  hashtable* ht_ptr;
  size_type index;
  HT_iterator(ListIterator li, hashtable* ht, size_type i)
    : cur(li), ht_ptr(ht), index(i){}
  HT_iterator() {}

  reference operator*() const
  {
    return *cur;
  }

  pointer operator->() const
  {
    return &(operator*());
  }

  iterator& operator++();

  iterator operator++(int)
  {
    iterator tmp = *this;
    ++*this;
    return tmp;
  }

  iterator& operator--();

  iterator operator--(int)
  {
    iterator tmp = *this;
    --*this;
    return tmp;
  }

  bool operator==(const iterator& it) const
  {
    return this->cur == it.cur;
  }

  bool operator!=(const iterator& it) const
  {
    return this->cur != it.cur;
  }
};

enum { _num_primes = 28 };

static const unsigned long _prime_list[_num_primes] =
{
  53ul,         97ul,         193ul,       389ul,       769ul,
  1543ul,       3079ul,       6151ul,      12289ul,     24593ul,
  49157ul,      98317ul,      196613ul,    393241ul,    786433ul,
  1572869ul,    3145739ul,    6291469ul,   12582917ul,  25165843ul,
  50331653ul,   100663319ul,  201326611ul, 402653189ul, 805306457ul,
  1610612741ul, 3221225473ul, 4294967291ul
};

inline unsigned long _next_prime(unsigned long n)
{
  const unsigned long *first = _prime_list;
  const unsigned long *last = _prime_list + _num_primes;
  const unsigned long *pos = lower_bound(first, last, n);
  return pos == last ? *(last - 1) : *pos;
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>&
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>::operator++()
{
  ++cur;
  if (cur!=ht_ptr->buckets_[index].end()) {
    return *this;
  } else {
    while (++index < ht_ptr->buckets_.size())
      if (!ht_ptr->buckets_[index].empty()) {
        cur = ht_ptr->buckets_[index].begin();
        return *this;
      }
  }
  --index;
  cur = ht_ptr->buckets_[index].end();
  return *this;


      //__list_node_base* next;)
  /*ListIterator old = cur;
  ++cur;
  size_type index = ht_ptr->_get_index(*old);
  if (cur == ht_ptr->buckets_[index].end()) {
    while (++index < ht_ptr->buckets_.size())
      if (!ht_ptr->buckets_[index].empty()) {
        cur = ht_ptr->buckets_[index].begin();
        break;
      }
  }
  return *this;*/
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>&
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>::operator--()
{
  if (cur!=ht_ptr->buckets_[index].begin()) {
    --cur;
    return *this;
  } else {
    while (--index >= 0)
      if (!ht_ptr->buckets_[index].empty()) {
        cur = --ht_ptr->buckets_[index].end();
        return *this;
      }
  }
  ++index;
  cur = ht_ptr->buckets_[index].end();
  return *this;
  /*ListIterator old = cur;
  --cur;
  size_type index = ht_ptr->_get_index(*old);
  //cout << "index:" << index << endl;
  if (cur == ht_ptr->buckets_[index].begin()) {
    while (--index >= 0)
      if (!ht_ptr->buckets_[index].empty()) {
        cur = --ht_ptr->buckets_[index].end();
        break;
      }
  }
  return *this;*/
}


template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
class hashtable
{
public:
  typedef Key               key_type;
  typedef Val               value_type;
  typedef HashFcn           hasher;
  typedef EqualKey          key_equal;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef Alloc             allocator_type;
private:
  typedef _MY_STL::list<Val>  node;
  typedef typename node::iterator LI;
  typedef typename node::const_iterator LCI;
public:
  typedef HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,LI>
          iterator;
  typedef HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,LCI>
          const_iterator;
  typedef _MY_STL::reverse_iterator<iterator> reverse_iterator;
  typedef _MY_STL::reverse_iterator<const_iterator> const_reverse_iterator;
private:
  friend struct
  HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,LI>;
  friend struct
  HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,LCI>;
private:
  hasher               hash_;
  key_equal            equals_;
  ExtractKey           get_key_;
  vector<node, Alloc>  buckets_;
  size_type            num_count_;
public:
  hashtable(size_type n,
            const HashFcn& hf = HashFcn(),
            const EqualKey& eql = EqualKey(),
            const ExtractKey& ext = ExtractKey(),
            const allocator_type& /*Al*/ = allocator_type())
    : hash_(hf),
      equals_(eql),
      get_key_(ext),
      num_count_(0)
  {
    _init_bucket(n);
  }

  hashtable(const hashtable& ht)
    : hash_(ht.hash_),
      equals_(ht.equals_),
      get_key_(ht.get_key_),
      num_count_(ht.num_count_)
  {
    _copy_from(ht);
  }

#ifdef MINI_STL_RVALUE_REFS
  hashtable(hashtable&& ht)
    : hash_(ht.hash_),
      equals_(ht.equals_),
      get_key_(ht.get_key_),
      num_count_(ht.num_count_)
  {
    buckets_ = _MY_STL::move(ht.buckets_);
  }

  hashtable& operator=(hashtable&& ht)
  {
    if (&ht != this) {
      hash_ = ht.hash_;
      equals_ = ht.equals_;
      get_key_ = ht.get_key_;
      num_count_ = ht.num_count_;
      buckets_ = move(ht.buckets_);
    }
  }
#endif
  hashtable& operator=(const hashtable& ht)
  {
    if (&ht != this) {
      hash_ = ht.hash_;
      equals_ = ht.equals_;
      get_key_ = ht.get_key_;
      num_count_ = ht.num_count_;
      _copy_from(ht);
    }
  }
public:
  iterator begin()
  {
    for(size_type index=0; index<buckets_.size(); ++index)
      if (!buckets_[index].empty()) {
          cout << "index:" << index << endl;
        return iterator(buckets_[index].begin(), this, index);
        }
    return end();
  }

  const_iterator begin() const
  {
    for(size_type index=0; index<buckets_.size(); ++index)
      if (!buckets_[index].empty())
        return const_iterator(buckets_[index].begin(), this, index);
    return end();
  }

  iterator end()
  {
    return iterator(buckets_[buckets_.size()-1].end(), this, buckets_.size()-1);
  }

  const_iterator end() const
  {
    return const_iterator(buckets_[buckets_.size()-1].end(), this, buckets_.size()-1);
  }

  reverse_iterator rbegin()
  {
    return reverse_iterator(end());
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(end());
  }

  reverse_iterator rend()
  {
    return reverse_iterator(begin());
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(begin());
  }

  const_iterator cbegin() const
  {
    return begin();
  }

  const_iterator cend() const
  {
    return end();
  }

  const_reverse_iterator crbegin() const
  {
    return rbegin();
  }

  const_reverse_iterator crend() const
  {
    return rend();
  }

  size_type bucket_count() const
  {
    return buckets_.size();
  }

  size_type max_bucket_count() const
  {
    return _prime_list[_num_primes  - 1];
  }

  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  size_type size() const
  {
    return num_count_;
  }

  size_type max_size() const
  {
    return static_cast<size_type>(-1);
  }

  bool empty() const
  {
    return size() == 0;
  }

  void swap(hashtable& ht)
  {
    _MY_STL::swap(this->equals_, ht.equals_);
    _MY_STL::swap(this->hash_, ht.hash_);
    _MY_STL::swap(this->get_key_, ht.get_key_);
    _MY_STL::swap(this->num_count_, ht.num_count_);
    buckets_.swap(ht.buckets_);
  }

  pair<iterator, bool> insert_unique(const value_type& val)
  {
    bool ok = true;
    size_type index = _get_index(val);
    LI result=buckets_[index].begin();
    for ( ;result!=buckets_[index].end(); ++result)
    {
      if (equals_(get_key_(val),get_key_(*result))) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ++num_count_;
      return pair<iterator,bool>
          (iterator(buckets_[index].insert(buckets_[index].end(),val),this,index),
           true);
    }
    else
      return pair<iterator,bool>
          (iterator(result, this, index), false);
  }

  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last)
  {
    while (first != last)
      insert_unique(*first);
  }

  iterator insert_equal(const value_type& val)
  {
    size_type index = _get_index(val);
    ++num_count_;
    return iterator(buckets_[index].insert(buckets_[index].end(), val),
                    this, index);
  }

  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last)
  {
    while (first != last)
      insert_equal(*first);
  }
protected:
  size_type _next_size(size_type n) const
  {
    return static_cast<size_type>(_next_prime(n));
  }

  void _init_bucket(size_type n)
  {
    const size_type bucket_count = _next_size(n);
    buckets_.resize(bucket_count, node());
    num_count_ = 0;
  }

  void _copy_from(const hashtable& ht)
  {
    buckets_.clear();
    buckets_.reserve(ht.buckets_.size());
    buckets_ = ht.buckets_;
  }

  size_type _get_index(const value_type& val, size_type n) const
  {
    return _get_index_for_key(get_key_(val), n);
  }

  size_type _get_index(const value_type &val) const
  {
    return _get_index_for_key(get_key_(val));
  }

  size_type _get_index_for_key(const key_type& k) const
  {
    return _get_index_for_key(k, buckets_.size());
  }

  size_type _get_index_for_key(const key_type &k, size_type n) const
  {
    return hash_(k) % n;
  }

  void resize_(size_type num);
};

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
resize(size_type num)
{
  const size_type old_bucket_num = buckets_.size();
  if (num > old_bucket_num) {
    const size_type n = _next_prime(num);

  }
}

MINI_STL_END
#endif // MINI_STL_HASHTABLE_H
