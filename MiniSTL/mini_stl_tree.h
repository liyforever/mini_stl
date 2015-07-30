#ifndef MINI_STL_TREE_H
#define MINI_STL_TREE_H
#include "mini_stl_memory.h"

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

  static base_ptr minimum(base_ptr _Ptr)
  {
    while (_Ptr->left)
      _Ptr = _Ptr->left;
    return _Ptr;
  }

  static base_ptr maximum(base_ptr _Ptr)
  {
    while (_Ptr->right)
      _Ptr = _Ptr->right;
    return _Ptr;
  }
};

inline void
rb_tree_transplant(rb_tree_node_base* _X,
                   rb_tree_node_base* _Y,
                   rb_tree_node_base*& _Root)
//use y transplant x position
{
  if (_X == _Root)
    _Root = _Y;
  else if (_X == _X->parent->left)
    _X->parent->left = _Y;
  else
    _X->parent->right = _Y;
  if (_Y)
    _Y->parent = _X->parent;
}

inline void
rb_tree_rotate_left(rb_tree_node_base* _X,
                    rb_tree_node_base*& _Root)
{
  rb_tree_node_base* y = _X->right;
  _X->right = y->left;
  if (y->left)
    y->left->parent = _X;
  y->parent = _X->parent;
  if (_X == _Root)
    _Root = y;
  else if (_X->parent->left == _X)
    _X->parent->left = y;
  else
    _X->parent->right = y;
  y->left = _X;
  _X->parent = y;
}

inline void
rb_tree_rotate_right(rb_tree_node_base* _X,
                     rb_tree_node_base*& _Root)
{
  rb_tree_node_base* y = _X->left;
  _X->left = y->right;
  if (y->right)
    y->right->parent = _X;
  y->parent = _X->parent;
  if (_X == _Root)
    _Root = y;
  else if (_X->parent->left == _X)
    _X->parent->left = y;
  else
    _X->parent->right = y;
  y->right = _X;
  _X->parent = y;
}

void rb_tree_insert_fixup(rb_tree_node_base* _X, rb_tree_node_base*& _Root)
{
  _X->color = rb_tree_red;
  while (_X!=_Root && _X->parent->color==rb_tree_red) {
    if (_X->parent == _X->parent->parent->left) {//case1:
      rb_tree_node_base* uncle = _X->parent->parent->right;
      if (uncle && uncle->color==rb_tree_red) {
        _X->parent->color = rb_tree_black;
        uncle->color = rb_tree_black;
        _X->parent->parent->color = rb_tree_red;
        _X = _X->parent->parent;
      } else {//case2:
        if (_X->parent->right == _X) {
          _X = _X->parent;
          rb_tree_rotate_left(_X, _Root);
        }
        _X->parent->color = rb_tree_black;
        _X->parent->parent->color = rb_tree_red;
        rb_tree_rotate_right(_X->parent->parent,_Root);
      }
    } else {
      rb_tree_node_base* uncle = _X->parent->parent->left;
      if (uncle && uncle->color==rb_tree_red) {
        _X->parent->color = rb_tree_black;
        uncle->color = rb_tree_black;
        _X->parent->parent->color = rb_tree_red;
        _X = _X->parent->parent;
      } else {//case2:
        if (_X->parent->left==_X) {
          _X = _X->parent;
          rb_tree_rotate_right(_X, _Root);
        }
        _X->parent->color = rb_tree_black;
        _X->parent->parent->color = rb_tree_red;
        rb_tree_rotate_left(_X->parent->parent,_Root);
      }
    }
  }
  _Root->color = rb_tree_black;
}

