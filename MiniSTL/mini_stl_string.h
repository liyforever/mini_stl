#ifndef MINI_STL_STRING_H
#define MINI_STL_STRING_H
#include "memory.h"
#include <string>
MINI_STL_BEGIN

std::string v;
template <class CharType, class Alloc = default_allocator>
class basic_string
{
public:  
  typedef CharType          value_type;
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
private:
  typedef simpleAlloc<value_type, Alloc> data_allocator_;
  iterator first_;
  iterator last_;
  iterator end_;
  size_t *use_;
public:
  basic_string();

  basic_string(const basic_string& right);
#ifdef MINI_STL_RVALUE_REFS
  basic_string(basic_string&& right);
#endif
  basic_string(
      const basic_string& _Right,
      size_type _Roff,
      size_type _Count = npos
  );
  basic_string(
      const basic_string& right,
      size_type _Roff,
      size_type _Count,
      const allocator_type& _Al
  );
  basic_string(
      const value_type *_Ptr,
      size_type _Count
  );
  basic_string(
      const value_type *_Ptr,
      size_type _Count,
      const allocator_type& _Al
  );
  basic_string(
      const value_type *_Ptr
  );
  basic_string(
      const value_type *_Ptr,
      const allocator_type& _Al
  );
  basic_string(
      size_type _Count,
      value_type _Ch
  );
  basic_string(
      size_type _Count,
      value_type _Ch,
      const allocator_type& _Al
  );
  template <class InputIterator>
      basic_string(
          InputIterator _First,
          InputIterator _Last
      );
  template <class InputIterator>
      basic_string(
          InputIterator _First,
          InputIterator _Last,
          const allocator_type& _Al
      );
  basic_string(
     const_pointer _First,
     const_pointer _Last
  );
  basic_string(
     const_iterator _First,
     const_iterator _Last
  );

};

MINI_STL_END
#endif // MINI_STL_STRING_H
