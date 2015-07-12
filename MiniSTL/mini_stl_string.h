#ifndef MINI_STL_STRING_H
#define MINI_STL_STRING_H
#include "memory.h"
#include "mini_stl_chartraits.h"
#include <iosfwd>
#define protected public
#define private public
MINI_STL_BEGIN


template <class CharType,
          class Traits = char_traits<CharType>,
          class Alloc = default_allocator>
class basic_string
{
public:  
  typedef CharType          value_type;
  typedef Traits            traits_type;
  typedef value_type*       pointer;
  typedef const value_type* const_pointer;
  typedef value_type&       reference;
  typedef const value_type& const_reference;
  typedef size_t            size_type;
  typedef ptrdiff_t         difference_type;
  typedef Alloc             allocator_type;
  typedef value_type*       iterator;
  typedef const value_type* const_iterator;
  typedef _MY_STL::reverse_iterator<iterator> reverse_iterator;
  typedef _MY_STL::reverse_iterator<const_iterator> const_reverse_iterator;
public:
  static const size_type npos;
private:
  typedef simpleAlloc<value_type, Alloc> data_allocator_;
  typedef simpleAlloc<size_type, Alloc> use_allocator_;
  iterator first_;
  iterator last_;
  iterator end_;
  size_type *use_;
public:
  size_type get_use() const
  {
    return *use_;
  }

  basic_string()
  {
    _init_block(8);
  }

  explicit basic_string(const allocator_type& /*AL*/)
  {
    _init_block(8);
  }

  basic_string(const basic_string& Right)
  {
    first_ = Right.first_;
    last_ = Right.last_;
    end_ = Right.end_;
    use_ = Right.use_;
    ++*use_;
  }

#ifdef MINI_STL_RVALUE_REFS
  basic_string(basic_string&& Right)
  {
    first_ = Right.first_;
    last_ = Right.last_;
    end_ = Right.end_;
    use_ = Right.use_;
    ++*use_;
  }
#endif
  basic_string(
      const basic_string& Right,
      size_type Pos,
      size_type Count = npos,
      const allocator_type& /*Al*/ = allocator_type()
  )
  {
    if (pos > Right.size())
      MINI_STL_THROW_RANGE_ERROR("basic_string");
    else
      _init_range(Right.begin() + Pos,
                  Right.begin() + Pos + min(Count, Right.size() - Pos));
  }

  basic_string(
      const_pointer Ptr,
      size_type Count,
      const allocator_type& /*Al*/ = allocator_type()
  )
  {
    _init_range(Ptr, Ptr + Count);
  }

  basic_string(const_pointer Ptr, const allocator_type& /*Al*/ = allocator_type())
  {
    _init_range(Ptr, Ptr + traits_type::length(Ptr));
  }

  basic_string(
      size_type Count,
      value_type Ch,
      const allocator_type& /*Al*/ = allocator_type()
  )
  {
    _init_block(Count + 1);
    traits_type::assign(first_, Count, Ch);
    last_ = first_ + Count;
  }

  template <class InputIterator>
      basic_string(
          InputIterator First,
          InputIterator Last,
          const allocator_type& /*Al*/ = allocator_type(),
          typename is_iterator<InputIterator>::ID = Identity()
      )
  {
    _init_range(First, Last);
  }

  basic_string(const_pointer First, const_pointer Last)
  {
    _init_range(First, Last);
  }

  basic_string& operator=(value_type Ch)
  {
    _destroy_block();
    _init_block(2);
    return *this;
  }

  basic_string& operator=(const_pointer Ptr);

  basic_string& operator=(const basic_string& Right);

#ifdef MINI_STL_RVALUE_REFS
  basic_string& operator=(basic_string&& Right);
#endif
  ~basic_string()
  {
    _destroy_block();
  }

public:
  allocator_type get_allocator() const
  {
    return allocator_type();
  }

  size_type max_size() const
  {
    return (size_type)(-1);
  }

  iterator begin()
  {
    return first_;
  }

  const_iterator begin() const
  {
    return first_;
  }

  iterator end()
  {
    return last_;
  }

  const_iterator end() const
  {
    return last_;
  }

  reverse_iterator rbegin()
  {
    return reverse_iterator(last_);
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(last_);
  }

  reverse_iterator rend()
  {
    return reverse_iterator(first_);
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(first_);
  }

  const_iterator cbegin() const
  {
    return first_;
  }

