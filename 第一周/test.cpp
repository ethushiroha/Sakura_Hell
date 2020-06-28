#include <iostream>
#include <stdio.h>
#include "stdout_vector.h"
#include <vector>

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
    std::vector<student*> v;
    for (int i = 0; i < 3; i++) {
        student *st = new student(i, i);
        v.push_back(st);
    } 
    std::vector<student*>::iterator it = v.end();
    v.erase(it);

    std::cout << (*it)->age << std::endl; 


    return 0;
}