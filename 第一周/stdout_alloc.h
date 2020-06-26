// finished
#ifndef __STDOUT_ALLOC_H_
#define __STDOUT_ALLOC_H_

#include "stdout_config.h"
#include <cstddef>
#include <cstdlib>
#include <iostream>
#include "stdout_construct.h"

STDOUT_START

using std::cerr;
using std::endl;

// 第一级配置器
// 直接调用malloc
template<int __inst>
class __malloc_alloc_template {
    public:
        static void* allocate(size_t __size) {
            void* tmp = malloc(__size);
            if (tmp == 0) {
                cerr << "_allocate failed" << endl;
                exit(-1); 
            }
            return tmp;
        }

        static void deallocate(void* p, size_t ) {
            if (p == nullptr) {
                return;
            } else {
                free(p);
            }
        }

        static void* reallocate(void *__p, size_t /* old */, size_t __size) {
            static void* tmp = realloc(__p, __size);
            if (tmp == 0) {
                cerr << "realloc error" << endl;
                exit(-1);
            } else {
                return tmp;
            }
        }
};



typedef __malloc_alloc_template<0> malloc_alloc;

// 对 _malloc_alloc 进行包装
template<class _Tp, class _Alloc>
class simple_alloc {
    public:
        static _Tp* allocate(size_t __n) {
            return __n == 0 ? nullptr : (_Tp*)_Alloc::allocate(__n * sizeof(_Tp));
        }

        static _Tp* allocate(void) {
            return (_Tp*)_Alloc::allocate(sizeof(_Tp));
        }

        static void deallocate(_Tp* __p, size_t __n) {
            if (__n == 0) {
                return ;
            } else {
                _Alloc::deallocate(__p, __n * sizeof(_Tp));
            }
        }

        static void deallocate(_Tp* __p) {
            _Alloc::deallocate(__p, sizeof(_Tp));
        }
};


typedef malloc_alloc alloc;



STDOUT_END


#endif // __STDOUT_ALLOC_H_