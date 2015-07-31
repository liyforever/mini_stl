#ifndef MINI_STL_STRING_H
#define MINI_STL_STRING_H
#include "mini_stl_memory.h"
#include "mini_stl_algorithm.h"
#include "mini_stl_chartraits.h"
#include <iosfwd>
#include <istream>

MINI_STL_BEGIN

template <class CharType,
          class Traits = _MY_STL::char_traits<CharType>,
          class Alloc = _MY_STL::default_allocator>
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
  enum { E_DEFAULT_BLOCK_ = 8};
  typedef simpleAlloc<value_type, Alloc> data_allocator_;
  typedef simpleAlloc<size_type, Alloc> use_allocator_;
  iterator Myfirst_;
  iterator Mylast_;
  iterator Myend_;
  size_type *Myuse_;
public:
  size_type get_use() const
  {
    return *Myuse_;
  }

  basic_string()
  {
    _init_block(E_DEFAULT_BLOCK_);
    _make_terinate();
  }

  explicit basic_string(const allocator_type& /*AL*/)
  {
    _init_block(E_DEFAULT_BLOCK_);
    _make_terinate();
  }

  basic_string(const basic_string& _Right)
  {
    Myfirst_ = _Right.Myfirst_;
    Mylast_ = _Right.Mylast_;
    Myend_ = _Right.Myend_;
    Myuse_ = _Right.Myuse_;
    ++*Myuse_;
  }

#ifdef MINI_STL_RVALUE_REFS
  basic_string(basic_string&& _Right)
  {
    Myfirst_ = _Right.Myfirst_;
    Mylast_ = _Right.Mylast_;
    Myend_ = _Right.Myend_;
    Myuse_ = _Right.Myuse_;
    ++*Myuse_;
  }
#endif
  basic_string(const basic_string& _Right,
               size_type _Pos,
               size_type _Count = npos,
               const allocator_type& /*Al*/
               = allocator_type())
  {
    MINI_STL_DEBUG_CHECK_POS(_Right.size(), _Pos, "basic_string basic_string");
    _init_range(_Right.begin() + _Pos,
                _Right.begin() + _Pos + min(_Count, _Right.size() - _Pos));
  }

  basic_string(const_pointer _Ptr,
               size_type _Count,
               const allocator_type& /*Al*/
               = allocator_type())
  {
    _init_range(_Ptr, _Ptr + _Count);
  }

  basic_string(const_pointer _Ptr,
               const allocator_type& /*Al*/
               = allocator_type())
  {
    MINI_STL_DEBUG_POINTER(_Ptr,"basic_string basic_string");
    _init_range(_Ptr, _Ptr + traits_type::length(_Ptr));
  }

  basic_string(size_type _Count,
               value_type _Ch,
               const allocator_type& /*Al*/
               = allocator_type())
  {
    _init_block(_Count + 1);
    traits_type::assign(Myfirst_, _Count, _Ch);
    Mylast_ = Myfirst_ + _Count;
    _make_terinate();
  }

  template <class InputIterator>
  basic_string(InputIterator _First,
               InputIterator _Last,
               const allocator_type& /*Al*/
               = allocator_type(),
               typename is_iterator<InputIterator>::ID
               = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_string basic_string");
    size_type n = DISTANCE(_First, _Last);
    _init_block(n + 1);
    Mylast_ = _MY_STL::uninitialized_copy(_First, _Last, Myfirst_);
    _make_terinate();
  }

  basic_string(const_pointer _First, const_pointer _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_string basic_string");
    _init_range(_First, _Last);
  }

  basic_string& operator=(value_type _Ch)
  {
    clear();
    insert(begin(), 1, _Ch);
    return *this;
  }

  basic_string& operator=(const_pointer _Ptr)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string operator=");
    clear();
    insert(begin, _Ptr, _Ptr+traits_type::length(_Ptr));
    return *this;
  }

  basic_string& operator=(const basic_string& _Right)
  {
    if (this != &_Right) {
      _destroy_block();
      this->Myfirst_ = _Right.Myfirst_;
      this->Mylast_ = _Right.Mylast_;
      this->Myend_ = _Right.Myend_;
      this->Myuse_ = _Right.Myuse_;
      ++*Myuse_;
    }
    return *this;
  }

