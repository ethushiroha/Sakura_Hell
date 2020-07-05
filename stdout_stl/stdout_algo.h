#ifndef _STDOUT_ALGO_H_
#define _STDOUT_ALGO_H_

#include "stdout_config.h"
#include "stdout_algobase.h"

STDOUT_START

// 使用对分查找，找 第一个不小于 val元素的迭代器
template<class _ForwardIterator, class _Tp, class _Distance>
_ForwardIterator __lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val, _Distance*) {
    _Distance __len = 0;
    distance(__first, __last, __len);
    _Distance __half;
    _ForwardIterator __midlle;

    while (__len > 0) {
        __half = __len >> 1;
        __midlle = __first;
        advance(__midlle, __half);
        if (*__midlle < __val) {
            __first = __midlle;
            ++__first;
            __len = __len - __half - 1;
        } else {
            __len = __half;
        }
    }
    return __first;
}

template<class _ForwardIterator, class _Tp>
inline _ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val) {
    return __lower_bound(__first, __last, __val, __DISTANCE_TYPE(__first));
}

template<class _ForwardIterator, class _Tp, class _Compare, class _Distance>
_ForwardIterator __lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val, _Compare __comp, _Distance*) {
    _Distance __len = 0;
    distance(__first, __last, __len);
    _Distance __half;
    _ForwardIterator __middle;

    while (__len > 0) {
        __half = __len >> 1;
        __middle = __first;
        advance(__middle, __half);
        if (__comp(*__middle, __val)) {
            __first = __middle;
            ++__first;
            __len = __len - __half - 1;
        } else {
            __len = __half;
        }
    }
    return __first;
}

template<class _ForwardIterator, class _Tp, class _Compare>
inline _ForwardIterator lower_bound(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __val, _Compare __comp) {
    __lower_bound(__first, __last, __val, __comp, __DISTANCE_TYPE(__first));
}


STDOUT_END

#endif // _STDOUT_ALGO_H_