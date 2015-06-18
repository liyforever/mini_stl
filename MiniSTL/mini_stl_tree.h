#ifndef MINI_STL_TREE_H
#define MINI_STL_TREE_H
#include "memory.h"
#include <iostream>
using std::cout;
using std::endl;
MINI_STL_BEGIN

typedef bool rb_tree_color_type;
const rb_tree_color_type rb_tree_red = false;
const rb_tree_color_type rb_tree_black = true;

struct rb_tree_node_base;
struct rb_tree_node_base
{
  typedef rb_tree_color_type color_type;
  typedef rb_tree_node_base* base_ptr;

  color_type color;
  base_ptr parent;
  base_ptr left;
  base_ptr right;

  static base_ptr minimum(base_ptr x)
  {
    while (x->left)
      x = x->left;
    return x;
  }

  static base_ptr maximum(base_ptr x)
  {
    while (x->right)
      x = x->right;
  }
};

template <class Type>
struct rb_tree_node : public rb_tree_node_base
{
  typedef rb_tree_node<Type>* node_ptr;
  Type data;
};

struct rb_tree_base_iterator
{
  typedef rb_tree_node_base::base_ptr base_ptr;
  typedef bidirectional_iterator_tag iterator_category;
  typedef ptrdiff_t difference_type;

  base_ptr node;

  void incr()
  {
    if (node->right) {
      node = node->right;
      while (node->left)
        node = node->left;
    } else {
      base_ptr p = node->parent;
      while (p->right==node) {
        node = p;
        p = p->parent;
      }
      if (node->right!=p)
        node = p;
    }

  }

  void decr()
  {
    //头结点情况
    if (node->color==rb_tree_red &&
        node->parent->parent == node) {
      node = node->right;
    } else if(node->left) {
      base_ptr c = node->left;
      while (c->left)
        c = c->left;
      node = c;
    } else {
      base_ptr p = node->parent;
      while (p->left==node) {
        node = p;
        p = p->parent;
      }
      node = p;
    }
  }
};

template <class Type, class Ref, class Ptr>
struct rb_tree_iterator : public rb_tree_base_iterator
{
  typedef Type  value_type;
  typedef Ref   reference;
  typedef Ptr   pointer;
  typedef rb_tree_iterator<Type,Type&,Type*>  iterator;
  typedef rb_tree_iterator<Type,const Type&,const Type*> const_iterator;
  typedef rb_tree_iterator<Type,Ref,Ptr>  Self;
  typedef rb_tree_node<Type>*   node_ptr;

  rb_tree_iterator() {}
  rb_tree_iterator(node_ptr x)
  {
    node = x;
  }
  rb_tree_iterator(const iterator& x)
  {
    node = x.node;
  }

  reference operator*() const
  {
    return ((node_ptr)(node))->data;
  }

  Self& operator++()
  {
    incr();
    return *this;
  }

  Self operator++(int)
  {
    Self tmp = *this;
    incr();
    return tmp;
  }

  Self& operator--()
  {
    decr();
    return *this;
  }

  Self operator--(int)
  {
    Self tmp = *this;
    decr();
    return tmp;
  }
};

inline bool operator==(const rb_tree_base_iterator& x,
                       const rb_tree_base_iterator& y)
{
  return x.node == y.node;
}

