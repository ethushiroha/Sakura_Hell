#include <iostream>
#include <stdio.h>
#include "stdout_vector.h"

class student {
    public:
        char id;
        int age;
        student(char id, int age) {
            this->id = id;
            this->age = age;
        }
};


int main() {
    stdout_stl::vector<student*> v;
    for (int i = 0; i < 3; i++) {
        student *st = new student(i, i);
        v.push_back(st);
    } 

    stdout_stl::vector<student*>::iterator it = v.end() - 1;
    
    v.pop_back();
    

    std::cout << (*it)->age << std::endl; 


    return 0;
}