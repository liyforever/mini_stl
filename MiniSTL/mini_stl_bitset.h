#ifndef MINI_STL_BITSET_H
#define MINI_STL_BITSET_H
#include <limits.h>
#include "mini_stl_string.h"
#include "mini_stl_config.h"
#include <stddef.h>
#include <stdexcept>
#include <bitset>
#include <iostream>
using std::cout;
using std::endl;
#define private public
#define protected public
MINI_STL_BEGIN
#define _WORD_BIT_NUM  ((CHAR_BIT)*sizeof(size_t))

#define _GET_ARRAY_SIZE(n) ((n) < 1 ? 1 : (((n) + _WORD_BIT_NUM - 1) / _WORD_BIT_NUM))


static unsigned char _COUNT_TABLE[256] =
{
  0, /*   0 */ 1, /*   1 */ 1, /*   2 */ 2, /*   3 */ 1, /*   4 */
  2, /*   5 */ 2, /*   6 */ 3, /*   7 */ 1, /*   8 */ 2, /*   9 */
  2, /*  10 */ 3, /*  11 */ 2, /*  12 */ 3, /*  13 */ 3, /*  14 */
  4, /*  15 */ 1, /*  16 */ 2, /*  17 */ 2, /*  18 */ 3, /*  19 */
  2, /*  20 */ 3, /*  21 */ 3, /*  22 */ 4, /*  23 */ 2, /*  24 */
  3, /*  25 */ 3, /*  26 */ 4, /*  27 */ 3, /*  28 */ 4, /*  29 */
  4, /*  30 */ 5, /*  31 */ 1, /*  32 */ 2, /*  33 */ 2, /*  34 */
  3, /*  35 */ 2, /*  36 */ 3, /*  37 */ 3, /*  38 */ 4, /*  39 */
  2, /*  40 */ 3, /*  41 */ 3, /*  42 */ 4, /*  43 */ 3, /*  44 */
  4, /*  45 */ 4, /*  46 */ 5, /*  47 */ 2, /*  48 */ 3, /*  49 */
  3, /*  50 */ 4, /*  51 */ 3, /*  52 */ 4, /*  53 */ 4, /*  54 */
  5, /*  55 */ 3, /*  56 */ 4, /*  57 */ 4, /*  58 */ 5, /*  59 */
  4, /*  60 */ 5, /*  61 */ 5, /*  62 */ 6, /*  63 */ 1, /*  64 */
  2, /*  65 */ 2, /*  66 */ 3, /*  67 */ 2, /*  68 */ 3, /*  69 */
  3, /*  70 */ 4, /*  71 */ 2, /*  72 */ 3, /*  73 */ 3, /*  74 */
  4, /*  75 */ 3, /*  76 */ 4, /*  77 */ 4, /*  78 */ 5, /*  79 */
  2, /*  80 */ 3, /*  81 */ 3, /*  82 */ 4, /*  83 */ 3, /*  84 */
  4, /*  85 */ 4, /*  86 */ 5, /*  87 */ 3, /*  88 */ 4, /*  89 */
  4, /*  90 */ 5, /*  91 */ 4, /*  92 */ 5, /*  93 */ 5, /*  94 */
  6, /*  95 */ 2, /*  96 */ 3, /*  97 */ 3, /*  98 */ 4, /*  99 */
  3, /* 100 */ 4, /* 101 */ 4, /* 102 */ 5, /* 103 */ 3, /* 104 */
  4, /* 105 */ 4, /* 106 */ 5, /* 107 */ 4, /* 108 */ 5, /* 109 */
  5, /* 110 */ 6, /* 111 */ 3, /* 112 */ 4, /* 113 */ 4, /* 114 */
  5, /* 115 */ 4, /* 116 */ 5, /* 117 */ 5, /* 118 */ 6, /* 119 */
  4, /* 120 */ 5, /* 121 */ 5, /* 122 */ 6, /* 123 */ 5, /* 124 */
  6, /* 125 */ 6, /* 126 */ 7, /* 127 */ 1, /* 128 */ 2, /* 129 */
  2, /* 130 */ 3, /* 131 */ 2, /* 132 */ 3, /* 133 */ 3, /* 134 */
  4, /* 135 */ 2, /* 136 */ 3, /* 137 */ 3, /* 138 */ 4, /* 139 */
  3, /* 140 */ 4, /* 141 */ 4, /* 142 */ 5, /* 143 */ 2, /* 144 */
  3, /* 145 */ 3, /* 146 */ 4, /* 147 */ 3, /* 148 */ 4, /* 149 */
  4, /* 150 */ 5, /* 151 */ 3, /* 152 */ 4, /* 153 */ 4, /* 154 */
  5, /* 155 */ 4, /* 156 */ 5, /* 157 */ 5, /* 158 */ 6, /* 159 */
  2, /* 160 */ 3, /* 161 */ 3, /* 162 */ 4, /* 163 */ 3, /* 164 */
  4, /* 165 */ 4, /* 166 */ 5, /* 167 */ 3, /* 168 */ 4, /* 169 */
  4, /* 170 */ 5, /* 171 */ 4, /* 172 */ 5, /* 173 */ 5, /* 174 */
  6, /* 175 */ 3, /* 176 */ 4, /* 177 */ 4, /* 178 */ 5, /* 179 */
  4, /* 180 */ 5, /* 181 */ 5, /* 182 */ 6, /* 183 */ 4, /* 184 */
  5, /* 185 */ 5, /* 186 */ 6, /* 187 */ 5, /* 188 */ 6, /* 189 */
  6, /* 190 */ 7, /* 191 */ 2, /* 192 */ 3, /* 193 */ 3, /* 194 */
  4, /* 195 */ 3, /* 196 */ 4, /* 197 */ 4, /* 198 */ 5, /* 199 */
  3, /* 200 */ 4, /* 201 */ 4, /* 202 */ 5, /* 203 */ 4, /* 204 */
  5, /* 205 */ 5, /* 206 */ 6, /* 207 */ 3, /* 208 */ 4, /* 209 */
  4, /* 210 */ 5, /* 211 */ 4, /* 212 */ 5, /* 213 */ 5, /* 214 */
  6, /* 215 */ 4, /* 216 */ 5, /* 217 */ 5, /* 218 */ 6, /* 219 */
  5, /* 220 */ 6, /* 221 */ 6, /* 222 */ 7, /* 223 */ 3, /* 224 */
  4, /* 225 */ 4, /* 226 */ 5, /* 227 */ 4, /* 228 */ 5, /* 229 */
  5, /* 230 */ 6, /* 231 */ 4, /* 232 */ 5, /* 233 */ 5, /* 234 */
  6, /* 235 */ 5, /* 236 */ 6, /* 237 */ 6, /* 238 */ 7, /* 239 */
  4, /* 240 */ 5, /* 241 */ 5, /* 242 */ 6, /* 243 */ 5, /* 244 */
  6, /* 245 */ 6, /* 246 */ 7, /* 247 */ 5, /* 248 */ 6, /* 249 */
  6, /* 250 */ 7, /* 251 */ 6, /* 252 */ 7, /* 253 */ 7, /* 254 */
  8  /* 255 */
};
template <size_t Num>
class bitset
{
private:
  enum : size_t{BIT_NUM = Num,
               ARRAY_SIZE_ = _GET_ARRAY_SIZE(Num),
               HIGH_OFF_SET_ = Num % _WORD_BIT_NUM};
  typedef size_t  value_type;
  value_type c_[ARRAY_SIZE_];
public:
  typedef bool element_type;
public:
  bitset()
  {
    cout << "ARRAY_SIZE:" << ARRAY_SIZE_ << endl;
    _do_reset();
  }

