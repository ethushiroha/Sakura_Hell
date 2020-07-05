// finished
#ifndef __STDOUT_ALGOBASE_H_
#define __STDOUT_ALGOBASE_H_

#include "stdout_config.h"
#include "stdout_iterator_base.h"
#include "stdout_iterator.h"
#include "stdout_type_traits.h"
#include <string.h>

STDOUT_START

// copy_backward
// 从后往前拷贝内容
template<class _BidrectionalIterator1, class _BidrectionalIterator2, class _Distance>
inline _BidrectionalIterator2 __copy_backward(
    _BidrectionalIterator1 __first,
    _BidrectionalIterator1 __last,
    _BidrectionalIterator2 __result,
    bidirectional_iterator_tag,
    _Distance*
) {
    while (__first != __last) 
        *--__result = *--__last;
    return __result;
}

template<class _RandomAccessIterator, class _BidrectionalIterator, class _Distance> 
inline _BidrectionalIterator __copy_backward(
    _RandomAccessIterator __first,
    _RandomAccessIterator __last,
    _BidrectionalIterator __result,
    random_access_iterator_tag,
    _Distance*
) {
    for (_Distance __n = __last - __first; __n > 0; --__n) 
        *--__result = *--__last;
    return __result;
}


template<class _BidrectionalIterator1, class _BidrectionalIterator2, class _BoolType>
struct __copy_backward_dispatch {
    typedef typename iterator_traits<_BidrectionalIterator1>::iterator_category  _Cat;
    typedef typename iterator_traits<_BidrectionalIterator1>::difference_type  _Distance;

    static _BidrectionalIterator2 copy(_BidrectionalIterator1 __first, _BidrectionalIterator1 __last, _BidrectionalIterator2 __result) {
        return __copy_backward(__first, __last, __result, _Cat(), (_Distance*) 0);
    }
};

template<class _Tp>
struct __copy_backward_dispatch<_Tp*, _Tp *, __true_type> {
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        const ptrdiff_t _Num = __last - __first;
        memmove(__result - _Num, __first, sizeof(_Tp) * _Num);
        return __result - _Num;
    }
};

template<class _Tp>
struct __copy_backward_dispatch<const _Tp*, _Tp*, __true_type> {
    static _Tp* copy(const _Tp* __first, const _Tp* __last, _Tp* __result) {
        return __copy_backward_dispatch<_Tp*, _Tp*, __true_type>::copy(__first, __last, __result);
    }
};

template<class _BI1, class _BI2>
inline _BI2 copy_backward(_BI1 __first, _BI1 __last, _BI2 __result) {
    typedef typename __type_traits<typename iterator_traits<_BI2>::value_type>::has_trivial_assignment_operator _Trivial;
    return __copy_backward_dispatch<_BI1, _BI2, _Trivial>::copy(__first, __last, __result);
}

// copy
template<class _InputIterator, class _OutputIterator, class _Tp>
inline _OutputIterator __copy_aux(
    _InputIterator __first,
    _InputIterator __last,
    _OutputIterator __result,
    _Tp*
) {
    typedef typename __type_traits<_Tp>::has_trivial_assignment_operator _Trivial;
    return __copy_aux2(__first, __last, __result, _Trivial());
}

