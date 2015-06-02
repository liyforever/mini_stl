#include <iostream>
#include "mini_stl_vector.h"
using namespace Mini_STL;
using std::cout;
using std::endl;


int main()
{
    vector<int> vv(5,1);
    vector<int> vv1(10,1);
    auto ii = vv.begin();
    cout << "iter1" << *ii << endl;
    auto ii2 = vv.erase(ii);
    cout << "iter2" << *ii2 << endl;
    cout << vv.size() << endl;
    cout << vv1.size() << endl;
    return 0;
}

