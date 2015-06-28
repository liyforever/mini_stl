#define LIB_TEST
#ifdef LIB_TEST

//#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include <string>
#include "mini_stl_multiset.h"
#include "mini_stl_stack.h"
#include "mini_stl_vector.h"
#include "mini_stl_list.h"
#include "mini_stl_deque.h"
#include "mini_stl_queue.h"
#include "mini_stl_set.h"
#include "mini_stl_tree.h"
#include "mini_stl_map.h"
using namespace Mini_STL;
//using std::cout;
//using std::endl;
using std::string;
#else
#include <vector>
#include <map>
#include <iostream>
#include <list>
#include <string>
#include <deque>
#include <set>
#include <memory>
using namespace std;
#endif
#include <functional>
#include "Windows.h"
//#include <unordered_set>
int main( )
{
  list <int> c1, c2, c3;
     list <int>::iterator c1_Iter;

     c1.push_back( 1 );
     c1.push_back( 2 );
     c1.push_back( 3 );
     c2.push_back( 10 );
     c2.push_back( 20 );
     c3.push_back( 100 );

     cout << "The original list c1 is:";
     for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
        cout << " " << *c1_Iter;
     cout << endl;

     c1.swap( c2 );

     cout << "After swapping with c2, list c1 is:";
     for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
        cout << " " << *c1_Iter;
     cout << endl;

     swap( c1,c3 );

     cout << "After swapping with c3, list c1 is:";
     for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
        cout << " " << *c1_Iter;
     cout << endl;
  }

#else
#include <iostream>
using std::cout;
using std::endl;
using std::string;
/*typedef rb_tree<int,int,identity<int>,less<int> > myRB;
void show(myRB& bb)
{
  for (auto II : bb)
    cout << II << "  ";
    cout << endl;
    cout << "size:" << bb.size() << endl;
}*/


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
}/*rb_tree<int,int,identity<int>,less<int> > tree;
  int A[] = {10,7,8,15,5,6,11,13,12};//1,2,3,9,8,7,6,4,5,10,22,22,22,1};
  cout << "main" << endl;
  tree.insert_equal(A,A+sizeof(A)/sizeof(int));//4);
  //for(auto i :A)
    //tree.insert_equal(i);

  cout << "\nsize:" << tree.size() << endl;
  cout << "test begin" << endl;
  auto r = tree.insert_unique(3);
  cout << "test endl" << endl;
  //bool bb = r.second;
  cout <<"first:" << *r.first << endl;
  cout << "second:" << r.second << endl;
  for(auto ii : tree)
    cout << ii << "  ";
  //tree.showTree();
  auto be = tree.begin();
  //while (*be != 8)
    //++be;
  for (int i=0;i!=9;++i)
    ++be;
  cout <<  "be:" <<*be<< endl;
  tree.erase(be);
  _Rb_tree_rebalance_for_erase(be.node,
                               tree.head_->parent,
                               tree.head_->left,
                               tree.head_->right);
  //tree.erase(be);
  //tree.showTree();
  for(auto ii : tree)
    cout << ii << "  ";
  cout << "\nsize:" << tree.size() << endl;
  tree.clear();
  cout << "clear:::" << endl;
  for(auto ii : tree)
    cout << ii << "  ";
  cout << "\nsize:" << tree.size() << endl;*/
}
#endif
