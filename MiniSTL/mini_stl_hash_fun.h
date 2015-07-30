#ifndef MINI_STL_HASH_FUN_H
#define MINI_STL_HASH_FUN_H
#include "mini_stl_config.h"
#include "mini_stl_memory.h"
MINI_STL_BEGIN

template <class Key>
struct hash {};

inline size_t _hash_to_string(const char* str)
{
  size_t result = 0;
  for (; *str; ++str)
    result = 10*result + *str;
  return result;
}

template <>
struct hash<char*>
{
  size_t operator()(const char* str) const
  {
    return _hash_to_string(str);
  }
};

template <>
struct hash<const char*>
{
  size_t operator()(const char* str)const
  {
    return _hash_to_string(str);
  }
};

template <>
struct hash<char>
{
  size_t operator()(char x)const
  {
    return x;
  }
};

template <>
struct hash<unsigned char>
{
  size_t operator()(unsigned char x)const
  {
    return x;
  }
};

template <>
struct hash<signed char>
{
  size_t operator()(signed char x)const
  {
    return x;
  }
};

template <>
struct hash<short>
{
  size_t operator()(short x)const
  {
    return x;
  }
};

template <>
struct hash<unsigned short>
{
  size_t operator()(unsigned short x)const
  {
    return x;
  }
};

template <>
struct hash<int>
{
  size_t operator()(int x)const
  {
    return x;
  }
};

template <>
struct hash<unsigned int>
{
  size_t operator()(unsigned int x)const
  {
    return x;
  }
};

template <>
struct hash<long>
{
  size_t operator()(long x)const
  {
    return x;
  }
};

template <>
struct hash<unsigned long>
{
  size_t operator()(unsigned long x)const
  {
    return x;
  }
};

MINI_STL_END
#endif // MINI_STL_HASH_FUN_H
