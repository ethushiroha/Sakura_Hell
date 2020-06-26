// finished
#ifndef __STDOUT_UNINITIALIZED_H_
#define __STDOUT_UNINITIALIZED_H_

#include "stdout_config.h"
#include "stdout_type_traits.h"
#include "stdout_construct.h"

STDOUT_START

template<class _InputIterator, class _ForwardIterator>
inline _ForwardIterator __uninitialized_copy_aux(_InputIterator __first, _InputIterator __last, _ForwardIterator __result, __true_type) {
    return copy(__first, __last, __result);
}

template<class _InputIterator, class _ForwardIterator>
_ForwardIterator __uninitialized_copy_aux(_InputIterator __first, _InputIterator __last, _ForwardIterator __result, __false_type) {
    _ForwardIterator __cur = __result;
    try {
        for ( ; __first != __last; ++ __first, ++__cur) {
            _Construct(&*__cur, *__first);
        }
        return __cur;
    }
    catch(...) {
        _Destroy(__result, __cur); 
        throw;
    }
}

template<class _InputIterator, class _ForwardIterator, class _Tp>
inline _ForwardIterator __uninitialized_copy(_InputIterator __first, _InputIterator __last, _ForwardIterator __result, _Tp*) {
    typedef typename __type_traits<_Tp>::is_POD_type _Is_POD;
    return __uninitialized_copy_aux(__first, __last, __result, _Is_POD());
}

template<class _InputIterator, class _ForwardIterator>
inline _ForwardIterator uninitialized_copy(_InputIterator __first, _InputIterator __last, _ForwardIterator __result) {
    return __uninitialized_copy(__first, __last, __result, __VALUE_TYPE(__result));
}

// fill
template<class _ForwardIterator, class _Tp>
inline void __uninitialized_fill_aux(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x, __true_type) {
    fill(__first, __last, __x);
}

template<class _ForwardIterator, class _Tp>
void __uninitialized_fill_aux(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x, __false_type) {
    _ForwardIterator __cur = __first;
    try {
        for ( ; __cur != __last; ++__cur) {
            _Construct(&*__cur, __x);
        }
    }
    catch(...) {
        _Destroy(__first, __cur);
        throw;
    }
}

template<class _ForwardIterator, class _Tp, class _Tp1>
inline void __uninitialized_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x, _Tp1*)  {
    typedef typename __type_traits<_Tp1>::is_POD_type   _Is_POD;
    return __uninitialized_fill_aux(__first, __last, __x, _Is_POD());
}

template<class _ForwardIterator, class _Tp>
inline void uninitialized_fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __x) {
    __uninitialized_fill(__first, __last, __x, __VALUE_TYPE(__first));
}

template<class _ForwardIterator, class _Size, class _Tp>
inline _ForwardIterator __uninitialized_fill_n_aux(_ForwardIterator __first, _Size __n, const _Tp& __x, __true_type) {
    return fill_n(__first, __n, __x);
}

template<class _ForwardIterator, class _Size, class _Tp>
_ForwardIterator __uninitialized_fill_n_aux(_ForwardIterator __first, _Size __n, const _Tp& __x, __false_type) {
    _ForwardIterator __cur = __first;
    try {
        for ( ; __n > 0; --__n, ++__cur) {
            _Construct(&*__cur, __x);
        }
        return __cur;
    } catch(...) {
        _Destroy(__first, __cur);
    }
}

template<class _ForwarddIterator, class _Size, class _Tp, class _Tp1>
inline _ForwarddIterator __uninitialized_fill_n(_ForwarddIterator __first, _Size __n, const _Tp& __x, _Tp1*) {
    typedef typename __type_traits<_Tp1>::is_POD_type   _Is_POD;
    return __uninitialized_fill_n_aux(__first, __n, __x, _Is_POD());
}

template<class _ForwardIterator, class _Size, class _Tp>
inline _ForwardIterator uninitialized_fill_n(_ForwardIterator __first, _Size __n, const _Tp& __x) {
    return __uninitialized_fill_n(__first, __n, __x, __VALUE_TYPE(__first));
} 



STDOUT_END

#endif // __STDOUT_UNINITIALIZED_H_