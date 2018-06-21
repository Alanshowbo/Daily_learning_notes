# C++ Primer Plus 第六版 学习笔记
> C++系统学习笔记，Created in 20180516
---

## Chapter 1 预备知识
+ 计算机语言要处理两个概念——数据和算法。C语言在最初面世时时过程性语言，即它强调的是编程的算法方面；
+ 与强调算法的过程性编程语言不同，OOP（面向对象编程）强调的是数据，其理念是设计与问题的本质特性相对应的数据格式；
+ 扩展名为o的目标代码. 生成目标代码后，编译器将目标代码传递给系统链接程序，该程序将代码与库代码结合，生成可执行文件；
+ 如果只有一个源文件，链接程序还将删除目标代码，若有多个源文件程序，则编译器不会删除目标代码文件；
+ 有时需显式指定一些库，如访问数学库中定义的函数，加上-lm标记
    ```bash
    CC usingmath.c -lm
    ```
+ 编译多个源文件，只需将它们全部放到命令行：
    ```
    g++ my_1.cpp my_2.cpp
    ```
---
## Chapter 2 开始学习C++
+ `int main()`: 该函数头描述的是main()和操作系统之间的接口，应避免使用`void main()`这样的格式；
+ C语言的传统是，头文件使用扩展名h，C++则不再这样，但对C的老式头文件保留了扩展；
+ 命名空间namespace：旨在使编写大型程序以及将多个厂商代码组合起来更容易（如处理同名函数的问题）；
+ `cout<<"Hello World!"<<endl;`中的`<<`表示将字符串发送给cout；
+ endl与\n换行符：\n是c语言符号；
+ `cout`与`printf`：`cout`比`printf`更“聪明”；
+ 类之于对象就像类型之于变量；
+ `#include <cmath>`
+ 在多函数程序中使用using编译指令，可将其放在函数外部（如前面）；
+ `cin`和`cout`为用于处理输入和输出的预定义对象，它们是istream和ostream类的实例；`cin`和`cout`是智能对象，可依据上下文自动将信息从一种形式转化为另一种形式。

---
## Chapter 3 处理数据
+ `&`运算符检索变量地址；
+ 以两个下划线或下划线与大写字母打头的名称被保留给实现；
+ 简单变量：
  + 整型：
    + C++的基本整型（按宽度递增的顺序排列）分别是char、short、int、long、C++11 新增的long long；
    + 计算机内存基本单元：位（bit）。不同的系统中，每种数据类型位宽度可能不同，C++标准规定：
      + short至少16位；
      + int至少和short一样长；
      + long至少32位，且至少与int一样从；
      + long long至少64位，且至少与long一样长。
    + 字节(byte)通常指8位的内存单元，区分于C++字节
    + 可用sizeof运算符返回类型或者变量的长度，在头文件climits(老式实现中为limits.h)中包含了整型限制的信息
    + climits中，`#define INT_MAX 32767`，该编译指令告诉预处理器，在程序中查找所有INT_MAX并将其替换为32767。`#define`编译指令是c语言留下的，C++有更好的创建符号常量的方法（使用关键字const）
    + 创建无符号版本的基本整型，使用`unsigned`修饰，如`unsigned short population`
    + 整型字面值：与C相同，C++能够以三种不同计数方式来书写整型：基数为10、基数为8、基数为16：
      + 若第一位为0 ~ 9，则基数为10；若第一位为0，第二位为1 ~ 7，则基数为8；若前两位为0x或0X，则基数为16
    + `cout<<hex;` 不会再屏幕上显示任何内容，二十修改cout显示整数的方式，如`cout<<waist;`, `cout<<inseam;`
    + C++ 确定常量类型：
      + 看后缀：U、L、LL、UL、ULL
      + 看长度：以能够储存该数的最小类型表示
    + char是另一种整型
    + **转义序列**，p50
    + 宽字符类型，Unicode，char16_t(16位)、char32_t(32位), 使用前缀u表示char16_t 常量，使用前缀U表示char16_t 常量
        ```cpp
        char16_t ch1 = u'q';
        char32_t ch2 = U'P';
        ```
    + const限定符：符号名称指明了常量表示的内容，若果在多个地方使用到一个常量，则需要修改此常量时，只要修改一个符号定义即可
        ```cpp
        const type name = value;
        ```
      在声明中应对const进行初始化，否则常量的值不确定，且无法修改
  + 浮点数：
    + 计算机将浮点数分两部分储存：一部分表示值，另一部分用于对值进行放大缩小，浮点因此得名；
    + C和C++对于浮点位数的要求：float至少32位，double至少48位，且不少于float；long double至少与double一样多。通常，float为32位，double为64位，long double为80、96或128位，另外这三种类型的指数范围至少是-37~37.
  + 类型转换：
    + 初始化和赋值进行的转换；
    + 以{}方式初始化时进行转换，常用于复杂的数据类型；
    + 传递参数时的转换；
    + 强制类型转换；
    + auto声明：c++ 11新增了auto，让编译器能够依据初始值的类型推断变量的类型。实用auto声明而不指定变量类型，编译器自动将变量类型设为与初始值相同；处理复杂类型时，自动推断比较有优势。

