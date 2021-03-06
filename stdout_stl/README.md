# 抄stl （顺带一点理解？



## stdout_alloc.h

内存配置器

### \_\_malloc\_alloc\_template

第一级内存配置器，单纯的用`malloc` `free` `realloc` 来实现

### simple_alloc

对 `__malloc_alloc_template` 的一个包装



## stdout_construct.h

构造和析构对象

（目前只有基础的 `_Construct` `_Distory` 函数， 萃取库还没抄）



## stdout_type_traits.h

利用萃取机对对象属性的萃取

初始化的`__false_type` 只是取一个保守值，对其他的值具像化

使用两个结构体`__true_type` 和 `__false_type` 来响应回答，萃取对象的属性

目的是为了效率的提高

### 萃取

1.  `has_trivial_default_constructor` —— 是否使用默认构造函数
2.  `has_trivial_copy_constructor` —— 是否使用默认拷贝构造函数
3.  `has_trivial_assignment_operator` —— 是否使用默认赋值运算符
4.  `has_trivial_destructor` —— 是否使用默认析构函数
5.  `is_POD_type` —— 是否是`POD`类型 返回的是`__true_type`或`__false_type`结构



## stdout_iterator_base.h

-   迭代器萃取机

    ```c
    emplate<class _Iterator>
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
    ```

-   距离计算函数（distance）

-   前进函数（advance）

-   五种迭代器的tag

    ```c
    struct input_iterator_tag {};
    struct output_iterator_tag {};
    struct forward_iterator_tag : public input_iterator_tag {};
    struct bidirectional_iterator_tag : public forward_iterator_tag {};
    struct random_access_iterator_tag : public bidirectional_iterator_tag {};
    ```

## stdout_iterator.h

### 三个迭代器配接器

将迭代器的的赋值操作修改为对应的操作



### 反向迭代器

-   内包含一个`current`指向正向迭代器
-   反向迭代通过`current`的相应操作来实现



### 重载运算

通过比较 `base()` （返回了`current`对象）来比较迭代器对象



## stdout_uninitialized.h

将内存分配和对象构造分开来

### uninitialized_copy

在`[first, last)`区间内拷贝对象



### uninitialized_fill

与`copy`操作差不多，只是把填充物变成了一个固定的对象？



### uninitialized_fill_n

填充`[first, first + n]`的地方



## stdout_algobase.h



### copy_backward

 从后往前拷贝元素，如果输入区间和输出区间重合，则会产生覆盖



### copy

从前往后拷贝元素，使用`memmove`实现



### fill

[__first, __last) 内的所有元素填充



### fill_n

填充n个



### max min

比较大小

并且提供了一个可以使用自己的判别函数的模版



### swap

交换



### equal

比较是否相等，同样提供了一个自己的判别函数的模板



### lexicographical_compare

使用字典序号来比较，

遍历比较

长度不一样，则长度短的小

也提供了自己的判别函数的模板



另外提供了一个`const unsigned char*`的版本，用于提高速度吧



## stdout_vector.h

`vector` 继承自`_Vector_base`类，迭代器是一个`RandomAccessIterator`

同时安放了一个顺序迭代器和一个反向迭代器（用来支持rbegin，rend）

### 成员变量

-   _M_start：用来标志vector容器目前使用的空间的头部
-   _M_finish：用来标志已使用的尾部
-   _M_end_of_storage：标志可使用空间的尾部



### 函数

-   begin: 返回_M_start;
-   end: 返回_M_finish;
-   size: 容器的大小，begin() - end()
-   empty: begin() == end()
-   其余的先省略了



### 可能存在的问题

-   `pop_back`函数中，只是将对象析构，并且`_M_finish`指针向前移动，对象的地址仍然存在于`vector`中，会造成**`UAF`**

-   `insert`函数中，没有检查`__pos`是否在`_M_start`到`_M_finish`之间，即存在**`下标溢出`**，通过这个可以修改某个地址

    ![DB326CEB4E7AC46B706074798EF19512](README.assets/DB326CEB4E7AC46B706074798EF19512.jpg)



### poc

```c
#include "stdout_vector.h"
#include <iostream>
#include <stdio.h>
// 在自带的stl 中也存在这个问题，结果是一样的
// （因为我是抄的嘛）
// #include <vector>

int main() {
    stdout_stl::vector<int> v;
    // 使_M_end_of_storage != _M_finish
    // 让insert函数调用_M_insert_aux
    // _M_insert_aux 也没有检查下标，*__position = _Tp()
    for (int i = 0; i < 5; i++) {
        v.push_back(i);
    } 
    stdout_stl::vector<int>::iterator it = v.begin();
    
    // 等价于 it -= 0x10;
    it += 0xfffffffffffffffe;
    std::cout << *v.begin() << std::endl;
    std::cout << (it == v.end()) << std::endl;
    std::cout << "size: " << v.size() << std::endl;
    
    // 使其在错误的地方构造
    v.insert(it, 0x80);
    
    // 析构的时候就会报错
    // 因为堆块的头已经被改变成0x80
    v1.~vector();
    
    return 0;
}
```



```c
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
    std::vector<student*>::iterator it = v.end() - 1;
    
    // 弹出元素
    v.pop_back();

    // 仍然可以使用
    std::cout << (*it)->age << std::endl; 


    return 0;
}
```

