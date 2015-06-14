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
  deque <int> c1, c2, c3;
     deque <int>::iterator c1_Iter;

     c1.push_back( 1 );
     c1.push_back( 2 );
     c1.push_back( 3 );
     c2.push_back( 10 );
     c2.push_back( 20 );
     c3.push_back( 100 );

     cout << "The original deque c1 is:";
     for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
        cout << " " << *c1_Iter;
     cout << endl;

     c1.swap( c2 );

     cout << "After swapping with c2, deque c1 is:";
     for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
        cout << " " << *c1_Iter;
     cout << endl;

     swap( c1,c3 );

     cout << "After swapping with c3, deque c1 is:";
     for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
        cout << " " << *c1_Iter;
     cout << endl;

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
