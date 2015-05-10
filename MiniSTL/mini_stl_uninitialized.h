#ifndef MINI_STL_UNINITIALIZED_H
#define MINI_STL_UNINITIALIZED_H
#include <new>
#include "global.h"
#include "mini_stl_iterator.h"
#include "mini_stl_type_traits.h"

MINI_STL_BEGIN
template <typename ForwardIterator, typename T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& x)
{

}

MINI_STL_END
#endif // MINI_STL_UNINITIALIZED_H
