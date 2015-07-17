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
    clear();
    insert(begin(), 1, Ch);
    return *this;
  }

  basic_string& operator=(const_pointer Ptr)
  {
    clear();
    insert(begin, Ptr, Ptr+traits_type::length(Ptr));
    return *this;
  }

  basic_string& operator=(const basic_string& Right)
  {
    if (this != &Right) {
      _destroy_block();
      this->first_ = Right.first_;
      this->last_ = Right.last_;
      this->end_ = Right.end_;
      this->use_ = Right.use_;
      ++*use_;
    }
    return *this;
  }

#ifdef MINI_STL_RVALUE_REFS
  basic_string& operator=(basic_string&& Right)
  {
    _destroy_block();
    this->first_ = Right.first_;
    this->last_ = Right.last_;
    this->end_ = Right.end_;
    this->use_ = Right.use_;
    ++*use_;
    return *this;
  }
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
    _detach();
    return first_;
  }

  const_iterator begin() const
  {
    return first_;
  }

  iterator end()
  {
    _detach();
    return last_;
  }

  const_iterator end() const
  {
    return last_;
  }

  reverse_iterator rbegin()
  {
    _detach();
    return reverse_iterator(last_);
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(last_);
  }

  reverse_iterator rend()
  {
    _detach();
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

  reference at(size_type Off)
  {
    _detach();
    return first_[Off];
  }

  const_reference at(size_type Off) const
  {
    return first_[Off];
  }

  reference operator[](size_type Off)
  {
    _detach();
    return first_[Off];
  }

  const_reference operator[](size_type Off) const
  {
    return first_[Off];
  }

  reference back()
  {
    _detach();
    return *(last_ - 1);
  }

  const_reference back() const
  {
    return *(last_ - 1);
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

  basic_string& assign(const value_type* Ptr)
  {
    size_type n = traits_type::length(Ptr);
    return assign(Ptr, Ptr+n);
  }

  basic_string& assign(const value_type* Ptr, size_type Count)
  {
    return assign(Ptr, Ptr+Count);
  }

  basic_string& assign(const basic_string& Str, size_type Off, size_type Count)
  {
    return assign(Str.begin()+Off, Str.begin()+Off+Count);
  }

  basic_string& assign(const basic_string& Str)
  {
    return assign(Str.begin(), Str.end());
  }

  basic_string& assign(size_type Count, value_type Ch)
  {
    clear();
    insert(begin(), Count, Ch);
    return *this;
  }

  template<class InputIterator>
    basic_string& assign(
      InputIterator First,
      InputIterator Last,
      typename is_iterator<InputIterator>::ID = Identity()
    )
  {
    clear();
    insert(begin(),First, Last);
    return *this;
  }

  template<class InputIterator>
    basic_string& append(
      InputIterator First,
      InputIterator Last,
      typename is_iterator<InputIterator>::ID = Identity()
    )
  {
    return insert(end(), First, Last);
  }

  basic_string& append(const value_type* Ptr)
  {
    return insert(end(), Ptr, Ptr+traits_type::length(Ptr));
  }

  basic_string& append(const value_type* Ptr, size_type Count)
  {
    return insert(end(), Ptr, Ptr+Count);
  }
  basic_string& append(
        const basic_string& Str,
        size_type Off,
        size_type Count
    )
  {
    return insert(end(), Str.begin()+Off, Str.begin()+Off+Count);
  }

   basic_string& append(const basic_string& Str)
  {
    return insert(end(), Str.begin(), Str.end());
  }

  basic_string& append(size_type Count, value_type Ch)
  {
    insert(end(), Count, Ch);
    return *this;
  }

  basic_string& erase(size_type Pos = 0, size_type Count = npos)
  {
    if (Pos > size())
      MINI_STL_THROW_RANGE_ERROR("basic_string");
    erase(first_ + Pos, first_ + Pos + min(Count, size() - Pos));
    return *this;
  }

  iterator erase(iterator Position)
  {
    return erase(Position, Position + 1);
  }

  iterator erase(iterator First, iterator Last)
  {
    if (*use_ == 1) {
      if (First != Last) {
        traits_type::move(First, Last, (last_ - Last) + 1);//include null
        const iterator new_last = last_ - (Last - First);
        destroy(new_last + 1, last + 1);
        last_ = new_last;
      }
      return First;
    } else {
      --*use_;
      iterator old_first = first_;
      iterator old_last = last_;
      size_type n1 = First - first_;
      size_type n2 = last_ - Last;
      _init_block(n1 + n2 + 1);
      last_ = uninitialized_copy(old_first, First, first_);
      last_ = uninitialized_copy(Last, old_last, last_);
      _make_terinate();
      return first_ + n1;
    }
  }

  void pop_back()
  {
    erase(--end());
  }

  void push_back(value_type Ch)
  {
    insert(end(), 1, Ch);
  }

  size_type find(value_type Ch, size_type Off = 0) const
  {

  }
    size_type find(
        const value_type* _Ptr,
        size_type _Off = 0
    ) const;
    size_type find(
        const value_type* _Ptr,
        size_type _Off,
        size_type _Count
    ) const;
    size_type find(
        const basic_string<CharType, Traits, Allocator>& _Str,
        size_type _Off = 0
    ) const;

  void clear()
  {
    if (*use_ == 1) {
      if (!empty()) {
        traits_type::assign(*first_, value_type());
        destroy(first_+1, last_+1);
        last_ = first_;
      }
    } else {
      --*use_;
      _init_block(8);
    }
  }

  size_type capacity() const
  {
    return end_ - first_ - 1;
  }

  const value_type *c_str() const
  {
    return first_;
  }

  const value_type *data() const
  {
    return first_;
  }
  size_type copy(value_type* Ptr, size_type Count, size_type Off = 0) const
  {\
    traits_type::copy(Ptr, first_+Off, Count);
    return Count;
  }

  basic_string& replace(size_type Pos1, size_type Num1, const value_type* Ptr)
  {
    return replace(begin()+Pos1, begin()+Pos1+Num1, 
                   Ptr, Ptr+traits_type::length(Ptr));
  }
    
  basic_string& replace(size_type Pos1, size_type Num1, const basic_string& Str)
  {
    return replace(begin()+Pos1, begin()+Pos1+Num1,
                   Str.begin(), Str.end());
  }
    
  basic_string& replace(size_type Pos1, size_type Num1, 
                        const value_type* Ptr, size_type Num2)
  {
    return replace(begin()+Pos1, begin()+Pos1+Num1,
                   Ptr, Ptr+Num2);
  }
    
  basic_string& replace(size_type Pos1, size_type Num1,
                        const basic_string& Str,
                        size_type Pos2, size_type Num2)
  {
    return replace(begin()+Pos1, begin()+Pos1+Num1,
                   Str.begin()+Pos2, Str.begin()+Pos2+Num2);
  }
    
 /* basic_string& replace(
       size_type Pos1,
       size_type Num1,
       size_type Count,
       value_type Ch
    )
  {
    return replace(begin()+Pos1, begin()+Pos1+Num1,)
  }*/
    
  basic_string& replace(
       iterator First,
       iterator Last,
       const value_type* Ptr
    )
  {
    return replace(First, Last, Ptr, Ptr+traits_type::length(Ptr));
  }
    
  basic_string& replace(
       iterator First,
       iterator Last,
       const basic_string& Str
    )
  {
    return replace(First, Last,
                   Str.begin(), Str.end());
  }
    
  basic_string& replace(
       iterator First,
       iterator Last,
       const value_type* Ptr,
       size_type Num2
    )
  {
    return replace(First, Last,
                   Ptr+Num2, Ptr+traits_type::length(Ptr));
  }
    
  /*basic_string& replace(
       iterator _First0,
       iterator _Last0,
       size_type _Num2,
       value_type _Ch
    );*/
    
  template<class InputIterator>
    basic_string& replace(
      iterator First1,
      iterator Last1,
      InputIterator First2,
      InputIterator Last2,
      typename is_iterator<InputIterator>::ID = Identity()
    );

  reference front()
  {
    _detach();
    return *first_;
  }

  const_reference front() const
  {
    return *first_;
  }

  size_type length( ) const
  {
    return size();
  }

  int compare(const basic_string& Str) const
  {
    return _compare_aux(first_, last_, Str.first_, Str.last_);
  }

  int compare(size_type Pos1, size_type Num1, const basic_string& Str) const
  {
    if (Pos1 > size())
      MINI_STL_THROW_RANGE_ERROR("basic_string");
    return _compare_aux(first_ + Pos1,
                        first_ + Pos1 + min(Num1, size() - Pos1),
                        Str.first_, Str.last_);
  }

  int compare(size_type Pos1, size_type Num1,
                const basic_string& Str,
                size_type Pos2, size_type Num2) const
  {
    if (Pos1 > size() || Pos2 > Str.size())
      MINI_STL_THROW_RANGE_ERROR("basic_string");
    return _compare_aux(first_ + Pos1,
                        first_ + Pos1 + min(Num1, size() - Pos1),
                        Str.first_ + Pos2,
                        Str.first_ + Pos2 + min(Num2, size() - Pos2));
  }

  int compare(const value_type* Ptr) const
  {
    return _compare_aux(first_, last_, Ptr, Ptr + traits_type::length(Ptr));
  }

  int compare(size_type Pos1, size_type Num1, const value_type* Ptr) const
  {
    if (Pos1 > size())
      MINI_STL_THROW_RANGE_ERROR("basic_string");
    return _compare_aux(first_ + Pos1,
                        first_ + Pos1 + min(Num1, size() - Pos1),
                        Ptr, Ptr + traits_type::length(ptr));
  }

  int compare(size_type Pos1, size_type Num1, const value_type* Ptr,
                size_type Num2) const
  {
    if (__pos1 > size())
      MINI_STL_THROW_RANGE_ERROR("basic_string");
    return _compare_aux(first_ + Pos1,
                        first_ + Pos1 + min(Num1, size() - Pos1),
                        Ptr, Ptr + Num2);
  }
    
  void swap(basic_string& Str)
  {
    _MY_STL::swap(this->first_, Str.first_);  
    _MY_STL::swap(this->last_, Str.last_);
    _MY_STL::swap(this->end_, Str.end_);
    _MY_STL::swap(this->use_, Str.use_);
  }
    
  basic_string substr(size_type Off = 0, size_type Count = npos) const
  {
    return basic_string(begin()+Off, 
                        begin()+Off + min(Count, size() - Pos));    
  }

  basic_string& operator+=(value_type Ch)
  {
    push_back(Ch);
    return *this;
  }

  basic_string& operator+=(const value_type* Ptr)
  {
    return insert(last_, Ptr);
  }

  basic_string& operator+=(const basic_string& Right)
  {
    return insert(last_, Right);
  }

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

  void _detach()
  {
    if (*use_ == 1)
      return;
    else {
      --*use_;
      iterator old_first = first_;
      iterator old_last = last_;
      _init_block(old_last - old_first + 1);
      insert(begin(), old_first, old_last);
    }
  }

  int _compare_aux(const value_type* First1, const value_type* Last1,
                   const value_type* First2, const value_type* Last2)
  {
    const difference_type n1 = Last1 - First1;
    const difference_type n2 = Last2 - First2;
    const int cmp = traits_type::compare(First1, First2, min(n1, 2));
    return cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0));
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
    if (First != Last) {
      size_type n = Last - First;
      if ((size_type)(end_ - last_) >= n) {
        iterator oldLast = last_;
        copy_backward((iterator)Position, oldLast, oldLast + n);
        copy(First, Last, (iterator)Position);
        last_ += n;
      } else {
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
    } else {
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
template<class InputIterator>
basic_string&
basic_string<CharType,Traits,Alloc>::replace(
    iterator First1,
    iterator Last1,
    InputIterator First2,
    InputIterator Last2,
    typename is_iterator<InputIterator>::ID
    )
{
  size_type insert_num = DISTANCE(First2, Last2);
  size_type replace_num = Last1 - First1;
  if (*use_ == 1) {
    if (replace_num >= insert_num) {
      _MY_STL::copy(First2, Last2, First1);
      erase(First1+insert_num, Last1);
    } else {
      for ( ;First1!=Last1; ++First1,++First2)
        *First1 = *First2;
      insert(Last1, First2, Last2);
    }
  } else {
    --*use_;
    iterator old_first = first_;
    iterator old_last_ = last_;
    _init_block(size()-replace_num+insert_num+1);
    last_ = uninitialized_copy(old_first, First1, first_);
    last_ = uninitialized_copy(First2, Last2, last_);
    last_ = uninitialized_copy(Last1, old_last_, last_);
    _make_terinate();
  }
  return *this;
}


template <class CharType,class Traits,class Alloc>
   void swap(
      basic_string<CharType,Traits,Alloc>& Left,
      basic_string<CharType,Traits,Alloc>& Right
   )
{
  Left.swap(Right);     
}
   
template<class CharType, class Traits, class Allocator>
   basic_string<CharType, Traits, Allocator> operator+(
       const basic_string<CharType, Traits, Allocator>& Left,
       const basic_string<CharType, Traits, Allocator>& Right
       )
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(Left);
  result.append(Right);
  return result;
}

template<class CharType, class Traits, class Allocator>
   basic_string<CharType, Traits, Allocator> operator+(
       const basic_string<CharType, Traits, Allocator>& Left,
       const CharType* Right
       )
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(Left); 
  result.append(Right);
  return result;  
}

template<class CharType, class Traits, class Allocator>
   basic_string<CharType, Traits, Allocator> operator+(
       const basic_string<CharType, Traits, Allocator>& Left,
       const CharType Right
       )
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(Left);  
  result.push_back(Right);
  return result;
}

