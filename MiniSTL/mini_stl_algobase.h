#ifndef MINI_STL_ALGOBASE_H
#define MINI_STL_ALGOBASE_H
#include "mini_stl_config.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"
#include "mini_stl_function.h"
#include <string.h>
#include <wchar.h>
#include <iostream>
using std::cout;
using std::endl;
MINI_STL_BEGIN

template <class Type>
inline const Type& max(const Type &left, const Type &right)
{
  return right < left ? left : right;
}

template <class Type, class Cmp>
inline const Type& max(const Type &left, const Type &right, Cmp cp)
{
  return cp(right, left) ? left : right;
}

template <class Type, class Cmp>
inline const Type& min(const Type &left, const Type &right, Cmp cp)
{
  return cp(left, right) ? left : right;
}

template <class Type>
inline const Type& min(const Type &left, const Type &right)
{
  return left < right ? left : right;
}

template <class Type>
inline void
swap(Type& lhs, Type& rhs)
{
  Type tmp = lhs;
  lhs = rhs;
  rhs = tmp;
}

template <class ForwardIter, class T>
inline void
fill(ForwardIter first, ForwardIter last, const T& value)
{
  for( ; first!=last; ++first)
    *first = value;
}


template <class OutputIter, class Size, class T>
inline OutputIter
fill_n(OutputIter first, Size n, const T& value)
{
  for( ; n>0; --n, ++first)
    *first = value;
  return first;
}

template <class RandomAccessIter, class OutputIter, class Distance>
inline OutputIter
__copy_d(RandomAccessIter first, RandomAccessIter last,
         OutputIter result, Distance*)
{
  for(Distance n = last - first; n>0; --n, ++result, ++first)
    *result = *first;
  return result;
}

template <class T>
inline T*
__copy_t(const T* first, const T* last, T* result,
         __true_type)
{
  memmove(result, first, sizeof(T) * (last - first));
  return result + (last - first);
}

template <class T>
inline T*
__copy_t(const T* first, const T* last, T* result,
         __false_type)
{
  return __copy_d(first, last, result, (ptrdiff_t*)0);
}

//InputIter 版本
template <class InputIter, class OutputIter>
inline OutputIter
__copy(InputIter first, InputIter last,
       OutputIter result, input_iterator_tag)
{
  for( ; first!=last; ++result, ++first)
    *result = *first;
  return result;
}

//RandomAccessIter 版本
template <class RandomAccessIter, class OutputIter>
inline OutputIter
__copy(RandomAccessIter first, RandomAccessIter last,
       OutputIter result, random_access_iterator_tag)
{
  return __copy_d(first, last, result, DISTANCE_TYPE(first));
}
template <class InputIter, class OutputIter>
struct __copy_dispatch
{
  OutputIter operator()(InputIter first, InputIter last,
                          OutputIter result)
  {
    return __copy(first, last, result, ITERATOR_CATEGORY(first));
  }
};

template <class T>
struct __copy_dispatch<T*, T*>
{
  T* operator ()(T* first, T* last, T* result)
  {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return __copy_t(first, last, result, t());
  }
};

template <class T>
struct __copy_dispatch<const T*, T*>
{
  T* operator ()(const T* first, const T* last, T* result)
  {
    typedef typename _type_traits<T>::has_trivial_assignment_operator t;
    return __copy_t(first, last, result, t());
  }
};
//copy对外接口
template <class InputIter, class OutputIter>
inline OutputIter
copy(InputIter first, InputIter last, OutputIter result)
{
  return __copy_dispatch<InputIter, OutputIter>()
            (first, last, result);
}
//char*特化版本
inline char* copy(const char* first, const char* last, char *result)
{
  memmove(result, first, last - first);
  return result + (last - first);
}
//wchar_t*特化版
inline wchar_t* copy(const wchar_t *first, const wchar_t *last, wchar_t *result)
{
  wmemmove(result, first, last - first);
  return result + (last - first);
}


/************************copy_backward__begin**************************/

