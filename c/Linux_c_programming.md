# Linux C编程一站式学习
> http://akaedu.github.io/book/

## Part1 C语言入门
## 1.1 程序的基本概念
+ 解释型语言, 编译型语言
+ 自然语言和形式语言
+ 程序的调试:编译时错误, 运行时错误, 逻辑错误和语义错误
+ 第一个程序
  ```
  $ gcc main.c
  $ gcc main.c -o main
  $ ./main
  $ gcc -Wall main.c
  ```
## 1.2 常量、变量和表达式
+ C标准规定的转义字符:

| 转义字符  | 含义    |
|:----:|:----:|
|  \'	|单引号'（Single Quote或Apostrophe）|
|  \"	|双引号"|
|  \?	|问号?（Question Mark）|
| \\	|反斜线\（Backslash）|
|  \a	|响铃（Alert或Bell）|
|  \b	|退格（Backspace）|
|  \f	|分页符（Form Feed）|
|  \n	|换行（Line Feed）|
|  \r	|回车（Carriage Return）|
|  \t	|水平制表符（Horizontal Tab）|
|  \v	|垂直制表符（Vertical Tab）|
> C语言规定了几个控制字符，不能用键盘直接输入，因此采用\加字母的转义序列表示。\a是响铃字符，在字符终端下显示这个字符的效果是PC喇叭发出嘀的一声，在图形界面终端下的效果取决于终端的实现。在终端下显示\b和按下退格键的效果相同。\f是分页符，主要用于控制打印机在打印源代码时提前分页，这样可以避免一个函数跨两页打印。\n和\r分别表示Line Feed和Carriage Return，这两个词来自老式的英文打字机，Line Feed是跳到下一行（进纸，喂纸，有个喂的动作所以是feed），Carriage Return是回到本行开头（Carriage是卷着纸的轴，随着打字慢慢左移，打完一行就一下子移回最右边），如果你看过欧美的老电影应该能想起来这是什么。用老式打字机打完一行之后需要这么两个动作，\r\n，所以现在Windows上的文本文件用\r\n做行分隔符，许多应用层网络协议（如HTTP）也用\r\n做行分隔符，而Linux和各种UNIX上的文本文件只用\n做行分隔符。在终端下显示\t和按下Tab键的效果相同，用于在终端下定位表格的下一列，\v用于在终端下定位表格的下一行。\v比较少用，\t比较常用，以后将“水平制表符”简称为“制表符”或Tab。请读者用printf语句试试这几个控制字符的作用。
+ 常量:常量（Constant）是程序中最基本的元素，有字符（Character）常量、整数（Integer）常量、浮点数（Floating Point）常量和枚举常量。
```c
printf("character: %c\ninteger: %d\nfloating point: %f\n", '}', 34, 3.14);
```
printf中的第一个字符串称为格式化字符串（Format String），它规定了后面几个常量以何种格式插入到这个字符串中，在格式化字符串中%号（Percent Sign）后面加上字母c、d、f分别表示字符型、整型和浮点型的转换说明（Conversion Specification），转换说明只在格式化字符串中占个位置，并不出现在最终的打印结果中，这种用法通常叫做占位符（Placeholder）。这也是一种字面意思与真实意思不同的情况，但是转换说明和转义序列又有区别：转义序列是编译时处理的，而转换说明是在运行时调用printf函数处理的。源文件中的字符串字面值是"character: %c\ninteger: %d\nfloating point: %f\n"，\n占两个字符，而编译之后保存在可执行文件中的字符串是character： %c换行integer: %d换行floating point: %f换行，\n已经被替换成一个换行符，而%c不变，然后在运行时这个字符串被传给printf，printf再把其中的%c、%d、%f解释成转换说明。
+ 运算符和表达式:
  + 等号运算符还有一个和+ - * /不同的特性，如果一个表达式中出现多个等号，不是从左到右计算而是从右到左计算;
  + 在C语言中整数除法总是把小数部分截掉，在数轴上向零的方向取整.


