#ifndef MINI_STL_HASHTABLE_H
#define MINI_STL_HASHTABLE_H
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
          Self;
  typedef _list_node<Val> node;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef typename ListIterator::pointer pointer;
  typedef typename ListIterator::reference reference;
  typedef typename ListIterator::value_type value_type;

  ListIterator cur;
  hashtable* ht_ptr;
  size_type index;
  HT_iterator()
    : ht_ptr(0), index(0)
  {}

  HT_iterator(const ListIterator& _Li, const hashtable* _Ht, size_type _Ind)
    : cur(_Li),
      ht_ptr(const_cast<hashtable*>(_Ht)),
      index(_Ind)
  {}

  template <class HT_Citerator>
  HT_iterator(const HT_Citerator& _Right)
    : cur(_Right.cur),
      ht_ptr(_Right.ht_ptr),
      index(_Right.index)
  {}

  HT_iterator(const HT_iterator& _Right)
    : cur(_Right.cur),
      ht_ptr(_Right.ht_ptr),
      index(_Right.index)
  {}

  Self& operator=(const HT_iterator& _Right)
  {
    this->cur = _Right.cur;
    this->ht_ptr = _Right.ht_ptr;
    this->index = _Right.index;
    return *this;
  }

  reference operator*() const
  {
    return cur.operator*();
  }

  pointer operator->() const
  {
    return &(operator*());
  }

  Self& operator++();

  Self operator++(int)
  {
    Self tmp = *this;
    ++*this;
    return tmp;
  }

  Self& operator--();

  Self operator--(int)
  {
    Self tmp = *this;
    --*this;
    return tmp;
  }

  bool operator==(const Self& _Iter) const
  {
    return this->cur == _Iter.cur;
  }

  bool operator!=(const Self& _Iter) const
  {
    return this->cur != _Iter.cur;
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

inline unsigned long _next_prime(unsigned long _Num)
{
  const unsigned long *first = _prime_list;
  const unsigned long *last = _prime_list + _num_primes;
  const unsigned long *pos = _MY_STL::lower_bound(first, last, _Num);
  return pos == last ? *(last - 1) : *pos;
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>&
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>::operator++()
{
  ++cur;
  if (cur!=ht_ptr->Mybuckets_[index].end()) {
    return *this;
  } else {
    while (++index < ht_ptr->Mybuckets_.size())
      if (!ht_ptr->Mybuckets_[index].empty()) {
        cur = ht_ptr->Mybuckets_[index].begin();
        return *this;
      }
  }
  --index;
  cur = ht_ptr->Mybuckets_[index].end();
  return *this;
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>&
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>::operator--()
{
  if (cur!=ht_ptr->Mybuckets_[index].begin()) {
    --cur;
    return *this;
  } else {
    while (--index >= 0)
      if (!ht_ptr->Mybuckets_[index].empty()) {
        cur = --ht_ptr->Mybuckets_[index].end();
        return *this;
      }
  }
  ++index;
  cur = ht_ptr->Mybuckets_[index].end();
  return *this;
}

const float _MAX_FACTOR = 0.75;
//hash_table load max factor

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
  hasher               Myhash_;
  key_equal            Myequals_;
  ExtractKey           Myget_key_;
  bucket_type          Mybuckets_;
  size_type            Mynum_count_;
  float                Mymax_factor_;
public:
  hashtable(size_type _Count,
            const HashFcn& _Hf = HashFcn(),
            const EqualKey& _Eql = EqualKey(),
            const ExtractKey& _Ext = ExtractKey(),
            const allocator_type& /*Al*/ = allocator_type())
    : Myhash_(_Hf),
      Myequals_(_Eql),
      Myget_key_(_Ext),
      Mynum_count_(0),
      Mymax_factor_(_MAX_FACTOR)
  {
    MINI_STL_DEBUG_POINTER(_Hf, "invalid HashFcn");
    MINI_STL_DEBUG_POINTER(_Eql, "invalid EqualKey");
    _init_bucket(_Count);
  }

  hashtable(const hashtable& _Ht)
    : Myhash_(_Ht.Myhash_),
      Myequals_(_Ht.Myequals_),
      Myget_key_(_Ht.Myget_key_),
      Mynum_count_(_Ht.Mynum_count_),
      Mymax_factor_(_Ht.Mymax_factor_)
  {
    _copy_from(_Ht);
  }

#ifdef MINI_STL_RVALUE_REFS
  hashtable(hashtable&& _Ht)
    : Myhash_(_Ht.Myhash_),
      Myequals_(_Ht.Myequals_),
      Myget_key_(_Ht.Myget_key_),
      Mynum_count_(_Ht.Mynum_count_),
      Mymax_factor_(_Ht.Mymax_factor_),
      Mybuckets_(_MY_STL::move(_Ht.Mybuckets_))
  {}

  hashtable& operator=(hashtable&& _Ht)
  {
    if (&_Ht != this) {
      _MY_STL::swap(Myhash_, _Ht.Myhash_);
      _MY_STL::swap(Myequals_, _Ht.Myequals_);
      _MY_STL::swap(Myget_key_, _Ht.Myget_key_);
      Mynum_count_ =  _Ht.Mynum_count_;
      Mybuckets_ = _MY_STL::move(_Ht.Mybuckets_);
    }
    return *this;
  }
#endif
  hashtable& operator=(const hashtable& _Ht)
  {
    if (&_Ht != this) {
      Myhash_ = _Ht.Myhash_;
      Myequals_ = _Ht.Myequals_;
      Myget_key_ = _Ht.Myget_key_;
      Mynum_count_ = _Ht.Mynum_count_;
      Mymax_factor_ = _Ht.Mymax_factor_;
      _copy_from(_Ht);
    }
    return *this;
  }
public:
  iterator begin()
  {
    for(size_type index=0; index < Mybuckets_.size(); ++index)
      if (!Mybuckets_[index].empty())
        return iterator(Mybuckets_[index].begin(), this, index);
    return end();
  }

  const_iterator begin() const
  {
    for(size_type index=0; index < Mybuckets_.size(); ++index)
      if (!Mybuckets_[index].empty())
        return const_iterator(Mybuckets_[index].cbegin(), this, index);
    return cend();
  }

  LI begin(size_type _Nbucket)
  {
    return Mybuckets_[_Nbucket].begin();
  }

  LCI begin(size_type _Nbucket) const
  {
    return Mybuckets_[_Nbucket].begin();
  }

  iterator end()
  {
    return iterator(Mybuckets_[Mybuckets_.size()-1].end(),
                    this,
                    Mybuckets_.size()-1);
  }

  const_iterator end() const
  {
    return const_iterator(Mybuckets_[Mybuckets_.size()-1].end(),
                          this,
                          Mybuckets_.size()-1);
  }

  LI end(size_type _Nbucket)
  {
    return Mybuckets_[_Nbucket].end();
  }

  LCI end(size_type _Nbucket) const
  {
    return Mybuckets_[_Nbucket].end();
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
    for (size_type index = 0; index != Mybuckets_.size(); ++index)
      Mybuckets_[index].clear();
    Mynum_count_ = 0;
  }

  size_type bucket(const Key& _KeyVal) const
  {
    return _get_index_for_key(_KeyVal);
  }

  size_type bucket_size(size_type _Nbucket) const
  {
    return Mybuckets_[_Nbucket].size();
  }

  size_type bucket_count() const
  {
    return Mybuckets_.size();
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
    return Mynum_count_;
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
    return Mymax_factor_;
  }

  void max_load_factor(float factor)
  {
    this->Mymax_factor_ = factor;
  }

  void swap(hashtable& _Ht)
  {
    _MY_STL::swap(this->Myequals_, _Ht.Myequals_);
    _MY_STL::swap(this->Myhash_, _Ht.Myhash_);
    _MY_STL::swap(this->Myget_key_, _Ht.Myget_key_);
    _MY_STL::swap(this->Mynum_count_, _Ht.Mynum_count_);
    _MY_STL::swap(this->Mymax_factor_, _Ht.Mymax_factor_);
    Mybuckets_.swap(_Ht.Mybuckets_);
  }

  _MY_STL::pair<iterator, bool> insert_unique(const value_type& _Val)
  {
    _check_size(Mybuckets_.size() + 1);
    bool ok = true;
    size_type index = _get_index(_Val);
    LI result=Mybuckets_[index].begin();
    for ( ;result!=Mybuckets_[index].end(); ++result)
    {
      if (Myequals_(Myget_key_(_Val),Myget_key_(*result))) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ++Mynum_count_;
      return _MY_STL::pair<iterator,bool>
          (iterator(Mybuckets_[index].insert(Mybuckets_[index].end(),_Val),this,index),
           true);
    }
    else
      return _MY_STL::pair<iterator,bool>
          (iterator(result, this, index), false);
  }
#ifdef MINI_STL_RVALUE_REFS
  _MY_STL::pair<iterator, bool> insert_unique(value_type&& _Val)
  {
    _check_size(Mybuckets_.size() + 1);
    bool ok = true;
    size_type index = _get_index(_Val);
    LI result = Mybuckets_[index].begin();
    for ( ;result!=Mybuckets_[index].end(); ++result)
    {
      if (Myequals_(Myget_key_(_Val),Myget_key_(*result))) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ++Mynum_count_;
      return _MY_STL::pair<iterator,bool>
          (iterator(Mybuckets_[index].insert(Mybuckets_[index].end(), _MY_STL::move(_Val)),
                    this,index),
           true);
    }
    else
      return _MY_STL::pair<iterator,bool>
          (iterator(result, this, index), false);
  }
#endif
  template <class InputIterator>
  void insert_unique(InputIterator _First, InputIterator _Last)
  {
    for (; _First != _Last; ++_First)
      insert_unique(*_First);
  }

  iterator insert_equal(const value_type& _Val)
  {
    _check_size(Mybuckets_.size() + 1);
    size_type index = _get_index(_Val);
    ++Mynum_count_;

    for(LI position = Mybuckets_[index].begin();
        position!=Mybuckets_[index].end();
        ++position)
      if(Myequals_(Myget_key_(*position), Myget_key_(_Val)))
        return iterator(Mybuckets_[index].insert(position, _Val),
                    this, index);
    return iterator(Mybuckets_[index].insert(Mybuckets_[index].end(), _Val),
                this, index);
  }
#ifdef MINI_STL_RVALUE_REFS
  iterator insert_equal(value_type&& _Val)
  {
    _check_size(Mybuckets_.size() + 1);
    size_type index = _get_index(_Val);
    ++Mynum_count_;

    for(LI position = Mybuckets_[index].begin();
        position!=Mybuckets_[index].end();
        ++position)
      if(Myequals_(Myget_key_(*position), Myget_key_(_Val)))
        return iterator(Mybuckets_[index].insert(position, _MY_STL::move(_Val)),
                    this, index);
    return iterator(Mybuckets_[index].insert(Mybuckets_[index].end(),
                                           _MY_STL::move(_Val)),
                this, index);
  }
#endif
  template <class InputIterator>
  void insert_equal(InputIterator _First, InputIterator _Last)
  {
    for (; _First != _Last; ++_First)
      insert_equal(*_First);
  }

  iterator find(const key_type& _Key)
  {
    size_type index = _get_index_for_key(_Key);
    LI result;
    for (result = Mybuckets_[index].begin();
         result!=Mybuckets_[index].end() &&
         !Myequals_(Myget_key_(*result), _Key);
         ++result)
      {}
    if (result==Mybuckets_[index].end())
      return end();
    else
      return iterator(result, this, index);
  }

  const_iterator find(const key_type& _Key) const
  {
    size_type index = _get_index_for_key(_Key);
    LI result;
    for (result = Mybuckets_[index].begin();
         result!=Mybuckets_[index].end() &&
         !Myequals_(Myget_key_(*result), _Key);
         ++result)
      {}
    if (result==Mybuckets_[index].end())
      return end();
    else
      return const_iterator(result, this, index);
  }

  size_type count(const key_type& _Key) const
  {
    const size_type index = _get_index_for_key(_Key);
    size_type result = 0;

    for (LCI cur = Mybuckets_[index].begin();
         cur != Mybuckets_[index].end(); ++cur)
      if (Myequals_(Myget_key_(*cur), _Key))
        ++result;
    return result;
  }

  pair<iterator, iterator>
  equal_range(const key_type& _Key);

  pair<const_iterator, const_iterator>
  equal_range(const key_type& _Key) const;

  size_type erase(const key_type& _Key);
  void erase(const iterator& _Iter);
  void erase(iterator _First, iterator _Last);

  void erase(const const_iterator& _Iter);
  void erase(const_iterator _First, const_iterator _Last);

  void rehash(size_type _Nbuckets);

  bool operator==(const hashtable& _Right)
  {
    return this->Mybuckets_ == _Right.Mybuckets_;
  }

  bool operator!=(const hashtable& _Right)
  {
    return this->Mybuckets_ != _Right.Mybuckets_;
  }
protected:
  size_type _next_size(size_type _Num) const
  {
    return (size_type)(_next_prime(_Num));
  }

  void _init_bucket(size_type _Num)
  {
    const size_type bucket_count = _next_size(_Num);
    Mybuckets_.resize(bucket_count, node());
    Mynum_count_ = 0;
  }

  void _copy_from(const hashtable& _Ht)
  {
    Mybuckets_.clear();
    Mybuckets_.reserve(_Ht.Mybuckets_.size());
    Mybuckets_ = _Ht.Mybuckets_;
  }

  size_type _get_index(const value_type& _Val, size_type _Num) const
  {
    return _get_index_for_key(Myget_key_(_Val), _Num);
  }

  size_type _get_index(const value_type& _Val) const
  {
    return _get_index_for_key(Myget_key_(_Val));
  }

  size_type _get_index_for_key(const key_type& _Key) const
  {
    return _get_index_for_key(_Key, Mybuckets_.size());
  }

  size_type _get_index_for_key(const key_type& _Key, size_type _Num) const
  {
    return Myhash_(_Key) % _Num;
  }

  void _check_size(size_type _Nbuckets,
                   bool _CheckOrRehash = false);
};

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
_check_size(size_type _Nbuckets, bool _CheckOrRehash)
{
  if (load_factor() > Mymax_factor_ || _CheckOrRehash) {
    const size_type new_bucket_size = _next_size(_Nbuckets);
    bucket_type tmp_bucket(new_bucket_size, node());
    for(int i=0; i < Mybuckets_.size(); ++i)
      if (!Mybuckets_[i].empty()) {
        for (size_type size_for_node = Mybuckets_[i].size();
             size_for_node >0; --size_for_node) {
          size_type index = _get_index(*Mybuckets_[i].begin(), new_bucket_size);
          tmp_bucket[index].splice(tmp_bucket[index].begin(),Mybuckets_[i],Mybuckets_[i].begin());
        }
      }
    Mybuckets_.swap(tmp_bucket);
  }
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator,
     typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::iterator>
hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::equal_range(const key_type& _Key)
{
  typedef pair<iterator, iterator> PII;
  const size_type index = _get_index_for_key(_Key);
  iterator PII_first = find(_Key);
  LRI PII_second = Mybuckets_[index].rbegin();
  for (; PII_second!=Mybuckets_[index].rend(); ++PII_second)
    if (Myequals_(Myget_key_(*PII_second),_Key))
      return PII(PII_first,
                 ++iterator(--PII_second.base(), this, index));

  return PII(end(), end());
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::const_iterator,
     typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::const_iterator>
hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::equal_range(const key_type& _Key) const
{
  typedef pair<const_iterator, const_iterator> PCICI;
  const size_type index = _get_index_for_key(_Key);
  const_iterator PII_first = find(_Key);
  LRCI PII_second = Mybuckets_[index].rbegin();
  for (; PII_second!=Mybuckets_[index].rend();++PII_second)
    if (Myequals_(Myget_key_(*PII_second),_Key))
      return PCICI(PII_first,
                 ++const_iterator(--PII_second.base(), this, index));

  return PCICI(end(), end());
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::size_type
hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::erase(const key_type& _Key)
{
  const size_type index = _get_index_for_key(_Key);
  size_type erase_num = 0;

  if (!Mybuckets_[index].empty()) {
    LI cur = Mybuckets_[index].begin();
    LI next = cur;
    ++next;
    while (cur!=Mybuckets_[index].end()) {
      if (Myequals_(Myget_key_(*cur), _Key)) {
        ++erase_num;
        --Mynum_count_;
        Mybuckets_[index].erase(cur);
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
erase(const iterator& _Iter)
{
  const size_type index = _get_index(*_Iter);
  --Mynum_count_;
  Mybuckets_[index].erase(_Iter.cur);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(const const_iterator& _Iter)
{
  const size_type index = _get_index(*_Iter);
  --Mynum_count_;
  Mybuckets_[index].erase(_Iter.cur);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(iterator _First, iterator _Last)
{
  while (first != _Last)
    erase(_First++);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
erase(const_iterator _First, const_iterator _Last)
{
  while (_First != _Last)
    erase(_First++);
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
void hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>::
rehash(size_type _Nbuckets)
{
  _check_size(_Nbuckets, true);
}

MINI_STL_END
#endif // MINI_STL_HASHTABLE_H
