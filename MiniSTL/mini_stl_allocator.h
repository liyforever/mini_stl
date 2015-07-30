#ifndef MINI_STL_ALLOCATOR_H
#define MINI_STL_ALLOCATOR_H
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include "mini_stl_config.h"

MINI_STL_BEGIN

class malloc_allocator
{
public:
  typedef void (* voidFunPtr)();
private:
  static void *oom_malloc(size_t);
  static void *oom_realloc(void *, size_t);
  static voidFunPtr malloc_alloc_oom_handler;
public:
  static void *allocate(size_t _Num)
  {
    void *result = malloc(_Num);
    if (!result)
      result = oom_malloc(_Num);
    return result;
  }

  static void deallocate(void *_Ptr, size_t /* n */)
  {
    free(_Ptr);
  }

  static void *reallocate(void *_Ptr,
                          size_t /* old_size*/,
                          size_t _NewSize)
  {
    void *result = realloc(_Ptr, _NewSize);
    if (!result)
      result = oom_realloc(_Ptr, _NewSize);
    return result;
  }

  static voidFunPtr set_malloc_handler(voidFunPtr _NewFunPtr)
  {
    voidFunPtr oldFunPtr = malloc_alloc_oom_handler;
    malloc_alloc_oom_handler = _NewFunPtr;
    return oldFunPtr;
  }
};

malloc_allocator::voidFunPtr malloc_allocator::malloc_alloc_oom_handler = 0;

void *malloc_allocator::oom_malloc(size_t _Num)
{
  void *result;
  for (;;) {
    if (!malloc_alloc_oom_handler)
    {
      MINI_STL_THROW_BAD_ALLOC;
    }
    (*malloc_alloc_oom_handler)();
    result = malloc(_Num);
    if(result)
      return result;
  }
}

void *malloc_allocator::oom_realloc(void *_Ptr, size_t _Num)
{
  void *result;
  for (;;) {
    if(!malloc_alloc_oom_handler)
    {
      MINI_STL_THROW_BAD_ALLOC;
    }
    (*malloc_alloc_oom_handler)();
    result = realloc(_Ptr, _Num);
    if (result)
      return result;
  }
}


//free list
enum {ALIGN = 8};
enum {MAX_BYTES = 128};
enum {FREELISTNUM = MAX_BYTES / ALIGN};

class default_allocator
{
private:
  union obj
  {
    union obj* free_list_link;
    char client_data[1];
  };
private:
  static obj *volatile free_list_[FREELISTNUM];

  static char *start_free_;
  static char *end_free_;
  static size_t heap_size_;
#if defined (MINI_STL_USE_THREAD) && defined (OS_WIN)

#else
  static pthread_mutex_t lock_;
#endif
  static size_t free_list_index(size_t _Bytes)
  {
    return (((_Bytes)+ALIGN-1) / ALIGN - 1);
  }
private:
    //adjust bytes to 8 multiple
  static size_t round_up(size_t _Bytes)
  {
    return (((_Bytes) + ALIGN-1) & ~(ALIGN - 1));
  }

  static void *refill(size_t _Num);
  static char *chunk_alloc(size_t _Size, int &_Nobjs);
public:
  static void *allocate(size_t _Num)
  {
    if (_Num > (size_t)MAX_BYTES)
      return malloc_allocator::allocate(_Num);
#if defined (MINI_STL_USE_THREAD) && defined (OS_WIN)

#else
    pthread_mutex_lock(&lock_);
#endif
    obj * volatile *my_free_list = free_list_ + free_list_index(_Num);
    obj * result = *my_free_list;
    if (!result) {
      void *r = refill(round_up(_Num));
      return r;
    }
    *my_free_list = result->free_list_link;
#if defined (MINI_STL_USE_THREAD) && defined (OS_WIN)

#else
    pthread_mutex_unlock(&lock_);
#endif
    return result;
  }

  static void deallocate(void *_Ptr, size_t _Num)
  {
    if (_Num > (size_t)MAX_BYTES)
      return malloc_allocator::deallocate(_Ptr, _Num);

#if defined (MINI_STL_USE_THREAD) && defined (OS_WIN)

#else
    pthread_mutex_lock(&lock_);
#endif
    obj *q = (obj*)_Ptr;
    obj * volatile *my_free_list = free_list_ + free_list_index(_Num);
    q->free_list_link = *my_free_list;
    *my_free_list = q;
#if defined (MINI_STL_USE_THREAD) && defined (OS_WIN)

#else
    pthread_mutex_unlock(&lock_);
#endif
  }

