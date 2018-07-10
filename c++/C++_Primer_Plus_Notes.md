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
    ```bash
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

+ `#include <string>`
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

### 4.7 指针和自由存储空间

+ 指针是一个变量，其存储的是值的地址，而不是值本身，对常规变量使用&运算符获取其地址；
+ *运算符，被称为间接值，将其应用于指针，可得到该地址处存储的值；
+ 指针声明：
    ```cpp
    int higgens = 5;
    int * pt = &higgens;
    ```
+ 指针的危险：c++创建指针时，会分配用来储存地址的内存，但不会分配指针所指向的数据的内存；
    ```cpp
    long * fellow;
    *fellow = 22333; //错误
    ```
+ 使用new在程序运行时分配内存：
    ```cpp
    int * pt = new int;
    *pt = 1001;
    ``` 
+ delete释放内存：
  + 使用delete时，后面加上指向内存块的指针（这些内存块最初是用new分配的）
    ```cpp
    int * pt = new int;
    ...
    delete pt;//释放pt指向的内存，不删除pt本身
    ```
  + new和delete一定要配合使用
  + 不要尝试释放已释放的内存块
+ 使用new创建动态数组：通常对于大型数据使用new，是new的用武之地
  + 假设程序是否需要用到数组取决于其运行时用户提供的信息，若通过声明创建数组，则不管是否用到数组都占用了内存，在编译时给数组分配内存成为静态联编
  + 使用new时，在程序运行时有需要才创建数组，且可以在程序运行时选择数组长度，这类称之为动态联编，这类数组称为动态数组
  + 静态联编必须在声明时指定数组长度，动态联编则在程序运行时确定数组长度
  + 使用new创建动态数组：
    ```cpp
    int * psome = new int[10];//new运算符返回第一个元素地址，该地址被赋予指针psome
    ...
    delete [] psome;//使用new[]分配内存，也必须用delete[]来释放
    ```
    ```cpp
    int size;
    cin>>size;
    int * pt1 = new int [size];
    ...
    delete [] pt1;
    ```
  + 无法使用sizeof来确定动态数组的字节数
  + 数组和指针基本等价：通过指针来访问动态数组，如`psome[0]`来访问上述数组第一个元素，ar[i]被解释为`*(ar+i)`,ar为数组第一个元素地址。
+ 指针算数：将指针增加1，增加的量等于其指向的类型的字节数
+ 指针和字符串：如果给cout提供一个指针，将打印地址，但如果指针类型是char *，将打印指向的字符串，必须使用强制类型转换才能显示地址
+ 使用new创建动态结构：创建一个未命名的inflatable类型，将其地址赋予一个指针：
  ```cpp
  inflatable *pt = new inflatable；
  ```
  比较麻烦的是访问动态结构的成员，不能将.运算符作用于动态结构名，c++使用->运算符访问成员，如`pt->price`是被指向结构的price成员
+ **自动存储、静态存储、动态存储**
+ 数组的替代品：
  + 动态数组模板类Vector：
    ```cpp
    vector<typeNme> vt(n_elem);
    ```
    ```cpp
    #include <vector>
    using namespace std;
    vector<int> vi;  //int空数组
    int n;
    cin>>n;
    vector<double> vd(n);  //double数组
    ```
    Vector自动完成new、delete操作，效率稍低；
  + 模板类array：
    array长度固定，但相比数组更加安全
    ```cpp
    array<typeNmae, n_elem> arr;
    ```
    ```cpp
    #include <array>
    array<int, 5> arr1;
    array<double, 3> arr2 = {1.0, 2.0, 3.0};
    ```
---

## Chapter 5 循环和关系表达式

### 5.1 for循环

+ for循环访问字符串：
    ```cpp
    string s1;
    for(int i=0; i<s1.size(); i++)
    {
      cout<<s1[i];
    }
    cout<<endl;
    ```

+ 省略for循环中的测试表达式时，测试结果为true，程序会一直运行：

  ```cpp
  for( ; ; )
  {
    cout<<"Hello World!";
  }
  ```

