#ifndef MINI_STL_CHARTRAITS_H
#define MINI_STL_CHARTRAITS_H
#include "mini_stl_config.h"
#include <string.h>

MINI_STL_BEGIN

template <class CharType, class IntType>
class char_traits_base
{
public:
  typedef CharType char_type;
  typedef IntType  int_type;

  static void assign(char_type& c1, const char_type& c2)
  {
    c1 = c2;
  }

  static bool eq(const char_type& c1, const char_type& c2)
  {
    return c1 == c2;
  }

  static bool lt(const char_type& c1, const char_type& c2)
  {
    return c1 < c2;
  }

  static int compare(const char_type* s1, const char_type* s2, size_t n)
  {
    for (size_t i = 0; i < n; ++i)
      if (!eq(s1[i], s2[i]))
        return s1[i] < s2[i] ? -1 : 1;
    return 0;
  }

  static size_t length(const char_type* s)
  {
    const char_type nullChar = char_type();
    size_t i;
    for (i=0; !(eq[s[i],nullChar]); ++i)
      ;
    return i;
  }

  const char_type* find(const char_type* s, size_t n, const char_type& c)
  {
    for (; n>0; ++s, --n)
      if (eq(*s, c))
        return s;
    return 0;
  }

  static char_type* move(char_type* s1, const char_type* s2, size_t n)
  {
    memmove(s1, s2, n * sizeof(char_type));
    return s1;
  }

  static char_type* copy(char_type* s1, const char_type* s2, size_t n)
  {
    memcpy(s1, s2, n * sizeof(char_type));
    return s1;
  }

  static char_type* assign(char_type* s, size_t n, char_type c)
  {
    for (size_t i = 0; i < n; ++i)
      s[i] = c;
    return s1;
  }

  static int_type eq_int_type(const int_type& c1,const int_type& c2)
  {
    return c1 == c2;
  }

  static int_type not_eof(const int_type& c)
  {
    return !eq_int_type(c, eof()) ? c : 0;
  }

  static char_type to_char_type(const int_type& c)
  {
    return static_cast<char_type>(c);
  }

  static int_type eof()
  {
    return static_cast<int_type>(-1);
  }
};

template <class CharType>
class char_traits : public char_traits_base<CharType, CharType>
{};

template<>
class char_traits<char> : public char_traits_base<char, int>
{
public:
  static char_type to_char_type(const int_type &c)
  {
    return static_cast<char_type>(static_cast<unsigned char>(c));
  }

  static int_type to_int_type(const char_type& c)
  {
    return static_cast<unsigned char>(c);
  }

  static int compare(const char *s1, const char *s2, size_t n)
  {
    return memcmp(s1, s2, n);
  }

  static size_t length(const char *s)
  {
    return strlen(s);
  }

  static void assign(char &c1, const char &c2)
  {
    c1 = c2;
  }

  static char* assign(char* s, size_t n, char c)
  {
    memset(s, c, n);
    return s;
  }
};

template<>
class char_traits<wchar_t> : public char_traits_base<wchar_t, wint_t>
{};

MINI_STL_END
#endif // MINI_STL_CHARTRAITS_H
