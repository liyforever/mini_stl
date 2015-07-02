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
      const basic_string& right,
      size_type roff,
      size_type count = npos
  );
  basic_string(
      const basic_string& right,
      size_type roff,
      size_type count,
      const allocator_type& Al
  );
  basic_string(
      const value_type *_Ptr,
      size_type count
  );
  basic_string(
      const value_type *ptr,
      size_type count,
      const allocator_type& Al
  );
  basic_string(
      const value_type *ptr
  );
  basic_string(
      const value_type *ptr,
      const allocator_type& Al
  );
  basic_string(
      size_type count,
      value_type ch
  );
  basic_string(
      size_type count,
      value_type ch,
      const allocator_type& Al
  );
  template <class InputIterator>
      basic_string(
          InputIterator first,
          InputIterator last
      );
  template <class InputIterator>
      basic_string(
          InputIterator first,
          InputIterator last,
          const allocator_type& Al
      );
  basic_string(
     const_pointer first,
     const_pointer last
  );
  basic_string(
     const_iterator first,
     const_iterator last
  );
};

MINI_STL_END
#endif // MINI_STL_STRING_H
