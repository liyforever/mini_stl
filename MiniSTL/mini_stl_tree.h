#ifndef MINI_STL_TREE_H
#define MINI_STL_TREE_H
#include "memory.h"
#ifdef MINI_STL_DEBUG
#include <iostream>
using std::cout;
using std::endl;
using std::cerr;
#endif

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
    return x;
  }
};

void rb_tree_transplant(rb_tree_node_base* x, rb_tree_node_base* y, rb_tree_node_base*& root)
//用y代替x的位置
{
  if (x==root)
    root = y;
  else if (x==x->parent->left)
    x->parent->left = y;
  else
    x->parent->right = y;
  if (y)
    y->parent = x->parent;
}

void rb_tree_rotate_left(rb_tree_node_base* x, rb_tree_node_base*& root)
{
  rb_tree_node_base* y = x->right;
  x->right = y->left;
  if (y->left)
    y->left->parent = x;
  y->parent = x->parent;
  if (x == root)
    root = y;
  else if (x->parent->left == x)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->left = x;
  x->parent = y;
}

void rb_tree_rotate_right(rb_tree_node_base* x, rb_tree_node_base*& root)
{
  rb_tree_node_base* y = x->left;
  x->left = y->right;
  if (y->right)
    y->right->parent = x;
  y->parent = x->parent;
  if (x == root)
    root = y;
  else if (x->parent->left == x)
    x->parent->left = y;
  else
    x->parent->right = y;
  y->right = x;
  x->parent = y;
}

void rb_tree_insert_fixup(rb_tree_node_base* x, rb_tree_node_base*& root)
{
  x->color = rb_tree_red;
  while (x!=root && x->parent->color==rb_tree_red) {
    if (x->parent == x->parent->parent->left) {//case1:
      rb_tree_node_base* uncle = x->parent->parent->right;
      if (uncle && uncle->color==rb_tree_red) {
        x->parent->color = rb_tree_black;
        uncle->color = rb_tree_black;
        x->parent->parent->color = rb_tree_red;
        x = x->parent->parent;
      } else {//case2:
        if (x->parent->right==x) {
          x = x->parent;
          rb_tree_rotate_left(x, root);
        }
        x->parent->color = rb_tree_black;
        x->parent->parent->color = rb_tree_red;
        rb_tree_rotate_right(x->parent->parent,root);
      }
    } else {
      rb_tree_node_base* uncle = x->parent->parent->left;
      if (uncle && uncle->color==rb_tree_red) {
        x->parent->color = rb_tree_black;
        uncle->color = rb_tree_black;
        x->parent->parent->color = rb_tree_red;
        x = x->parent->parent;
      } else {//case2:
        if (x->parent->left==x) {
          x = x->parent;
          rb_tree_rotate_right(x, root);
        }
        x->parent->color = rb_tree_black;
        x->parent->parent->color = rb_tree_red;
        rb_tree_rotate_left(x->parent->parent,root);
      }
    }
  }
  root->color = rb_tree_black;
}

