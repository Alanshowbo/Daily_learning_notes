# Learning Python -- Python 学习手册笔记

## Chapter1 Q&A

## Chapter2 Python 如何运行程序

## Chapter3 如何运行程序

+ UNIX可执行脚本(#!)
  + 第一行特定，以#!开头，其后跟着机器Python解释器的路径
  + 一般都拥有可执行的权限：chmod +x file.py
  + 也可借助UNIX的env查找技巧避免硬编码Python解释器的路径
    ```python
    #!/usr/local/bin/python
    print('Hello World!')
    ```
    ```python
    #!/usr/bin/env python
    print('Hello World!')
    ```

+ 模块导入和重载
  + 每一个.py文件都是一个模块, python每次第一次导入模块都会运行一遍
  + 第二次导入不会再运行，可以使用reload运行：
    ```python
    import myModule  # 运行myModule内容
    import myModule  # 不运行myModule内容
    ```
    ```python
    import imp
    import myModule
    imp.reload(myModule)
    ```
  + 可使用内置函数dir()获得模块内部的可用变量名的列表
    ```python
    dir(tuple)  # 可用来查看内部变量名、方法名等
    ```
+ python每个模块是独立完备的变量包，即一个命名空间，from的导入方法某种程度上破坏了名称空间的分隔目的，但使用from导入一般也表示已明确想导入的问题

## Chapter4 python对象类型

### python核心数据类型

+ 数字、字符串、列表、字典、元组、文件、集合、其他类型(None、布尔等)、编程单元类型(函数、模块、类)、与实现相关的类型(编译的代码堆栈跟踪)
+ 序列：
  + 字符串、列表、元组
  + 序列操作：反向索引、切片、+、*
  + 序列的操作通用，类型的特定方法不通用
+ 不可变性：
  + 数字、字符串、元组，如字符串方法不改变原字符串
    ```bash
    >>> S = 'Hello'
    >>> S.replace('l','L')
    'HeLLo'
    >>> S
    'Hello'
    ```

+ shallow copy & deep copy, 主要体现在如子列表的拷贝上
+ 寻求帮助:
  + dir()
  + help()
+ 通用型操作和类型特定操作：通用性操作一般以内置函数或表达式的方式出现(len(s)、X[0]等), 类型特定的操作一般以方法调用的形式出现
+ 数字：
  + 不可变对象
  + python包含了一些较为少见的数字对象，如复数、固定精度的十进制数、有理数、集合和布尔值
+ 字符串：
  + 序列、不可变对象
  + 格式化操作
    ```python
    '{0} this is python {1}'.format('Hello', '!')  # 'Hello this is python !'
    ```
  + python中单引号和双引号相同, 三个引号中包含多行字符串常量, 所有行合并在一起, 并在行尾增加换行符
+ 列表：
  + 列表和字典的操作会对其直接进行改变（可变对象）
  + 列表解析(list comprehension expression)：
    + 通过对序列来运行一个表达式来获取新列表
        ```python
        M= [[1, 2, 3],
            [4, 5, 6],
            [7, 8, 9]]
        col2 = [s[1] for s in M]  # [2, 5, 8]
        col2_ = [s[1] for s in M if s[1]%2 == 0]  # [2, 8]
        diag = [M[i][i] for i in [0, 1, 2]]  # [1, 5, 9]
        ```
+ 字典：
  + 字典的键必须不可变，因此可用不变的对象充当，不能用list等
  + 字典排序
    ```python
    D = {'b': 1, 'a': 2, 'c': 3}
    sorted(D)  # ['a', 'b', 'c']
    D  # {'b': 1, 'a': 2, 'c': 3}
    ```
  + 使用in来检查键是否在字典中
  + 使用get方法避免获取不存在的字典键：
    ```python
    D = {'b': 1, 'a': 2, 'c': 3}
    value = D.get('x', 0)
    value  # 0
    ```
+ 元组：
  + 元组不可变的特点
  + 元组类型方法：count、index
+ 文件：
  + python中没有特定的常量语法创建文件，需使用open函数传递字符串形式的文件名和处理模式
  + 如今读取一个文件的最佳方式是不去读它，文件提供了一个迭代器，它在for循环或其它环境中自动一行一行地读
+ 其它类型：
  + 集合：{1, 2, 3}
  + 占位符对象None, 通常用来初始化名字和对象

## Chapter5 数字

## Chapter6 动态类型简介

## Chapter7 字符串
格式化操作
模式匹配

## Chapter8 列表和字典

列表解析