#ifdef MINI_STL_RVALUE_REFS
  basic_string& operator=(basic_string&& _Right)
  {
    _destroy_block();
    this->Myfirst_ = _Right.Myfirst_;
    this->Mylast_ = _Right.Mylast_;
    this->Myend_ = _Right.Myend_;
    this->Myuse_ = _Right.Myuse_;
    ++*Myuse_;
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
    return Myfirst_;
  }

  const_iterator begin() const
  {
    return Myfirst_;
  }

  iterator end()
  {
    _detach();
    return Mylast_;
  }

  const_iterator end() const
  {
    return Mylast_;
  }

  reverse_iterator rbegin()
  {
    _detach();
    return reverse_iterator(Mylast_);
  }

  const_reverse_iterator rbegin() const
  {
    return const_reverse_iterator(Mylast_);
  }

  reverse_iterator rend()
  {
    _detach();
    return reverse_iterator(Myfirst_);
  }

  const_reverse_iterator rend() const
  {
    return const_reverse_iterator(Myfirst_);
  }

  const_iterator cbegin() const
  {
    return Myfirst_;
  }

  const_iterator cend() const
  {
    return Mylast_;
  }

  const_reverse_iterator crbegin() const
  {
    return rbegin();
  }

  const_reverse_iterator crend() const
  {
    return rend();
  }

  reference at(size_type _Off)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string at");
    _detach();
    return Myfirst_[_Off];
  }

  const_reference at(size_type _Off) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string at");
    return Myfirst_[_Off];
  }

  reference operator[](size_type _Off)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string operator[]");
    _detach();
    return Myfirst_[_Off];
  }

  const_reference operator[](size_type _Off) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string operator[]");
    return Myfirst_[_Off];
  }

  size_type _Copy_s(value_type *_Dest, size_type _Dest_size,
                    size_type _Count, size_type _Off = 0) const
  {
    const_iterator iter;
    for (iter= begin() + _Off;
         iter != end() && _Dest_size > 0 && _Count > 0;
         --_Dest_size, ++iter, ++_Dest, --_Count)
      *_Dest = *iter;
    return iter - begin() - _Off;
  }

  reference back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "basic_string back");
    _detach();
    return *(Mylast_ - 1);
  }

  const_reference back() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "basic_string back");
    return *(Mylast_ - 1);
  }

  size_type size() const
  {
    return Mylast_ - Myfirst_;
  }

  bool empty() const
  {
    return Myfirst_ == Mylast_;
  }

  basic_string& insert(size_type _Pos, const value_type* _Ptr)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string insert");
    insert(Myfirst_ + _Pos, _Ptr, _Ptr + traits_type::length(_Ptr));
    return *this;
  }

  basic_string& insert(size_type _Off,
                       const value_type* _Ptr,
                       size_type _Count)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string insert");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string insert");
    insert(Myfirst_ + _Off, _Ptr, _Ptr + _Count);
    return *this;
  }

  basic_string& insert(size_type _Off,const basic_string& _Str)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string insert");
    insert(Myfirst_ + _Off, _Str.begin(), _Str.end());
    return *this;
  }

  basic_string& insert(size_type _Off1,
                       const basic_string& _Str,
                       size_type _Off2,
                       size_type _Count)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off1, "basic_string insert");
    MINI_STL_DEBUG_CHECK_POS(_Str.size()+1, _Off2+_Count, "basic_string insert");
    insert(Myfirst_ + _Off1, _Str.begin() + _Off2,
           _Str.begin() + _Off2 + _Count);
    return *this;
  }

  basic_string& insert(size_type _Off, size_type _Count, value_type _Ch);

  iterator insert(iterator _Position)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(Myfirst_, _Position, "basic_string insert");
    size_type off = _Position - Myfirst_;
    insert(off, 1, value_type());
    return Myfirst_ + _off;
  }

  iterator insert(iterator _Position, value_type _Ch)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(Myfirst_, _Position, "basic_string insert");
    size_type off = _Position - Myfirst_;
    insert(off, 1, _Ch);
    return Myfirst_ + _off;
  }

  void insert(iterator _Position,size_type _Count, value_type _Ch)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(Myfirst_, _Position, "basic_string insert");
    size_type off = _Position - Myfirst_;
    insert(off, _Count, _Ch);
  }

  template<class InputIterator>
  void insert(iterator _Position,
              InputIterator _First,
              InputIterator _Last,
              typename is_iterator<InputIterator>::ID
              = Identity());

  basic_string& assign(const value_type* _Ptr)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string assign");
    size_type n = traits_type::length(_Ptr);
    return assign(_Ptr, _Ptr + n);
  }

  basic_string& assign(const value_type* _Ptr, size_type _Count)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string assign");
    return assign(_Ptr, _Ptr + _Count);
  }

  basic_string& assign(const basic_string& _Str, size_type _Off, size_type _Count)
  {
    MINI_STL_DEBUG_CHECK_POS(_Str.size() + 1, _Off, "basic_string assign");
    MINI_STL_DEBUG_CHECK_POS(_Str.size() + 1, _Off + _Count, "basic_string assign");
    return assign(_Str.begin()+_Off, _Str.begin()+_Off+_Count);
  }

  basic_string& assign(const basic_string& _Str)
  {
    return assign(_Str.begin(), _Str.end());
  }

  basic_string& assign(size_type _Count, value_type _Ch)
  {
    clear();
    insert(begin(), _Count, _Ch);
    return *this;
  }

  template<class InputIterator>
  basic_string& assign(InputIterator _First,
                       InputIterator _Last,
                       typename is_iterator<InputIterator>::ID
                       = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_sting assing");
    clear();
    insert(begin(), _First, _Last);
    return *this;
  }

  template<class InputIterator>
  basic_string& append(InputIterator _First,
                       InputIterator _Last,
                       typename is_iterator<InputIterator>::ID
                       = Identity())
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_string append");
    insert(end(), _First, _Last);
    return *this;
  }

  basic_string& append(const value_type* _Ptr)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string append");
    insert(end(), _Ptr, _Ptr+traits_type::length(_Ptr));
    return *this;
  }

  basic_string& append(const value_type* _Ptr, size_type _Count)
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string append");
    MINI_STL_DEBUG_CHECK_POS(traits_type::length(_Ptr) + 1, _Count, "basic_string append");
    insert(end(), _Ptr, _Ptr+_Count);
    return *this;
  }

  basic_string& append(const basic_string& _Str,
                       size_type _Off,
                       size_type _Count)
  {
    MINI_STL_DEBUG_CHECK_POS(_Str.size() + 1, _Off, "basic_string append");
    MINI_STL_DEBUG_CHECK_POS(_Str.size() + 1, _Off + _Count, "basic_string append");
    insert(end(), _Str.begin()+_Off, _Str.begin()+_Off+_Count);
    return *this;
  }

  basic_string& append(const basic_string& _Str)
  {
    insert(end(), _Str.begin(), _Str.end());
    return *this;
  }

  basic_string& append(size_type _Count, value_type _Ch)
  {
    insert(end(), _Count, _Ch);
    return *this;
  }

  basic_string& erase(size_type _Pos = 0, size_type _Count = npos)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos, "basic_string erase");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos+_Count, "basic_string erase");
    erase(Myfirst_ + _Pos, Myfirst_ + _Pos + min(_Count, size() - _Pos));
    return *this;
  }

  iterator erase(iterator _Position)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), DISTANCE(begin(), _Position), "basic_string erase");
    return erase(_Position, _Position + 1);
  }

  iterator erase(iterator _First, iterator _Last)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_stringerase");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, DISTANCE(begin(), _First), "basic_string erase");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, DISTANCE(begin(), _Last), "basic_string erase");
    if (*Myuse_ == 1) {
      if (_First != _Last) {
        traits_type::move(_First, _Last, (Mylast_ - _Last) + 1);//include null
        const iterator new_last = Mylast_ - (_Last - _First);
        destroy(new_last + 1, _Last + 1);
        Mylast_ = new_last;
      }
      return _First;
    } else {
      --*Myuse_;
      const_iterator old_first = Myfirst_;
      const_iterator old_last = Mylast_;
      size_type n1 = _First - Myfirst_;
      size_type n2 = Mylast_ - _Last;
      _init_block(n1 + n2 + 1);
      Mylast_ = _MY_STL::uninitialized_copy(old_first, _First, Myfirst_);
      Mylast_ = _MY_STL::uninitialized_copy(_Last, old_last, Mylast_);
      _make_terinate();
      return Myfirst_ + n1;
    }
  }

  void pop_back()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "basic_string pop_back");
    erase(--end());
  }

  void push_back(value_type Ch)
  {
    insert(end(), 1, Ch);
  }

  size_type find(const basic_string& _Str, size_type _Off = 0) const
  {
    MINI_STL_DEBUG_CHECK_POS(_Str.size() + 1, _Off, "basic_string find");
    return find(_Str.begin(), _Off, _Str.size());
  }

  size_type find(const value_type* _Ptr, size_type _Off = 0) const
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string find");
    return find(_Ptr, _Off, traits_type::length(_Ptr));
  }
  size_type find(value_type _Ch, size_type _Off = 0) const
  {
    if (_Off >= size())
      return npos;
    const iterator result = find(begin() + _Off, end(),
                                  _Ch);
    return result != end() ? result - begin()
                           : npos;
  }

  size_type find(const value_type* _Ptr,
                 size_type _Off,
                 size_type _Count) const
  {
    if (_Off + _Count > size());
      return npos;
    const const_iterator result = _MY_STL::search(begin() + _Off, end(),
                                                  _Ptr, _Ptr + _Count);
    return result != end() ? result - begin()
                           : npos;
  }

  size_type find_first_not_of(const basic_string& _Str,
                              size_type _Off = 0) const
  {
    return find_first_not_of(_Str.begin(), _Off, _Str.size());
  }

  size_type find_first_not_of(const value_type* _Ptr,
                              size_type _Off = 0) const
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string find_first_not_of");
    return find_first_not_of(_Ptr, _Off, traits_type::length(_Ptr));
  }

  size_type find_first_not_of(const value_type* _Ptr,
                              size_type _Off,
                              size_type _Count) const
  {

  }

  size_type find_first_not_of(value_type _Ch, size_type _Off = 0) const
  {

  }


  void clear()
  {
    if (*Myuse_ == 1) {
      if (!empty()) {
        traits_type::assign(*Myfirst_, value_type());
        destroy(Myfirst_+1, Mylast_+1);
        Mylast_ = Myfirst_;
      }
    } else {
      --*Myuse_;
      _init_block(8);
      _make_terinate();
    }
  }

  size_type capacity() const
  {
    return Myend_ - Myfirst_ - 1;
  }

  const value_type *c_str() const
  {
    return Myfirst_;
  }

  const value_type *data() const
  {
    return Myfirst_;
  }

  size_type copy(value_type* _Ptr, size_type _Count, size_type _Off = 0) const
  {\
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string copy");
    size_type len = traits_type::length(_Ptr);
    MINI_STL_DEBUG_CHECK_POS(len, _Off, "basic_string copy");
    MINI_STL_DEBUG_CHECK_POS(len, _Off + _Count, "basic_string copy");
    traits_type::copy(_Ptr, Myfirst_+_Off, _Count);
    return _Count;
  }

  basic_string& replace(size_type _Pos1,
                        size_type _Num1,
                        const value_type* _Ptr)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1+1+_Num1, "basic_string replace");
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string replace");
    return replace(begin()+_Pos1, begin()+_Pos1+_Num1,
                   _Ptr, _Ptr+traits_type::length(_Ptr));
  }
    
  basic_string& replace(size_type _Pos1,
                        size_type _Num1,
                        const basic_string& _Str)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1+1+_Num1, "basic_string replace");
    return replace(begin()+_Pos1,
                   begin()+_Pos1+_Num1,
                   _Str.begin(),
                   _Str.end());
  }
    
  basic_string& replace(size_type _Pos1,
                        size_type _Num1,
                        const value_type* _Ptr,
                        size_type _Num2)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1+1+_Num1, "basic_string replace");
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(traits_type::length(_Ptr) + 1, _Num2, "basic_string replace");
    return replace(begin()+_Pos1,
                   begin()+_Pos1+_Num1,
                   _Ptr, _Ptr+_Num2);
  }
    
  basic_string& replace(size_type _Pos1,
                        size_type _Num1,
                        const basic_string& _Str,
                        size_type _Pos2,
                        size_type _Num2)
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Pos1+1+_Num1, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(_Str.size()+1, _Pos2, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(_Str.size()+1, _Pos2+_Num2, "basic_string replace");
    return replace(begin()+_Pos1,
                   begin()+_Pos1+_Num1,
                   _Str.begin()+_Pos2,
                   _Str.begin()+_Pos2+_Num2);
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
    
  basic_string& replace(iterator _First,
                        iterator _Last,
                        const value_type* _Ptr)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_sting replace");
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string replace");
    return replace(_First, _Last, _Ptr, _Ptr+traits_type::length(_Ptr));
  }
    
  basic_string& replace(iterator _First,
                        iterator _Last,
                        const basic_string& _Str)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_sting replace");
    return replace(_First, _Last,
                   _Str.begin(), _Str.end());
  }
    
  basic_string& replace(iterator _First,
                        iterator _Last,
                        const value_type* _Ptr,
                        size_type _Num2)
  {
    MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_sting replace");
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_string replace");
    MINI_STL_DEBUG_CHECK_POS(traits_type::length(_Ptr) + 1,_Num2, "basic_string assign");
    return replace(_First, _Last,
                   _Ptr + _Num2,
                   _Ptr+traits_type::length(_Ptr));
  }
    
  /*basic_string& replace(
       iterator _First0,
       iterator _Last0,
       size_type _Num2,
       value_type _Ch
    );*/
    
  template<class InputIterator>
  basic_string& replace(iterator _First1,
                        iterator _Last1,
                        InputIterator __First2,
                        InputIterator _Last2,
                        typename is_iterator<InputIterator>::ID
                        = Identity());

  reference front()
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "basic_string front");
    _detach();
    return *Myfirst_;
  }

  const_reference front() const
  {
    MINI_STL_DEBUG_CHECK_SIZE(this->size(), "basic_string front");
    return *Myfirst_;
  }

  size_type length() const
  {
    return size();
  }

  int compare(const basic_string& _Str) const
  {
    return _compare_aux(this->begin(), this->end(),
                        _Str.begin(), _Str.end());
  }

  int compare(size_type _Pos1, size_type _Num1, const basic_string& _Str) const
  {
    MINI_STL_DEBUG_CHECK_POS(_Str.size(), _Pos1, "basic_string compare");
    MINI_STL_DEBUG_CHECK_POS(_Str.size(), _Pos1 + _Num1, "basic_string compare");
    return _compare_aux(Myfirst_ + _Pos1,
                        Myfirst_ + _Pos1 + min(_Num1, size() - _Pos1),
                        _Str.begin(), _Str.end());
  }

  int compare(size_type _Pos1,
              size_type _Num1,
              const basic_string& _Str,
              size_type _Pos2,
              size_type _Num2) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos1, "basic_string compare");
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos1 + _Num1, "basic_string compare");
    MINI_STL_DEBUG_CHECK_POS(_Str.size(), _Pos2, "basic_string compare");
    MINI_STL_DEBUG_CHECK_POS(_Str.size(), _Pos2 + _Num2, "basic_string compare");
    return _compare_aux(Myfirst_ + _Pos1,
                        Myfirst_ + _Pos1 + min(_Num1, size() - _Pos1),
                        _Str.Myfirst_ + _Pos2,
                        _Str.Myfirst_ + _Pos2 + min(_Num2, _Str.size()-_Pos2));
  }

  int compare(const value_type* _Ptr) const
  {
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_sting compare");
    return _compare_aux(Myfirst_, Mylast_, _Ptr, _Ptr + traits_type::length(Ptr));
  }

  int compare(size_type _Pos1, size_type _Num1, const value_type* _Ptr) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos1, "basic_string compare");
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos1 + _Num1, "basic_string compare");
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_sting compare");
    return _compare_aux(Myfirst_ + _Pos1,
                        Myfirst_ + _Pos1 + min(_Num1, size() - _Pos1),
                        _Ptr, _Ptr + traits_type::length(_ptr));
  }

  int compare(size_type _Pos1, size_type _Num1, const value_type* _Ptr,
                size_type _Num2) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos1, "basic_string compare");
    MINI_STL_DEBUG_CHECK_POS(this->size(), _Pos1 + _Num1, "basic_string compare");
    MINI_STL_DEBUG_POINTER(_Ptr, "basic_sting compare");
    MINI_STL_DEBUG_CHECK_POS(traits_type::length(_ptr)+1, _Num2, "basic_string compare");
    return _compare_aux(Myfirst_ + _Pos1,
                        Myfirst_ + _Pos1 + min(_Num1, size() - _Pos1),
                        _Ptr, _Ptr + _Num2);
  }
    
  void swap(basic_string& _Str)
  {
    _MY_STL::swap(this->Myfirst_, _Str.Myfirst_);
    _MY_STL::swap(this->Mylast_, _Str.Mylast_);
    _MY_STL::swap(this->Myend_, _Str.Myend_);
    _MY_STL::swap(this->Myuse_, _Str.Myuse_);
  }
    
  basic_string substr(size_type _Off = 0, size_type _Count = npos) const
  {
    MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_string compare");
    return basic_string(begin()+_Off,
                        begin()+_Off + min(_Count, size() - _Pos));
  }

  basic_string& operator+=(value_type _Ch)
  {
    push_back(_Ch);
    return *this;
  }

  basic_string& operator+=(const value_type* _Ptr)
  {
    return insert(Mylast_, _Ptr);
  }

  basic_string& operator+=(const basic_string& _Right)
  {
    insert(end(), _Right.begin(), _Right.end());
    return *this;
  }

