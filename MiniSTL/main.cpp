#include <iostream>
#include "mini_stl_vector.h"
using namespace Mini_STL;

int main()
{
    char i=5;
    vector<int> vv(i,1);
    std::cout << vv.size();
    return 0;
}

