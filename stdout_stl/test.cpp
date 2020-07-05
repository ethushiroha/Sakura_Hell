#include <iostream>
#include <stdio.h>
#include "stdout_vector.h"
#include "stdout_list.h"
#include "stdout_function.h"

void push_back_flag(stdout_stl::vector<char>& v) {
    char a[] = "afctf{0xDktb_tql}";
    stdout_stl::vector<char> s(&a[0], &a[17]);
    v = s;
}

void push_key(stdout_stl::list<char>& l) {
    l.push_back('0');
    l.push_back('x');
    l.push_back('D');
    l.push_back('k');
    l.push_back('t');
    l.push_back('b');
}

stdout_stl::vector<unsigned int> encode(stdout_stl::vector<char>& v, stdout_stl::list<char>& l) {
    int v_size = v.size(); // 18
    int l_size = l.size(); // 6
    stdout_stl::vector<unsigned int> ans;
    stdout_stl::vector<char>::iterator v_it = v.begin();
    for (int i = 0; i < (v.size() / l.size()); ++i) {
        stdout_stl::list<char>::iterator l_it = l.begin();
        for ( ; l_it != l.end(); ++l_it) {
            stdout_stl::plus<char> pl;
            int c  = pl(*l_it, *v_it);
            ans.push_back(c);
            ++v_it;
        }
    }
    return ans;
}

int main() {
    stdout_stl::vector<char> v;
    push_back_flag(v);
    stdout_stl::list<char> l;
    push_key(l);
    stdout_stl::vector<unsigned int> ans = encode(v, l);
    for (stdout_stl::vector<unsigned int>::iterator it = ans.begin(); it != ans.end(); ++it) {
        std::cout << *it << std::endl;
    }

}