protected:
  pointer _allocate(size_t _Count)
  {
    return data_allocator_::allocate(_Count);
  }

  void _deallocate(pointer _Ptr, size_t _Count)
  {
    if (_Ptr)
      data_allocator_::deallocate(_Ptr, _Count);
  }

  void _init_block(size_t _Count)
  {
    Myfirst_ = _allocate(_Count);
    Mylast_ = Myfirst_;
    Myend_ = Myfirst_ + _Count;
    Myuse_ = use_allocator_::allocate();
    construct(Myuse_, size_t(1));
  }

  void _destroy_block()
  {
    if (--*Myuse_ == 0) {
      _deallocate(Myfirst_, Myend_ - Myfirst_);
      use_allocator_::deallocate(Myuse_);
    }
  }

  void _make_terinate()
  //on *Mylast make '\0'
  {
    MINI_STL_TRY {
      _construct_null(Mylast_);
    }
    MINI_STL_UNWIND(_MY_STL::destroy(Myfirst_, Mylast_));
  }

  void _construct_null(pointer _Ptr)
  {
    _MY_STL::construct(_Ptr);
    MINI_STL_TRY {
      *_Ptr = (value_type)(0);
    }
    MINI_STL_UNWIND(_MY_STL::destroy(_Ptr));
  }

  void _init_range(const_pointer _first, const_pointer _last)
  {
    size_type n = _last - _first;
    _init_block(n + 1);
    Mylast_ = _MY_STL::uninitialized_copy(_first, _last, Myfirst_);
    _make_terinate();
  }

  void _detach()
  {
    if (*Myuse_ == 1)
      return;
    else {
      --*Myuse_;
      iterator old_first = Myfirst_;
      iterator old_last = Mylast_;
      _init_range(old_first, old_last);
    }
  }

  int _compare_aux(const value_type* _First1, const value_type* _Last1,
                   const value_type* _First2, const value_type* _Last2) const
  {
    const difference_type n1 = _Last1 - _First1;
    const difference_type n2 = _Last2 - _First2;
    const int cmp = traits_type::compare(_First1, _First2, min(n1, (const difference_type)(2)));
    return cmp != 0 ? cmp : (n1 < n2 ? -1 : (n1 > n2 ? 1 : 0));
  }
};

