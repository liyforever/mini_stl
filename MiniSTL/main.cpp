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

vector<int> aa()
{
  vector<int> ii;
  ii.push_back(1111);
  ii.push_back(2222);
  ii.push_back(3333);
  cout << 1111111111111111111;
  cout << ii.size() << endl;
  return ii;
}

int main( )
{

  vector <int> v1;

  v1.push_back( 10 );
  v1.push_back( 20 );
  v1.push_back( 30 );

  v1.resize( 4,40 );
  cout << "The size of v1 is " << v1.size( ) << endl;
  cout << "The value of the last object is " << v1.back( ) << endl;

  v1.resize( 0 );
  cout << "The size of v1 is now " << v1.size( ) << endl;
  cout << "The value of the last object is now " << v1.back( ) << endl;
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