+ 递增运算符(++)和递减运算符(--)
  + 前缀和后缀对操作数的影响是一样的，但影响时间不同；
  + a++意味着使用a当前的值计算表达式，然后将a的值加1；++b意味着先将b的值加1，然后使用新的值计算表达式；
  + 副作用和顺序点：
    + 副作用(side effect)是指在计算表达式时对某些东西进行了修改；
    + 顺序点是程序中的一个点，在这里，进入下一步之前将确保对所有副作用进行了评估；
    + 语句中的分号就是一个顺序点，这意味着在执行下一步程序前，赋值运算法、递增、递减运算符执行的所有修改必须完成
    + 任何完整表达式（不是另一个更大表达式的子表达式）的末尾都是一个顺序点
      ```cpp
      while(guests++<10)
          cout<<guests<<endl;//程序输出结果为11，递增运算符副作用在执行cout之前完成
      ```
  + 递增/递减运算符和指针
    + 前缀递增、递减和解除引用运算符的优先级相同，以从右到左的方式结合
    + 后缀递增、递减运算符优先级相同，但比前缀运算符优先级高
      ```cpp
      double arr[5] = {21.1, 32.8, 23.4, 45.2, 37.4};
      double *pt = arr; //pt指向arr[0]
      ++pt;//pt指向arr[1]
      double x = *++pt; //pt指向arr[2]，x为23.4
      ++*pt; //pt仍指向arr[2]，先取得pt指向的值，然后增加至24.4
      (*pt)++; //先对指针解除引用，再加1得到25.4，pt仍指向arr[2]
      x = *pt++; //后缀运算符优先级更高，故++作用于pt，而不是*pt，后缀运算符意味着x的值为arr[2]，即25.4，该语句执行完毕后，pt指向arr[3]
      ```
+ 逗号表达式允许将两个表达式方法c++只允许放一个表达式的地方，如`for(j=0, i=word.size()-1; j<i; --i, ++j)`
+ 逗号表达式的值是第二部分的值，如`i=20, j = 2*i;`的值为40
+ C-风格字符串比较：word为数组名，要比较数组中的字符串是否为mate：

  ```cpp
  word=="mate";//错误
  ```

  **数组名是数组的地址**，应使用C-风格字符串库中的strcmp()来比较，该函数接受两个字符串地址作为参数，即参数可以是指针、字符串常量或者字符串数组名。当两字符串相同，strcmp()返回0，否则依据字母顺序"按系统排序"
+ 相比C-风格字符串，string类字符串的比较更加简介，因为类函数重载了比较运算符

### 5.2 while循环

+ 逐字符遍历字符串直至遇到空字符，是c++处理C-风格字符串的标准方法：

  ```cpp
  int Arsize = 20;
  char name[Arsize];
  cin>>name;
  int i = 0;
  while(name[i]!='\0')
  {
    cout<<name[i]<<": "<<int(name[i]);
    i++;
  }
  cout<<endl;
  ```
+ for与while: for循环一般用于知道循环次数，while一般用于无法预知循环次数的场景
+ 延时：clock()函数，头文件ctime，ctime将clock_t作为clock()返回类型的别名，即将变量定义为clock_t类型，编译器会自动将其转化为long、unsigned int或其它适合系统的类型：

  ```cpp
  #include <iostream>
  #include <ctime>
  int main()
  {
    using namespace std;
    float secs;
    cin>>secs;
    delay = secs*CLOCKS_PER_SEC;
    clock_t start = clock();
    while(clock() - start < delay)
      ;
    cout<<"done"<<endl;
    return 0;
  }
  
  ```
+ *类型别名*

### 5.3 do while循环
...

### 5.4 基于范围的for循环（c++ 11）
+ c++ 11新增基于范围（range based）的for循环，简化了一种常见任务：对数组每个元素执行相同操作
  ```cpp
  double prices[] = {1, 2, 3, 4};
  for(double x : prices)
    cout<<x<<endl;
  ```