  const_iterator cend() const
  {
    return last_;
  }

  const_reverse_iterator crbegin() const
  {
    return rbegin();
  }

  const_reverse_iterator crend() const
  {
    return rend();
  }

  size_type size() const
  {
    return last_ - first_;
  }

  bool empty() const
  {
    return first_ == last_;
  }

  basic_string& insert(size_type Pos, const value_type* Ptr)
  {
    insert(first_ + Pos, Ptr, Ptr + traits_type::length(Ptr));
    return *this;
  }

  basic_string& insert(
       size_type Off,
       const value_type* Ptr,
       size_type Count
    )
  {
    insert(first_ + Off, Ptr, Ptr + Count);
    return *this;
  }

  basic_string& insert(size_type Off,const basic_string& Str)
  {
    insert(first_ + Off, Str.begin(), Str.end());
    return *this;
  }

  basic_string& insert(
      size_type Off1,
      const basic_string& Str,
      size_type Off2,
      size_type Count
    )
  {
    insert(first_ + Off1, Str.begin() + Off2, Str.begin() + Off2 + Count);
    return *this;
  }

  basic_string& insert(size_type Off, size_type Count, value_type Ch);

  iterator insert(iterator Position)
  {
    size_type off = Position - first_;
    insert(off, 1, value_type());
    return first_ + off;
  }

  iterator insert(iterator Position, value_type Ch)
  {
    size_type off = Position - first_;
    insert(off, 1, Ch);
    return first_ + off;
  }

  void insert(iterator Position,size_type Count, value_type Ch)
  {
    size_type off = Position - first_;
    insert(off, Count, Ch);
  }

  template<class InputIterator>
    void insert(
        iterator Position,
        InputIterator First,
        InputIterator Last,
        typename is_iterator<InputIterator>::ID = Identity()
      );



  /*basic_string& assign(const value_type* ptr)
  {

  }

  basic_string& assign(const value_type* ptr, size_type count);

  basic_string& assign(const basic_string& str, size_type off, size_type count);

  basic_string& assign(const basic_string& str);

  basic_string& assign(size_type _Count, value_type ch);

  template<class InputIterator>
    basic_string& assign(
            InputIterator _First,
            InputIterator _Last
        );

  basic_string& assign(const_iterator first, const_iterator last);


  /*basic_string& append(const_pointer* ptr)
  {
    if (*use_ == 1) {
      ;
    else {

    }
  }

  basic_string& append(const value_type* _Ptr, size_type _Count)
  {
    ;
  }
  basic_string& append(
        const basic_string& _Str,
        size_type _Off,
        size_type _Count
    );
    basic_string<CharType, Traits, Allocator>& append(
        const basic_string<CharType, Traits, Allocator>& _Str
    );
    basic_string<CharType, Traits, Allocator>& append(
        size_type _Count,
        value_type _Ch
    );
    template<class InputIterator>
        basic_string<CharType, Traits, Allocator>& append(
            InputIterator _First,
            InputIterator _Last
        );
    basic_string<CharType, Traits, Allocator>& append(
        const_pointer _First,
        const_pointer _Last
    );
    basic_string<CharType, Traits, Allocator>& append(
        const_iterator _First,
        const_iterator _Last
    );*/

protected:
  pointer _allocate(size_t n)
  {
    return data_allocator_::allocate(n);
  }

  void _deallocate(pointer p, size_t n)
  {
    if (p)
      data_allocator_::deallocate(p, n);
  }

  void _init_block(size_t n)
  {
    first_ = _allocate(n);
    last_ = first_;
    end_ = first_ + n;
    use_ = use_allocator_::allocate();
    construct(use_, size_t(1));
  }

  void _destroy_block()
  {
    if (--*use_ == 0) {
      _deallocate(first_, end_ - first_);
      use_allocator_::deallocate(use_);
    }
  }

  void _make_terinate()
  {
    MINI_STL_TRY {
      _construct_null(last_);
    }
    MINI_STL_UNWIND(destroy(first_, last_));
  }

  void _construct_null(pointer p)
  {
    construct(p);
    MINI_STL_TRY {
      *p = (value_type)(0);
    }
    MINI_STL_UNWIND(destroy(p));
  }

  void _init_range(const_pointer first, const_pointer last)
  {
    size_type n = last - first;
    _init_block(n + 1);
    last_ = uninitialized_copy(first, last, first_);
    _make_terinate();
  }
};

