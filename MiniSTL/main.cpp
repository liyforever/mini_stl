#define LIB_TEST
#ifdef LIB_TEST
#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include <utility>
#include "mini_stl_debug.h"
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
//using namespace Mini_STL;
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
#include <algorithm>
using namespace std;
#endif
#include <unordered_set>
#include <functional>
#include <map>
#include <bitset>
#include "mini_stl_bitset.h"
#include <memory>
#include <algorithm>
#include "mini_stl_algorithm.h"
#include <exception>
#include <time.h>
#include "Windows.h"
#include <cmath>
#include <vector>
#include "mini_stl_function.h"
//#define TEST_LIB
void TT(int msg,int m1, int m2) {cout << "TT" << msg<<endl;}
#define TESTBBB(msg, m1, m2) TT(msg, m1, m2)
class A{};
bool twice (int elem1,int e2)
{
  return elem1 < e2;
}

int pa(const void* a,const void* b)
{
  return *(int*)a<*(int*)b;
}

bool mod_lesser ( int elem1, int elem2 ) {
   if (elem1 < 0)
      elem1 = - elem1;
   if (elem2 < 0)
      elem2 = - elem2;
   return elem1 < elem2;
}

int main()
{
  //cout << _MY_STL::__lg(8) << endl;
  int u1[10] = {1,1,1,1,2,2,4,5,6,7};
  int u3[10];
  int u2[6] = {72,6,0,4,3,9};
  int result[20];
  int result1[20];
  char SO[1000000];
  int M1[10];
  int M2[10];
  int M3[10];
  int RE[50];
  //int SO1[200000];
  int off;
  try {
  for (int loop=0;loop!=1000;++loop) {
  for (int i=0;i!=10;++i)
    M3[i]=M2[i] = M1[i] = i;//rand()%100;
  off = rand()%10;
  std::rotate(M1,M1+off,M1+10);
  _MY_STL::rotate(M2,M2+off,M2+10);
    if (_MY_STL::equal(M1,M1+10,M2)==false)
      MINI_STL_THROW_RANGE_ERROR("cuowu");
    }
  }
  catch(std::exception& e)
  {
    cout << "yichang:" << e.what() << endl;
    cout << "offset:" << off << endl;
    cout << "M3:" << endl;
    for (auto ii : M3)
      cout << ii << " ";
    cout << endl;
    cout << "MI:" << endl;
    for (auto ii : M1)
      cout << ii << " ";
    cout << endl;
    cout << "M2:" << endl;
    for (auto ii : M2)
      cout << ii << " ";
    cout << endl;
  }

  //auto r = _MY_STL::partition_point(M1,M1+20,twice);
  //if (r!=u2+6)
  //cout << *r << endl;
  //else
    //cout <<"NO" <<  endl;

  //cout << *PR.first << "  " <<*PR.second << endl;
      //{3,15,20,21,16,20,16,10,5,22};
  /*srand(time(NULL));
  try {
  for (size_t cishu=0;cishu!=100000;++cishu) {
    for (size_t i=0;i!=20;++i) {
      M1[i] = rand()%5000;
      //SO1[i] =  SO[i];
      }
    for (size_t i=0;i!=30;++i) {
      M2[i] = rand()%5000;
      //SO1[i] =  SO[i];
      }
    _MY_STL::sort(M1,M1+20,mod_lesser);
    _MY_STL::sort(M2,M2+30,mod_lesser);
    MINI_STL_DEBUG_ORDER_COMP(M1,M1+20,mod_lesser,"m1");
    MINI_STL_DEBUG_ORDER_COMP(M2,M2+30,mod_lesser,"m2");
    _MY_STL::merge(M1,M1+20,M2,M2+30,RE,mod_lesser);
    MINI_STL_DEBUG_ORDER_COMP(RE,RE+50,mod_lesser,"RE");
  //cout << "\n\n\n\n" << endl;
    //t1 = GetTickCount();
    //std::sort(SO,SO+1000000,twice);
    //qsort(SO,1000000,sizeof(char),pa);
    //cout << "my_stl:" << GetTickCount() - t1<< endl;
    //t2 = GetTickCount();
    //std::sort(SO1,SO1+200000);
    //cout << "pj_stl:" << GetTickCount() - t2 << endl;
    //auto pa = _MY_STL::sort(SO,SO+10);
    //cout << *pa.first << "  " << *pa.second << endl;
   //for (auto t : SO)
     // cout << t << " ";
    //cout << endl;
   //MINI_STL_DEBUG_ORDER_COMP(SO,SO+1000000,twice,"TEST");
    //}

  }
  }*/


  /*catch (std::exception& e) {
    cout << "exception:";
    cout << e.what() << endl;
  }*/
  cout << "test end" << endl;
  int A[]={1,//28
           2,2,
           3,3,3,
           4,4,4,4,
           5,5,5,5,5,
           6,6,6,6,6,6,
           7,7,7,7,7,7,7};
  int B[] = {2,4,4};
  int D[] = {2,4,5};
  int C[4] = {1,2,3,4};
  int C1[9] = {1,2,3,4,9,8,2,5,4};

  cout << endl;
  getchar();
  //auto ff = _MY_STL::find_if_not(A,A+6,twice,twice);
  //cout << "FF:" << *ff << endl;
  /*for (int i=0;i!=3;++i)
  std::next_permutation(C,C+4);//,twice);
  //cout << "bool:" << bo << endl;
  for (int i=0;i!=2;++i)
  _MY_STL::prev_permutation(C,C+4);
  for (auto III : C)
    cout << III << "  ";
  cout << endl;
  int val = 5;*/
  //if (IPR.first!=B+3 && IPR.second!=D+3)
  //out << "IPR:" << *(IPR.first)<<"  " << *(IPR.second) << endl;
  //else
    //cout << "== == "<< endl;
  //for(auto CC:C)
    //cout << CC << "  ";
  //cout << endl;
  //_MY_STL::list<int> aa;
  /*for (int i=1;i!=8;++i)
    for (int j=0;j<i;++j) {
        //cout << "i:"<<i <<"j:" << j << endl;
      aa.push_back(i);
      }
  cout << "size of AA:" << aa.size() << endl;*/
  //try{
    //auto SS = _MY_STL::find_end(aa.begin(),aa.end(),B,B+3);//,twice);
  //}//search(A,A+28,B,B+7,_MY_STL::equal_to<int>());
  /*catch(std::exception& e) {
    cout << "catch:" << endl;
      cout << e.what() << endl;
  }*/
  /*if (SS!=aa.end())
  cout << _MY_STL::DISTANCE(aa.begin(),SS) << "  " << *(SS)<< endl;
  else
    cout << "no find" << endl;*/
  /*int B[5];
  try{
    for (val =0;val!=9;++val) {
      //cout << "val:" <<val<< endl;
      auto r =  _MY_STL::equal_range(A,A+28,val,twice);
      cout << "for Array:" ;
      for (;r.first!=r.second;++r.first)
        cout << *r.first <<" ";
      cout << endl;
      cout << "for list :";
      auto rr = _MY_STL::equal_range(aa.begin(),aa.end(),val,twice);
      for (;rr.first!=rr.second;++rr.first)
        cout << *rr.first <<" ";
      cout << endl;
    }
  }
  catch(std::exception & e) {
    cout << "catch" << endl;
    cout << e.what() << endl;
  }*/
  return 0;
}
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
  //cout << *ii;
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
#include "memory.h"
#include <stdio.h>
using namespace Mini_STL;
int i=1;
template<class _Ty1,
	class _Ty2> inline
	void _Construct(_Ty1 *_Ptr, _Ty2&& _Val)
	{	// construct object at _Ptr with value _Val
	void *_Vptr = _Ptr;
	::new (_Vptr) _Ty1(_MY_STL::move(_Val));
	}

class TC
{
public:
  TC() { cout <<"TC()" << endl;}
  TC(const TC& r)
  {
    cout << "TC(const TC& r)" << endl;
  }

  TC(TC&& r)
  {
    cout << "TC(TC&& r)" << endl;
  }

  TC& operator=(const TC& r)
  {
    cout << "operator=(const TC& r)" << endl;
    return *this;
  }

  TC& operator=(TC&& r)
  {
    cout << "operator=(const TC&& r)" << endl;
    return *this;
  }
private:
  int a,b,c;
};




int main()
{
  TC A[9] ;//= {0,1,2,3,4,5,6,7,8};
  auto a = ::copy_backward(A+2, A+7,A+9);
 // for (auto i : A)
   // cout << i << ' ';
  cout << a - A << endl;
  cout << endl;
}/* TC A;
  typedef _MY_STL::simpleAlloc<TC,_MY_STL::default_allocator> myAlloc;
  cout << "test begin--------------" << endl;
   TC* ptr = myAlloc::allocate(1);
   _Construct(ptr, _MY_STL::move(A));
   cout << "test endl--------------" << endl;
rb_tree<int,int,identity<int>,less<int> > tree;
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
