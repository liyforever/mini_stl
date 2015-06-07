#ifndef MINI_STL_LIST_H
#define MINI_STL_LIST_H
#include "memory.h"
#include "mini_stl_iterator.h"
#include <iostream>
using std::cout;
using std::endl;
MINI_STL_BEGIN

struct __list_node_base;

struct __list_node_base
{
    __list_node_base* _M_prev;
    __list_node_base* _M_next;
};

template <class Type>
struct _list_node : public __list_node_base
{
    Type _M_data;
};

struct __list_iterator_base
{
    typedef size_t                      size_type;
    typedef ptrdiff_t                   difference_type;
    typedef bidirectional_iterator_tag  iterator_category;

    __list_node_base* _M_node;

    __list_iterator_base() {}
    __list_iterator_base(__list_node_base* x) : _M_node(x) {}

    void _M_incr() { _M_node = _M_node->_M_next; }
    void _M_decr() { _M_node = _M_node->_M_prev; }

    bool operator==(const __list_iterator_base& rhs) const
    {
        return _M_node == rhs._M_node;
    }
    bool operator!=(const __list_iterator_base& rhs) const
    {
        return _M_node != rhs._M_node;
    }
};

template <class Type, class Ref, class Ptr>
struct _list_iterator : public __list_iterator_base
{
    typedef _list_iterator<Type,Type*,Type&>             iterator;
    typedef _list_iterator<Type,const Type&,const Type*> const_iterator;
    typedef _list_iterator<Type,Ref,Ptr>                 Self;

    typedef Type             value_type;
    typedef Ptr              pointer;
    typedef Ref              reference;
    typedef _list_node<Type> node_type;

    _list_iterator() {}
    _list_iterator(node_type* x) : __list_iterator_base(x) {}
    _list_iterator(const iterator& x) : __list_iterator_base(x._M_node) {}

    reference operator*() const
    {
        return static_cast<node_type*>(_M_node)->_M_data;
    }

    pointer operator->() const
    {
        return &(operator *());
    }

    Self& operator++()
    {
        this->_M_incr();
        return *this;
    }

    Self operator++(int)
    {
        Self tmp = *this;
        this->_M_incr();
        return tmp;
    }

    Self& operator--()
    {
        this->_M_decr();
        return *this;
    }

    Self operator--(int)
    {
        Self tmp = *this;
        this->_M_decr();
        return tmp;
    }
};


template <class Type, class Alloc = default_allocator>
class list
{
public:
    typedef Type              value_type;
    typedef value_type*       pointer;
    typedef const value_type* const_pointer;
    typedef value_type&       reference;
    typedef const value_type& const_reference;
    typedef size_t            size_type;
    typedef ptrdiff_t         difference_type;
    typedef Alloc             allocator_type;
    typedef _list_iterator<Type,Type&,Type*>             iterator;
    typedef _list_iterator<Type,const Type&,const Type*> const_iterator;
protected:
    typedef _list_node<Type>    node_type;
    typedef _list_node<Type>*   node_ptr;
    typedef simpleAlloc<node_type, default_allocator>   _M_data_allocator;
    node_ptr _M_head;
    size_type _M_size;
protected://aux_function
    void _M_empty_init()
    {
        _M_head->_M_next = _M_head;
        _M_head->_M_prev = _M_head;
        _M_size = 0;
    }

    node_ptr _M_create_node(const Type& val)
    {
        node_ptr tmp = _M_data_allocator::allocate(1);
        MINI_STL_TRY {
            construct(&tmp->_M_data, val);
        }
        MINI_STL_UNWIND(_M_data_allocator::deallocate(tmp));
        return tmp;
    }

    void _M_destroy_node(node_ptr p)
    {
        destroy(&p->_M_data);
        _M_data_allocator::deallocate(p);
    }

public://constructor and destroy
    explicit list()
    {
        _M_head = _M_data_allocator::allocate();
        _M_empty_init();
    }
    explicit list(size_type count)
    {
        _M_head = _M_data_allocator::allocate();
        _M_empty_init();
        insert(begin(), count, Type());
    }

    list(size_type count, const Type& val)
    {
        _M_head = _M_data_allocator::allocate();
        _M_empty_init();
        insert(begin(), count, val);
    }

    list(const list& rhsList)
    {
        _M_head = _M_data_allocator::allocate();
        _M_empty_init();
        _insert(begin(), rhsList.begin(), rhsList.end());
    }

    ~list()
    {
        clear();
        _M_data_allocator::deallocate(_M_head);
    }

#ifdef MINI_STL_MEMBER_TEMPLATES
   // template <class InputIterator>
    //list(InputIterator first, InputIterator last);
#endif