template <class BidirectionalIter1, class BidirectionalIter2>
inline BidirectionalIter2 copy_backward(BidirectionalIter1 first,
                                        BidirectionalIter1 last,
                                        BidirectionalIter2 result)
{
  while (first != last)
    *--result = *--last;
  return result;
}

template <class RandomAccessIter, class BidirectionalIter, class Distance>
inline BidirectionalIter copy_backward(RandomAccessIter first,
                                       RandomAccessIter last,
                                       BidirectionalIter result,
                                       Distance*)
{
  for (Distance n = last - first; n > 0; --n)
    *--result = *--last;
  return result;
}
/************************copy_backward__end**************************/

/************************heap_begin**********************************/
template <class RandomAccessIterator, class Distance, class Type>
void _push_heap(RandomAccessIterator first, Distance holeIndex,
                Distance topIndex, Type val)
{
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex>topIndex && *(first + parent)<val) {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = val;
}

template <class RandomAccessIterator, class Distance, class Type>
inline void push_heap_aux(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Distance*, Type*)
{
  _push_heap(first, Distance((last - first) - 1), Distance(0),
             Type(*(last - 1)));
}

template <class RandomAccessIterator>
inline void push_heap(RandomAccessIterator first,
                      RandomAccessIterator last)
{
  push_heap_aux(first, last, DISTANCE_TYPE(first),
                VALUE_TYPE(first));
}

template <class RandomAccessIterator>
inline void pop_heap(RandomAccessIterator first,
                     RandomAccessIterator last)
{
  pop_heap_aux(first, last, VALUE_TYPE(first));
}

template <class RandomAccessIterator, class Type>
inline void pop_heap_aux(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Type*)
{
  _pop_heap(first, last - 1, last - 1, Type(*(last-1)),
            DISTANCE_TYPE(first));
}

template <class RandomAccessIterator, class Type,
          class Distance>
inline void _pop_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      RandomAccessIterator result,
                      Type val, Distance*)
{
  *result = *first;
  _adjust_heap(first, Distance(0), Distance(last - first), val);
}

template <class RandomAccessIterator, class Distance, class Type>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex,
                  Distance len, Type val)
{
  Distance topIndex = holeIndex;
  Distance rChild = 2 * holeIndex + 2;
  while (rChild<len) {
    if (*(first+rChild) < *(first+(rChild-1)))
      --rChild;
    *(first + holeIndex) = *(first + rChild);
    holeIndex = rChild;
    rChild = 2 * (rChild + 1);
  }
  if (rChild==len) {
    *(first + holeIndex) = *(first + (rChild - 1));
    holeIndex = rChild - 1;
  }
  _push_heap(first, holeIndex, topIndex, val);
}

template <class RandomAccessIterator>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last
               )
{
  while (last-first>1)
    pop_heap(first, last--);
}

template <class RandomAccessIterator>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last)
{
  make_heap_aux(first, last, VALUE_TYPE(first),
                DISTANCE_TYPE(first));
}

template <class RandomAccessIterator, class Type, class Distance>
void make_heap_aux(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Type*, Distance*)
{
  if (last-first<2)
    return;
  Distance len = last - first;

  Distance parent = (len-2)/2;
  for (;;)
  {
    _adjust_heap(first, parent, len, Type(*(first+parent)));
    if (parent==0)
      return;
    --parent;
  }
}


template <class RandomAccessIterator, class Distance, class Type,
          class Compare>
void _push_heap(RandomAccessIterator first, Distance holeIndex,
                Distance topIndex, Type val, Compare comp)
{
  Distance parent = (holeIndex - 1) / 2;
  while (holeIndex>topIndex && comp(*(first + parent),val)) {
    *(first + holeIndex) = *(first + parent);
    holeIndex = parent;
    parent = (holeIndex - 1) / 2;
  }
  *(first + holeIndex) = val;
}

template <class RandomAccessIterator, class Distance, class Type,
          class Compare>
inline void push_heap_aux(RandomAccessIterator first,
                          RandomAccessIterator last,
                          Distance*, Type*,
                          Compare comp)
{
  _push_heap(first, Distance((last - first) - 1), Distance(0),
             Type(*(last - 1)), comp);
}