void rb_tree_erase_fixup(rb_tree_node_base* _X,
                         rb_tree_node_base* _X_parent,
                         rb_tree_node_base*& _Root)
{
  while (_X!=_Root && (_X ==0  ||
                     _X->color==rb_tree_black)) {
    if (_X == _X_parent->left) {
      rb_tree_node_base* brother = _X_parent->right;
      if (brother->color == rb_tree_red) {
        brother->color = rb_tree_black;
        _X->parent->color = rb_tree_red;
        rb_tree_rotate_left(_X_parent, _Root);
        brother = _X_parent->right;
      }
      if ((brother->left == 0 ||
          brother->left->color == rb_tree_black) &&
           (brother->right ==0 ||
            brother->right->color == rb_tree_black)) {
        brother->color = rb_tree_red;
        _X = _X_parent;
        _X_parent = _X_parent->parent;
      } else if (brother->right ==0 ||
                 brother->right->color == rb_tree_black) {
        if (brother->left)
          brother->left->color = rb_tree_black;
        brother->color = rb_tree_red;
        rb_tree_rotate_right(brother, _Root);
        brother = _X_parent->right;
      }
      brother->color = _X_parent->color;
      _X_parent->color = rb_tree_black;
      if (brother->right)
        brother->right->color = rb_tree_black;
      rb_tree_rotate_left(_X_parent, _Root);
      _X = _Root;
    } else {
      rb_tree_node_base* brother = _X_parent->left;
      if (brother->color == rb_tree_red) {
        brother->color = rb_tree_black;
        _X_parent->color = rb_tree_red;
        rb_tree_rotate_right(_X_parent, _Root);
        brother = _X_parent->left;
      }
      if ((brother->right==0 ||brother->right->color==rb_tree_black) &&(
            brother->left ==0||brother->left->color==rb_tree_black )) {
        brother->color = rb_tree_red;
        _X = _X_parent;
        _X_parent = _X_parent->parent;
      } else {
        if (brother->left ==0||
            brother->left->color == rb_tree_black) {
          if (brother->right)
            brother->right->color = rb_tree_black;
          brother->color = rb_tree_red;
          rb_tree_rotate_left(brother, _Root);
          brother = _X_parent->left;
        }
        brother->color = _X_parent->color;
        _X_parent->color = rb_tree_black;
        if (brother->left)
        brother->left->color = rb_tree_black;
        rb_tree_rotate_right(_X_parent, _Root);
        _X = _Root;
      }
    }
    _X->color = rb_tree_black;
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
      while (p->right == node) {
        node = p;
        p = p->parent;
      }
      if (node->right != p)
        node = p;
    }

  }

  void decr()
  {
    if (node->color == rb_tree_red &&
        node->parent->parent == node) {
        //node is head case
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
  rb_tree_iterator(node_ptr _Ptr)
  {
    node = _Ptr;
  }
  rb_tree_iterator(const iterator& _Right)
  {
    node = _Right.node;
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

inline bool operator==(const rb_tree_base_iterator& _Left,
                       const rb_tree_base_iterator& _Right)
{
  return _Left.node == _Right.node;
}

inline bool operator!=(const rb_tree_base_iterator& _Left,
                       const rb_tree_base_iterator& _Right)
{
  return _Left.node != _Right.node;
}

template <class Key, class Value, class KeyOfValue,
          class Compare,
          class Alloc = _MY_STL::default_allocator>
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
  size_type Mynode_count_;
  node_ptr Myhead_;
  Compare Mycomp_;
  KeyOfValue MygetKey_;
public:
  rb_tree(const Compare& _Comp = Compare())
    : Mynode_count_(0), Mycomp_(_Comp)
  {
    _init_empty();
  }

  rb_tree(const rb_tree& _Right);
#ifdef MINI_STL_RVALUE_REFS
  rb_tree(rb_tree&& _Right)
  {
    this->Myhead_ = _Right.Myhead_;
    this->Mynode_count_ = _Right.Mynode_count_;
    this->Mycomp_ = _MY_STL::move(_Right.Mycomp_);
    this->MygetKey_ = _MY_STL::move(_Right.MygetKey_);
    _Right.Myhead_ = nullptr;
    _Right.Mynode_count_ = 0;
  }

  rb_tree& operator=(rb_tree&& _Right)
  {
    if (this != &_Right) {
      _MY_STL::swap(this->Myhead_, _Right.Myhead_);
      _MY_STL::swap(this->Mynode_count_, _Right.Mynode_count_);
      _MY_STL::swap(this->Mycomp_, _Right.Mycomp_);
      _MY_STL::swap(this->MygetKey_, _Right.MygetKey_);
    }
    return *this;
  }
#endif //MINI_STL_RVALUE_REFS

  rb_tree& operator=(const rb_tree& _Right);

  ~rb_tree()
  {
    clear();
    _put_node(Myhead_);
  }

public:
  Compare key_comp() const
  {
    return Mycomp_;
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
    return Myhead_;
  }

  const_iterator end() const
  {
    return Myhead_;
  }

  reverse_iterator rend()
  {
    return reverse_iterator(this->begin());
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(this->begin());
  }

  const_iterator cbegin() const
  {
    return this->begin();
  }

  const_iterator cend() const
  {
    return this->end();
  }

  const_reverse_iterator crbegin() const
  {
    return this->rbegin();
  }

  const_reverse_iterator crend() const
  {
    return this->rend();
  }

  bool empty() const
  {
    return Mynode_count_ == 0;
  }

  size_type size() const
  {
    return Mynode_count_;
  }

  size_type max_size() const
  {
    return (size_type)(-1);
  }

  iterator insert_equal(iterator _Position, const Value& _Val);
  iterator insert_equal(const Value& _Val);
#ifdef MINI_STL_RVALUE_REFS
  iterator insert_equal(iterator _Position, Value&& _Val);
  iterator insert_equal(Value&& _Val);
#endif
  template <class InputIterator>
  void insert_equal(InputIterator _First,
                    InputIterator _Last,
                    typename is_iterator<InputIterator>::ID
                    = Identity());

  template <class InputIterator>
  void insert_unique(InputIterator _First,
                     InputIterator _Last,
                     typename is_iterator<InputIterator>::ID
                     = Identity());

  _MY_STL::pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,
  bool>
  insert_unique(const Value& _Val);

  iterator insert_unique(iterator _Position, const Value& _Val);
#ifdef MINI_STL_RVALUE_REFS
  _MY_STL::pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator,
  bool>
  insert_unique(Value&& _Val);

  iterator insert_unique(iterator _Position, Value&& _Val);
#endif
  void erase(iterator _Position);

  void erase(iterator _First, iterator _Last)
  {
    if (_First==begin() && _Last==end())
      clear();
    else
      while(_First != _Last) erase(_First++);
  }

  size_type erase(const key_type& _Key);

  iterator find(const key_type& _Key)
  {
    node_ptr y = Myhead_;
    node_ptr x = root();
    while (x) {
      if (!MINI_STL_DEBUG_COMP(Mycomp_, key(x), _Key)) {
        y = x;
        x = left(x);
      } else {
        x = right(x);
      }
    }
    iterator j = iterator(y);
    return (j == end() || MINI_STL_DEBUG_COMP(Mycomp_, _Key, key(j.node))) ?
         end() : j;
  }

  const_iterator find(const Key& _Key) const
  {
    node_ptr y = Myhead_;
    node_ptr x = root();
    while (x) {
      if (!MINI_STL_DEBUG_COMP(Mycomp_, key(x), _Key)) {
        y = x;
        x = left(x);
      } else {
        x = right(x);
      }
    }
    const_iterator j = const_iterator(y);
    return (j == end() || MINI_STL_DEBUG_COMP(Mycomp_, _Key, key(j.node))) ?
         end() : j;
  }

  size_type count(const key_type& _Key) const
  {
    pair<const_iterator,const_iterator> p = this->equal_range(_Key);
    difference_type n = DISTANCE(p.first, p.second);
    return (size_type)(n);
  }

  iterator lower_bound(const key_type& _Key) ;

  const_iterator lower_bound(const key_type& _Key) const;

  iterator upper_bound(const key_type& _Key);

  const_iterator upper_bound(const key_type& _Key) const;

  _MY_STL::pair<iterator, iterator>
  equal_range(const key_type& _Key)
  {
    return _MY_STL::pair<iterator,iterator>
        (lower_bound(_Key), upper_bound(_Key));
  }

  _MY_STL::pair<const_iterator, const_iterator>
  equal_range(const key_type& _Key) const
  {
    return _MY_STL::pair<const_iterator,const_iterator>
        (lower_bound(_Key), upper_bound(_Key));
  }

  void clear()
  {
    if (Mynode_count_!=0) {
      _clear_aux(root());
      root() = 0;
      leftmost() = Myhead_;
      rightmost() = Myhead_;
      Mynode_count_ = 0;
    }
  }

  void swap(rb_tree& _Right)
  {
    _MY_STL::swap(this->Myhead_, _Right.Myhead_);
    _MY_STL::swap(this->Mynode_count_, _Right.Mynode_count_);
    _MY_STL::swap(this->Mycomp_, _Right.Mycomp_);
  }

protected:
  node_ptr& root() const
  {
    return (node_ptr&)(Myhead_->parent);
  }

  node_ptr& leftmost() const
  {
    return (node_ptr&)(Myhead_->left);
  }

  node_ptr& rightmost() const
  {
    return (node_ptr&)(Myhead_->right);
  }

  static node_ptr& left(node_ptr _Ptr)
  {
    return (node_ptr&)(_Ptr->left);
  }

  static node_ptr& right(node_ptr _Ptr)
  {
    return (node_ptr&)(_Ptr->right);
  }

  static node_ptr& parent(node_ptr _Ptr)
  {
    return (node_ptr&)(_Ptr->parent);
  }

  static reference value(node_ptr _Ptr)
  {
    return _Ptr->data;
  }

  static const Key& key(node_ptr _Ptr)
  {
    return KeyOfValue()((value(_Ptr)));
  }

  static color_type& color(node_ptr _Ptr)
  {
    return (color_type&)(_Ptr->color);
  }

  static node_ptr& left(base_ptr _Ptr)
  {
    return (node_ptr&)(_Ptr->left);
  }

  static node_ptr& right(base_ptr _Ptr)
  {
    return (node_ptr&)(_Ptr->right);
  }

  static node_ptr& parent(base_ptr _Ptr)
  {
    return (node_ptr&)(_Ptr->parent);
  }

  static reference value(base_ptr _Ptr)
  {
    return ((node_ptr)_Ptr)->data;
  }

  static color_type& color(base_ptr _Ptr)
  {
    return (color_type&)((node_ptr)(_Ptr)->color);
  }

  static const Key& key(base_ptr _Ptr)
  {
    return KeyOfValue()((value(_Ptr)));
  }

  static node_ptr mimimum(node_ptr _Ptr)
  {
    return (node_ptr)rb_tree_node_base::minimum(_Ptr);
  }

  static node_ptr maximum(node_ptr _Ptr)
  {
    return (node_ptr)rb_tree_node_base::maximum(_Ptr);
  }
protected:
  node_ptr _get_node()
  {
    return node_allocator_::allocate();
  }

  void _put_node(node_ptr _Ptr)
  {
    node_allocator_::deallocate(_Ptr);
  }

  node_ptr _create_node(const value_type& _Val)
  {
    node_ptr tmp = _get_node();
    MINI_STL_TRY {
      _MY_STL::construct(&tmp->data, _Val);
    }
    MINI_STL_UNWIND(_put_node(tmp));
    return tmp;
  }
#ifdef MINI_STL_RVALUE_REFS
  node_ptr _create_node(value_type&& _Val)
  {
    node_ptr tmp = _get_node();
    MINI_STL_TRY {
      _MY_STL::construct(&tmp->data, _MY_STL::move(_Val));
    }
    MINI_STL_UNWIND(_put_node(tmp));
    return tmp;
  }
#endif
  node_ptr _clone_node(node_ptr _Ptr)
  {
    node_ptr tmp = _create_node(_Ptr->data);
    tmp->color = _Ptr->color;
    tmp->left = nullptr;
    tmp->right = nullptr;
    return tmp;
  }

  void _destroy_node(node_ptr _Ptr)
  {
    destroy(&_Ptr->data);
    _put_node(_Ptr);
  }

  void _init_empty()
  {
    Myhead_ = _get_node();
    color(Myhead_) = rb_tree_red;
    root() = 0;
    leftmost() = Myhead_;
    rightmost() = Myhead_;
  }

  iterator _insert_aux(base_ptr x_, base_ptr y_, const Value& _Val);
#ifdef MINI_STL_RVALUE_REFS
  iterator _insert_aux(base_ptr x_, base_ptr y_, Value&& _Val);
#endif
  void _erase_aux(base_ptr _Ptr);

  void _clear_aux(node_ptr _Ptr);

  node_ptr _copy_aux(node_ptr _In, node_ptr _To);
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
};

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::rb_tree(const rb_tree& _Right)
{
  if (_Right.size() == 0) {
    _init_empty();
  } else {
    this->Myhead_ = _get_node();
    this->Myhead_->color = rb_tree_red;
    root() = _copy_aux(_Right.root(), this->Myhead_);
    leftmost() = mimimum(this->root());
    rightmost() = maximum(this->root());
  }
  this->Mynode_count_ = _Right.Mynode_count_;
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>&
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  operator =(const rb_tree& _Right)
{
  if (this != &_Right) {
    clear();
    this->Mycomp_ = _Right.Mycomp_;
    this->MygetKey_ = _Right.MygetKey_;
    if (_Right.size()) {
      root() = _copy_aux(_Right.root(), this->Myhead_);
      this->Mynode_count_ = _Right.Mynode_count_;
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
  _copy_aux(node_ptr _In, node_ptr _To)
{
  node_ptr top = _clone_node(_In);
  top->parent = _To;
  if (_In->right)
    top->right = _copy_aux(right(_In), top);
  _To = top;
  _In = left(_In);

  while (_In != 0) {
    node_ptr y = _clone_node(_In);
    _To->left = y;
    y->parent = _To;
    if (_In->right)
      y->right = _copy_aux(right(_In), y);
    _To = y;
    _In = left(_In);
  }
  return top;
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_equal(const Value& _Val)
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  while (x) {
    y = x;
    x = MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(x)) ?
          left(x) : right(x);
  }
  return _insert_aux(x, y, _Val);
}

template<class Key, class Value, class KeyOfValue,
class Compare, class Alloc>
template <class InputIterator>
void
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
insert_equal(InputIterator _First,
             InputIterator _Last,
             typename is_iterator<InputIterator>::ID)
{
  while (_First != _Last)
    insert_equal(*_First++);
}

template<class Key, class Value, class KeyOfValue,
class Compare, class Alloc>
template <class InputIterator>
void
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
insert_unique(InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID)
{
  while (_First != _Last)
    insert_unique(*_First++);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
_MY_STL::pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>
              ::iterator,bool>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_unique(const Value& _Val)
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  bool result = true;
  while (x) {
    y = x;
    result = MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(x));
    x = result ? left(x) : right(x);
  }
  iterator j = iterator(y);
  if (result)
    if (j == begin())
      return pair<iterator,bool>(_insert_aux(x, y, _Val),true);
    else
      --j;
  if (MINI_STL_DEBUG_COMP(Mycomp_, key(j.node), MygetKey_(_Val)))
    return _MY_STL::pair<iterator,bool>(_insert_aux(x, y, _Val),true);
  return _MY_STL::pair<iterator,bool>(j, false);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_unique(iterator _Position, const Value& _Val)
{
  if (_Position.node == Myhead_->left) { // begin()
    if (size() > 0 &&
        MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val),
                            key(_Position.node)))
      return _insert_aux(_Position.node, _Position.node, _Val);
    else
      return insert_unique(_Val).first;
  } else if (_Position.node == Myhead_) { // end()
    if (MINI_STL_DEBUG_COMP(Mycomp_, key(rightmost()), MygetKey_(_Val)))
      return _insert_aux(0, rightmost(), _Val);
    else
      return insert_unique(_Val).first;
  } else {
    iterator before = _Position;
    --before;
    if (MINI_STL_DEBUG_COMP(Mycomp_, key(before.node), MygetKey_(_Val))
        && MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(_Position.node))) {
      if (right(before.node) == 0)
        return _insert_aux(0, before.node, _Val);
      else
        return _insert_aux(_Position.node, _Position.node, _Val);
    } else
      return insert_unique(_Val).first;
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_equal(iterator _Position, const Value& _Val)
{
  if (_Position.node == Myhead_->left) { // begin()
    if (size() > 0 &&
        !MINI_STL_DEBUG_COMP(Mycomp_, key(_Position.node), MygetKey_(_Val)))
      return _insert_aux(_Position.node, _Position.node, _Val);
    else
      return insert_equal(_Val);
  } else if (_Position.node == Myhead_) {// end()
    if (!MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(rightmost())))
      return _insert_aux(0, rightmost(), _Val);
    else
      return insert_equal(_Val);
  } else {
    iterator before = _Position;
    --before;
    if (!MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(before.node))
        && !MINI_STL_DEBUG_COMP(Mycomp_, key(_Position.node), MygetKey_(_Val))) {
      if (right(before.node) == 0)
        return _insert_aux(0, before.node, _Val);
      else
        return _insert_aux(_Position.node, _Position.node, _Val);
    } else
      return insert_equal(_Val);
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::_insert_aux(
    base_ptr x_, base_ptr y_, const Value& _Val)
{
  node_ptr x = (node_ptr)x_;
  node_ptr y = (node_ptr)y_;
  node_ptr z = _create_node(_Val);
  if (y == Myhead_) {//empty tree
    root() = z;
    leftmost() = z;
    rightmost() = z;
  } else if (MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val),key(y))) {//insert left
    left(y) = z;
    if (y == leftmost())
      leftmost() = z;
  } else {
    right(y) = z;
    if (y == rightmost())
      rightmost() = z;
  }
  parent(z) = y;
  left(z) = 0;
  right(z) = 0;
  ++Mynode_count_;
  rb_tree_insert_fixup(z, Myhead_->parent);
  return iterator(z);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
void rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  erase(iterator _Position)
{
  iterator tmp = _Position;
  if (_Position == begin()) {
    ++tmp;
    leftmost() = (node_ptr)(tmp.node);
    _erase_aux(_Position.node);
  } else if (_Position == --end()) {
    --tmp;
    rightmost() = (node_ptr)(tmp.node);
    _erase_aux(_Position.node);
  } else
    _erase_aux(_Position.node);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::size_type
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
erase(const key_type& _Key)
{
  pair<iterator,iterator> p = this->equal_range(_Key);
  size_type n = DISTANCE(p.first, p.second);
  erase(p.first, p.second);
  return n;
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
void rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
 _erase_aux(base_ptr _Ptr)
{
  base_ptr y = _Ptr;
  base_ptr z;
  base_ptr z_parent;
  color_type oldColor = y->color;
  if (!_Ptr->left) {
    z = _Ptr->right;
    z_parent = _Ptr->parent;
    rb_tree_transplant(_Ptr, _Ptr->right, Myhead_->parent);
  } else if (!_Ptr->right) {
    z = _Ptr->left;
    z_parent = _Ptr->parent;
    rb_tree_transplant(_Ptr, _Ptr->left, Myhead_->parent);
  } else {
    y = mimimum((node_ptr)(_Ptr->right));
    oldColor = y->color;
    z = y->right;
    if (y->parent == _Ptr) {
      z_parent = y;
    } else {
      z_parent = y->parent;
      rb_tree_transplant(y, y->right, Myhead_->parent);
      y->right = _Ptr->right;
      y->right->parent = y;
    }
    rb_tree_transplant(_Ptr, y, Myhead_->parent);
    y->left = _Ptr->left;
    y->left->parent = y;
    y->color = _Ptr->color;
  }
  if (oldColor == rb_tree_black)
    rb_tree_erase_fixup(z, z_parent, Myhead_->parent);
  --Mynode_count_;
  _destroy_node((node_ptr)_Ptr);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
void rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  _clear_aux(node_ptr _Ptr)
{
  while (_Ptr!=0) {
    _clear_aux(right(_Ptr));
    node_ptr y = left(_Ptr);
    _destroy_node(_Ptr);
    _Ptr = y;
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
lower_bound(const key_type& _Key)
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  while (x) {
    if (!MINI_STL_DEBUG_COMP(Mycomp_, key(x), _Key)) {
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
lower_bound(const key_type& _Key) const
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  while (x) {
    if (!MINI_STL_DEBUG_COMP(Mycomp_, key(x), _Key)) {
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
upper_bound(const key_type& _Key)
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  while (x) {
    if (MINI_STL_DEBUG_COMP(Mycomp_, _Key, key(x)))
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
upper_bound(const key_type& _Key) const
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  while (x) {
    if (MINI_STL_DEBUG_COMP(Mycomp_, _Key, key(x)))
      y = x, x = left(x);
    else
      x = right(x);
  }
  return const_iterator(y);
}

#ifdef MINI_STL_RVALUE_REFS
template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::insert_equal(Value&& _Val)
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  while (x) {
    y = x;
    x = MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(x)) ?
          left(x) : right(x);
  }
  return _insert_aux(x, y, _MY_STL::move(_Val));
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
_MY_STL::pair<typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>
              ::iterator,bool>
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_unique(Value&& _Val)
{
  node_ptr y = Myhead_;
  node_ptr x = root();
  bool result = true;
  while (x) {
    y = x;
    result = MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(x));
    x = result ? left(x) : right(x);
  }
  iterator j = iterator(y);
  if (result)
    if (j == begin())
      return pair<iterator,bool>
          (_insert_aux(x, y, _MY_STL::move(_Val)),true);
    else
      --j;
  if (MINI_STL_DEBUG_COMP(Mycomp_, key(j.node), MygetKey_(_Val)))
    return _MY_STL::pair<iterator,bool>
        (_insert_aux(x, y, _MY_STL::move(_Val)),true);
  return _MY_STL::pair<iterator,bool>(j, false);
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_unique(iterator _Position, Value&& _Val)
{
  if (_Position.node == Myhead_->left) { // begin()
    if (size() > 0 &&
        MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val),
                            key(_Position.node)))
      return _insert_aux(_Position.node, _Position.node, _Val);
    else
      return insert_unique(_Val).first;
  } else if (_Position.node == Myhead_) { // end()
    if (MINI_STL_DEBUG_COMP(Mycomp_, key(rightmost()), MygetKey_(_Val)))
      return _insert_aux(0, rightmost(), _MY_STL::move(_Val));
    else
      return insert_unique(_MY_STL::move(_Val)).first;
  } else {
    iterator before = _Position;
    --before;
    if (MINI_STL_DEBUG_COMP(Mycomp_, key(before.node), MygetKey_(_Val))
        && MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(_Position.node))) {
      if (right(before.node) == 0)
        return _insert_aux(0, before.node, _MY_STL::move(_Val));
      else
        return _insert_aux(_Position.node, _Position.node, _MY_STL::move(_Val));
    } else
      return insert_unique(_MY_STL::move(_Val)).first;
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::
  insert_equal(iterator _Position, Value&& _Val)
{
  if (_Position.node == Myhead_->left) { // begin()
    if (size() > 0 &&
        !MINI_STL_DEBUG_COMP(Mycomp_, key(_Position.node), MygetKey_(_Val)))
      return _insert_aux(_Position.node, _Position.node, _Val);
    else
      return insert_equal(_MY_STL::move(_Val));
  } else if (_Position.node == Myhead_) {// end()
    if (!MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(rightmost())))
      return _insert_aux(0, rightmost(), _MY_STL::move(_Val));
    else
      return insert_equal(_MY_STL::move(_Val));
  } else {
    iterator before = _Position;
    --before;
    if (!MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val), key(before.node))
        && !MINI_STL_DEBUG_COMP(Mycomp_, key(_Position.node), MygetKey_(_Val))) {
      if (right(before.node) == 0)
        return _insert_aux(0, before.node, _MY_STL::move(_Val));
      else
        return _insert_aux(_Position.node, _Position.node, _MY_STL::move(_Val));
    } else
      return insert_equal(_MY_STL::move(_Val));
  }
}

template<class Key, class Value, class KeyOfValue,
         class Compare, class Alloc>
typename rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::iterator
rb_tree<Key,Value,KeyOfValue,Compare,Alloc>::_insert_aux(
    base_ptr x_, base_ptr y_, Value&& _Val)
{
  node_ptr x = (node_ptr)x_;
  node_ptr y = (node_ptr)y_;
  node_ptr z = _create_node(_MY_STL::move(_Val));
  if (y == Myhead_) {//empty tree
    root() = z;
    leftmost() = z;
    rightmost() = z;
  } else if (MINI_STL_DEBUG_COMP(Mycomp_, MygetKey_(_Val),key(y))) {//insert left
    left(y) = z;
    if (y == leftmost())
      leftmost() = z;
  } else {
    right(y) = z;
    if (y == rightmost())
      rightmost() = z;
  }
  parent(z) = y;
  left(z) = 0;
  right(z) = 0;
  ++Mynode_count_;
  rb_tree_insert_fixup(z, Myhead_->parent);
  return iterator(z);
}
#endif
template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator==(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  return _Left.size() ==  _Right.size() &&
         _MY_STL::equal(_Left.begin(), _Left.end(), _Right.begin());
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator<(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
          const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  return _MY_STL::lexicographical_compare(_Left.begin(),
                                          _Left.end(),
                                          _Right.begin(),
                                          _Right.end());
}
template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator!=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  return !(_Left == _Right);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator>(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
          const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  return _Right < _Left;
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator<=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  return !(_Right < _Left);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline bool
operator>=(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
           const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  return !(_Left < _Right);
}

template <class Key, class Value, class KeyOfValue,
          class Compare, class Alloc>
inline void
swap(const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Left,
     const rb_tree<Key,Value,KeyOfValue,Compare,Alloc>& _Right)
{
  _Left.swap(_Right);
}

MINI_STL_END
#endif // MINI_STL_TREE_H