    list& operator=(const list& rhsList)
    {
        if(this != &rhsList)
            _assign(rhsList.begin(), rhsList.end());
        return *this;
    }

#ifdef MINI_STL_HAS_MOVE
    list(list&& rhsList)
    {
        _M_head = rhsList._M_head;
        _M_size = rhsList._M_size;
        rhsList._M_head = 0;
    }

    list& operator=(list&& rhsList)
    {
        clear();
        _M_data_allocator::deallocate(_M_head);
        _M_head = rhsList._M_head;
        _M_size = rhsList._M_size;
        rhsList._M_head = nullptr;
        return *this;
    }

#endif //MINI_STL_HAS_MOVE
public:
    iterator begin()
    {
        return (node_ptr)(_M_head->_M_next);
    }
    const_iterator begin() const
    {
        return (node_ptr)(_M_head->_M_next);
    }
    iterator end()
    {
        return _M_head;
    }
    const_iterator end() const
    {
        return _M_head;
    }

    const_iterator cbegin() const
    {
        return (node_ptr)(_M_head->_M_next);
    }
    const_iterator cend() const
    {
        return _M_head;
    }

    reference front()
    {
        return *begin();
    }

    const_reference front() const
    {
        return *begin();
    }

    reference back()
    {
        return *(--end());
    }

    const_reference back() const
    {
        return *(--end());
    }

    size_type size() const
    {
        return _M_size;
    }

    size_type max_size() const
    {
        return size_type(-1);
    }
    bool empty() const
    {
        return _M_size == 0;
    }

    void clear();
    iterator insert(iterator position, const Type& val)
    {
        node_ptr tmp = _M_create_node(val);
        tmp->_M_prev = position._M_node->_M_prev;
        tmp->_M_next = position._M_node;
        position._M_node->_M_prev->_M_next = tmp;
        position._M_node->_M_prev = tmp;
        ++_M_size;
        return tmp;
    }

    void insert(iterator position, size_type count, const Type &val)
    {
        iterator tmp = position;
        for( ; count; --count)
        {
            tmp = insert(tmp, val);
        }
    }
//#ifndef MINI_STL_MEMBER_TEMPLATES
#ifdef MINI_STL_MEMBER_TEMPLATES
    template <class InputIterator>
    void _insert(iterator position,
                InputIterator first,
                InputIterator end)
    {
        while(first != end)
            insert(position,*first++);
    }
#endif

#ifdef MINI_STL_HAS_MOVE
    //iterator insert(iterator position, const Type&& val);
#endif //MINI_STL_HAS_MOVE

    iterator erase(iterator position);
    iterator erase(iterator first, iterator last);

    void remove(const Type& val);
#ifdef MINI_STL_MEMBER_TEMPLATES
    template <class Predicate>
    void remove_if(Predicate pre);
#endif //MINI_STL_MEMBER_TEMPLATES

    void push_back(const Type& val)
    {
        insert(end(), val);
    }

    void assign(size_type count, const Type& val);
#ifdef MINI_STL_MEMBER_TEMPLATES
    template <class InputIterator>
    void _assign(
            InputIterator first,
            InputIterator last
        );

    template<class BinaryPredicate>
       void unique(
          BinaryPredicate Pred
       );
#endif //MINI_STL_MEMBER_TEMPLATES

   void unique();
#ifdef MINI_STL_MEMBER_TEMPLATES
   template <class BinaryPredicate>
   void sort(BinaryPredicate pred);

#endif //MINI_STL_MEMBER_TEMPLATES

   void sort();

// list cmp
    bool operator == (const list& rhsList)
    {
        if(size() != rhsList.size())
            return false;
        iterator lhsIter =  begin();
        const_iterator rhsIter = rhsList.begin();
        for( ; lhsIter!=end(); ++lhsIter,++rhsIter)
        {
            if(*lhsIter != *rhsIter)
                return false;
        }
        return true;
    }
    bool operator != (const list& rhsList)
    {
        if(size() != rhsList.size())
            return true;
        return !(*this == rhsList);
    }

