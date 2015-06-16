#ifndef MINI_STL_TREE_H
#define MINI_STL_TREE_H
#include "memory.h"
MINI_STL_BEGIN

typedef bool rb_tree_color_type;
const tb_tree_color_type rb_tree_red = false;
const tb_tree_color_type rb_tree_black = true;

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
        node->parent->parent = node) {
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
    return (node_ptr)(node)->data;
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

MINI_STL_END
#endif // MINI_STL_TREE_H
