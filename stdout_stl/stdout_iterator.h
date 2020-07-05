// finished
#ifndef __STDOUT_ITERATOR_H_
#define __STDOUT_ITERATOR_H_

#include "stdout_config.h"
#include "stdout_iterator_base.h"

STDOUT_START

// 是一个迭代器配接器，
// 用来将某个迭代器赋值修改为插入操作——插在尾端（back_insert)
template<class _Container>
class back_insert_iterator {
    protected:
        _Container* container;
    public:
        typedef _Container          container_type;
        typedef output_iterator_tag iterator_category;
        typedef void                value_type;
        typedef void                difference_type;
        typedef void                pointer;
        typedef void                reference;

        explicit back_insert_iterator(_Container& __x) : container(&__x) {}
        back_insert_iterator<_Container>&
        operator=(const typename _Container::value_type& __value) {
            container->push_back(__value);
            return *this;
        }

        // 关闭下面三个操作符的功能
        back_insert_iterator<_Container>& operator*() { return *this; }
        back_insert_iterator<_Container>& operator++() { return *this; }
        back_insert_iterator<_Container>& operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

    template<class _Container>
    inline output_iterator_tag iterator_category(const back_insert_iterator<_Container>&) {
        return output_iterator_tag();
    }

#endif // #ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template<class _Container>
inline back_insert_iterator<_Container> back_inserter(_Container& __x) {
    return back_insert_iterator<_Container>(__x);
}

// 将赋值操作修改为插入——在前端(front_insert)
template<class _Container>
class front_insert_iterator {
    protected:
        _Container container;
    public:
        typedef _Container              container_type;
        typedef output_iterator_tag     iterator_category;
        typedef void                    value_type;
        typedef void                    difference_type;
        typedef void                    pointer;
        typedef void                    reference;

        explicit front_insert_iterator(_Container& __x) : container(&__x) {};
        front_insert_iterator<_Container>&
        operator=(const typename _Container::value_type& __value) {
            container->push_front(__value);
            return *this;
        }

        front_insert_iterator<_Container>& operator*() { return *this; }
        front_insert_iterator<_Container>& operator++() { return *this; }
        front_insert_iterator<_Container>& operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template<class _Container>
inline output_iterator_tag
iterator_category(const front_insert_iterator<_Container>&) {
    return output_iterator_tag();
}

#endif // __STL_CLASS_PARTIAL_SPECIALIZATION

template<class _Container>
inline front_insert_iterator<_Container> front_inserter(_Container& __x) {
    return front_insert_iterator<_Container>(__x);
}

// 赋值修改为插入操作——在特定的位置上进行
template<class _Container>
class insert_iterator {
    protected:
        _Container container;
        typename _Container::iterator   iter;
    public:
    typedef _Container              container_type;
    typedef output_iterator_tag     iterator_category;
    typedef void                    value_type;
    typedef void                    difference_type;
    typedef void                    pointer;
    typedef void                    reference;

    insert_iterator(_Container& __x, typename _Container::iterator __i) : container(&__x), iter(__i) {}
    insert_iterator<_Container>&
    operator=(const typename _Container::value_type& __value) {
        iter = container->insert(iter, __value);
        ++iter;
        return *this;
    }
    insert_iterator<_Container>& operator*() { return *this; }
    insert_iterator<_Container>& operator++() { return *this; }
    insert_iterator<_Container>& operator++(int) { return *this; }
};

#ifndef __STL_CLASS_PARTIAL_SPECIALIZATION

template<class _Container>
inline output_iterator_tag
iterator_category(const insert_iterator<_Container>&) {
    return output_iterator_tag();
}

#endif // __STL_CLASS_PARTIAL_SPECIALIZATION

template<class _Container, class _Iterator>
inline insert_iterator<_Container> inserter(_Container& __x, _Iterator __i) {
    typedef typename _Container::iterator   __iter;
    return insert_iterator<_Container>(__x, __iter(__i));
}

// 反向迭代器
template<class _Iterator>
class reverse_iterator {
    protected:
        _Iterator current; // 正向迭代器
    public:
        typedef typename iterator_traits<_Iterator>::iterator_category  iterator_category;
        typedef typename iterator_traits<_Iterator>::value_type         value_type;
        typedef typename iterator_traits<_Iterator>::difference_type    difference_type;
        typedef typename iterator_traits<_Iterator>::pointer            pointer;
        typedef typename iterator_traits<_Iterator>::reference          reference;

        typedef _Iterator iterator_type;
        typedef reverse_iterator<_Iterator> _Self;

    public:
        reverse_iterator() {}
        explicit reverse_iterator(iterator_type __x) : current(__x) {}

        reverse_iterator(const _Self& __x) : current(__x.current) {}

        #ifdef __STL_MEMBER_TEMPLATES
        template<class _Iter>
        reverse_iterator(const reverse_iterator<_Iter>& __x) : current(__x.base()) {}
        #endif // __STL_MEMBER_TEMPLATES
        
        iterator_type base() const { return current; }
        reference operator*() const {
            _Iterator __tmp = current;
            return *--__tmp;
        }

        #ifndef __SGI_STL_NO_ARROW_OPERATOR
        pointer operator->() const { return &(operator*()); }
        #endif // __SGI_STL_NO_ARROW_OPERATOR

        _Self& operator++() {
            --current;
            return *this;
        }
        _Self operator++(int) {
            _Self __tmp = *this;
            --current;
            return __tmp;
        }
        _Self& operator--() {
            ++current;
            return *this;
        }
        _Self operator--(int) {
            _Self __tmp = *this;
            ++current;
            return __tmp;
        }
        _Self operator+(difference_type __n) const {
            return _Self(current - __n);
        }
        _Self operator+=(difference_type __n) {
            current -= __n;
            return *this;
        }
        _Self operator-(difference_type __n) const {
            return _Self(current + __n);
        }
        _Self operator-=(difference_type __n) {
            current += __n;
            return *this;
        }
        reference operator[](difference_type __n) const { 
            return *(*this + __n);
        }
};

// 对比较符号的重载
template<class _Iterator>
inline bool operator==(
    const reverse_iterator<_Iterator>& __x,
    const reverse_iterator<_Iterator>& __y
) {
    return __x.base() == __y.base();
}

template<class _Iterator>
inline bool operator!=(
    const reverse_iterator<_Iterator>& __x,
    const reverse_iterator<_Iterator>& __y
) {
    return !(__x == __y);
}

template<class _Iterator>
inline bool operator<(
    const reverse_iterator<_Iterator>& __x,
    const reverse_iterator<_Iterator>& __y
) {
    return (__x.base() < __y.base());
}

template<class _Iterator>
inline bool operator>=(
    const reverse_iterator<_Iterator>& __x,
    const reverse_iterator<_Iterator>& __y
) {
    return !(__x < __y);
}

template<class _Iterator>
inline bool operator>(
    const reverse_iterator<_Iterator>& __x,
    const reverse_iterator<_Iterator>& __y
) {
    return __y < __x;
}

template<class _Iterator>
inline bool operator<=(
    const reverse_iterator<_Iterator>& __x,
    const reverse_iterator<_Iterator>& __y
) {
    return !(__y < __x);
} 



STDOUT_END

#endif // __STDOUT_ITERATOR_H_