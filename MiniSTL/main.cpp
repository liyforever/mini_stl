#define LIB_TEST
#ifdef LIB_TEST

#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include <utility>
#include "mini_stl_multiset.h"
#include "mini_stl_stack.h"
#include "mini_stl_vector.h"
#include "mini_stl_list.h"
#include "mini_stl_deque.h"
#include "mini_stl_queue.h"
#include "mini_stl_set.h"
#include "mini_stl_hashtable.h"
#include "mini_stl_string.h"
#include "mini_stl_tree.h"
#include "mini_stl_map.h"
using namespace Mini_STL;
using std::cout;
using std::endl;
using std::cin;
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
#include <unordered_set>
#include <functional>
#include "Windows.h"
#include <map>
#include <bitset>
#include "mini_stl_bitset.h"
#include <memory>
#define TEST_LIB
class A{};
int main()
{
#ifdef TEST_LIB
  std::bitset<5> ss;
  _MY_STL::bitset<5> bb;
  //ss.to_string
  //cout << bb << endl;
  cout << bb.count() << endl;
  //cout << bb.flip() << endl;
  cout << "bt Begin" << endl;
  cout << bb.test(4) << endl;
  bb.flip(4);
  cout << "bt End" << endl;
  cout << bb.test(4) << endl;
  //cout << bb << endl;
  cout << "a" << endl;
  cout << bb.count() << endl;
  _MY_STL::string result = bb.to_string();

  cout << "result"<< result << endl;
  //cout << _GET_ARRAY_SIZE(65) << endl;
  //cout << sizeof(size_t) << endl;
#else
  const char* a= "100101000000000";
  std::bitset<10> bb(a);
  cout << bb << endl;

  /*typedef hashtable<int,
                    int,
                    hash<int>,
                    identity<int>,
                    equal_to<int>,
                    default_allocator>
      MyHT;
  MyHT
      ht1(50);
  //for (int cc=0;cc!=10;++cc) {
  for (int i=0;i!=10;++i) {
    cout << ht1.buckets_.size() << endl;
    ht1.insert_equal(i);
  }
  for (int i=0;i!=10;++i) {
    cout << ht1.buckets_.size() << endl;
    ht1.insert_equal(i);
  }
  for (int i=0;i!=10;++i) {
    cout << ht1.buckets_.size() << endl;
    ht1.insert_equal(54);
  }*/
    //}
  //if (ht1.begin() == ht1.end())
    //cout << "===" << endl;
  //for (auto &cc : ht1.buckets_)
    //cout << cc.size() << endl;
  //for(auto ii = ht1.begin();ii!=ht1.end();++ii)
    //cout << (*ii);
  //auto TI = ht1.begin();
  //cout << *TI << endl;
  //for (auto aa = ht1.begin();aa!=ht1.end();++aa)//(int i=0;i!=10;++i,++TI)
    //cout << "myIndex:" << aa.index
      //      << "value:" << *aa << endl;

  //for (auto aa = ht1.rbegin();aa!=ht1.rend();++aa)
    //cout << "myIndex:" << aa.base().index
      //      << "value:" << *aa << endl;
  //cout << endl;
  //auto ii = ht1.end();
  //--ii;
  //for (; ii!=ht1.begin();--ii)
    ;//cout << *ii;
  //for (auto ii = ht1.rbegin();ii!=ht1.rend();++ii)
    //cout << *ii ;
 /* cout << endl;
  cout << ht1.buckets_.size() << endl;
  cout << "htSize:" << ht1.size() << endl;
  cout << "htCount:" << ht1.count(1) << endl;
  cout << "rbegin test:" << endl;
  for (auto rI = ht1.rbegin();rI!=ht1.rend();++rI)
    cout << *rI << " ";
  cout <<"\n rbegni end" << endl;

  cout << "begin test:" << endl;
  for (auto rI = ht1.begin();rI!=ht1.end();++rI)
    cout << *rI << " ";
  cout <<"\n begni end" << endl;

  int index = ht1._get_index_for_key(54);
  for (auto ii: ht1.buckets_[index])
    cout << ii << " ";
  cout << "equal_range test:" << endl;
  //auto fI = ht1.find(54);
  //cout << *--fI << endl;
  auto PRI1 = ht1.equal_range(54);
  auto PRI = PRI1;
  cout << *PRI1.first << "  "<< *PRI1.second << endl;
  //for(int cc=0;cc!=10;++cc)
    //cout << *++PRI1.first << endl;
  if (PRI1.first==PRI1.second)
    cout << "xiangdeng =============" << endl;
  //cout << *PRI1.first << "  "<<*--PRI1.second << endl;
  for (;PRI.first!=PRI.second;++PRI.first)
    cout << *PRI.first <<" ";
  cout << "test endl" << endl;
  cout << "erase test : "<<endl;
  /*int eNum = *///ht1.erase(PRI1.first,PRI1.second);
   //ht1.erase(ht1.find(54));
   //cout << "begin:"<<*ht1.begin() << endl;
  //cout << "eNum:" << eNum << endl;
  /*int in1 = ht1._get_index_for_key(54);
  for (auto ii: ht1.buckets_[in1])
    cout << ii << " ";
  cout << "test end:" << endl;
  cout << ht1.size() << endl;*/
  return 0;
#endif
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
#include <stdio.h>
int i=1;
int main()
{
  int a[]={6,7,8,9,10};
  int *p = a;
  ++p;
  printf("%d,%d",*p,*++p);
  cout << "\n" << sizeof(int) << endl;
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

#endif