template <class CharType,class Traits,class Alloc>
const typename basic_string<CharType,Traits,Alloc>::size_type
basic_string<CharType,Traits,Alloc>::npos = (basic_string<CharType,Traits,Alloc>::size_type)(-1);


template <class CharType,class Traits,class Alloc>
template<class InputIterator>
void basic_string<CharType,Traits,Alloc>::
insert(iterator _Position,
       InputIterator _First,
       InputIterator _Last,
       typename is_iterator<InputIterator>::ID)
{
  MINI_STL_DEBUG_CHECK_POS(this->size()+1, DISTANCE(begin(), _Position), "basic_sting insert");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First, _Last, "basic_sting insert");
  if (*Myuse_ == 1) {
    if (_First != _Last) {
      size_type n = DISTANCE(_First, _Last);
      if ((size_type)(Myend_ - Mylast_-1) >= n) {
        iterator oldLast = Mylast_;
        _MY_STL::copy_backward((iterator)_Position, oldLast, oldLast + n);
        _MY_STL::copy(_First, _Last, (iterator)_Position);
        Mylast_ += n;
      } else {
        const size_type oldSize = size();
        const size_type newSize = oldSize + max((size_type)oldSize, (size_type)n);
        iterator newFirst = data_allocator_::allocate(newSize+1);
        iterator newLast = newFirst;
        MINI_STL_TRY {
          newLast = _MY_STL::uninitialized_copy(Myfirst_, (iterator)_Position, newFirst);
          newLast = _MY_STL::uninitialized_copy(_First, _Last, newLast);
          newLast = _MY_STL::uninitialized_copy((iterator)_Position, Mylast_, newLast);
        }
        MINI_STL_UNWIND((_MY_STL::destroy(newFirst,newLast),
                            data_allocator_::deallocate(newFirst, newSize)));
        _MY_STL::destroy(Myfirst_, Mylast_);
        _deallocate(Myfirst_, Myend_ - Myfirst_);
        Myfirst_ = newFirst;
        Mylast_ = newLast;
        Myend_ = newFirst + newSize;
      }
    }
    _make_terinate();
  } else {
    --*Myuse_;
    size_type length = size();
    size_type insert_length = DISTANCE(_First, _Last);
    iterator old_first = Myfirst_;
    iterator old_last = Mylast_;
    _init_block(length + insert_length + 1);
    Mylast_ = _MY_STL::uninitialized_copy(old_first, _Position, Myfirst_);
    Mylast_ = _MY_STL::uninitialized_copy(_First, _Last, Mylast_);
    Mylast_ = _MY_STL::uninitialized_copy(_Position, old_last, Mylast_);
    _make_terinate();
  }
}

