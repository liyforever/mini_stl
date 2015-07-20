#ifndef MINI_STL_HASHTABLE_H
#define MINI_STL_HASHTABLE_H
#include "memory.h"
#include "mini_stl_list.h"
#include "mini_stl_vector.h"
#include "mini_stl_hash_fun.h"

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
}

const float _MAX_FACTOR = 0.75;
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
  typedef _MY_STL::vector<node,Alloc> bucket_type;
public:
  typedef typename node::iterator LI;
  typedef typename node::const_iterator LCI;
  typedef typename node::reverse_iterator LRI;
  typedef typename node::const_reverse_iterator LRCI;
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
  bucket_type          buckets_;
  size_type            num_count_;
  float                max_factor_;
public:
  hashtable(size_type n,
            const HashFcn& hf = HashFcn(),
            const EqualKey& eql = EqualKey(),
            const ExtractKey& ext = ExtractKey(),
            const allocator_type& /*Al*/ = allocator_type())
    : hash_(hf),
      equals_(eql),
      get_key_(ext),
      num_count_(0),
      max_factor_(_MAX_FACTOR)
  {
    _init_bucket(n);
  }

  hashtable(const hashtable& ht)
    : hash_(ht.hash_),
      equals_(ht.equals_),
      get_key_(ht.get_key_),
      num_count_(ht.num_count_),
      max_factor_(ht.max_factor_)
  {
    _copy_from(ht);
  }

#ifdef MINI_STL_RVALUE_REFS
  hashtable(hashtable&& ht)
    : hash_(ht.hash_),
      equals_(ht.equals_),
      get_key_(ht.get_key_),
      num_count_(ht.num_count_),
      max_factor_(ht.max_factor_),
      buckets_(_MY_STL::move(ht.buckets_))
  {}

  hashtable& operator=(hashtable&& ht)
  {
    if (&ht != this) {
      hash_ = ht.hash_;
      equals_ = ht.equals_;
      get_key_ = ht.get_key_;
      num_count_ = ht.num_count_;
      max_factor_ = ht.max_factor_;
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
      max_factor_ = ht.max_factor_;
      _copy_from(ht);
    }
  }
