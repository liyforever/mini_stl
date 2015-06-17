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



template <class Key, class Value, class Compare,
          class Alloc = default_allocator>
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
  typedef rb_tree_node* node_ptr;
  typedef simpleAlloc<tree_node,Alloc>  node_allocator_;
protected:
  size_type node_count_;
  node_ptr head_;
  Compare comp_;
public:
  rb_tree(const Compare& comp = Compare())
    : node_count_(0), comp_(comp)
  {
    _init_empty();
  }

  ~rb_tree()
  {
    clear();
    _put_node(head_);
  }

public:

protected:
  static node_ptr& root() const
  {
    return (node_ptr&)(head_->parent);
  }

  static node_ptr& leftmost() const
  {
    return (node_ptr&)(head_->left);
  }

  static node_ptr& rightmost() const
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

  static color_type& key(node_ptr x)
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
};

MINI_STL_END
#endif // MINI_STL_TREE_H