void rb_tree_erase_fixup(rb_tree_node_base* x,
                         rb_tree_node_base* x_parent,
                         rb_tree_node_base*& root)
{
  while (x!=root && (x ==0  ||
                     x->color==rb_tree_black)) {
    if (x == x_parent->left) {
      rb_tree_node_base* brother = x_parent->right;
      if (brother->color==rb_tree_red) {
        brother->color = rb_tree_black;
        x->parent->color = rb_tree_red;
        rb_tree_rotate_left(x_parent, root);
        brother = x_parent->right;
      }
      if ((brother->left==0 ||
          brother->left->color==rb_tree_black) &&
           (brother->right ==0 ||
            brother->right->color==rb_tree_black)) {
        brother->color = rb_tree_red;
        x = x_parent;
        x_parent = x_parent->parent;
      } else if (brother->right ==0 ||
                 brother->right->color == rb_tree_black) {
        if (brother->left)
          brother->left->color = rb_tree_black;
        brother->color = rb_tree_red;
        rb_tree_rotate_right(brother, root);
        brother = x_parent->right;
      }
      brother->color = x_parent->color;
      x_parent->color = rb_tree_black;
      if (brother->right)
        brother->right->color = rb_tree_black;
      rb_tree_rotate_left(x_parent, root);
      x = root;
    } else {
      rb_tree_node_base* brother = x_parent->left;
      if (brother->color==rb_tree_red) {
        brother->color = rb_tree_black;
        x_parent->color = rb_tree_red;
        rb_tree_rotate_right(x_parent, root);
        brother = x_parent->left;
      }
      if ((brother->right==0 ||brother->right->color==rb_tree_black) &&(
            brother->left ==0||brother->left->color==rb_tree_black )) {
        brother->color = rb_tree_red;
        x = x_parent;
        x_parent = x_parent->parent;
      } else {
        if (brother->left ==0||
            brother->left->color == rb_tree_black) {
          if (brother->right)
            brother->right->color = rb_tree_black;
          brother->color = rb_tree_red;
          rb_tree_rotate_left(brother, root);
          brother = x_parent->left;
        }
        brother->color = x_parent->color;
        x_parent->color = rb_tree_black;
        if (brother->left)
        brother->left->color = rb_tree_black;
        rb_tree_rotate_right(x_parent, root);
        x = root;
      }
    }
    x->color = rb_tree_black;
  }
}

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

  pointer operator->() const
  {
    return &(operator *());
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
  typedef _MY_STL::reverse_iterator<iterator> reverse_iterator;
  typedef _MY_STL::reverse_iterator<const_iterator> const_reverse_iterator;
protected:
  typedef void* void_pointer;
  typedef rb_tree_node_base* base_ptr;
  typedef rb_tree_node<Value> tree_node;
  typedef rb_tree_color_type  color_type;
  typedef tree_node* node_ptr;
  typedef simpleAlloc<tree_node,Alloc>  node_allocator_;
protected:
  size_type node_count_;
  node_ptr head_;
  Compare comp_;
  KeyOfValue getKey_;
public:
  rb_tree(const Compare& comp = Compare())
    : node_count_(0), comp_(comp)
  {
    cout << "rb_tree" << endl;
    _init_empty();
  }

  rb_tree(const rb_tree& rhs);
#ifdef MINI_STL_RVALUE_REFS
  rb_tree(rb_tree&& rhs)
  {
    cout << "rb_tree&& rhs" << endl;
    this->head_ = rhs.head_;
    this->node_count_ = rhs.node_count_;
    this->comp_ = rhs.comp_;
    this->getKey_ = rhs.getKey_;
    rhs.head_ = nullptr;
    rhs.node_count_ = 0;
  }

  rb_tree& operator=(rb_tree&& rhs)
  {
    cout << "rb_tree=&&" << endl;
    if (this!=&rhs) {
      clear();
      this->head_ = rhs.head_;
      this->node_count_ = rhs.node_count_;
      this->comp_ = rhs.comp_;
      this->getKey_ = rhs.getKey_;
      rhs.head_ = nullptr;
      rhs.node_count_ = 0;
    }
    return *this;
  }
#endif

  rb_tree& operator=(const rb_tree& rhs);

  ~rb_tree()
  {
        cout << "rb_tree~" << endl;
    clear();
    _put_node(head_);
  }

public:
  Compare key_comp() const
  {
    return comp_;
  }

  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  iterator begin()
  {
    return leftmost();
  }

  const_iterator begin() const
  {
    return leftmost();
  }

  reverse_iterator rbegin()
  {
    return reverse_iterator(this->end());
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(this->end());
  }

  iterator end()
  {
    return head_;
  }

  const_iterator end() const
  {
    return head_;
  }

  reverse_iterator rend()
  {
    return reverse_iterator(this->begin());
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(this->begin());
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

  iterator insert_equal(iterator position, const Value& val);
  iterator insert_equal(const Value& val);

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

  pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,
  bool>
  insert_unique(const Value& val);

  iterator insert_unique(iterator position, const Value& val);

  void erase(iterator position);

  void erase(iterator first, iterator last)
  {
    //cout << "rb-----\n--*first:" << *first <<"last:" << *last<<endl;
    if (first==begin() && last==end())
      clear();
    else
      while(first!=last) erase(first++);
  }

  //void erase(const key_type* first, const key_type* last);

  size_type erase(const key_type& k);

  iterator find(const key_type& k)
  {
    node_ptr y = head_;
    node_ptr x = root();
    while (x) {
      if (!comp_(key(x), k)) {
        y = x;
        x = left(x);
      } else {
        x = right(x);
      }
    }
    iterator j = iterator(y);
    return (j == end() || comp_(k, key(j.node))) ?
         end() : j;
  }

  const_iterator find(const Key& k) const
  {
    node_ptr y = head_;
    node_ptr x = root();
    while (x) {
      if (!comp_(key(x), k)) {
        y = x;
        x = left(x);
      } else {
        x = right(x);
      }
    }
    const_iterator j = const_iterator(y);
    return (j == end() || comp_(k, key(j.node))) ?
         end() : j;
  }

  size_type count(const key_type& k) const
  {
    pair<const_iterator,const_iterator> p = this->equal_range(k);
    difference_type n = DISTANCE(p.first, p.second);
    return (size_type)(n);
  }

  iterator lower_bound(const key_type& k) ;

  const_iterator lower_bound(const key_type& k) const;

  iterator upper_bound(const key_type& k);

  const_iterator upper_bound(const key_type& k) const;

  pair<iterator, iterator>
  equal_range(const key_type& k)
  {
    return pair<iterator,iterator>
        (lower_bound(k), upper_bound(k));
  }

  pair<const_iterator, const_iterator>
  equal_range(const key_type& k) const
  {
    return pair<const_iterator,const_iterator>
        (lower_bound(k), upper_bound(k));
  }

  void clear()
  {
    if (node_count_!=0) {
      _clear_aux(root());
      root() = 0;
      leftmost() = head_;
      rightmost() = head_;
      node_count_ = 0;
    }
  }

  void swap(rb_tree& rhs)
  {
    _MY_STL::swap(this->head_, rhs.head_);
    _MY_STL::swap(this->node_count_, rhs.node_count_);
    _MY_STL::swap(this->comp_, rhs.comp_);
  }

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
    destroy(&p->data);
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

  void _erase_aux(base_ptr x);

  void _clear_aux(node_ptr x);

  node_ptr  _copy_aux(node_ptr in, node_ptr to);
public:
  void showTree()
  {
    cout << "value   parent   left   right   color" << endl;
    node_ptr r = root();
    node_ptr tmp;
    queue<node_ptr> qq;
    qq.push(r);
    while (!qq.empty()) {
      tmp = qq.front();
      qq.pop();
      //cout << "queue size:" << qq.size() << endl;
      //cout << "show queue::"  << endl;
      //if (!qq.empty())
      //cout << qq.front()->data << endl;
      cout << endl;
      if (left(tmp)) {
          //cout << "if left .data::" << left(tmp)->data << endl;
        qq.push(left(tmp));
        }
      if (right(tmp)) {
          //cout << "if right .data::" << right(tmp)->data << endl;
        qq.push(right(tmp));
        }
      cout << tmp->data<< "   ";
      if (parent(tmp))
        cout <<"p::" << parent(tmp)->data<<"   ";
      else
        cout << "    " ;
      if (left(tmp))
        cout << "l::" <<left(tmp)->data << "   ";
      else
        cout << "     ";
      if(right(tmp))
        cout << "r::"<<right(tmp)->data << "   ";
      else
        cout << "    ";

      cout << "c::" << color(tmp) << endl;
            cout << endl;
    }
  }
#ifdef MINI_STL_DEBUG
  template <class InputIterator>
  void _check_range(InputIterator first,
                    InputIterator last) const
  {
    difference_type n = DISTANCE(first, last);
    if (n<0) {
      cerr << "rb_tree:InputIterator last - first < 0" << endl;
      MINI_STL_THROW_RANGE_ERROR("rb_tree");
    }
  }
#endif
};

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::rb_tree(const rb_tree &rhs)
{
      cout << "rb_tree&" << endl;
  if (rhs.size()==0) {
    _init_empty();
  } else {
    this->head_ = _get_node();
    this->head_->color = rb_tree_red;
    root() = _copy_aux(rhs.root(), this->head_);
    leftmost() = mimimum(this->root());
    rightmost() = maximum(this->root());
  }
  this->node_count_ = rhs.node_count_;
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>&
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  operator =(const rb_tree& rhs)
{
      cout << "rb_tree=" << endl;
  if (this != &rhs) {
    clear();
    comp_ = rhs.comp_;
    getKey_ = rhs.getKey_;
    if (rhs.size()) {
      root() = _copy_aux(rhs.root(), this->head_);
      this->node_count_ = rhs.node_count_;
      leftmost() = mimimum(this->root());
      rightmost() = maximum(this->root());
    }
  }
  return *this;
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::node_ptr
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  _copy_aux(node_ptr in, node_ptr to)
{
  node_ptr top = _clone_node(in);
  top->parent = to;
  if (in->right)
    top->right = _copy_aux(right(in), top);
  to = top;
  in = left(in);

  while (in != 0) {
    node_ptr y = _clone_node(in);
    to->left = y;
    y->parent = to;
    if (in->right)
      y->right = _copy_aux(right(in), y);
    to = y;
    in = left(in);
  }
  return top;
}

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
#ifdef MINI_STL_DEBUG
  _check_range(first, last);
#endif
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
#ifdef MINI_STL_DEBUG
  _check_range(first, last);
#endif
  while (first != last)
    insert_unique(*first++);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,bool>
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
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_unique(iterator position, const Value& val)
{
  if (position.node == head_->left) { // begin()
    if (size() > 0 &&
        comp_(getKey_(val), key(position.node)))
      return _insert_aux(position.node, position.node, val);
    else
      return insert_unique(val).first;
  } else if (position.node == head_) { // end()
    if (comp_(key(rightmost()), getKey_(val)))
      return _insert_aux(0, rightmost(), val);
    else
      return insert_unique(val).first;
  } else {
    iterator before = position;
    --before;
    if (comp_(key(before.node), getKey_(val))
        && comp_(getKey_(val), key(position.node))) {
      if (right(before.node) == 0)
        return _insert_aux(0, before.node, val);
      else
        return _insert_aux(position.node, position.node, val);
    } else
      return insert_unique(val).first;
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_equal(iterator position, const Value& val)
{
  if (position.node == head_->left) { // begin()
    if (size() > 0 &&
        !comp_(key(position.node), getKey_(val)))
      return _insert_aux(position.node, position.node, val);
    else
      return insert_equal(val);
  } else if (position.node == head_) {// end()
    if (!comp_(getKey_(val), key(rightmost())))
      return _insert_aux(0, rightmost(), val);
    else
      return insert_equal(val);
  } else {
    iterator before = position;
    --before;
    if (!comp_(getKey_(val), key(before.node))
        && !comp_(key(position.node), getKey_(val))) {
      if (right(before.node) == 0)
        return _insert_aux(0, before.node, val);
      else
        return _insert_aux(position.node, position.node, val);
    } else
      return insert_equal(val);
  }
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
  rb_tree_insert_fixup(z, head_->parent);
  return iterator(z);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
void rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  erase(iterator position)
{
  iterator tmp = position;
  if (position==begin()) {
    ++tmp;
    leftmost() = (node_ptr)(tmp.node);
    _erase_aux(position.node);
  } else if (position==--end()) {
    --tmp;
    rightmost() = (node_ptr)(tmp.node);
    _erase_aux(position.node);
  } else
    _erase_aux(position.node);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::size_type
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
erase(const key_type& k)
{
  pair<iterator,iterator> p = this->equal_range(k);
  size_type n = DISTANCE(p.first, p.second);
  erase(p.first, p.second);
  return n;
}

//void erase(const key_type* first, const key_type* last);
template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
void rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
 _erase_aux(base_ptr x)
{
  base_ptr y = x;
  base_ptr z;
  base_ptr z_parent;
  color_type oldColor = y->color;
  if (!x->left) {
    z = x->right;
    z_parent = x->parent;
    rb_tree_transplant(x, x->right, head_->parent);
  } else if (!x->right) {
    z = x->left;
    z_parent = x->parent;
    rb_tree_transplant(x, x->left, head_->parent);
  } else {
    y = mimimum((node_ptr)(x->right));
    oldColor = y->color;
    z = y->right;
    //cout << ((node_ptr)(y))->data << endl;
    if (y->parent == x) {
      z_parent = y;
    } else {
      z_parent = y->parent;
      rb_tree_transplant(y, y->right, head_->parent);
      y->right = x->right;
      y->right->parent = y;
    }
    rb_tree_transplant(x, y, head_->parent);
    y->left = x->left;
    y->left->parent = y;
    y->color = x->color;
  }
  //cout << "UUUUUU" << endl;
  //showTree();
  /*if (z)
  cout << "x::" <<((node_ptr)(z))->data << endl;
  if (z_parent)
  cout << "p::" << ((node_ptr)(z_parent))->data << endl;
  if (root())
  cout << "root::" << ((node_ptr)(root()))->data << endl;*/
  if (oldColor == rb_tree_black)
    rb_tree_erase_fixup(z, z_parent, head_->parent);
  --node_count_;
  _destroy_node((node_ptr)x);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
void rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  _clear_aux(node_ptr x)
{
  while (x!=0) {
    _clear_aux(right(x));
    node_ptr y = left(x);
    _destroy_node(x);
    x = y;
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
lower_bound(const key_type& k)
{
  node_ptr y = head_;
  node_ptr x = root();
  while (x) {
    if (!comp_(key(x), k)) {
      y = x;
      x = left(x);
    } else {
      x = right(x);
    }
  }
  return iterator(y);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::const_iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
lower_bound(const key_type& k) const
{
  node_ptr y = head_;
  node_ptr x = root();
  while (x) {
    if (!comp_(key(x), k)) {
      y = x;
      x = left(x);
    } else {
      x = right(x);
    }
  }
  return const_iterator(y);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
upper_bound(const key_type& k)
{
  node_ptr y = head_;
  node_ptr x = root();
  while (x) {
    if (comp_(k, key(x)))
      y = x, x = left(x);
    else
      x = right(x);
  }
  return iterator(y);
}

template<class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::const_iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
upper_bound(const key_type& k) const
{
  node_ptr y = head_;
  node_ptr x = root();
  while (x) {
    if (comp_(k, key(x)))
      y = x, x = left(x);
    else
      x = right(x);
  }
  return const_iterator(y);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator==(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  return lhs.size() == rhs.size() &&
         equal(lhs.begin(), lhs.end(), rhs.begin());
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator<(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
          const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  return lexicographical_compare(lhs.begin(), lhs.end(),
                                 rhs.begin(), rhs.end());
}
template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator!=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  return !(lhs == rhs);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator>(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
          const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  return rhs < lhs;
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator<=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  return !(rhs < lhs);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator>=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  return !(lhs < rhs);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline void
swap(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& lhs,
     const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& rhs)
{
  lhs.swap(rhs);
}


MINI_STL_END
#endif // MINI_STL_TREE_H
