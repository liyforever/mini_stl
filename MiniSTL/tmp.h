#ifndef TMP_H
#define TMP_H
#include <stddef.h>
template <size_t C_ARRAY_SIZE>
struct bitset_base
{
  enum : size_t {ARRAR_SIZE_ = C_ARRAY_SIZE};
  typedef size_t value_type;
  value_type c_[ARRAR_SIZE_];

  void _do_reset()
  {
    memset(c_,0,ARRAY_SIZE_*sizeof(size_t));
  }

  value_type& _get_high()
  {
    return c_[ARRAY_SIZE_-1];
  }

  value_type _get_high() const
  {
    return c_[ARRAY_SIZE_-1];
  }

  size_t _get_index(size_t pos) const
  {
    return _GET_ARRAY_SIZE(pos) - 1;
  }

  size_t _get_offset(size_t pos) const
  {
    return pos % _WORD_BIT_NUM;
  }

  size_t _get_mask(size_t pos) const
  {
    return (static_cast<value_type>(1)) << _get_offset(pos);
  }

  size_t& _get_word(size_t pos)
  {
    return c_[_get_index(pos)];
  }

  size_t _get_word(size_t pos) const
  {
    return c_[_get_index(pos)];
  }
};

#endif // TMP_H
