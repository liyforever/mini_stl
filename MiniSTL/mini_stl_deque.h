#ifndef MINI_STL_DEQUE_H
#define MINI_STL_DEQUE_H
#include "memory.h"
#include <iostream>
using std::cout;
using std::endl;
MINI_STL_BEGIN

inline size_t __deque_buf_size(size_t n, size_t sz)
{
    return n!=0? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
}

template <class Type, class Ref, class Ptr, size_t BuffSize>
struct _deque_iterator
{
    typedef _deque_iterator<Type,Type&,Type*,BuffSize>    iterator;
    typedef _deque_iterator<Type,const Type&,const Type*,BuffSize> const_iterator;

    typedef random_access_iterator_tag  iterator_category;
    typedef Type                value_type;
    typedef Ptr                 pointer;
    typedef Ref                 reference;
    typedef size_t              size_type;
    typedef ptrdiff_t           difference_type;
    typedef Type**              map_pointer;
    typedef _deque_iterator     Self;
    static size_t _M_buffer_size_aux()
    {
        return __deque_buf_size(BuffSize, sizeof(Type));
    }

    void _M_set_node(map_pointer newNode)
    {
        _M_map_node = newNode;
        _M_first = *newNode;
        _M_last = *newNode + _M_buffer_size_aux();
    }

    Type* _M_cur;
    Type* _M_first;
    Type* _M_last;
    map_pointer _M_map_node;
//constructor
    _deque_iterator():
        _M_cur(0),_M_first(0),_M_last(0),_M_map_node(0){}
    _deque_iterator(Type* x, map_pointer mapPtr)
    {
        _M_cur = x;
        _M_first = *mapPtr;
        _M_last = *mapPtr + _M_buffer_size_aux();
        _M_map_node = mapPtr;
    }

    _deque_iterator(const iterator x)
    {
        _M_cur = x._M_cur;
        _M_first = x._M_first;
        _M_last = x._M_last;
        _M_map_node = x._M_map_node;
    }

    reference operator*() const
    {
        return *cur;
    }

    pointer operator->() const
    {
        return cur;
    }

    difference_type operator-(const Self& x) const
    {
        return difference_type(_M_buffer_size_aux() * (_M_map_node - x._M_map_node - 1)
                               + (_M_cur - _M_first) + (x._M_last - x._M_cur));
    }

    Self& operator++()
    {
        ++_M_cur;
        if(_M_cur==_M_last) {
            _M_set_node(_M_map_node + 1);
            _M_cur = _M_first;
        }
        return *this;
    }

    Self operator++(int)
    {
        Self tmp = *this;
        ++*this;
        return tmp;
    }

    Self& operator--()
    {
        if(_M_cur==_M_first) {
            _M_set_node(_M_map_node - 1);
            _M_cur = _M_last;
        }
        --_M_cur;
        return *this;
    }

    Self operator--(int)
    {
        Self tmp = *this;
        --*this;
        return tmp;
    }

    Self& operator+=(difference_type n)
    {
        //需要移动的距离
        difference_type offset = n + (_M_cur - _M_first);
        if(offset>=0 && offset < difference_type(_M_buffer_size_aux()))
            _M_cur += n;
        else {
            difference_type map_node_offset =
                    offset > 0 ? offset / difference_type(_M_buffer_size_aux())
                               : -difference_type((-offset - 1) / _M_buffer_size_aux()) - 1;
            _M_set_node(_M_map_node + map_node_offset);
            _M_cur = _M_first + (offset - map_node_offset * difference_type(_M_buffer_size_aux()));
        }
        return *this;
    }

    Self operator+(difference_type n) const
    {
        Self tmp = *this;
        return tmp += n;
    }

    Self& operator-=(difference_type n)
    {
        return *this += -n;
    }

    Self operator-(difference_type n) const
    {
        Self tmp = *this;
        return tmp -= n;
    }

    reference operator[](difference_type pos) const
    {
        return *(*this + pos);
    }

    bool operator==(const Self& x) const
    {
        return _M_cur == x._M_cur;
    }

    bool operator!=(const Self& x) const
    {
        return _M_cur != x._M_cur;
    }

    bool operator<(const Self& x) const
    {
        return (_M_map_node == x._M_map_node) ?
                    (_M_cur < x._M_cur) : (_M_map_node < x._M_map_node);
    }

    bool operator>(const Self& x) const
    {
        return x < *this;
    }
};

template <class Type, class Alloc = default_allocator, size_t BuffSize = 0>
class deque
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
    typedef _deque_iterator<Type,Type&,Type*,BuffSize>  iterator;
    typedef _deque_iterator<Type,const Type&, const Type*,BuffSize> const_iterator;
protected:
    typedef pointer*    map_pointer;
    typedef simpleAlloc<value_type,Alloc> _M_data_allocator;
    typedef simpleAlloc<pointer,Alloc> _M_map_allocator;
    iterator _M_first;
    iterator _M_last;
    map_pointer _M_map;
    size_type _M_map_size;
    enum {_M_init_map_size = 8};