  static void *reallocate(void *_Ptr, size_t _OldSize, size_t _NewSize);
};

char *default_allocator::start_free_ = 0;
char *default_allocator::end_free_ = 0;
size_t default_allocator::heap_size_ = 0;
default_allocator::obj *volatile
default_allocator::free_list_[FREELISTNUM] = {0,0,0,0,
                                              0,0,0,0,
                                              0,0,0,0,
                                              0,0,0,0};

#if defined (MINI_STL_USE_THREAD) && defined (OS_WIN)

#else
  pthread_mutex_t default_allocator::lock_= PTHREAD_MUTEX_INITIALIZER;
#endif


void *default_allocator::reallocate(void *_Ptr, size_t _OldSize, size_t _NewSize)
{
  void* result;
  size_t copySize;

  if (_OldSize > (size_t) MAX_BYTES && _NewSize > (size_t) MAX_BYTES) {
    return(realloc(_Ptr, _NewSize));
  }
  if (round_up(_OldSize) == round_up(_NewSize))
    return _Ptr;
  result = allocate(_NewSize);
  copySize = _NewSize > _OldSize? _OldSize : _NewSize;
  memcpy(result, _Ptr, copySize);
  deallocate(_Ptr, _OldSize);
  return result;
}

void *default_allocator::refill(size_t _Num)
{
  int nobjs = 20;
  char *chunk = chunk_alloc(_Num, nobjs);
  obj * volatile *my_free_list;
  obj * result;
  obj * currentObj;
  obj * nextObj;
  if(1==nobjs)
    return chunk;

  my_free_list = free_list_ + free_list_index(_Num);

  result = (obj*)chunk;
  *my_free_list = nextObj = (obj*)(chunk + _Num);

  for (int i=1; ;++i) {
    currentObj = nextObj;
    nextObj = (obj*)((char*)nextObj + _Num);
    if(nobjs - 1 == i) {
      currentObj->free_list_link = 0;
      break;
    } else {
      currentObj->free_list_link = nextObj;
    }
  }
  return result;
}

char *default_allocator::chunk_alloc(size_t _Size, int &_Nobjs)
{
  char * result;
  size_t totalBytes = _Size * _Nobjs;
  size_t bytesLeft = end_free_ - start_free_;

  if (bytesLeft >= totalBytes) {
    result = start_free_;
    start_free_ += totalBytes;
    return result;
  } else if (bytesLeft >= _Size) {
    _Nobjs = (int)(bytesLeft / _Size);
    totalBytes = _Size * _Nobjs;
    result = start_free_;
    start_free_ += totalBytes;
    return result;
  } else {
    size_t bytes_to_get = 2 * totalBytes + round_up(heap_size_>>4);
    if (bytesLeft > 0) {
      obj * volatile *my_free_list =
          free_list_ + free_list_index(bytesLeft);
      ((obj*)start_free_)->free_list_link = *my_free_list;
      *my_free_list = (obj*)start_free_;
    }

    start_free_ = (char *)malloc(bytes_to_get);
    if (!start_free_) {
      obj * volatile *my_free_list, *p;

      for(int i=(int)_Size; i <= MAX_BYTES; i+= ALIGN) {
        my_free_list = free_list_ + free_list_index(i);
        p = *my_free_list;
       if (p) {
         *my_free_list = p->free_list_link;
         start_free_ = (char *)p;
         end_free_ = start_free_ + i;
         return chunk_alloc(_Size, _Nobjs);
       }
    }
      end_free_ = 0;
      start_free_ = (char *)malloc_allocator::allocate(bytes_to_get);
    }
    heap_size_ += bytes_to_get;
    end_free_ = start_free_ + bytes_to_get;
    return chunk_alloc(_Size, _Nobjs);
  }
}

//wrap default_allocator
template <class Type, class Alloc>
class simpleAlloc
{
public:
  static Type* allocate(size_t _Num)
  {
     return 0 == _Num? 0 : (Type*) Alloc::allocate(_Num * sizeof(Type));
  }

  static Type* allocate()
  {
    return (Type*)Alloc::allocate(sizeof(Type));
  }

  static void deallocate(Type* _Ptr, size_t _Num)
  {
    if (_Num)
      Alloc::deallocate(_Ptr, _Num * sizeof(Type));
  }

  static void deallocate(Type* _Ptr)
  {
    if (_Ptr)
      Alloc::deallocate(_Ptr, sizeof(Type));
  }
};

MINI_STL_END
#endif // MINI_STL_ALLOCATOR_H
