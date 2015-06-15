#define LIB_TEST
#ifdef LIB_TEST

#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include <string>
#include "mini_stl_stack.h"
#include "mini_stl_vector.h"
#include "mini_stl_list.h"
#include "mini_stl_deque.h"
#include "mini_stl_queue.h"
using namespace Mini_STL;
using std::cout;
using std::endl;
using std::string;
#else
#include <vector>
#include <iostream>
#include <list>
#include <string>
#include <deque>
using namespace std;
#endif
#include <functional>
#include "Windows.h"


deque<int> aa()
{

  deque<int> mm;
  mm.push_back(111);
  mm.push_back(222);
  mm.push_back(333);
  return mm;
}

int main( )
{
  priority_queue <int> q1, s2;

  q1.push( 10 );
  q1.push( 20 );
  q1.push( 30 );

  priority_queue <int>::size_type i, iii;
  i = q1.size( );
  cout << "The priority_queue length is " << i << "." << endl;

  const int& ii = q1.top( );
  cout << "The element at the top of the priority_queue is "
       << ii << "." << endl;

  q1.pop( );

  iii = q1.size( );
  cout << "After a pop, the priority_queue length is "
       << iii << "." << endl;

  const int& iv = q1.top( );
  cout << "After a pop, the element at the top of the "
       << "priority_queue is " << iv << "." << endl;

  }
#else
#include <iostream>
using std::cout;
using std::endl;
using std::string;


class BB{};

template <class T>
struct enable_if
{
  typedef int value_type;
};

template<>
struct enable_if<int>
{

};

template<class Type>
void
aaa(Type val,Type x,typename enable_if<Type>::value_type = 0)
{
  cout << "111" << endl;
}

void aaa(size_t x,int a)
{
  cout << "222222222" << endl;
}

int main()
{
  BB b1,b2;
  aaa(1,1);
}
#endif