---
## Chapter 4 复合类型
### 4.1 数组：
+ 声明：`typeName arrayName[arraySize]`;声明中arraySize不能是变量，可以用new运算符来规避这种限制；
+ sizeof运算符用于返回类型或数据对象的长度，如果将sizeof用于数组名，得到的是整个数组的字节数；
+ *只有在数组定义时才能初始化，以后就不能使用了，也不能将一个数组赋给另一数组*，如果只对数组一部分初始化，则其它元素为0；如果初始化数组时[]中为空，编译器会自动计算元素个数；
### 4.2 字符串：
+ C++处理字符串方式有两种：来自C语言的C-Style string、基于string类库的方法；
+ C-style string：
  + C-style string具有特殊性质，以空字符结尾，空字符被写作`\0`，用来标记字符串的结尾
    ```cpp
    char dog[8]={'d','o','g',`e`};
    char cat[8]={'c','a','t','\0'};
    ```
    使用cout打印dog数组（不是字符串），打印完4个字符后，会接着将内存中随后的字节解释为要打印的字符，直至遇到空字符；
  + 用引号括起的字符串隐式地包含结尾的空字符，不必显式地包括它；
  + 字符串常量（双引号）不能与字符常量（单引号）互换
    ```cpp
    char shirt_size = 'S'; //correct
    char shirt_size = "S"; //error, "S"表示两个字符：'S'和'\0'
    ```
  + 拼接字符串：第一个字符串的\0将被第二个字符串的第一个字符取代；
  + strlen()可返回存储在数组中的字符串长度, sizeof()返回整个数组的长度；
  + 字符串输入
    + cin使用空白确定字符串的结束位置（空格、制表符、换行符），每次只能读取一个单词
    + 整行输入采用cin.getline()，getline使用换行符来确定行尾，存储字符串时使用空字符来替换换行符
### 4.3 string类
+ #include <string>
+ 可以使用数组表示法来访问存储在string对象中的字符
+ 确定字符串中的字符数：
  ```cpp
  int len1 = str1.size();
  int len2 = strlen(charr);
  ```
+ string IO:
  ```cpp
  string str;
  getline(cin,str); //此处的getline不是cin对象的方法
  ```
+ 其它形式的字符串字面值：
  + 使用前缀L、u、U表示wchar_t、char16_t、char32_t
  ```cpp
  wchar_t title[] = L"Chief";
  char16_t name[] = u"Felonia";
  char32_t car[] = "Audi";
  ```
  + 原始字符串（raw）:在原始字符串中，字符表示的就是自己，无需使用繁琐的转义字符：使用前缀R和"( )"
  ```cpp
  cout<<R"("This line" uses "\n" instead od endl)"<<endl; //输出"This line" uses "\n" instead od endl
  ```
### 4.4 结构简介
+ 定义：
  ```cpp
  struct mystruct
  {
    char name[20];
    float length;
    double price;
  };
  ```
+ 相比c语言，c++允许在声明结构变量时省略关键字struct
+ 使用.操作符访问结构成员
+ 可以同时完成结构体定义和结构体变量创建的工作：将变量名放在结束括号后.
+ 结构中的位字段：与c语言一样，c++也允许指定占用特定位数的结构成员，这使得创建与某个硬件设备上的寄存器对应的数据结构非常方便。还可以使用没有名称的字段来提供间距。
  ```cpp
  struct torgle_register
  {
    unsigned int SN : 4;
    unsigned int : 4;
    bool goodIn : 1;
    bool goodTorgle : 1;
  }
  ```
### 4.5 共用体
共用体（union）是一种数据格式，它能够存储不同的数据类型，但只能同时存储其中一种类型。当数据项使用两种或更多类型时，可节省空间。
  ```cpp
  union one4all
  {
    int int_val;
    long long_val;
    double double_val;
  };
  ```
### 4.6 枚举
c++的enum工具提供了另一种创建符号常量的方式，这种方式可替代const。
```cpp
enum spectrum {Red, orange, yellow, green, blue, violet, indigo, ultraviolet};
```
上面的语句完成两项工作：
+ 让spectrum成为新类型的名称；
+ 将red、yellow等作为符号常量，它们对应整数值0~7；这些量成为枚举量(enumerator)
```cpp
spectrum band;
band = blue;
```