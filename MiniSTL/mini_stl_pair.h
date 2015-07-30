#ifndef MINI_STL_PAIR_H
#define MINI_STL_PAIR_H
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"

MINI_STL_BEGIN

template <class Type1, class Type2>
struct pair
{
  typedef Type1 first_type;
  typedef Type2 second_type;
  Type1 first;
  Type2 second;
  pair() : first(Type1()), second(Type2()) {}
  pair(const Type1& _First, const Type2& _Second)
    : first(_First), second(_Second)
  {}

  template <class Other1, class Other2>
  pair(const pair<Other1, Other2>& _Val) :
    first(_Val.first), second(_Val.second)
  {}

#ifdef MINI_STL_RVALUE_REFS
  template<class Other1, class Other2>
  pair(Other1&& _Val1, Other2&& _Val2)
    : first(_MY_STL::move(_Val1)) ,
      second(_MY_STL::move(_Val2))
  {}

  template<class Other1, class Other2>
  pair& operator=(pair<Other1, Other2>&& _Right)
  {
    this->first = _MY_STL::move(_Right.first);
    this->second = _MY_STL::move(_Right.second);
    return *this;
  }
#endif
  template<class Other1, class Other2>
  pair& operator=(const pair<Other1, Other2>& _Right)
  {
    first = _Right.first;
    second = _Right.second;
    return *this;
  }
};

template <class Type1, class Type2>
inline bool
operator==(const pair<Type1, Type2>& _Left,
           const pair<Type1, Type2>& _Right)
{
  return _Left.first == _Right.first
      && _Left.second == _Right.second;
}

template <class Type1, class Type2>
inline bool
operator<(const pair<Type1, Type2>& _Left,
          const pair<Type1, Type2>& _Right)
{
  return _Left.first < _Right.first ||
         (!(_Right.first < _Right.first)
          &&_Left.second < _Right.second);
}


template <class Type1, class Type2>
inline bool
operator!=(const pair<Type1, Type2>& _Left,
           const pair<Type1, Type2>& _Right)
{
  return !(_Left == _Right);
}

template <class Type1, class Type2>
inline bool
operator>(const pair<Type1, Type2>& _Left,
          const pair<Type1, Type2>& _Right)
{
  return _Right < _Left;
}

template <class Type1, class Type2>
inline bool
operator<=(const pair<Type1, Type2>& _Left,
           const pair<Type1, Type2>& _Right)
{
  return !(_Right < _Left);
}

template <class Type1, class Type2>
inline bool
operator>=(const pair<Type1, Type2>& _Left,
           const pair<Type1, Type2>& _Right)
{
  return !(_Left < _Right);
}


template <class Type1, class Type2>
inline _MY_STL::pair<Type1, Type2>
make_pair(const Type1& _Val1, const Type2& _Val2)
{
  return pair<Type1, Type2>(_Val1, _Val2);
}


template<class Type1, class Type2>
inline _MY_STL::pair<Type1, Type2>
make_pair(Type1& _Val1, Type2&& _Val2)
{
  return pair<Type1, Type2>
      (_Val1, _MY_STL::move(_Val2));
}

template<class Type1, class Type2>
inline _MY_STL::pair<Type1, Type2>
make_pair(Type1&& _Val1,Type2& _Val2)
{
  return pair<Type1, Type2>
      (_MY_STL::move(_Val1), _Val2);
}

template<class Type1, class Type2>
inline _MY_STL::pair<Type1, Type2>
make_pair(Type1&& _Val1, Type2&& _Val2)
{
  return pair<Type1, Type2>
      (_MY_STL::move(_Val1),
       _MY_STL::move(_Val2));
}

MINI_STL_END
#endif // MINI_STL_PAIR_H
