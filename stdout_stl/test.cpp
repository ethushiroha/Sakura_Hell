#include <iostream>
#include <stdio.h>
#include "stdout_vector.h"
#include "stdout_list.h"

int main() {
    stdout_stl::list<int> l;
    l.push_back(1);
    l.push_front(2);
    std::cout << l.size() << std::endl;
    stdout_stl::list<int>::iterator it = l.begin();
    for ( ; it != l.end(); ++it)
        std::cout << *it << std::endl;
    return 0;
}