template <class CharType,class Traits,class Alloc>
basic_string<CharType,Traits,Alloc>&
basic_string<CharType,Traits,Alloc>::
insert(size_type _Off, size_type _Count, value_type _Ch)
{
  MINI_STL_DEBUG_CHECK_POS(this->size()+1, _Off, "basic_sting insert");
  iterator Position = Myfirst_ + _Off;
  if (*Myuse_ == 1) {
    if ((size_type)(Myend_ - Mylast_ - 1) >= _Count) {//has free space
      iterator oldLast = Mylast_;
      _MY_STL::copy_backward((iterator)Position, oldLast, oldLast + _Count);
      traits_type::assign(Position, _Count, _Ch);
      Mylast_ += _Count;
    } else {
      const size_type oldSize = size();
      const size_type newSize = oldSize + max((size_type)oldSize, (size_type)_Count);
      iterator newFirst = data_allocator_::allocate(newSize+1);
      iterator newLast = newFirst;
      MINI_STL_TRY {
        newLast = _MY_STL::uninitialized_copy(Myfirst_, (iterator)Position, newFirst);
        traits_type::assign(newLast, _Count, _Ch);
        newLast += _Count;
        newLast = _MY_STL::uninitialized_copy(Position, Mylast_, newLast);
      }
      MINI_STL_UNWIND((_MY_STL::destroy(newFirst,newLast),
                            data_allocator_::deallocate(newFirst, newSize)));
      destroy(Myfirst_, Mylast_);
      _deallocate(Myfirst_, Myend_ - Myfirst_);
      Myfirst_ = newFirst;
      Mylast_ = newLast;
      Myend_ = newFirst + newSize;
    }
    _make_terinate();
  } else {//no free space
    --*Myuse_;
    size_type length = size();
    iterator old_first = Myfirst_;
    iterator old_last = Mylast_;
    _init_block(length + _Count + 1);
    Mylast_ = _MY_STL::uninitialized_copy(old_first, Position, Myfirst_);
    traits_type::assign(Mylast_, _Count, _Ch);
    Mylast_ += _Count;
    Mylast_ = _MY_STL::uninitialized_copy(Position, old_last, Mylast_);
    _make_terinate();
  }
  return *this;
}

