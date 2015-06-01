#ifndef VECTOR_H
#define VECTOR_H
#include "global.h"
#include "mini_stl_iterator.h"
#include "mini_stl_allocator.h"

MINI_STL_BEGIN

template <typename T>
class vector : public iterator<random_access_iterator_tag,T>
{
public:
    vector();
    vector(size_type count, const value_type val);
    vector(size_type count, const value_type val, const allocator& alloc);
    vector(const vector& Vec);
    template <typename InuputIterator>
    vector(InputIterator first,
           InputIterator last);
    template <typename InputIterator>
    vector(InputIterator first,
           InputIterator last,
           allocator alloc);
#ifdef C_PLUS_PLUS_11
    vector(vector&& rightVec);
#endif
    void swap(vector<value_type,allocator> rightVec);
    size_type size() const;

};

MINI_STL_END
#endif // VECTOR_H