template<class _InputIterator, class _OutputIterator>
inline _OutputIterator __copy_aux2(
    _InputIterator __first,
    _InputIterator __last,
    _OutputIterator __result,
    __false_type
) {
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

template<class _InputIterator, class _OutputIterator>
inline _OutputIterator __copy_aux2(
    _InputIterator __first,
    _InputIterator __last,
    _OutputIterator __result,
    __true_type
) {
    return __copy(__first, __last, __result, __ITERATOR_CATEGORY(__first), __DISTANCE_TYPE(__first));
}

template<class _Tp>
inline _Tp* __copy_aux2(
    const _Tp* __first, 
    const _Tp* __last, 
    _Tp* __result, 
    __true_type) {
    return __copy_trivial(__first, __last, __result);
}

template<class _InputIterator, class _OutputIterator, class _Distance>
inline _OutputIterator __copy(
    _InputIterator __first,
    _InputIterator __last,
    _OutputIterator __result,
    input_iterator_tag,
    _Distance*
) {
    for ( ; __first != __last; ++__result, ++__first) {
        *__result = *__first;
    }
    return __result;
}

template <class _RandomAccessIter, class _OutputIter, class _Distance>
inline _OutputIter
__copy(_RandomAccessIter __first, _RandomAccessIter __last,
       _OutputIter __result, random_access_iterator_tag, _Distance*)
{
  for (_Distance __n = __last - __first; __n > 0; --__n) {
    *__result = *__first;
    ++__first;
    ++__result;
  }
  return __result;
}

template<class _InputIterator, class _OutputIterator>
inline _OutputIterator copy(
    _InputIterator __first,
    _InputIterator __last,
    _OutputIterator __result
) {
    return __copy_aux(__first, __last, __result, __VALUE_TYPE(__first));
}

// fill
// [__first, __last) 内的所有元素填充
template<class _ForwardIterator, class _Tp>
void fill(_ForwardIterator __first, _ForwardIterator __last, const _Tp& __value) {
    for ( ; __first != __last; ++__first)
        *__first = __value;
}

// fill_n
template<class _OutputIterator, class _Size, class _Tp>
_OutputIterator fill_n(_OutputIterator __first, _Size __n, const _Tp& __value) {
    for ( ; __n > 0; --__n, ++__first)
        *__first = __value;
    return __first;
}

// max
template<class _Tp>
inline const _Tp& max(const _Tp& __a, const _Tp& __b) {
    return __a < __b ? __b : __a;
}

template<class _Tp, class _Compare> 
inline const _Tp& max(const _Tp& __a, const _Tp& __b, _Compare __compare) {
    return __compare(__a, __b) ? __b : __a;
}

// min
template<class _Tp>
inline const _Tp& min(const _Tp& __a, const _Tp& __b) {
    return __b < __a ? __b : __a;
}

template<class _Tp, class _Compare>
inline const _Tp& min(const _Tp& __a, const _Tp& __b, _Compare __compare) {
    return __compare(__b, __a) ? __b : __a;
}

// __iter_swap
template<class _ForwardIterator1, class _ForwardIterator2, class _Tp>
inline void __iter_swap(_ForwardIterator1 __a, _ForwardIterator2 __b, _Tp*) {
    _Tp __tmp = *__a;
    *__a = *__b;
    *__b = __tmp;
}

template<class _ForwardIteratoe1, class _ForwardIterator2, class _Tp>
inline void __iter_swap(_ForwardIteratoe1 __a, _ForwardIterator2 __b) {
    __iter_swap(__a, __b, __VALUE_TYPE(__a));
}

// swap
template<class _Tp>
inline void swap(_Tp& __a, _Tp& __b) {
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}

// equal
template<class _InputIterator1, class _InputIterator2>
inline bool equal(
    _InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2
) {
    for ( ; __first1 != __last1; ++__first1, ++__first2) {
        if (*__first1 != *__first2) {
            return false;
        }
    }
    return true;
}

template<class _InputIterator1, class _InputIterator2, class _BinaryPredicate>
inline bool equal(
    _InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2,
    _BinaryPredicate __binary_pred
) {
    for ( ; __first1 != __last1; ++__first1, ++__first2) {
        if (!__binary_pred(*__first1, *__first2)) {
            return false;
        }
    }
    return true;
}

// lexicographical_compare
// 字典排列方式对两个序列比较
template<class _InputIterator1, class _InputIterator2>
bool lexicographical_compare(
    _InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2,
    _InputIterator2 __last2
) {
    for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
        if (*__first1 < *__first2) {
            return true;
        } else if (*__first2 < *__first1) {
            return false;
        }
    } 
    // 长度不一样的时候，短的比较小
    return __first1 == __last1 && __first2 != __last2;
}

// 提供一个自己比较的版本
template<class _InputIterator1, class _InputIterator2, class _Compare>
bool lexicographical_compare(
    _InputIterator1 __first1,
    _InputIterator1 __last1,
    _InputIterator2 __first2,
    _InputIterator2 __last2,
    _Compare        __compare
) {
    for ( ; __first1 != __last1 && __first2 != __last2; ++__first1, ++__first2) {
        if (__compare(*__first1, *__first2)) {
            return true;
        }else if (__compare(*__first2, *__first1)) {
            return false;
        }
    }
    return __first1 == __last1 && __first2 != __last2;
}

// 特化版本
// 用于加快速度, 使用memcmp
inline bool lexicographical_compare(
    const unsigned char* __first1,
    const unsigned char* __last1,
    const unsigned char* __first2,
    const unsigned char* __last2
) {
    const size_t __len1 = __last1 - __first1;
    const size_t __len2 = __last2 - __first2;
    const int __result = memcmp(__first1, __first2, min(__len1, __len2));
    return __result != 0 ? __result < 0 : __len1 < __len2;
}


STDOUT_END

#endif // __STDOUT_ALGOBASE_H_