template <class CharType,class Traits,class Alloc>
template<class InputIterator>
basic_string<CharType,Traits,Alloc>&
basic_string<CharType,Traits,Alloc>::
replace(iterator _First1,
        iterator _Last1,
        InputIterator _First2,
        InputIterator _Last2,
        typename is_iterator<InputIterator>::ID)
{
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First1, _Last1, "basic_sting replace");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(this->begin(), _First1, "basic_sting replace");
  MINI_STL_DEBUG_CHECK_POS(this->size(), DISTANCE(this->begin(), _Las1), "basic_sting replace");
  MINI_STL_DEBUG_RANGE_OF_ITERATOR(_First2, _Last2, "basic_sting replace");

  size_type insert_num = DISTANCE(_First2, _Last2);
  size_type replace_num = _Last1 - _First1;
  if (*Myuse_ == 1) {
    if (replace_num >= insert_num) {
      _MY_STL::copy(_First2, _Last2, _First1);
      erase(_First1+insert_num, _Last1);
    } else {
      for ( ;_First1!=Last1; ++_First1,++_First2)
        *_First1 = *_First2;
      insert(_Last1, _First2, _Last2);
    }
  } else {
    --*Myuse_;
    iterator old_first = Myfirst_;
    iterator old_Mylast_ = Mylast_;
    _init_block(size()-replace_num+insert_num+1);
    Mylast_ = _MY_STL::uninitialized_copy(old_first, _First1, Myfirst_);
    Mylast_ = _MY_STL::uninitialized_copy(_First2, _Last2, Mylast_);
    Mylast_ = _MY_STL::uninitialized_copy(_Last1, old_Mylast_, Mylast_);
    _make_terinate();
  }
  return *this;
}