template<class CharType, class Traits, class Allocator>
   basic_string<CharType, Traits, Allocator> operator+(
       const CharType* Left,
       const basic_string<CharType, Traits, Allocator>& Right
       )
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(Left);
  result.append(Right);
  return result;
}
   
template<class CharType, class Traits, class Allocator>
   basic_string<CharType, Traits, Allocator> operator+(
       const CharType Left,
       const basic_string<CharType, Traits, Allocator>& Right
       )
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(Left);
  result.append(Right);
  return result;
}
#ifdef MINI_STL_RVALUE_REFS
template<class CharType, class Traits, class Allocator>
       basic_string<CharType, Traits, Allocator>&& operator+(
           const basic_string<CharType, Traits, Allocator>& _Left,
           const basic_string<CharType, Traits, Allocator>&& _Right
   );
   template<class CharType, class Traits, class Allocator>
      basic_string<CharType, Traits, Allocator>&& operator+(
         const basic_string<CharType, Traits, Allocator>&& _Left,
         const basic_string<CharType, Traits, Allocator>& _Right
   );
   template<class CharType, class Traits, class Allocator>
      basic_string<CharType, Traits, Allocator>&& operator+(
         const basic_string<CharType, Traits, Allocator>&& _Left,
         const basic_string<CharType, Traits, Allocator>&& _Right
   );
   template<class CharType, class Traits, class Allocator>
      basic_string<CharType, Traits, Allocator>&& operator+(
         const basic_string<CharType, Traits, Allocator>&& _Left,
         const CharType *_Right
   );
   template<class CharType, class Traits, class Allocator>
      basic_string<CharType, Traits, Allocator>&& operator+(
         const basic_string<CharType, Traits, Allocator>&& _Left,
         CharType _Right
   );
   template<class CharType, class Traits, class Allocator>
      basic_string<CharType, Traits, Allocator>&& operator+(
         const CharType *_Left,
         const basic_string<CharType, Traits, Allocator>&& _Right
   );
   template<class CharType, class Traits, class Allocator>
      basic_string<CharType, Traits, Allocator>&& operator+(
         CharType _Left,
         const basic_string<CharType, Traits, Allocator>&& _Right
   );
