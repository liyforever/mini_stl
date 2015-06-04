#define LIB_TEST
#ifdef LIB_TEST

#define MINI_STL_TEST
#ifdef MINI_STL_TEST
#include <iostream>
#include "mini_stl_vector.h"
using namespace Mini_STL;
using std::cout;
using std::endl;
#else
#include <vector>
#include <iostream>
using namespace std;
#endif



int main()
{
#ifdef MINI_STL_MEMBER_TEMPLATES
    cout << "has tt" << endl;
#else
    cout << "no tt" << endl;
#endif
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
#include "memory.h"
using Mini_STL::default_allocator;
class TT{};
template <class T,class Alloc=default_allocator>
class MYS
{
public:
    template <class ITER>
    void ii(int c,ITER A,ITER B)
    {
        cout<< "iter,b" << endl;
    }
};

int main()
{
    char A[10] = {1,2,3,4,5,6,7,8,9,10};
    MYS<int> mys;
    mys.ii(10,A,A);
    return 0;
}
#endif
