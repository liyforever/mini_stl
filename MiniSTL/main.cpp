#define LIB_TEST
#ifdef LIB_TEST

#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include <string>
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
int main( )
{
  deque <int> c1, c2;
    deque <int>::iterator Iter;

    c1.push_back( 10 );
    c1.push_back( 20 );
    c1.push_back( 30 );
    c2.push_back( 40 );
    c2.push_back( 50 );
    c2.push_back( 60 );

    cout << "[ c1 = ";
    for ( Iter = c1.begin( ); Iter != c1.end( ); Iter++ )
       cout << *Iter << " ";
    cout << "]" << endl;

    Iter = c1.begin( );
    //Iter++;
    c1.insert( Iter, 100 );
    cout << "[ c1 = ";
    for ( Iter = c1.begin( ); Iter != c1.end( ); Iter++ )
       cout << *Iter << " ";
    cout << "]" << endl;
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
