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
#include "mini_stl_set.h"
#include "mini_stl_tree.h"
using namespace Mini_STL;
//using std::cout;
//using std::endl;
using std::string;
#else
#include <vector>
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
/*typedef rb_tree<int,int,identity<int>,less<int> > myRB;
void show(myRB& bb)
{
  for (auto II : bb)
    cout << II << "  ";
    cout << endl;
    cout << "size:" << bb.size() << endl;
}*/

int main( )
{

  vector <int> v1;
     vector <int>::iterator Iter;
     v1.push_back( 10 );
     v1.push_back( 20 );
     v1.push_back( 30 );

     cout << "v1 =" ;
     for ( Iter = v1.begin( ) ; Iter != v1.end( ) ; Iter++ )
        cout << " " << *Iter;
     cout << endl;

     v1.insert( v1.begin( ) + 1, 40 );
     cout << "v1 =";
     for ( Iter = v1.begin( ) ; Iter != v1.end( ) ; Iter++ )
        cout << " " << *Iter;
     cout << endl;
     v1.insert( v1.begin( ) + 2, 4, 50 );

     cout << "v1 =";
     for ( Iter = v1.begin( ) ; Iter != v1.end( ) ; Iter++ )
        cout << " " << *Iter;
     cout << endl;

     v1.insert( v1.begin( )+1, v1.begin( )+2, v1.begin( )+4 );
     cout << "v1 =";
     for (Iter = v1.begin( ); Iter != v1.end( ); Iter++ )
        cout << " " << *Iter;
     cout << endl;

  // initialize a vector of vectors by moving v1
     vector < vector <int> > vv1;

     cout << "main move begin------" << endl;
     vv1.insert( vv1.begin(), move( v1 ) );
     cout << "zhunbei----------------" << endl;
     cout << "v1:size------------" << v1.size() << endl;
     cout << "end-----" << endl;

     //for(auto CC : v1)
       //cout << CC << "  ";
     if (v1.empty())
       cout << "v1 empty" << endl;

        cout << "vv1[0] = size::" << vv1[0].size() << endl;
        for (auto II =vv1[0].begin();II!=vv1[0].end();++II )
           cout << " " << *II;
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
