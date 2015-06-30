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
};

MINI_STL_END
#endif // MINI_STL_STRING_H