template <class CharType,class Traits,class Alloc>
const typename basic_string<CharType,Traits,Alloc>::size_type
basic_string<CharType,Traits,Alloc>::npos = (basic_string<CharType,Traits,Alloc>::size_type)(-1);


template <class CharType,class Traits,class Alloc>
template<class InputIterator>
void basic_string<CharType,Traits,Alloc>::insert(
    iterator Position,
    InputIterator First,
    InputIterator Last,
    typename is_iterator<InputIterator>::ID
  )
{
  if (*use_ == 1) {
    if (First != Last) {//范围不空
      size_type n = Last - First;
      if ((size_type)(end_ - last_) >= n) {//还有空间
        iterator oldLast = last_;
        copy_backward((iterator)Position, oldLast, oldLast + n);
        copy(First, Last, (iterator)Position);
        last_ += n;
      } else {//空间不够
        const size_type oldSize = size();
        const size_type newSize = oldSize + max((size_type)oldSize, (size_type)n);
        iterator newFirst = data_allocator_::allocate(newSize);
        iterator newLast = newFirst;
        MINI_STL_TRY {
          newLast = uninitialized_copy(first_, (iterator)Position, newFirst);
          newLast = uninitialized_copy(First, Last, newLast);
          newLast = uninitialized_copy((iterator)Position, last_, newLast);
        }
        MINI_STL_UNWIND((destroy(newFirst,newLast),
                            data_allocator_::deallocate(newFirst, newSize)));
        destroy(first_, last_);
        _deallocate(first_, end_ - first_);
        first_ = newFirst;
        last_ = newLast;
        end_ = newFirst + newSize;
      }
    }
    _make_terinate();
  } else {
    --*use_;
    size_type length = size();
    size_type insert_length = DISTANCE(First, Last);
    iterator old_first = first_;
    iterator old_last = last_;
    _init_block(length + insert_length + 1);
    last_ = uninitialized_copy(old_first, Position, first_);
    last_ = uninitialized_copy(First, Last, last_);
    last_ = uninitialized_copy(Position, old_last, last_);
    _make_terinate();
  }
}

template <class CharType,class Traits,class Alloc>
basic_string<CharType,Traits,Alloc>&
basic_string<CharType,Traits,Alloc>::insert(
          size_type Off, size_type Count, value_type Ch)
{
  iterator Position = first_ + Off;
  if (*use_ == 1) {
    if ((size_type)(end_ - last_) >= Count) {//还有空间
      iterator oldLast = last_;
      copy_backward((iterator)Position, oldLast, oldLast + Count);
      traits_type::assign(Position, Count, Ch);
      last_ += Count;
    } else {//空间不够
      const size_type oldSize = size();
      const size_type newSize = oldSize + max((size_type)oldSize, (size_type)Count);
      iterator newFirst = data_allocator_::allocate(newSize);
      iterator newLast = newFirst;
      MINI_STL_TRY {
        newLast = uninitialized_copy(first_, (iterator)Position, newFirst);
        traits_type::assign(newLast, Count, Ch);
        newLast += Count;
        newLast = uninitialized_copy(Position, last_, newLast);
      }
      MINI_STL_UNWIND((destroy(newFirst,newLast),
                            data_allocator_::deallocate(newFirst, newSize)));
      destroy(first_, last_);
      _deallocate(first_, end_ - first_);
      first_ = newFirst;
      last_ = newLast;
      end_ = newFirst + newSize;
    }
    _make_terinate();
  } else {
    --*use_;
    size_type length = size();
    iterator old_first = first_;
    iterator old_last = last_;
    _init_block(length + Count + 1);
    last_ = uninitialized_copy(old_first, Position, first_);
    traits_type::assign(last_, Count, Ch);
    last_ += Count;
    last_ = uninitialized_copy(Position, old_last, last_);
    _make_terinate();
  }
  return *this;
}

template <class CharType,class Traits,class Alloc>
std::ostream& operator <<(std::ostream& os, const basic_string<CharType,Traits,Alloc>& Str)
{
  os << Str.first_;
  return os;
}

typedef basic_string<char, char_traits<char>, default_allocator> string;
typedef basic_string<wchar_t, char_traits<wchar_t>, default_allocator> wstring;
MINI_STL_END
#endif // MINI_STL_STRING_H