  bitset(size_t Val)
  {
    _do_reset();
    c_[0] = Val;
  }

  explicit bitset(const char * _CStr)
  {
    size_type len = strlen(_CStr);
    _do_reset();
    if (BIT_NUM >= len) {
      size_type index_for_bit = 0;
      size_type index_for_CStr = len - 1;
      for ( ; index_for_CStr>=0; --index_for_CStr,++index_for_bit)
        switch (_CStr[index_for_CStr]) {
          case '0':
            break;
          case '1':
            flip(index_for_bit);
            break;
          default:
            MINI_STL_THROW_INVALID_ERROR("bitset");
            break;
          }
    }
  }

  /*template<
    class CharType,
    class Traits,
    class Allocator
  >
    explicit bitset(
      const basic_string< CharType, Traits, Allocator >& _Str,
      typename basic_string<
        CharType, Traits, Allocator >::size_type _Pos = 0
    );
  template<
    class CharType,
    class Traits,
    class Allocator
  >
   explicit bitset(
    const basic_string< CharType, Traits, Allocator >& _Str,
    typename basic_string<
      CharType, Traits, Allocator >::size_type _Pos,
    typename basic_string<
      CharType, Traits, Allocator >::size_type _Count,
    CharType _Zero = CharType (’0’),
    CharType _One  = CharType (’1’)
  );*/
public:
  bool all() const
  {
    for (int i=0; i<ARRAY_SIZE_; ++i)
      if (c_[i]!=~static_cast<value_type>(0))
        return false;
    return true;
  }

