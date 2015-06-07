#define LIB_TEST
#ifdef LIB_TEST

#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include <string>
#include "mini_stl_vector.h"
#include "mini_stl_list.h"
using namespace Mini_STL;
using std::cout;
using std::endl;
using std::string;
#else
#include <vector>
#include <iostream>
#include <list>
#include <string>
using namespace std;
#endif
#include <functional>
#include "Windows.h"
int main( )
{
    list <int> c1, c2, c3, c4;
    list <int>::iterator c1_Iter, c2_Iter, w_Iter, f_Iter, l_Iter;

    c1.push_back( 10 );
    c1.push_back( 11 );
    c2.push_back( 12 );
    c2.push_back( 20 );
    c2.push_back( 21 );
    c3.push_back( 30 );
    c3.push_back( 31 );
    c4.push_back( 40 );
    c4.push_back( 41 );
    c4.push_back( 42 );

    cout << "c1 =";
    for ( c1_Iter = c1.begin( ); c1_Iter != c1.end( ); c1_Iter++ )
       cout << " " << *c1_Iter;
    cout << endl;

    cout << "c2 =";
    for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
       cout << " " << *c2_Iter;
    cout << endl;

    w_Iter = c2.begin( );
    w_Iter++;
    c2.splice( w_Iter,c1 );
    cout << "After splicing c1 into c2: c2 =";
    for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
       cout << " " << *c2_Iter;
    cout << endl;

    f_Iter = c3.begin( );
    c2.splice( w_Iter,c3, f_Iter );
    cout << "After splicing the first element of c3 into c2: c2 =";
    for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
       cout << " " << *c2_Iter;
    cout << endl;

    f_Iter = c4.begin( );
    l_Iter = c4.end( );
    l_Iter--;
    c2.splice( w_Iter,c4, f_Iter, l_Iter );
    cout << "After splicing a range of c4 into c2: c2 =";
    for ( c2_Iter = c2.begin( ); c2_Iter != c2.end( ); c2_Iter++ )
       cout << " " << *c2_Iter;
    cout << endl;
    cout << "c1" << endl;
    for(auto i: c1)
        cout << i << "  ";
    cout << "\n size:" << c1.size() << endl;

    cout << "c2" << endl;
    for(auto i: c2)
        cout << i << "  ";
    cout << "\n size:" << c2.size() << endl;

    cout << "c3" << endl;
    for(auto i: c3)
        cout << i << "  ";
    cout << "\n size:" << c3.size() << endl;

    cout << "c4" << endl;
    for(auto i: c4)
        cout << i << "  ";
    cout << "\n size:" << c4.size() << endl;
 }
#else
#include <iostream>
#include "mini_stl_iterator.h"
#include "memory.h"
using namespace Mini_STL;
using std::cout;
using std::endl;
using std::string;
/*template <class T>
typename _type_traits<T>::is_POD_type
tt(const T*)
{

    typedef typename _type_traits<T>::is_POD_type ii;
    return ii();
}
void cc(__true_type)
{
    cout << "true_type" << endl;
}
void cc(__false_type)
{
    cout << "false_type" << endl;
}
void ii(input_iterator_tag)
{
    cout << "input_iterator_tag" << endl;
}

void ii(output_iterator_tag)
{
    cout << "output_iterator_tag" << endl;
}


void ii(forward_iterator_tag)
{
    cout << "forward_iterator_tag" << endl;
}
void ii(bidirectional_iterator_tag)
{
    cout << "bidirectional_iterator_tag" << endl;
}
void ii(random_access_iterator_tag)
{
    cout << "random_access_iterator_tag" << endl;
}*/
#include <iostream>
#include <vector>
#include <set>
#include <time.h>
#include "Windows.h"
using namespace std;

int main()
{
    DWORD start,end;
    start = GetTickCount();
    set<int> mySet;
    for(int i=0;i!=1000000;++i)
        mySet.insert(rand());
    end = GetTickCount();
    cout << end - start << endl;
}
#endif
