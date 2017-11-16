# Chapter 01 c# 与.NET框架
+ LINQ(Language-Intergrated Query 语言集成查询)

# Chapter 02 出发："Hello World"
+ 方法返回多个值：
  + 按引用传递多个对象给方法；
  + 返回一个对象，但此对象是集合对象；
+ 命名空间：
  + 帮助划分对象的层次以降低复杂性；
  + using语句(using Directive)只能用于命名【】空间，如using System; using System.Console是error的；
  + using语句虽减小了输入工作，但降低了命名空间的优越性；
+ c#区分大小写，Microsoft有相应的编程风格准则
+ static void Main(),static 表示可在创建类的对象之前调用Main();
+ 调试器：断点、下一步(F11)

# Chapter 03 C#语言基础
+ c#是类型明确的语言，即总是显式地声明一个变量，但在使用关键字var时，c#可推断对象类型，此时类型是隐含的
+ c#中带小数点的默认为double类型，声明为float需在后面加f，声明为decimal需加m。如8.8f、8.8m
+ 类型转换：分显式和隐式
  + 隐式：编译器自动完成，不损失信息的转换，如short转换为int
  + 显式：需明确写明，如(short) intnum
+ 变量与常量
  + 变量是具有类型的存储位置；
  + c#要求明确赋值，即变量在使用前必须初始化或者赋值；
  + 常量声明，const int FreezingPoint = 0;
+ 枚举(enumeration)
  + 枚举是一种常量，由许多有名字的常量组成(枚举列表)
  + 每个枚举都有底层类型，可以使任何整数类型，定义如下：
    ```
    [性质] [修饰符] enum 标识符 [:基类型]
    {枚举列表};
    ```
    基类型是枚举的底层类型，常省略，默认为int
  + 枚举的每个常量均对应一个数值，如不特别设置，从0开始，逐1累加。枚举是正规类型，其与整数间的转换需显式进行
  ```
  public string getTimeOfDay(TimeOfDay time)
    {
        string result = string.Empty;
        switch (time)
        {
            case TimeOfDay.Moning:
                result = "上午";
                break;
            case TimeOfDay.Afternoon:
                result = "下午";
                break;
            case TimeOfDay.Evening:
                result = "晚上";
                break;
            default:
                result = "未知";
                break;
        }
        return result;
    }
  ```
+ 条件语句
  + 与c/c++中田间语句中能够使用任何表达式不同，c#要求条件表达式必须为布尔值
  + 常用switch语句替代嵌套if语句
  + case:
    ```
    case1:
        TakeSomeAction();
        goto case2;
    case2:
        ...
    ```
  + while与do...while：若条件为假，while循环中的语句一次也不会执行，使用do...while可先执行一次
+ 操作符
  + =：可在一条语句中将一值赋给任意多的变量：a=b=c=d=16;
  + 前缀与后缀操作符
     ```
     int firstValue;
     int secondValue = 10;
     firstValue = secondValue++; // 先赋值，再增量，firstValue为10，secondValue为11
     ```
     ---
     ```
     int firstValue;
     int secondValue = 10;
     firstValue = ++secondValue; // 先增量，再赋值，firstValue为11，secondValue为11
     ```
  + 三元操作符：条件表达式? 表达式1:表达式2
  + 预处理指令：c#编译代码前，会先运行预处理器
    ```
    #define DEBUG
    #if DEBUG
    //此代码将编译
    #endif
    #undef DEBUG
    #if DEBUG
    //此代码不会编译
    #endif
    ```
    ---
    ```
    #define DEBUG
    #if DEBUG
    //code0
    #elif DEBUG1
    //code1
    #else DEBUG2
    //code2
    #endif
    ```