    bool operator > (const list& rhsList)
    {
        iterator lhsIter = begin();
        iterator rhsIter = rhsList.begin();
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsList.end(); ++lhsIter,++rhsIter)
        {
            if(*rhsIter < *lhsIter)
                return true;
            else
                return false;
        }
    }

    bool operator <= (const list& rhsList)
    {
        iterator lhsIter = begin();
        iterator rhsIter = (iterator)(rhsList.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsList.end(); ++lhsIter,++rhsIter)
        {
            if(*lhsIter < *rhsIter)
                return true;
            else if(*rhsIter < *lhsIter)
                return false;
        }
        return size() <= rhsList.size() ? true : false;
    }

    bool operator < (const list& rhsList)
    {
        iterator lhsIter = begin();
        iterator rhsIter = (iterator)(rhsList.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsList.end(); ++lhsIter,++rhsIter)
        {
            if(*lhsIter < *rhsIter)
                return true;
            else if(*rhsIter < *lhsIter)
                return false;
        }
    }

    bool operator >= (const list& rhsList)
    {
        iterator lhsIter = begin();
        iterator rhsIter = (iterator)(rhsList.begin());
        for( ; lhsIter!=this->_M_last &&
             rhsIter!=rhsList.end(); ++lhsIter,++rhsIter)
        {
            if(*rhsIter < *lhsIter)
                return true;
            else if(*lhsIter < *rhsIter)
                return false;
        }
        return size() >= rhsList.size() ? true : false;
    }
};

/************************public function__begin******************************/

template <class Type, class Alloc>
typename list<Type,Alloc>::iterator
list<Type,Alloc>::erase(iterator position)
{
    __list_node_base* prev_node = position._M_node->_M_prev;
    __list_node_base* next_node = position._M_node->_M_next;
    prev_node->_M_next = next_node;
    next_node->_M_prev = prev_node;
    destroy(&*position);
    _M_data_allocator::deallocate((node_ptr)(position._M_node));
    --_M_size;
    return (node_ptr)(next_node);
}

template <class Type, class Alloc>
typename list<Type,Alloc>::iterator
list<Type,Alloc>::erase(iterator first, iterator last)
{
    for( ; first!=last; )
        first = erase(first);
    return first;
}

template <class Type, class Alloc>
void list<Type,Alloc>::clear()
{
    if(!_M_head)
        return;
    node_ptr cur = (node_ptr)(_M_head->_M_next);
    node_ptr tmp;
    while(cur != _M_head) {
        tmp = cur;
        cur = (node_ptr)(tmp->_M_next);
        destroy(&tmp);
        _M_data_allocator::deallocate(tmp);
    }
    _M_empty_init();
}

template <class Type, class Alloc>
void list<Type,Alloc>::assign(size_type count, const Type& val)
{
    iterator i = begin();
    for( ; i!=end() && count > 0; ++i, --count)
        *i = val;
    if(count > 0)
        insert(end(), count, val);
    else
        erase(i, end());
}

#ifdef MINI_STL_MEMBER_TEMPLATES
template <class Type, class Alloc>
template <class InputIterator>
void list<Type,Alloc>::_assign(
        InputIterator first,
        InputIterator last
      )
{
    size_type newSize = 0;
    iterator cur = begin();
    for( ; cur!=end() && first != last;
         ++cur, ++first) {
        *cur = *first;
        ++newSize;
    }
    if(cur==end()) {
        for( ; first != last; ++first)
            insert(cur, *first);
    } else {
        erase(cur, end());
    }
    _M_size = newSize;
}
template <class Type, class Alloc>
template <class Predicate>
void list<Type,Alloc>::remove_if(Predicate pre)
{
    for(iterator beg = begin(); beg != end(); ++beg)
        if(pre(*beg))
            beg = erase(beg);
}

#endif //MINI_STL_MEMBER_TEMPLATES

template <class Type, class Alloc>
void list<Type,Alloc>::remove(const Type& val)
{
    for(iterator beg = begin(); beg != end(); ++beg)
        if(*beg == val)
            beg = erase(beg);
}
#ifdef MINI_STL_MEMBER_TEMPLATES
template <class Type, class Alloc>
template<class BinaryPredicate>
   void list<Type,Alloc>::unique(
           BinaryPredicate Pred
           )
   {
       iterator first = begin();
       iterator last = end();
       if(first == last)
           return;
       iterator next = first;
       while(++next != last) {
           if(Pred(*next,*first))
               erase(next);
           else
               first = next;
           next = first;
       }
   }

#endif //MINI_STL_MEMBER_TEMPLATES

template <class Type, class Alloc>
void list<Type,Alloc>::unique()
{
    iterator first = begin();
    iterator last = end();
    if(first == last)
        return;
    iterator next = first;
    while(++next != last) {
        if(*next == *first)
            erase(next);
        else
            first = next;
        next = first;
    }
}

#ifdef MINI_STL_MEMBER_TEMPLATES
template <class Type, class Alloc>
template <class BinaryPredicate>
void sort(BinaryPredicate pred)
{

}

#endif //MINI_STL_MEMBER_TEMPLATES

void sort()
{

}

/************************public function__end********************************/

MINI_STL_END
#endif // MINI_STL_LIST_H