### 5.5 循环和文本输入
+ 逐字符读取来自文件或键盘的文本：
  ```cpp
  # include<iostream>
  int main()
  {
    using namespace std;
    char ch;
    int count = 0;
    cout<<"Enter characters, enter # to quit"<<endl;
    cin>>ch;
    //cin.get(ch);
    while(ch!='#')
    {
      count++;
      cout<<ch;
      cin>>ch;
      //cin.get(ch);
    }
    cout<<count<<" characters read"<<endl;
    return 0;
  }
  ```
  cin读取char时，会忽略空格与换行符；此外，发送给cin的输入被缓冲，按下回车键后输入的内容才会发送给程序；采用cin.get()可以读取输入中的下一个字符，包括空格
+ 文件尾条件：
  + *重定向*
  + cin的eof()和fail()方法：cin.eof()和cin.fail()报告最近读取的结果，因此应放在读取之后：

    ```cpp
    #include<iostream>
    int main()
    {
      using namespace std;
      char ch;
      int count = 0;
      cin.get(ch);
      while(cin.fail() == false)
      {
        cout<<ch;
        count++;
        cin.get(ch);
      }
    }
    ```
    上面使用了常见的字符输入做法：
    ```cpp
    cin.get(ch);
    while(cin.fail() == false)
    {
      ...
      cin.get(ch);
    }
    ```
    一种更为精简的方法：
    ```cpp
    while(cin>>ch)
    {
      ...
    }
    ```
    ```cpp
    while(cin.get(ch))
    {
      ...
    }
    ```
+ 另一个cin.get()版本：C语言中的I/O函数：getchar()和putchar();

### 5.6嵌套循环和二维数组

+ 在输出中使用制表符使数据排列更加规则：'\t'
---

## 6 分支语句和逻辑运算符

### 6.1 if语句

### 6.2 逻辑表达式

### 6.3 字符函数库 cctype
`#include<cctype>`
| 函数名称 | 返回值 |
|:---|:---|
|isalnum()|若参数是字母数字，返回true|
|isalpha()|若参数为字幕，返回true|
|iscntrl()|若参数是控制字符，返回true|
|isdigit()|若参数为数字（0~9），返回true|
|isgraph()|若参数是除空格之外的打印字符，返回true|
|islower()|若参数为小写字母，返回true|
|isprint()|若参数为打印字符（包括空格），返回true|
|ispunct()|若参数是标点符号，返回true|
|isspace()|若参数为标准空白字符，如空格、进纸、换行、回车、水平、垂直制表符，返回true|
|isupper()|若参数为大写字母，返回true|
|isxdigit()|若参数为16进制数字，返回true|
|tolower()|若参数为大写字母，则返回小写，否则返回原参数|
|toupper()|若参数为小写字母，则返回大写，否则返回原参数|

### 6.4 ?:运算符

+ `expression1 ? expression2 : expression3` 若expression1为true则整个表达式返回expression2的值，否则返回expression3的值

### 6.5 switch语句

```cpp
switch(integer-expression)
{
  case label1 : statement(s)
  case label2 : statement(s)
  ...
  default : statement(s)
}
```

一般配合break语句确保只执行语句中的特定部分

### 6.6 break & continue

### 6.7 读取数字的循环

```cpp
int n
cin>>n;
```
如果输入的n不是数字，而是一个字符串，会发生以下情况：

+ n值保持不变；
+ 不匹配的输入被保留在输入队列中；
+ cin对象的一个错误标记被设置；
+ 对cin对象调用将返回false

若要继续输入，需要使用clear()方法重置错误标记

```cpp
#include"stdafx.h"
#include<iostream>
#include<ctime>
const int MAX = 5;
int main()
{
  using namespace std;
  int gamescore[MAX];
  cout<<"Please enter your game scores for "<<MAX<<"rounds"<<endl;
  int i = 0;
  for(int i=0; i<MAX; i++)
  {
    cout<<"Round "<<i+1<<":"<<endl;
    while(!(cin>>gamescore[i])) //拒绝、删除格式不符的输入
    {
      cin.clear();
      while(cin.get()!='\n')
        continue;
      cout<<"Please enter a number"<<endl;
    }
  }
  int Allscore = 0;
  for(int i = 0; i<MAX; i++)
  {
    Allscore += gamescore[i];
  }
  cout<<"Average score:\n"<<Allscore/MAX<<endl;
  clock_t start = clock();
  while(clock() - start < 5*CLOCKS_PER_SEC)
    ;
  return 0;
}
```

