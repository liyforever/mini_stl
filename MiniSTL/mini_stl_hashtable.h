#ifndef MINI_STL_HASHTABLE_H
#define MINI_STL_HASHTABLE_H
#include "memory.h"
#include <unordered_map>
#include "mini_stl_list.h"
MINI_STL_BEGIN

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
class hashtable;

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
struct HT_iterator;

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
struct HT_const_iterator;

template <class Val, class Key, class HashFcn,
          class ExtractKey, class EqualKey, class Alloc>
struct HT_iterator
{
  typedef hashtable<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>
         hashtable;
  typedef HT_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>
         iterator;
  typedef HT_const_iterator<Val,Key,HashFcn,ExtractKey,EqualKey,Alloc>
        const_iterator;
  typedef _list_node<Val> node;
  typedef bidirectional_iterator_tag iterator_category;
  typedef Val value_type;
  typedef ptrdiff_t difference_type;
  typedef size_t size_type;
  typedef Val& reference;
  typedef Val* pointer;

  node* cur;
  hashtable* ht_ptr;
  HT_iterator(node* n, hashtable* ht)
    : cur(n), ht_ptr(ht) {}
  HT_iterator() {}

};

MINI_STL_END
#endif // MINI_STL_HASHTABLE_H