  bool any() const
  {
    for (int i=0; i<ARRAY_SIZE_; ++i)
      if (c_[i])
        return true;
    return false;
  }

  bool none() const
  {
    return !any();
  }

  size_t count() const
  {
    size_t result = 0;
    const unsigned char* begin = (const unsigned char*)(c_);
    const unsigned char* end = begin + sizeof(value_type);
    for (; begin != end; ++begin)
      result += _COUNT_TABLE[*begin];
    return result;
  }

  bitset& reset()
  {
    _do_reset();
    return *this;
  }

  bitset& reset(size_t Pos)
  {
    _get_word(pos) & ~_get_mask(Pos);
  }

  bitset& flip()
  {
    for (int i=0; i!=ARRAY_SIZE_; ++i)
      c_[i] = ~c_[i];
    _adjust();
    return *this;
  }

  bitset& flip(size_t pos)
  {
    if(pos >= BIT_NUM)
      MINI_STL_THROW_RANGE_ERROR("bitset");
    cout << "_get_index(pos):" << _get_index(pos) << endl;
    _get_word(pos) ^= _get_mask(pos);
    return *this;
  }

  size_t size() const
  {
    return BIT_NUM;
  }

  bool test(size_t pos) const
  {
    if (pos >= BIT_NUM)
      MINI_STL_THROW_RANGE_ERROR("bitset");
    return static_cast<bool>(_get_word(pos) & _get_mask(pos));
  }

  template<class CharType, class Traits, class Alloc>
  basic_string<CharType, Traits, Alloc> to_string() const
  {
    basic_string<CharType, Traits, Alloc> tmp(BIT_NUM, '0');
    for (int i=0; i!=BIT_NUM; ++i)
      if (test(i))
        tmp[i] = '1';
    return tmp;
  }

  string to_string() const
  {
    return to_string<char, _MY_STL::char_traits<char>,_MY_STL::default_allocator>();
  }

  unsigned long long to_ullong() const
  {
    for (int i=0; i!=ARRAY_SIZE_; ++i)
      if (c_[i])
        MINI_STL_THROW_OVERFLOW_ERROR("bitset");
    return to_ullong();
  }

  unsigned long to_ulong() const
  {
    return to_ullong();
  }

  bitset& operator&=(const bitset& _Right)
  {
    for (int i=0; i!=ARRAY_SIZE_; ++i)
      c_[i] &= _Right.c_[i];
    _adjust();
    return *this;
  }

  bitset& operator^=(const bitset& _Right)
  {
    for (int i=0; i!=ARRAY_SIZE_; ++i)
      c_[i] ^= _Right.c_[i];
    _adjust();
    return *this;
  }

