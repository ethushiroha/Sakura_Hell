#ifndef _STDOUT_FUNCTION_H_
#define _STDOUT_FUNCTION_H_
/** 仿函数 行为类似于函数，理解是：对函数进行了抽象？
 *  仿函数主要是重载了() 的class，或者是template
 */



#include "stdout_config.h"

STDOUT_START

// 一元函数的仿函数
template<class _Arg, class _Result>
struct unary_function {
    typedef _Arg    argument_type;
    typedef _Result result_type;
};

// 二元函数的仿函数



STDOUT_END

#endif // _STDOUT_FUNCTION_H_