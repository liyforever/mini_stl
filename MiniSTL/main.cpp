#include <iostream>
#include <mini_stl_config.h>
#include "mini_stl_memory.h"
using namespace std;
typedef Mini_STL::simpleAlloc<int, Mini_STL::default_allocator> Myalloc;

int *p[20];
void* fn1(void*)
{
  for (int i=0; i!=10; ++i) {
    p[i] = Myalloc::allocate();
    *p[i] = i;
  }
  return NULL;
}

void* fn2(void*)
{
  for (int i=10; i!=20; ++i) {
    p[i] = Myalloc::allocate();
    *p[i] = i;
  }
  return NULL;
}

int main()
{
  pthread_t t1, t2;
  pthread_create(&t1, NULL, fn1, NULL);
  pthread_create(&t1, NULL, fn2, NULL);
  pthread_join(t1, NULL);
  pthread_join(t2, NULL);
  for (int i=0; i!=20; ++i)
    cout << *p[i] << endl;
}