### 6.8 简单文件输入/输出

+ cin不断读取，直至遇到空白字符；cin.getline()不断读取，直至遇到换行符；
+ 写入到文本文件：
  + 类比cout用于控制台输出：
    + 必须包含头文件iostream；
    + 头文件iostream定义了一个用于处理输出的ostream类
    + 头文件iostream声明了一个名为cout的ostream对象
    + 必须指明命名空间std，或通过std::
    + 结合使用cout和运算符<<来显示各种类型的数据
  + 文件输出与此类似：
    + 必须包含头文件fstream
    + fstream定义了一个用于处理输出的ofstream类
    + 需要声明ofstream对象
    + 指明命名空间std
    + 需要将ofstream对象和文件关联起来，可使用open()方法
    + 使用完文件后，应使用close()方法将其关闭
    + 可结合ofstream对象和运算符<<来输出各种类型的数据
      ```cpp
      #include<iostream>
      #include<fstream>
      #include<string>
      int main()
      {
        using namespace std;
        string str[] = { "Hello", "This is C++ Primer" };
        cout << sizeof(str);
        ofstream outfile;
        outfile.open("hello_cpp.txt");
        if(outfile.is_open())
        {
          for (int i = 0; i< sizeof(str)/sizeof(str[0]); i++)
          {
            outfile << str[i] << endl;
          }
        }
        
        outfile.close();
        return 0;
      }
      ```
  + 总结使用文件输出的主要步骤：
    1. 包含头文件fstream
    2. 创建一个ofstream对象
    3. 将ofstream对象与文件关联起来
    4. 像cout那样使用ofstream对象
  + 在程序运行前，若文件不存在，则会创建文件；若文件已存在，默认方式下会先清空文件，再向文件输出内容
  + 打开文件可能会失败，一般要加`if(outfile.is_open())`判断
+ 读取文本文件：
  + 类比cin用于控制台输入：
    + 必须包含头文件iostream
    + 头文件iostream定义了一个用于处理输入的istream类
    + 头文件iostream声明了一个istream对象cin
    + 指明命名空间std
    + 使用cin和>>运算符来读取各种类型的数据
    + 可以使用cin和get()方法读取一个字符，使用cin.getline()读取一行字符
    + 结合使用cin和eof()、fail()方法来判断输入是否成功
    + cin本身被用作测试条件时，若最后一个读取成功，则其被转换为true，否则被转换为false
  + 文件输入与此类似：
    + 必须包含头文件fstream
    + 头文件fstream定义了一个用于处理输入的ifstream类
    + 声明一个ifstream对象
    + 指明命名空间std
    + 将ifstream对象与文件相关联
    + 使用ifstream对象和>>运算符来读取各种类型的数据
    + 可以使用ifstream对象和get()方法读取一个字符，使用ifstream对象和getline()读取一行字符
    + 结合使用ifstream对象和eof()、fail()方法来判断输入是否成功
    + ifstream本身被用作测试条件时，若最后一个读取成功，则其被转换为true，否则被转换为false
    + 文件使用完后使用close()方法关闭
  + 如果试图打开不存在的文件进行输入，通过is_open()方法检测
    ```cpp
    infile.open("hello_cpp.txt")
    if(!infile.is_open())
    {
      exit(EXIT_FAILURE);//通过头文件#include<cstdlib>引入对exit()的支持
    }
    ```
  + 文件输入示例：
    ```cpp
    #include"stdafx.h"
    #include <string>
    #include <iostream>
    #include <fstream>
    #include <cstdlib>
    #include <ctime>
    int main()
    {
      using namespace std;
      ifstream infile;
      int line_count = 0;
      string *pt = new string[100];
      infile.open("hello_cpp.txt");
      if (not infile.is_open())
      {
        exit(EXIT_FAILURE);
      }
      getline(infile, pt[0]);
      line_count++;
      cout << pt[0] << endl;
      while (infile.good())
      {
        line_count++;
        getline(infile, pt[line_count]);
        cout << pt[line_count] << endl;
      }
      infile.close();
      clock_t start = clock();
      delete[] pt;
      cout << line_count << " lines read";
      while (clock() - start < 5 * CLOCKS_PER_SEC)
        ;
      return 0;
    }
    ```
    更好的方案：
    ```cpp
    #include"stdafx.h"
    #include <string>
    #include <iostream>
    #include <fstream>
    #include <cstdlib>
    #include <ctime>
    #include <vector>
    int main()
    {
      using namespace std;
      ifstream infile;
      string line;
      int line_count = 0;
      vector<string>  words;
      infile.open("hello_cpp.txt");
      if (not infile.is_open())
      {
        exit(EXIT_FAILURE);
      }
      getline(infile, line);
      words.push_back(line);
      line_count++;
      while (infile.good())
      {
        line_count++;
        getline(infile, line);
        words.push_back(line);
      }
      infile.close();
      for (string line : words)
        cout << line << endl;
      cout << line_count << " lines read" << endl;
      clock_t start = clock();
      while (clock() - start < 5 * CLOCKS_PER_SEC)
        ;
      return 0;
    }
    ```
  + infile.good()方法在发生任何错误时返回false，相比检测eof和fail更加简便；good()方法指出最后一次读取操作是否成功，即执行读取操作后进行测试，一种标准的方法是，在循环测试之前放置一条输入语句，并在循环末尾放置一条输入语句：
    ```cpp
    infile>>value;
    while (infile.good())
    {
      //loop body
      infile>>value;
    }
    ```