template <class RandomAccessIterator, class Compare>
inline void push_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      Compare comp)
{
  push_heap_aux(first, last, DISTANCE_TYPE(first),
                VALUE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Compare>
inline void pop_heap(RandomAccessIterator first,
                     RandomAccessIterator last,
                     Compare comp)
{
  pop_heap_aux(first, last, VALUE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Type, class Compare>
inline void pop_heap_aux(RandomAccessIterator first,
                    RandomAccessIterator last,
                    Type*, Compare comp)
{
  _pop_heap(first, last - 1, last - 1, Type(*(last-1)),
            DISTANCE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Type,
          class Distance, class Compare>
inline void _pop_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      RandomAccessIterator result,
                      Type val, Distance*,
                      Compare comp)
{
  *result = *first;
  _adjust_heap(first, Distance(0), Distance(last - first), val,
               comp);
}


template <class RandomAccessIterator, class Distance, class Type,
          class Compare>
void _adjust_heap(RandomAccessIterator first, Distance holeIndex,
                  Distance len, Type val, Compare comp)
{
  Distance topIndex = holeIndex;
  Distance rChild = 2 * holeIndex + 2;
  while (rChild<len) {
    if (comp(*(first+rChild),*(first+(rChild-1))))
      --rChild;
    *(first + holeIndex) = *(first + rChild);
    holeIndex = rChild;
    rChild = 2 * (rChild + 1);
  }
  if (rChild==len) {
    *(first + holeIndex) = *(first + (rChild - 1));
    holeIndex = rChild - 1;
  }
  _push_heap(first, holeIndex, topIndex, val, comp);
}

template <class RandomAccessIterator, class Compare>
void sort_heap(RandomAccessIterator first,
               RandomAccessIterator last,
               Compare comp)
{
  while (last-first>1)
    pop_heap(first, last--, comp);
}

template <class RandomAccessIterator, class Compare>
inline void make_heap(RandomAccessIterator first,
                      RandomAccessIterator last,
                      Compare comp)
{
  make_heap_aux(first, last, VALUE_TYPE(first),
                DISTANCE_TYPE(first), comp);
}

template <class RandomAccessIterator, class Type, class Distance,
          class Compare>
void make_heap_aux(RandomAccessIterator first,
                   RandomAccessIterator last,
                   Type*, Distance*, Compare comp)
{
  if (last-first<2)
    return;
  Distance len = last - first;

  Distance parent = (len-2)/2;
  for (;;)
  {
    _adjust_heap(first, parent, len, Type(*(first+parent)),comp);
    if (parent==0)
      return;
    --parent;
  }
}


/************************heap_end**********************************/

template <class InputIterator1, class InputIterator2>
inline bool
 equal(InputIterator1 first1, InputIterator1 last1,
       InputIterator2 first2)
{
  for (; first1!=last1; ++first1,++first2)
    if (*first1!=*first2)
      return false;
  return true;
}

template <class InputIterator1, class InputIterator2,
          class BinaryPredicate>
inline bool
 equal(InputIterator1 first1, InputIterator1 last1,
       InputIterator2 first2, BinaryPredicate comp)
{
  for (; first1!=last1; ++first1,++first2)
    if (!comp(*first1,*first2))
      return false;
  return true;
}

template <class InputIterator1, class InputIterator2>
inline bool
lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2)
{
  for (; first1!=last1 && first2!=last2; ++first1,++first2) {
    if (*first1<*first2)
      return true;
    if (*first2<*first1)
      return false;
  }
  return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2,
          class BinaryPredicate>
inline bool
lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
                        InputIterator2 first2, InputIterator2 last2,
                        BinaryPredicate comp)
{
  for (; first1!=last1 && first2!=&&last2; ++first1,++first2) {
    if (comp(*first1,*first2))
      return true;
    if (comp(*first2,*first1))
      return false;
  }
  return first1 == last1 && first2 != last2;
}
MINI_STL_END
#endif // MINI_STL_ALGOBASE_H
