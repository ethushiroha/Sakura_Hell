#ifndef _STDOUT_FUNCTION_H_
#define _STDOUT_FUNCTION_H_
/** 仿函数 行为类似于函数，理解是：对函数进行了抽象？
 *  仿函数主要是重载了() 的class，或者是template
 */

#include "stdout_config.h"

STDOUT_START

// 仿函数都必须继承下面两个模板中的一个
// 一元函数的仿函数
template<class _Arg, class _Result>
struct unary_function {
    typedef _Arg    argument_type;
    typedef _Result result_type;
};

// 二元函数的仿函数
template<class _Arg1, class _Arg2, class _Result>
struct binary_function {
    typedef _Arg1       first_argument_type;
    typedef _Arg2       second_argument_type;
    typedef _Result     result_type;
};

// 加
template<class _Tp>
struct plus : public binary_function<_Tp, _Tp, _Tp> {
    _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x + __y;
    }
};

// 减
template<class _Tp>
struct minus : binary_function<_Tp, _Tp, _Tp> {
    _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x - __y;
    }
};

// 乘
template<class _Tp>
struct multiplies : public binary_function<_Tp, _Tp, _Tp> {
    _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x * __y;
    }
};

// 除
template<class _Tp>
struct divides : public binary_function<_Tp, _Tp, _Tp> {
    _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x / __y;
    }
};

// 加法和乘法的 基元(群里面叫的那个啥忘了)
template<class _Tp>
inline _Tp identity_element(plus<_Tp>) {
    return _Tp(0);
}
template<class _Tp>
inline _Tp identity_element(multiplies<_Tp>) {
    return _Tp(1);
}

// mod
template<class _Tp>
struct modulus : public binary_function<_Tp, _Tp, _Tp> {
    _Tp operator()(const _Tp& __x, const _Tp& __y) const {
        return __x % __y;
    }
};
// -ans
template<class _Tp>
struct negate : public unary_function<_Tp, _Tp> {
    _Tp operator()(const _Tp& __x) const {
        return -__x;
    }
};
// ==
template<class _Tp>
struct equal_to : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x == __y;
    }
};
// !=
template<class _Tp>
struct not_equal_to : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x != __y;
    }
};
// >
template<class _Tp>
struct greater : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x > __y;
    }
};
// <
template<class _Tp>
struct less : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x < __y;
    }
};
// >=
template<class _Tp>
struct greater_equal : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x >= __y;
    }
};
// <=
template<class _Tp>
struct less_equal : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x <= __y;
    }
};
// and
template<class _Tp>
struct logical_and : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x && __y;
    }
};
// or 
template<class _Tp>
struct logical_or : public binary_function<_Tp, _Tp, bool> {
    bool operator()(const _Tp& __x, const _Tp& __y) const {
        return __x || __y;
    }
};
// not
template<class _Tp>
struct logical_not : public unary_function<_Tp, bool> {
    bool operator()(const _Tp __x) const {
        return !__x;
    }
};

// 适配器
template<class _Predicate>
class unary_negate : public unary_function<typename _Predicate::argument_type, bool> {
    protected:
        _Predicate _M_pred;
    public:
        explicit unary_negate(const _Predicate& __x) : _M_pred(__x) {}
        bool operator()(const typename _Predicate::argument_type& __x) const {
            return !_M_pred(__x);
        }
};

template<class _Pair>
struct _Select1st : public unary_function<_Pair, typename _Pair::first_type> {
    const typename _Pair::first_type& operator()(const _Pair& __x) const {
        return __x.first;
    }
};

template<class _Pair>
struct _Select2nd : public unary_function<_Pair, typename _Pair::second_type> {
    const typename _Pair::second_type& operator()(const _Pair& __x) const {
        return __x.second;
    }
};

template<class _Tp>
struct _Identity : public unary_function<_Tp, _Tp> {
    const _Tp& operator()(const _Tp& __x) const {
        return __x;
    }
};


STDOUT_END

#endif // _STDOUT_FUNCTION_H_