protected://aux_function
    pointer _M_allocate_node()
    {
        return _M_data_allocator::allocate(__deque_buf_size(BuffSize, sizeof(Type)));
    }

    void _M_deallocate_node(pointer p)
    {
        _M_data_allocator::deallocate(p, __deque_buf_size(BuffSize, sizeof(Type)));
    }

    map_pointer _M_allocate_map(size_t n)
    {
        _M_map_allocator::allocate(n);
    }

    void _M_deallocate_map(map_pointer p, size_t n)
    {
        _M_map_allocator::deallocate(p, n);
    }

    void _M_create_node(map_pointer first, map_pointer last)
    {
        map_pointer cur;
        MINI_STL_TRY {
        for(cur=first; cur!=last; ++cur)
            *cur = _M_allocate_node();
        }
        MINI_STL_UNWIND(_M_destroy_node(first, cur));
    }

    void _M_destroy_node(map_pointer first, map_pointer last)
    {
        for( ;first!=last;++first)
            _M_data_allocator(*first);
    }

    void _M_fill_init_aux(size_type n, Type& val);

    void _M_init_map(size_type n);

    void _M_deallocate_node(pointer p)
    {
        _M_deallocate_node(p);
    }

    size_t _M_buffer_size()
    {
        return _deque_iterator::_M_buffer_size_aux();
    }

public://constructor and destructor
    explicit deque() :
        _M_first(),_M_last(),_M_map(0),_M_map_size(0) {}

    explicit deque(size_type n)
        :_M_first(),_M_last(),_M_map(0),_M_map_size(0)
    {
        _M_fill_init_aux(n, Type());
    }
    deque(size_type n, const Type& val)
        :_M_first(),_M_last(),_M_map(0),_M_map_size(0)
    {
        _M_fill_init_aux(n, val);
    }
    deque(
       const deque& _Right
    );
#ifdef MINI_STL_MEMBER_TEMPLATES
    template<class InputIterator>
       deque(
          InputIterator _First,
          InputIterator _Last
       );
#endif //MINI_STL_MEMBER_TEMPLATES

#ifdef MINI_STL_HAS_MOVE
    deque(deque&& rhs)
    {
        _M_first = rhs._M_first;
        _M_last = rhs._M_last;
        _M_map_size = rhs._M_map_size;
        _M_map = rhs._M_map;
        rhs._M_map = 0;
    }

#endif //MINI_STL_HAS_MOVE
     ~deque()
     {
         if(_M_map)
         {
             _M_destroy_node(_M_first._M_map_node, _M_last._M_map_node);
             _M_deallocate_map(_M_map, _M_map_size);
         }
     }

public:
    allocator_type get_allocator( ) const
    {
        return Alloc();
    }

    iterator begin()
    {
        return _M_first;
    }

    const_iterator begin() const
    {
        return _M_first;
    }

    iterator end()
    {
        return _M_last;
    }

    const_iterator end() const
    {
        return _M_last;
    }

    const_iterator cbegin() const
    {
        return _M_first;
    }

    const_iterator cend() const
    {
        return _M_last;
    }

    reference front()
    {
        return *_M_first;
    }

    const_reference front()
    {
        return *_M_first;
    }

    reference back()
    {
        return *(_M_last - 1);
    }

    const_reference back() const
    {
        return *(_M_last - 1);
    }

    reference operator[](size_type pos)
    {
        return _M_first[difference_type(pos)];
    }

    const_reference operator [](size_type pos) const
    {
        return _M_first[difference_type(pos)];
    }

    size_type max_size() const
    {
        return size_type(-1);
    }

    size_type size() const
    {
        return _M_last - _M_first;
    }

    bool empty() const
    {
        return _M_last == _M_first;
    }
};
/************************protected aux_function__begin**************************/
template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_M_init_map(size_type n)
{
    size_type mapNodeSize = n / _M_buffer_size() + 1;
    _M_map_size = max(_M_init_map_size, mapNodeSize + 2);
    _M_map = _M_map_allocator::allocate(_M_map_size);

    //使first last保持在中间
    map_pointer newFirst = _M_map + (_M_map_size - mapNodeSize) / 2;
    map_pointer newLast = _M_map + newFirst + mapNodeSize - 1;

    map_pointer curPtr;
    MINI_STL_TRY{
        _M_create_node(newFirst, newLast + 1);
    }
    MINI_STL_UNWIND((_M_deallocate_map(newFirst,_M_map_size),
                    _M_map = 0, _M_map_size = 0));

    //调整迭代器
    _M_first._M_set_node(newFirst);
    _M_last._M_set_node(newLast);
    _M_first._M_cur = _M_first._M_first;
    _M_last._M_cur = _M_last._M_first + _M_map_size % _M_buffer_size();
}

template <class Type, class Alloc, size_t BuffSize>
void deque<Type,Alloc,BuffSize>::_M_fill_init_aux(size_type n, Type& val)
{
    _M_init_map(n);
    map_pointer cur;
    MINI_STL_TRY {
        for(cur=_M_first._M_map_node; cur<_M_last._M_map_node;++cur)
            uninitialized_fill(*cur,*cur + _M_buffer_size(), val);
        uninitialized_fill(_M_last._M_first,_M_last._M_cur, val);
    }
    MINI_STL_UNWIND(destroy(_M_first,iterator(*cur, cur)));
}

/************************protected aux_function__end****************************/
MINI_STL_END
#endif // MINI_STL_DEQUE_H