---

## 7 函数 —— C++编程模块

### 7.1 函数Basics

+ c++对函数返回值类型有一定限制：不能返回数组，其它类型均可；虽然不能返回数组，但能够将数组作为结构或对象的组成部分来返回
+ 函数原型一般隐藏在include文件中，函数原型将函数返回值类型和参数类型、数量告诉编译器，达到捕获错误、提升效率的目的；函数原型的参数列表中，一般只指明参数类型即可，无需写明参数名称

### 7.2 函数参数和按值传递

### 7.3 函数和数组

+ `int sum_arr(int arr[], int n)` 语句中，arr实际上不是数组，而是指针，但在函数中可以将arr当作数组使用
+ c++将数组名解释为其第一个元素的地址：
  ```cpp
  arr == &arr[0];
  ```
  以下几种情况例外：
  + 数组声明使用数组名来标记存储位置
  + 对数组名使用sizeof会得到整个数组的长度
  + 将地址运算符&应用于数组名时，将返回整个数组的地址（加1会增加整个数组长度）
+ 对于函数头`int sum_arr(int arr[], int n)`，以下的函数头也是正确的，`int sum_arr(int * arr, int n)`. c++中，仅当用于函数头或者函数原型中，`int arr[]`和`int * arr`才是等价的
+ 传递常规变量时，函数使用该变量的拷贝；但传递数组时，函数使用原来的数组；这不违背函数按值传递的方法，只是这个值换成了地址
+ 使用const保护数组：
  ```cpp
  void show_array(const double ar[], int n);
  ```
  这意味着不能使用ar修改该数据，即不能再show_array函数中修改数组内容
+ 使用数组区间的函数：通过数组头尾两个指针和指针加法遍历数组内容
+ 指针和const：
  + 两种不同方式将const应用于指针：
    + 让指针指向一个常量对象，方式使用该指针修改所指向的值
    + 将指针本身声明为常量，防止改变指针所指向的位置
  + **const变量的地址可赋给指向const的指针，不能赋给常规指针**
    ```cpp
    const int months[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int sum(int arr[], int n)
    {...}
    int i = sum(months, 12); //not allowed
    ```
  + const与指针结合的三种方式：
    ```cpp
    int sloth = 3;
    const int * pt = &sloth;//防止通过pt修改其所指向的sloth
    int * const ps = &sloth;//防止改变ps所指向的位置
    const int * const pp =&sloth;//指向const对象的const指针
    ```