  bitset& operator|=(const bitset& _Right)
  {
    for (int i=0; i!=ARRAY_SIZE_; ++i)
      c_[i] |= _Right.c_[i];
    _adjust();
    return *this;
  }

  bitset& operator<<=(size_t _Pos);

  bitset& operator>>=(size_t _Pos);

  bitset operator<<(size_t _Pos) const
  {
    return bitset<Num>(*this).operator <<=(_Pos);
  }

  bool operator[](size_t _Pos) const
  {
    return this->test(_Pos);
  }

  bitset operator>>(size_t _Pos) const
  {
    return bitset<Num>(*this).operator >>=(_Pos);
  }

  bitset operator~()
  {
    return bitset<BIT_NUM>(*this).flip();
  }

  bool operator ==(const bitset& _Right) const
  {
    return _MY_STL::equal(c_, &c_[ARRAY_SIZE_], _Right.c_);
  }

  bool operator !=(const bitset& _Right)
  {
    return !(*this == _Right);
  }

private:
  void _do_reset()
  {
    memset(c_,0,ARRAY_SIZE_*sizeof(size_t));
  }

  value_type& _get_high()
  {
    return c_[ARRAY_SIZE_-1];
  }

  void _adjust()
  {
    _get_high() &= (~((~static_cast<value_type>(0))<<HIGH_OFF_SET_));
  }

  size_t _get_offset(size_t pos) const
  {
    return pos % _WORD_BIT_NUM;
  }

  size_t _get_index(size_t pos) const
  {
    return _GET_ARRAY_SIZE(pos) - 1;
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

  void _copy_for_CStr(const char* )
  {

  }
};

template <size_t Num>
bitset<Num>& bitset<Num>::operator<<=(size_t _Pos)
{
  if (_Pos >= BIT_NUM) {
    _do_reset();
    return *this;
  }
  const size_t offset = _get_offset(_Pos);
  const size_t move_limit = _Pos / _WORD_BIT_NUM;
  if (offset == 0) {
    for (int n=ARRAY_SIZE_-1; n>=move_limit; --n)
      c_[n] = c_[n - move_limit];
  } else {
    const size_t sub_offset = _WORD_BIT_NUM - offset;
    for (int n=ARRAY_SIZE_-1; n>move_limit; --n)
      c_[n] = (c_[n-move_limit] << offset) |
              (c_[n-move_limit-1] >> sub_offset);
    c_[move_limit] = c_[0] << offset;
  }
  _adjust();
  return *this;
}

template <size_t Num>
bitset<Num>& bitset<Num>::operator<<=(size_t _Pos)
{
  if (_Pos >= BIT_NUM) {
    _do_reset();
    return *this;
  }
  const size_t offset = _get_offset(_Pos);
  const size_t move_limit = _Pos / _WORD_BIT_NUM;
  if (offset == 0) {
    for (int n = 0; n <= move_limit; ++n)
      c_[n] = c_[n + move_limit];
  } else {
    const size_t sub_offset = _WORD_BIT_NUM - offset;
    for (int n = 0; n<move_limit; ++n)
      c_[n] = (c_[n+move_limit] >> offset) |
              (c_[n+move_limit+1] << sub_offset);
    c_[move_limit] = c_[0] >> offset;
  }
  _adjust();
  return *this;
}

template <size_t Num>
bitset<Num> operator&(const bitset<Num>& _Left,
                      const bitset<Num>& _Right)
{
  bitset<Num> result(_Left);
  return result &= _Right;
}

template <size_t Num>
bitset<Num> operator|(const bitset<Num>& _Left,
                      const bitset<Num>& _Right)
{
  bitset<Num> result(_Left);
  return result |= _Right;
}

template <size_t Num>
bitset<Num> operator^(const bitset<Num>& _Left,
                      const bitset<Num>& _Right)
{
  bitset<Num> result(_Left);
  return result ^= _Right;
}
MINI_STL_END
#endif // MINI_STL_BITSET_H