public:
  iterator begin()
  {
    for(size_type index=0; index<buckets_.size(); ++index)
      if (!buckets_[index].empty())
        return iterator(buckets_[index].begin(), this, index);

    return end();
  }

  const_iterator begin() const
  {
    for(size_type index=0; index<buckets_.size(); ++index)
      if (!buckets_[index].empty())
        return const_iterator(buckets_[index].begin(), this, index);
    return end();
  }

  LI begin(size_type nbucket)
  {
    return buckets_[nbucket].begin();
  }

  LCI begin(size_type nbucket) const
  {
    return buckets_[nbucket].begin();
  }

  iterator end()
  {
    return iterator(buckets_[buckets_.size()-1].end(), this, buckets_.size()-1);
  }

  const_iterator end() const
  {
    return const_iterator(buckets_[buckets_.size()-1].end(), this, buckets_.size()-1);
  }

  LI end(size_type nbucket)
  {
    return buckets_[nbucket].end();
  }

  LCI end(size_type nbucket) const
  {
    return buckets_[nbucket].end();
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

  hasher hash_function() const
  {
    return hasher();
  }

  key_equal key_eq() const
  {
    return key_equal();
  }

  float load_factor() const
  {
    return static_cast<float>(size()) /
        static_cast<float>(bucket_count());
  }

  void clear()
  {
    buckets_.clear();
    num_count_ = 0;
  }

  size_type bucket(const Key& keyval) const
  {
    const size_type index = _get_index_for_key(keyval);
    return buckets_[index].size();
  }

  size_type bucket_size(size_type nbucket) const
  {
    return buckets_[nbucket].size();
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

  float max_load_factor() const
  {
    return max_factor_;
  }

  void max_load_factor(float factor)
  {
    this->max_factor_ = factor;
  }

  void swap(hashtable& ht)
  {
    _MY_STL::swap(this->equals_, ht.equals_);
    _MY_STL::swap(this->hash_, ht.hash_);
    _MY_STL::swap(this->get_key_, ht.get_key_);
    _MY_STL::swap(this->num_count_, ht.num_count_);
    _MY_STL::swap(this->max_factor_, ht.max_factor_);
    buckets_.swap(ht.buckets_);
  }

  pair<iterator, bool> insert_unique(const value_type& val)
  {
    _check_size();
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
#ifdef MINI_STL_RVALUE_REFS
  pair<iterator, bool> insert_unique(value_type&& val)
  {
    _check_size();
    bool ok = true;
    size_type index = _get_index(val);
    LI result = buckets_[index].begin();
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
          (iterator(buckets_[index].insert(buckets_[index].end(),_MY_STL::move(val)),
                    this,index),
           true);
    }
    else
      return pair<iterator,bool>
          (iterator(result, this, index), false);
  }
#endif
  template <class InputIterator>
  void insert_unique(InputIterator first, InputIterator last)
  {
    while (first != last)
      insert_unique(*first);
  }

  iterator insert_equal(const value_type& val)
  {
    _check_size();
    size_type index = _get_index(val);
    ++num_count_;

    for(LI position = buckets_[index].begin();
        position!=buckets_[index].end();
        ++position)
      if(equals_(get_key_(*position), get_key_(val)))
        return iterator(buckets_[index].insert(position, val),
                    this, index);
    return iterator(buckets_[index].insert(buckets_[index].end(), val),
                this, index);
  }
#ifdef MINI_STL_RVALUE_REFS
  iterator insert_equal(value_type&& val)
  {
    _check_size();
    size_type index = _get_index(val);
    ++num_count_;

    for(LI position = buckets_[index].begin();
        position!=buckets_[index].end();
        ++position)
      if(equals_(get_key_(*position), get_key_(val)))
        return iterator(buckets_[index].insert(position, _MY_STL::move(val)),
                    this, index);
    return iterator(buckets_[index].insert(buckets_[index].end(),
                                           _MY_STL::move(val)),
                this, index);
  }
#endif
  template <class InputIterator>
  void insert_equal(InputIterator first, InputIterator last)
  {
    while (first != last)
      insert_equal(*first);
  }

  iterator find(const key_type& k)
  {
    size_type index = _get_index_for_key(k);
    LI result;// = buckets_[index].begin();
    for (result = buckets_[index].begin();
         result!=buckets_[index].end() &&
         !equals_(get_key_(*result), k);
         ++result)
      {}
    if (result==buckets_[index].end())
      return end();
    else
      return iterator(result, this, index);
  }

  const_iterator find(const key_type& k) const
  {
    size_type index = _get_index_for_key(k);
    LI result;// = buckets_[index].begin();
    for (result = buckets_[index].begin();
         result!=buckets_[index].end() &&
         !equals_(get_key_(*result), k);
         ++result)
      {}
    if (result==buckets_[index].end())
      return end();
    else
      return const_iterator(result, this, index);
  }

  size_type count(const key_type& k) const
  {
    const size_type index = _get_index_for_key(k);
    size_type result = 0;

    for (LCI cur = buckets_[index].begin();
         cur != buckets_[index].end(); ++cur)
      if (equals_(get_key_(*cur), k))
        ++result;
    return result;
  }

  pair<iterator, iterator>
  equal_range(const key_type& k);

  pair<const_iterator, const_iterator>
  equal_range(const key_type& k) const;

  size_type erase(const key_type& k);
  void erase(const iterator& i);
  void erase(iterator first, iterator last);

  void erase(const const_iterator& it);
  void erase(const_iterator first, const_iterator last);

  void rehash(size_type nbuckets);

  bool operator==(const hashtable& rhs)
  {
    return this->buckets_ == rhs.buckets_;
  }

  bool operator!=(const hashtable& rhs)
  {
    return this->buckets_ != rhs.buckets_;
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

  void _check_size(size_type nbuckets = buckets_.size()+1,
                   bool checkOrResash = false);
};

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
_check_size(size_type nbuckets, bool checkOrResash)
{
  if (load_factor() > max_factor_ || checkOrResash) {
    const size_type new_bucket_size = _next_size(nbuckets);
    bucket_type tmp_bucket(new_bucket_size, node());
    for(int i=0; i<buckets_.size(); ++i)
      if (!buckets_[i].empty()) {
        for (size_type size_for_node = buckets_[i].size();
             size_for_node >0; --size_for_node) {
          size_type index = _get_index(*buckets_[i].begin(), new_bucket_size);
          tmp_bucket[index].splice(tmp_bucket[index].begin(),buckets_[i],buckets_[i].begin());
        }
      }
    buckets_.swap(tmp_bucket);
  }
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator,
     typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator>
hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::equal_range(const key_type& k)
{
  typedef pair<iterator, iterator> PII;
  const size_type index = _get_index_for_key(k);
  iterator PII_first = find(k);
  LRI PII_second = buckets_[index].rbegin();
  for (; PII_second!=buckets_[index].rend();++PII_second)
    if (equals_(get_key_(*PII_second),k))
      return PII(PII_first,
                 ++iterator(--PII_second.base(), this, index));

  return PII(end(), end());
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::const_iterator,
     typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::const_iterator>
hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::equal_range(const key_type& k) const
{
  typedef pair<const_iterator, const_iterator> PCICI;
  const size_type index = _get_index_for_key(k);
  const_iterator PII_first = find(k);
  LRCI PII_second = buckets_[index].rbegin();
  for (; PII_second!=buckets_[index].rend();++PII_second)
    if (equals_(get_key_(*PII_second),k))
      return PCICI(PII_first,
                 ++iterator(--PII_second.base(), this, index));

  return PCICI(end(), end());
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::size_type
hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(const key_type& k)
{
  const size_type index = _get_index_for_key(k);
  size_type erase_num = 0;

  if (!buckets_[index].empty()) {
    LI cur = buckets_[index].begin();
    LI next = cur;
    ++next;
    while (cur!=buckets_[index].end()) {
      if (equals_(get_key_(*cur), k)) {
        ++erase_num;
        --num_count_;
        buckets_[index].erase(cur);
        cur = next;
        ++next;
      } else {
        ++cur;
        next = cur;
        ++next;
      }
    }
  }
  return erase_num;
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(const iterator& i)
{
  const size_type index = _get_index(*i);
  --num_count_;
  buckets_[index].erase(i.cur);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(const const_iterator& i)
{
  const size_type index = _get_index(*i);
  --num_count_;
  buckets_[index].erase(i.cur);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(iterator first, iterator last)
{
  while (first!=last)
    erase(first++);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(const_iterator first, const_iterator last)
{
  while (first!=last)
    erase(first++);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
rehash(size_type nbuckets)
{
  _check_size(nbuckets, true);
}

MINI_STL_END
#endif // MINI_STL_HASHTABLE_H