inline bool operator!=(const rb_tree_base_iterator& x,
                       const rb_tree_base_iterator& y)
{
  return x.node != y.node;
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc = default_allocator>
class rb_tree
{
public:
  typedef Key               key_type;
  typedef Value             value_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef Alloc             allocator_type;
  typedef rb_tree_iterator<value_type,reference,pointer>  iterator;
  typedef rb_tree_iterator<value_type,const value_type&,const value_type*> const_iterator;
protected:
  typedef void* void_pointer;
  typedef rb_tree_node_base* base_ptr;
  typedef rb_tree_node<Value> tree_node;
  typedef rb_tree_color_type  color_type;
  typedef tree_node* node_ptr;
  typedef simpleAlloc<tree_node,Alloc>  node_allocator_;
protected:
public:
  size_type node_count_;
  node_ptr head_;
  Compare comp_;
  KeyOfValue getKey_;
public:
  rb_tree(const Compare& comp = Compare())
    : node_count_(0), comp_(comp)
  {
    _init_empty();
  }

  ~rb_tree()
  {
    //clear();
    _put_node(head_);
  }

public:
  iterator begin()
  {
    return leftmost();
  }

  const_iterator begin() const
  {
    return leftmost();
  }

  iterator end()
  {
    return head_;
  }

  const_iterator end() const
  {
    return head_;
  }

  bool empty() const
  {
    return node_count_ == 0;
  }

  size_type size() const
  {
    return node_count_;
  }

  size_type max_size() const
  {
    return (size_type)(-1);
  }

  iterator insert_equal(const Value& val);
#ifdef MINI_STL_MEMBER_TEMPLATES
  template <class InputIterator>
  void insert_equal(InputIterator first,
                    InputIterator last,
                    typename is_iterator<InputIterator>::ID = Identity()
      );

  template <class InputIterator>
  void insert_unique(InputIterator first,
                     InputIterator last,
                     typename is_iterator<InputIterator>::ID = Identity()
      );
#endif
  pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,
  bool>
  insert_unique(const Value& val);
protected:
  node_ptr& root() const
  {
    return (node_ptr&)(head_->parent);
  }

  node_ptr& leftmost() const
  {
    return (node_ptr&)(head_->left);
  }

  node_ptr& rightmost() const
  {
    return (node_ptr&)(head_->right);
  }

  static node_ptr& left(node_ptr x)
  {
    return (node_ptr&)(x->left);
  }

  static node_ptr& right(node_ptr x)
  {
    return (node_ptr&)(x->right);
  }

  static node_ptr& parent(node_ptr x)
  {
    return (node_ptr&)(x->parent);
  }

  static reference value(node_ptr x)
  {
    return x->data;
  }

  static const Key& key(node_ptr x)
  {
    return KeyOfValue()((value(x)));
  }

  static color_type& color(node_ptr x)
  {
    return (color_type&)(x->color);
  }

  static node_ptr& left(base_ptr x)
  {
    return (node_ptr&)(x->left);
  }

  static node_ptr& right(base_ptr x)
  {
    return (node_ptr&)(x->right);
  }

  static node_ptr& parent(base_ptr x)
  {
    return (node_ptr&)(x->parent);
  }

  static reference value(base_ptr x)
  {
    return ((node_ptr)x)->data;
  }

  static color_type& color(base_ptr x)
  {
    return (color_type&)((node_ptr)(x)->color);
  }

  static const Key& key(base_ptr x)
  {
    return KeyOfValue()((value(x)));
  }

  static node_ptr mimimum(node_ptr x)
  {
    return (node_ptr)rb_tree_node_base::minimum(x);
  }

  static node_ptr maximum(node_ptr x)
  {
    return (node_ptr)rb_tree_node_base::maximum(x);
  }
protected:
  node_ptr _get_node()
  {
    return node_allocator_::allocate();
  }

  void _put_node(node_ptr p)
  {
    node_allocator_::deallocate(p);
  }

  node_ptr _create_node(const value_type& val)
  {
    node_ptr tmp = _get_node();
    MINI_STL_TRY {
      construct(&tmp->data, val);
    }
    MINI_STL_UNWIND(_put_node(tmp));
    return tmp;
  }

  node_ptr _clone_node(node_ptr p)
  {
    node_ptr tmp = _create_node(p->data);
    tmp->color = p->color;
    tmp->left = nullptr;
    tmp->right = nullptr;
    return tmp;
  }

  void _destroy_node(node_ptr p)
  {
    destroy(p->data);
    _put_node(p);
  }

  void _init_empty()
  {
    head_ = _get_node();
    color(head_) = rb_tree_red;
    root() = 0;
    leftmost() = head_;
    rightmost() = head_;
  }

  iterator _insert_aux(base_ptr x_, base_ptr y_, const Value& val);
};

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_equal(const Value& val)
{
  node_ptr y = head_;
  node_ptr x = root();
  while(x) {
    y = x;
    x = comp_(getKey_(val), key(x)) ?
          left(x) : right(x);
  }
  return _insert_aux(x, y, val);
}

#ifdef MINI_STL_MEMBER_TEMPLATES
template<class Key, class Value, class KeyOfValue,
class Compare, class Alloc>
template <class InputIterator>
void
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
     insert_equal(InputIterator first,
                  InputIterator last,
                  typename is_iterator<InputIterator>::ID
      )
{
  while(first != last)
    insert_equal(*first++);
}

template<class Key, class Value, class KeyOfValue,
class Compare, class Alloc>
template <class InputIterator>
void
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
    insert_unique(InputIterator first,
                  InputIterator last,
                  typename is_iterator<InputIterator>::ID = Identity()
      )
{
  while (first != last)
    insert_unique(*first++);
}

#endif
template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,
bool>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_unique(const Value& val)
{
  node_ptr y = head_;
  node_ptr x = root();
  bool result = true;
  while (x) {
    y = x;
    result = comp_(getKey_(val), key(x));
    x = result ? left(x) : right(x);
  }
  iterator j = iterator(y);
  if (result)
    if (j==begin())
      return pair<iterator,bool>(_insert_aux(x,y,val),true);
    else
      --j;
  if (comp_(key(j.node), getKey_(val)))
    return pair<iterator,bool>(_insert_aux(x,y,val),true);
  return pair<iterator,bool>(j,false);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::_insert_aux(
    base_ptr x_, base_ptr y_, const Value& val)
{
  node_ptr x = (node_ptr)x_;
  node_ptr y = (node_ptr)y_;
  node_ptr z = _create_node(val);
  if (y==head_) {//空的红黑树
    root() = z;
    leftmost() = z;
    rightmost() = z;
  } else if (comp_(getKey_(val),key(y))) {//插入左边
    left(y) = z;
    if (y==leftmost())
      leftmost() = z;
  } else {
    right(y) = z;
    if (y==rightmost())
      rightmost() = z;
  }
  parent(z) = y;
  left(z) = nullptr;
  right(z) = nullptr;
  ++node_count_;
  return iterator(z);
}

MINI_STL_END
#endif // MINI_STL_TREE_H
