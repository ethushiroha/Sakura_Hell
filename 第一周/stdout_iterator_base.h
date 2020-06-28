// finished
#ifndef __STDOUT_ITERATOR_BASE_H_
#define __STDOUT_ITERATOR_BASE_H_

#include "stdout_config.h"
#include <stddef.h>

STDOUT_START

// 对应五种迭代器类型
// 和__true_type __false_type 一样，只是作为响应标志
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public output_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

// 只读的迭代器
// istream
template<class _Tp, class _Distance>
struct input_iterator {
    typedef input_iterator_tag  iterator_category;
    typedef _Tp                 value_type;
    typedef _Distance           difference_type;
    typedef _Tp*                pointer;
    typedef _Tp&                reference;
};

// 只写的迭代器
// ostream
struct output_iterator {
    typedef output_iterator_tag iterator_category;
    typedef void                value_type;
    typedef void                difference;
    typedef void                pointer;
    typedef void                reference;
};

// 允许写入算法 在迭代器形成的区间上写入
template<class _Tp, class _Distance>
struct forward_iterator {
    typedef forward_iterator_tag    iterator_category;
    typedef _Tp                     value_type;
    typedef _Distance               difference_type;
    typedef _Tp*                    pointer;
    typedef _Tp&                    reference;
};

// 可以双向移动的迭代器
template<class _Tp, class _Distance>
struct bidirectional_iterator {
    typedef bidirectional_iterator_tag  iterator_category;
    typedef _Tp                         value_type;
    typedef _Distance                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
};

// 上面的只实现了 ++ --
// 随机访问迭代器，实现了指针的所有计算方法
template<class _Tp, class _Distance>
struct random_access_iterator {
    typedef random_access_iterator_tag  iterator_category;
    typedef _Tp                         value_type;
    typedef _Distance                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
};

// 用户自己写的 iterator 继承自这个
template<class _Category, class _Tp, class _Distance = ptrdiff_t, class _Pointer = _Tp*, class _Reference = _Tp&>
struct iterator {
    typedef _Category                   iterator_category;
    typedef _Tp                         value_type;
    typedef _Distance                   difference_type;
    typedef _Pointer                    pointer;
    typedef _Reference                  reference;
};

// 萃取各种iterator 的特性
template<class _Iterator>
struct iterator_traits {
    typedef typename _Iterator::iterator_category   iterator_category;
    typedef typename _Iterator::value_type          value_type;
    typedef typename _Iterator::difference_type     difference_type;
    typedef typename _Iterator::pointer             pointer;
    typedef typename _Iterator::reference           reference;
};

// 对于指针的偏特化版本
template<class _Tp>
struct iterator_traits<_Tp*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef _Tp*                        pointer;
    typedef _Tp&                        reference;
};

// 对于const 指针的偏特化版本
template<class _Tp>
struct iterator_traits<const _Tp*> {
    typedef random_access_iterator_tag  iterator_category;
    typedef _Tp                         value_type;
    typedef ptrdiff_t                   difference_type;
    typedef const _Tp*                  pointer;
    typedef const _Tp&                  reference;        
};

// 用于萃取 category value_type difference_type 
template<class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
__iterator_category(const _Iter&) {
    typedef typename iterator_traits<_Iter>::iterator_category  _Category;
    return _Category();
}

template<class _Iter>
inline typename iterator_traits<_Iter>::value_type*
__value_type(const _Iter&) {
    return static_cast<typename iterator_traits<_Iter>::value_type*>(0);
}

template<class _Iter>
inline typename iterator_traits<_Iter>::difference_type*
__distance_type(const _Iter&) {
    return static_cast<typename iterator_traits<_Iter>::difference_type*>(0);
}

// 对上面三个函数封装
template<class _Iter>
inline typename iterator_traits<_Iter>::iterator_category
iterator_category(const _Iter& __i) {
    return __iterator_category(__i);
}

template<class _Iter>
inline typename iterator_traits<_Iter>::value_type*
value_type(const _Iter& __i) {
    return __value_type(__i);
}

template<class _Iter>
inline typename iterator_traits<_Iter>::difference_type*
distance_type(const _Iter& __i) {
    return __distance_type(__i);
} 

#define __ITERATOR_CATEGORY(__i)    __iterator_category(__i)
#define __DISTANCE_TYPE(__i)        __distance_type(__i)
#define __VALUE_TYPE(__i)           __value_type(__i)

/**
 * distance 函数
 */
// 逐一累计距离
template<class _InputIterator, class _Distance>
inline void __distance(_InputIterator __first, _InputIterator __last,
                        _Distance& __n, input_iterator_tag) {
    while (__first != __last) {
        ++__first;
        ++__n;
    }
}

// 直接计算距离
template<class _RandomAccessIterator, class _Distance>
inline void __distance(_RandomAccessIterator __first, _RandomAccessIterator __last,
                        _Distance& __n, random_access_iterator_tag) {
    // __STL_REQUESTS(_Random..., _Random...)
    __n += __last - __first;
}

// 包装
template<class _InputIterator, class _Distance>
inline void distance(_InputIterator __first, _InputIterator __last, _Distance& __n) {
    // __STL_REQUESTS(_Inp, _Inp)
    __distance(__first, __last, __n, iterator_category(__first));
}

#ifdef __STL_CLASS_PARTIAL_SPECIALIZATION

template<class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
__distance(_InputIterator __first, _InputIterator __last, input_iterator_tag) {
    typename iterator_traits<_InputIterator>::difference_type __n = 0;
    while (__first != __last) {
        ++__first;
        ++__n;
    }
    return __n;
}

template<class _RandomAccessIterator>
inline typename iterator_traits<_RandomAccessIterator>
__distance(_RandomAccessIterator __first, _RandomAccessIterator __last, random_access_iterator_tag) {
    return __last - __first;
}

template<class _InputIterator>
inline typename iterator_traits<_InputIterator>::difference_type
distance(_InputIterator __first, _InputIterator __last) {
    typedef typename iterator_traits<_InputIterator>::iterator_category _Category;
    return __distance(__first, __last, _Category());
}

#endif // __STL_CLASS_PARTIAL_SPECIALIZATION

// advance
template<class _InputIterator, class _Distance>
inline void __advance(_InputIterator& __i, _Distance __n, input_iterator_tag) {
    while(__n--) 
        ++__i;
}

template<class _BidirectionalIterator, class _Distance>
inline void __advance(_BidirectionalIterator& __i, _Distance __n, bidirectional_iterator_tag) {
    if (__n >= 0)
        while (__n--)
            ++__i;
    else 
        while (__n++)
            --__i;
}

template<class _RandomAccessIterator, class _Distance>
inline void __advance(_RandomAccessIterator& __i, _Distance __n, random_access_iterator_tag) {
    __i += __n;
}

template<class _InputIterator, class _Distance>
inline void advance(_InputIterator& __i, _Distance __n) {
    __advance(__i, __n, iterator_category(__i));
}



STDOUT_END

#endif // __STDOUT_ITERATOR_BASE_H_