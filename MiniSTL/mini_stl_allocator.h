#ifndef MINI_STL_ALLOCATOR_H
#define MINI_STL_ALLOCATOR_H
#include <new>
#include <cstddef>
#include <cstdlib>
#include <climits>
#include <iostream>

#include "mini_stl_config.h"
MINI_STL_BEGIN
#define MINI_STL_SET_MALLOC_HANLDER
#define THROW_BAD_ALLOC throw std::bad_alloc()

class malloc_allocator
{
public:
#ifdef MINI_STL_SET_MALLOC_HANLDER
    typedef void (* voidFunPtr)();
#endif
private:
    static void *oom_malloc(size_t);
    static void *oom_realloc(void *, size_t);
#ifdef MINI_STL_SET_MALLOC_HANLDER
    static voidFunPtr malloc_alloc_oom_handler;
#endif
public:
    static void *allocate(size_t n)
    {
#ifdef MINI_STL_SET_MALLOC_HANLDER
        void *result = malloc(n);
        if(!result)
            result = oom_malloc(n);
        return result;
#else
        return malloc(n);
#endif
    }

    static void deallocate(void *p, size_t /* n */)
    {
        free(p);
    }

    static void *reallocate(void *p, size_t /* old_size*/, size_t newSize)
    {
#ifdef MINI_STL_SET_MALLOC_HANLDER
        void *result = realloc(p, newSize);
        if(!result)
            result = oom_realloc(p, newSize);
        return result;
#else
        return realloc(p, newSize);
#endif
    }

#ifdef MINI_STL_SET_MALLOC_HANLDER
    static voidFunPtr set_malloc_handler(voidFunPtr newFunPtr)
    {
        voidFunPtr oldFunPtr = malloc_alloc_oom_handler;
        malloc_alloc_oom_handler = newFunPtr;
        return oldFunPtr;
    }
#endif
};

#ifdef MINI_STL_SET_MALLOC_HANLDER
malloc_allocator::voidFunPtr malloc_allocator::malloc_alloc_oom_handler = 0;

void *malloc_allocator::oom_malloc(size_t n)
{
    void *result;
    for(;;) {
        if (!malloc_alloc_oom_handler)
        {
            THROW_BAD_ALLOC;
        }
        (*malloc_alloc_oom_handler)();
        result = malloc(n);
        if(result)
            return result;
    }
}

void *malloc_allocator::oom_realloc(void *p, size_t n)
{
    void *result;
    for(;;) {
        if(!malloc_alloc_oom_handler)
        {
            THROW_BAD_ALLOC;
        }
        (*malloc_alloc_oom_handler)();
        result = realloc(p, n);
        if(result)
            return result;
    }
}

#endif

//自由链表分配器
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
    static obj *volatile free_list[FREELISTNUM];

    static char *start_free;
    static char *end_free;
    static size_t heap_size;
    static size_t free_list_index(size_t bytes)
    {
        return (((bytes) + ALIGN-1)/ALIGN-1);
    }

private:
    //调整bytes至8的倍数
    static size_t round_up(size_t bytes)
    {
        return (((bytes) + ALIGN-1) & ~(ALIGN - 1));
    }

    static void *refill(size_t n);
    static char *chunk_alloc(size_t size, int &nobjs);
public:
    static void *allocate(size_t n)
    {
        if(n > (size_t)MAX_BYTES) {
            return malloc_allocator::allocate(n);
        }
        obj * volatile *my_free_list = free_list + free_list_index(n);
        obj * result = *my_free_list;
        if(!result) {
            void *r = refill(round_up(n));
            return r;
        }
        *my_free_list = result->free_list_link;
        return result;
    }

    static void deallocate(void *p, size_t n)
    {
        if(n>(size_t)MAX_BYTES) {
            return malloc_allocator::deallocate(p, n);
        }
        obj *q = (obj*)p;
        obj * volatile *my_free_list = free_list + free_list_index(n);
        q->free_list_link = *my_free_list;
        *my_free_list = q;
    }

    static void *reallocate(void *p, size_t oldSize, size_t newSize);
};

char *default_allocator::start_free = 0;
char *default_allocator::end_free = 0;
size_t default_allocator::heap_size = 0;
default_allocator::obj *volatile
default_allocator::free_list[FREELISTNUM] = {0,0,0,0,
                                             0,0,0,0,
                                             0,0,0,0,
                                             0,0,0,0};