### 7.4 函数和二维数组
```cpp
int data[3][4] = {{1,2,3,4},{5,6,7,8},{9,10,11,12}};
int sum_data = sum(data, int size);
int sum(int (*arr)[4], int size)
//int sum(int arr[][4], int size)
{...}
```

### 7.5 函数和C-风格字符串
函数处理字符串的标准方式：
```cpp
while(*str)
{
  statements
  str++;
}
```
```cpp
#include"stdafx.h"
#include <iostream>
#include <ctime>
int print_str(const char * str)
{
    using namespace std;
    while (*str)
    {
      cout <<* str << endl;
      str++;
    }
    return 0;
}
int main()
{
    using namespace std;
    char str[] = "Hello";
    print_str(str);
    cout<<str<<endl;
    clock_t start = clock();
    while (clock() - start < 5 * CLOCKS_PER_SEC)
      ;
    return 0;
}
```

### 7.6 函数和结构

+ 结构与普通变量一样，按值传递，函数使用原结构的副本；但如果结构很大，按值传递会增加内存要求，降低系统运行速度，基于这个原因，许多C程序员倾向于传递结构地址，然后使用指针访问结构内容
+ 通过传递结构地址、使用指针访问结构内容的方式，可用const修饰防止结构内容被修改，此外通过指针访问结构内容需要使用间接成员运算符->

### 7.7 函数和string对象

```cpp
int size = 5;
string list[size];
for(int i = 0; i<size; i++)
{
  cout<<i+1<<": "<<endl;
  getline(cin,list[i]);
}
...
```

### 7.8 函数与array对象

+ array默认按值传递，可通过引用的方式让函数操作原始数据
  ```cpp
  std::array<double, 4> expenses;
  show(expenses);
  fill(&expenses);
  ```
对应的函数原型：
  ```cpp
  show(std::array<double, 4> para_1);
  fill(std::array<double, 4> * para_2);
  ```
通过(*para_2)[i]访问、修改expenses元素

### 7.9 *递归*

### 7.10 *函数指针*


## 8 函数探幽

### 8.1 内联函数

+ 常规函数调用使程序跳到另一个地址，并在函数结束时返回；对于内联代码，程序无需跳到另一位置再返回，内联函数运行速度稍快，但占用更多内存；如果程序在10个地方调用10次内联函数，则该程序包含10个该函数的代码；
+ 内联函数在函数定以前加inline:
  ```cpp
  inline double square(double t) {return t*t;}
  ```
+ 内联与宏：C语言使用预处理器语句#define来提供宏，如一个计算平方的宏：
  ```cpp
  #define SQUARE(X) X*X
  ```
但这不是通过传递参数实现的，只是通过文本替换实现的，如：
  ```cpp
  double b =SQUARE(4.5+7.5);//被替换为double b = 4.5+7.5*4.5+7.5;
  ```

### 8.2 引用变量

+ 通过引用变量作为函数参数，函数可对原始数据做操作与修改，同时为函数处理大型结构提供了便捷的途径
```cpp
int rats;
int & rodents = rats;
```
+ 可以通过初始化来设置引用，但不能通过赋值来设置
+ 引用作为函数参数：
  ```cpp
  int m= 1,n=2;
  int swap(int &a, int &b)
  {
    int t =0;
    t=a;
    a=b;
    b=t
    return 0;
  }
  swap(m,n);
  ```
+ 如果只是引用变量信息，而不做修改，可加const修饰

### 8.3 默认参数

+ 必须通过函数原型设置默认值，函数原型设置后，函数定义和没有默认参数完全相同；

### 8.4 函数重载

+ 函数重载及函数多态
+ 特征标、参数数目、参数排列顺序
+ 一般只在函数实现相同功能，但使用不同形式的数据时使用函数重载

### 8.5 函数模版

+ 函数模板是通用的函数描述，使用泛型来定义函数，其中泛型可使用具体类型（double、int等）来替换