#endif
template<class CharType, class Traits, class Allocator>
inline bool
operator==(const basic_string<CharType, Traits, Allocator>& lhs,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return lhs.size() == rhs.size() &&
      Traits::compare(lhs.data(), rhs.data(), lhs.size()) == 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator==(const CharType* ptr,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  size_t n = Traits::length(ptr);
  return n == rhs.size() && Traits::compare(ptr, rhs.data(), n) == 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator==(const basic_string<CharType, Traits, Allocator>& lhs,
           const CharType* ptr)
{
  size_t n = Traits::length(ptr);
  return lhs.size() == n && Traits::compare(lhs.data(), ptr, n) == 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator!=(const basic_string<CharType, Traits, Allocator>& lhs,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(lhs == rhs);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator!=(const CharType* ptr,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(ptr == rhs);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator!=(const basic_string<CharType, Traits, Allocator>& lhs,
           const CharType* ptr)
{
  return !(lhs == ptr);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<(const basic_string<CharType, Traits, Allocator>& lhs,
          const basic_string<CharType, Traits, Allocator>& rhs)
{
  return lhs.compare(rhs) < 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<(const CharType* ptr,
          const basic_string<CharType, Traits, Allocator>& rhs)
{
  return rhs.compare(ptr) > 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<(const basic_string<CharType, Traits, Allocator>& lhs,
          const CharType* ptr)
{
  return lhs.compare(ptr) < 0;
}


template<class CharType, class Traits, class Allocator>
inline bool
operator>(const basic_string<CharType, Traits, Allocator>& lhs,
          const basic_string<CharType, Traits, Allocator>& rhs)
{
  return rhs < lhs;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>(const CharType* ptr,
          const basic_string<CharType, Traits, Allocator>& rhs)
{
  return ptr < rhs;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>(const basic_string<CharType, Traits, Allocator>& lhs,
          const CharType* ptr)
{
  return ptr < lhs;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<=(const basic_string<CharType, Traits, Allocator>& lhs,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(rhs < lhs);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<=(const CharType* ptr,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(rhs < ptr);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<=(const basic_string<CharType, Traits, Allocator>& lhs,
           const CharType* ptr)
{
  return !(ptr < lhs);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>=(const basic_string<CharType, Traits, Allocator>& lhs,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(rhs > lhs);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>=(const CharType* ptr,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(rhs > ptr);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>=(const basic_string<CharType, Traits, Allocator>& lhs,
           const CharType* ptr)
{
  return !(ptr > lhs);
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
