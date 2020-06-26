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
-   距离计算函数（distance）
-   前进函数（advance）



## stdout_iterator.h

### 三个迭代器配接器

将迭代器的的赋值操作修改为对应的操作



### 反向迭代器

-   内包含一个`current`指向正向迭代器
-   反向迭代通过`current`的相应操作来实现



### 重载运算

通过比较 `base()` （返回了`current`对象）来比较迭代器对象



## stdout_uninitialized.h



### copy

将`alloc`和`construct`分开来

-   配置内存
-   使用`uninitialized_copy`构造对象



### fill

与`copy`操作差不多，只是把填充物变成了一个固定的对象？



### fill_n

填充`[first, first + n]`的地方



## stdout_algobase.h



### copy_backward

 从后往前拷贝元素，如果输入区间和输出区间重合，则会产生覆盖



### copy

从前往后拷贝元素













