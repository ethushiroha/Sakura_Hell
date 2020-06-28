#include "stdout_vector.h"
#include <iostream>
#include <stdio.h>
#include <vector>


int main() {
    stdout_stl::vector<int> v;
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.pop_back();
    

    stdout_stl::vector<int>::iterator it = v.begin();
    for ( ; it != v.end(); ++it) {
        std::cout << *it << std::endl;
    }
    

    return 0;
}