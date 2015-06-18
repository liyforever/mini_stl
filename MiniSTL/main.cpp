#define LIB_TEST
#ifdef LIB_TEST

#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
//#include <string>
#include "mini_stl_stack.h"
#include "mini_stl_vector.h"
#include "mini_stl_list.h"
#include "mini_stl_deque.h"
#include "mini_stl_queue.h"
#include "mini_stl_tree.h"
using namespace Mini_STL;
//using std::cout;
//using std::endl;
//using std::string;
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



int main( )
{
  rb_tree<int,int,identity<int>,less<int> > tree;
  int A[14] = {1,2,3,9,8,7,6,4,5,10,22,22,22,1};
  cout << "main" << endl;
  tree.insert_equal(A,A+5);//4);
  //for(auto i :A)
    //tree.insert_equal(i);
  for(auto ii : tree)
    cout << ii << "  ";
  cout << "\nsize:" << tree.size() << endl;
  cout << "test begin" << endl;
  auto r = tree.insert_unique(3);
  cout << "test endl" << endl;
  //bool bb = r.second;
  cout <<"first:" << *r.first << endl;
  cout << "second:" << r.second << endl;
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