# Chapter 04 类与对象
+ 创建并使用新类型的能力是面向对象语言的特征，面向对象编程的优势在于能够将实体的特性和能力封装到一个独立且自给自足的代码单元中
+ 与c++不同，c#不要求方法在定以前一定要被声明，同时其也不支持将方法的声明放在一个文件中，而方法的定义放在另一个文件中。c#中没有头文件的概念
+ 访问修饰符：protected internal等效于protected或internal
+ 创建对象：c#的基本类型(int、char等)是值类型，在栈中创建；对象是引用类型，创建于堆中。`Time t = new Time();`中t并不保存Time对象的值，保存的是堆中创建的这个值的地址，t本身是指向该对象的一个引用
+ 构造方法：实例化对象时要调用构造方法，如无特殊声明，CLR会自动提供
+ 初始化语句与构造方法：若先在初始化语句中声明了成员值，又在构造函数中赋值该成员变量，则构造方法会覆盖初始化值
+ ICloneable接口支持复制构造函数的概念
+ this关键字指向对象的当前实例。this**引用**是类中所有非静态方法的隐藏指针，每个方法均能通过this引用指向对象的其它方法和变量。
  + this的使用一定程度上解决参数和成员变量混淆的问题
    ```
    public void SomeMethod (int hour)
    {
        this.hour = hour;
    }
    ```
  + this可把当前对象作为参数传递给另一个方法
    ```
    class MyClass
    {
        public void Foo(OtherClass otherObject)
        {
            otherObject.Bar(this);
        }
    }
    ```
  + this的第三种使用方法与索引器有关
  + this的第四种使用方法是在重载构造方法中调用另一个
    ```
    class MyClass
    {
        public MyClass(int i) {//...}
        public MyClass() : this(8) {//...}
    }
    ```
  + this的第五种使用方法是显式调用一个类的成员和方法，增强可读性
+ 静态成员：
  + 类的方法可以是实例成员，也可以是静态成员。实例成员与类的实例有关，静态成员则是类的一部分，可直接通过类名访问静态成员。如有一个Button类，实例化该类生成btnUpdate和btnDelete，Button类有一静态方法SomeMethod(),则访问此方法的方式是`Button.SomeMethod()`，而不是`btnDelete.SomeMethod()`，c#中通过实例访问静态方法或静态成员变量是不合法的。
  + 静态方法不能直接访问非静态成员，如Main()调用非静态方法，必须先实例化一个对象，通过对象调用方法
  + <font color=blue>静态构造方法</font>
  + 静态类：c#中，不存在全局方法或全局变量，我们可以创建小的工具类，其中仅有静态成员，创建这种类意味着我们不想创建它的任何实例。静态类是密封的，不能创建派生类型，也没有构造方法。
  + 静态字段：静态成员变量常用来保存当前类的实例数目
+ 销毁对象;c#提供了垃圾回收器，因此无需显式地销毁对象，但若对象控制<font color=blue>非托管资源</font>，用完后显式地释放还是需要的。对非托管资源的隐式控制是析构方法(destructor)提供的，它会在对象销毁时由垃圾回收器调用。<font color=red>析构方法应该只释放对象占用的资源，不应引用其它对象</font>。
+ c#析构方法：`~MyClass(){}`
+ <font color=blue>析构方法与Dispose方法</font>，有时可创建一个私有的Dispose()方法和公共的close()方法，close()方法调用Dispose()方法用以实现close()方法
+ c#提供了using语句保证Dispose()语句尽可能早地调用。惯用法是声明正在使用的对象，并用大括号创建对象的作用域，执行到结束大括号时，Dispose()方法自动调用。
  ```
  #region using directives
  using System;
  using System.Collections.Generic;
  using System.Drawing;
  using System.Text;
  #endregion
  namespace usingStatement
  {
      class Tester
      {
          public static void Main()
          {
              using(Font theFont = new Font("Aerial",10.0f))
              {
                  //...
              }//编辑器调用theFont的Dispose
          }
      }
  }
  ```
+ 传递参数：c#中按值传递是常规方式，但很多时候需要用到按引用传递值对象。c#中提供了ref参数修饰符用于按引用把值对象传递给方法，还有out修饰符用于不经初始化就传递给ref变量。c#还支持params修饰符，允许方法接受数目可变的参数。
  + 按引用传递，方法参数使用ref修饰符、调用时也需声明以引用传递参数
    ```
    public void GetTime(ref int h, ref int m, ref int s)
    {
        h = Hour;
        m = Minute;
        s = Second;
    }

    t.GetTime(ref theHour, ref theMinute, ref theSecond);
    ```
  + <font color=blue>通过out克服明确赋值问题</font>：c#要求所有变量在使用前必须赋值，有时这一步显得无意义。c#为此提供了out参数修饰符，免除引用参数初始化的要求。
    ```
    //
    public void GetTime(out int h, out int m, out int s)
    {
        h = Hour;
        m = Minute;
        s = Second;
    }

    t.GetTime(out theHour, out theMinute, out theSecond);
    ```
+ 方法重载：方法的签名由其名字和参数列表定义，名字和参数列表不同，其签名就不同，参数列表的不同可以是参数数目或者类型的不同。一个类中的不同方法只要签名不同即可。重载方法时，必须改变签名，可选改变返回值类型。但仅改变返回值类型不能重载方法，创建两个签名相同但返回类型不同的方法会产生编译错误。
