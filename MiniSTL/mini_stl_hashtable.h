#ifndef MINI_STL_HASHTABLE_H
#define MINI_STL_HASHTABLE_H
#include "memory.h"
#include "mini_stl_list.h"
#include "mini_stl_vector.h"
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
  HT_iterator(ListIterator* li, hashtable* ht)
    : cur(li), ht_ptr(ht) {}
  HT_iterator() {}

  reference operator*() const
  {
    return cur->data;
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

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>&
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>::operator++()
{
  ListIterator old = cur;
  ++cur;
  size_type index = ht->bkt_num(old->data);
  if (cur == ht->buckets_[index].end()) {
    while (++index < ht->buckets_.size())
      if (!ht->buckets_[index].empty()) {
        cur = ht->buckets_[index].begin();
        break;
      }
  }
  return *this;
}

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc,
          class ListIterator>
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>&
HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc,ListIterator>::operator--()
{
  ListIterator old = cur;
  --cur;
  size_type index = ht->bkt_num(old->data);
  if (cur == ht->buckets_[index].begin()) {
    while (--index >= 0)
      if (!ht->buckets_[index].empty()) {
        cur = --ht->buckets_[index].end();
        break;
      }
  }
  return *this;
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
  vector<node, Alloc> buckets_;
  size_type            num_count_;
public:
  hashtable(size_type n,
            const HashFcn& hf,
            const EqualKey& eql,
            const ExtractKey& ext,
            const allocator_type& /*Al*/ = allocator_type())
    : hash_(hf),
      equals_(eql),
      get_key_(ext),
      num_count_(0)
  {
    _init_bucket(n);
  }

  allocator_type get_allocator() const
  {
    return allocator_type();
  }
protected:
  void _init_bucket(size_type n)
  {

  }
};

MINI_STL_END
#endif // MINI_STL_HASHTABLE_H