template <class CharType,class Traits,class Alloc>
inline void
swap(basic_string<CharType,Traits,Alloc>& _Left,
     basic_string<CharType,Traits,Alloc>& _Right)
{
  _Left.swap(_Right);
}
   
template<class CharType, class Traits, class Allocator>
inline basic_string<CharType, Traits, Allocator>
operator+(const basic_string<CharType, Traits, Allocator>& _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(_Left);
  result.append(_Right);
  return result;
}

template<class CharType, class Traits, class Allocator>
inline basic_string<CharType, Traits, Allocator>
operator+(const basic_string<CharType, Traits, Allocator>& _Left,
          const CharType* _Right)
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(_Left);
  result.append(_Right);
  return result;  
}

template<class CharType, class Traits, class Allocator>
inline basic_string<CharType, Traits, Allocator>
operator+(const basic_string<CharType, Traits, Allocator>& _Left,
          const CharType _Right)
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(_Left);
  result.push_back(_Right);
  return result;
}

template<class CharType, class Traits, class Allocator>
inline basic_string<CharType, Traits, Allocator>
operator+(const CharType* _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(_Left);
  result.append(_Right);
  return result;
}
   
template<class CharType, class Traits, class Allocator>
inline basic_string<CharType, Traits, Allocator>
operator+(const CharType _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  typedef basic_string<CharType, Traits, Allocator> Str;
  Str result(_Left);
  result.append(_Right);
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
operator==(const basic_string<CharType, Traits, Allocator>& _Left,
           const basic_string<CharType, Traits, Allocator>& _Right)
{
  return _Left.size() == _Right.size() &&
      Traits::compare(_Left.data(), _Right.data(), _Left.size()) == 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator==(const CharType* _Left,
           const basic_string<CharType, Traits, Allocator>& _Right)
{
  size_t n = Traits::length(_Left);
  return n == _Right.size() && Traits::compare(_Left, _Right.data(), n) == 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator==(const basic_string<CharType, Traits, Allocator>& _Left,
           const CharType* _Right)
{
  size_t n = Traits::length(_Right);
  return _Right.size() == n && Traits::compare(_Left.data(), _Right, n) == 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator!=(const basic_string<CharType, Traits, Allocator>& _Left,
           const basic_string<CharType, Traits, Allocator>& _Right)
{
  return !(_Left == _Right);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator!=(const CharType* _Left,
           const basic_string<CharType, Traits, Allocator>& _Right)
{
  return !(_Left == _Right);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator!=(const basic_string<CharType, Traits, Allocator>& _Left,
           const CharType* _Right)
{
  return !(_Left == _Right);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<(const basic_string<CharType, Traits, Allocator>& _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  return _Left.compare(_Right) < 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<(const CharType* _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  return _Right.compare(_Left) > 0;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<(const basic_string<CharType, Traits, Allocator>& _Left,
          const CharType* _Right)
{
  return _Left.compare(_Right) < 0;
}


template<class CharType, class Traits, class Allocator>
inline bool
operator>(const basic_string<CharType, Traits, Allocator>& _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  return _Right < _Left;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>(const CharType* _Left,
          const basic_string<CharType, Traits, Allocator>& _Right)
{
  return _Left < _Right;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>(const basic_string<CharType, Traits, Allocator>& _Left,
          const CharType* _Right)
{
  return _Right < _Left;
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<=(const basic_string<CharType, Traits, Allocator>& _Left,
           const basic_string<CharType, Traits, Allocator>& _Right)
{
  return !(_Right < _Left);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<=(const CharType* _Left,
           const basic_string<CharType, Traits, Allocator>& rhs)
{
  return !(_Right < _Left);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator<=(const basic_string<CharType, Traits, Allocator>& _Left,
           const CharType* _Right)
{
  return !(_Right < _Left);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>=(const basic_string<CharType, Traits, Allocator>& _Left,
           const basic_string<CharType, Traits, Allocator>& _Right)
{
  return !(_Right > _Left);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>=(const CharType* _Left,
           const basic_string<CharType, Traits, Allocator>& _Right )
{
  return !(_Right  > _Left);
}

template<class CharType, class Traits, class Allocator>
inline bool
operator>=(const basic_string<CharType, Traits, Allocator>& _Left,
           const CharType* _Right)
{
  return !(_Right > _Left);
}

template <class CharType,class Traits,class Alloc>
std::ostream& operator <<(std::ostream& _Os, const basic_string<CharType,Traits,Alloc>& _Str)
{
  _Os << _Str.c_str();
  return _Os;
}

typedef basic_string<char, char_traits<char>, default_allocator> string;
typedef basic_string<wchar_t, char_traits<wchar_t>, default_allocator> wstring;
typedef basic_string<char16_t, char_traits<char16_t>, default_allocator> u16string;
typedef basic_string<char32_t, char_traits<char32_t>, default_allocator> u32string;

MINI_STL_END
#endif // MINI_STL_STRING_H