void *default_allocator::reallocate(void *p, size_t oldSize, size_t newSize)
{
    void* result;
    size_t copySize;

    if (oldSize > (size_t) MAX_BYTES && newSize > (size_t) MAX_BYTES) {
        return(realloc(p,newSize));
    }
    if (round_up(oldSize) == round_up(newSize))
        return p;
    result = allocate(newSize);
    copySize = newSize > oldSize? oldSize : newSize;
    memcpy(result, p, copySize);
    deallocate(p, oldSize);
    return result;
}

void *default_allocator::refill(size_t n)
{
    int nobjs = 20;
    char *chunk = chunk_alloc(n, nobjs);
    obj * volatile *my_free_list;
    obj * result;
    obj * currentObj;
    obj * nextObj;
    //如果只有一个块,就返回给调用者
    if(1==nobjs)
        return chunk;
    //加入新的节点
    my_free_list = free_list + free_list_index(n);
    //返回其中一块
    result = (obj*)chunk;
    *my_free_list = nextObj = (obj*)(chunk + n);

    for(int i=1; ;++i) {
        currentObj = nextObj;
        nextObj = (obj*)((char*)nextObj +n);
        if(nobjs - 1 == i) {
            currentObj->free_list_link = 0;
            break;
        } else {
            currentObj->free_list_link = nextObj;
        }
    }
    return result;
}

char *default_allocator::chunk_alloc(size_t size, int &nobjs)
{
    char * result;
    size_t totalBytes = size * nobjs;
    size_t bytesLeft = end_free - start_free;

    if(bytesLeft>=totalBytes) {
        //case1:空间够分配
        result = start_free;
        start_free += totalBytes;
        return result;
    } else if(bytesLeft >= size) {
        //case2:够分配1个或则以上
        nobjs = (int)(bytesLeft / size);
        totalBytes = size * nobjs;
        result = start_free;
        start_free += totalBytes;
        return result;
    } else {
        //内存池不够提供
        size_t bytes_to_get = 2 * totalBytes + round_up(heap_size>>4);

        if(bytesLeft>0) {
            obj * volatile *my_free_list =
                    free_list + free_list_index(bytesLeft);
            ((obj*)start_free)->free_list_link = *my_free_list;
            *my_free_list = (obj*)start_free;
        }

        start_free = (char *)malloc(bytes_to_get);
        if (!start_free) {
            obj * volatile *my_free_list, *p;

            for(int i=(int)size; i<=MAX_BYTES; i+= ALIGN) {
                my_free_list = free_list + free_list_index(i);
                p = *my_free_list;
                if(p) {
                    *my_free_list = p->free_list_link;
                    start_free = (char *)p;
                    end_free = start_free + i;
                    return chunk_alloc(size, nobjs);
                }
            }
            end_free = 0;
            start_free = (char *)malloc_allocator::allocate(bytes_to_get);
        }
        heap_size += bytes_to_get;
        end_free = start_free + bytes_to_get;
        return chunk_alloc(size, nobjs);
    }
}

//包装双层分配器
template <typename T, class Alloc>
class simpleAlloc
{
public:
    static T* allocate(size_t n)
    {
        return 0==n? 0 : (T*) Alloc::allocate(n * sizeof(T));
    }

    static T* allocate(void)
    {
        return (T*)Alloc::allocate(sizeof(T));
    }

    static void deallocate(T* ptr, size_t n)
    {
        if(!n)
            Alloc::deallocate(ptr, n * sizeof(T));
    }

    static void deallocate(T* ptr)
    {
        Alloc::deallocate(ptr, sizeof(T));
    }
};
/*#include <winerror.h>
template <typename T>
class allocator {
public:
    typedef T           value_type;
    typedef T*          pointer;
    typedef const T*    const_pointer;
    typedef T&          reference;
    typedef const T&    const_reference;
    typedef size_t      size_type;
    typedef ptrdiff_t   difference_type;
    pointer allocate(size_type n)
    {
        set_new_handler(0);
        pointer tmp = (T*)(::operator new((size_t)(size * sizeof(T))));
        if (tmp == 0)
        {
            std::cerr << "out of memory" << std::endl;
            exit(1);
        }
        return tmp;
    }

    void deallocate(T* buffer)
    {
        ::operator delete(buffer);
    }

    pointer address(reference x)
    {
        return (pointer)&x;
    }

    const_pointer const_address(const_reference x)
    {
        return (const_address)&x;
    }

    size_type init_page_size()
    {

    }

    size_type max_size() const
    {

    }
};

template<>
class allocator<void>
{
    typedef void* pointer;
};*/
MINI_STL_END
#endif // MINI_STL_ALLOCATOR_H
