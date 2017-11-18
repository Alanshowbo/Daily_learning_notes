# Chapter 01 c# 与.NET框架
+ LINQ(Language-Intergrated Query 语言集成查询)

# Chapter 02 出发："Hello World"
+ 方法返回多个值：
  + 按引用传递多个对象给方法
  + 返回一个对象，但此对象是集合对象
+ 命名空间：
  + 帮助划分对象的层次以降低复杂性
  + using语句(using Directive)只能用于命名空间，如using System; using System.Console是error的
  + using语句虽减小了输入工作，但降低了命名空间的优越性
+ c#区分大小写，Microsoft有相应的编程风格准则
+ static void Main(),static 表示可在创建类的对象之前调用Main()
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
  public enum TimeOfDay
  {
      Morning = 0,
      Afternoon = 1,
      Evening =2,
  };
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
  + 类的方法可以是实例成员，也可以是静态成员。实例成员与类的实例有关，静态成员则是类的一部分，可直接通过类名访问静态成员。如有一个Button类，实例化该类生成btnUpdate和btnDelete，Button类有一静态方法SomeMethod(),则访问此方法的方式是`Button.SomeMethod()`，而不是`btnDelete.SomeMethod()`，c#中通过实例访问静态方法或静态成员变量是不合法的
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
+ <font color=blue>用属性封装数据</font>
  + get 访问方法
  + set 访问方法
  + 属性访问修饰符：可设置访问修饰符(protected、internal、private)限制get和set访问方法的访问权限。为此，属性必须既有get又有set访问方法，且只能限制其中一个。同样，修饰符必须比属性或索引器已有的可访问级别低。
  + <font color=blue>只读字段readonly</font>

# Chapter 05 继承与多态
+ WPF(Windows Presentation Foundation)
+ 继承(Inheritance)是具象化(Specilization)最常用和最自然的方式
+ 多态(Polymorphism).继承有两个强大功能，一是代码重用，二是多态
  + 创建多态类型：Listbox是一种Control，Button也是一种Control，我们希望在任何要求Control的情况下均能使用这些类型。如一个表单想保存它所管理的所有Control实例集合，表单打开时，可以让所有Control自我绘制。对于此操作，表单无需知道哪个元素是Listbox、哪个元素是Button，只需把几何遍历一次，让元素自己绘制，即表单多态地处理Control对象
  + 创建多态方法：创建支持多态的方法，只需在其基类中将其标注为Virtual，如指明Control中的DrawWindow()方法是多态的，`public virtual void DrawWindow()`.这样每个派生类可以自由地实现各自版本的DrawWindow(),只需在派生类的方法定义中用关键字override覆盖基类的虚方法，如在派生自Control的Listbox中定义DrawWindow():
    ```
    public override void DrawWindow()
    {
        base.DrawWindow();//调用基方法
        //...
    }
    ```
  + 调用基类构造方法：Listbox有自己的构造方法，参数为两个整数，一个字符串，通过在参数列表后放一个冒号，而后用base关键字调用基类，来调用其父类Control的构造方法：
    ```
    public Listbox(int theTop, int theLeft, string s):base(theTop, theLeft)
    ```
    因为类不继承构造方法，派生类必须实现自己的构造方法，只能通过显式调用来实现基类的构造方法
+ <font color=blue>使用关键字new和override进行版本处理</font>
+ 抽象类(Abstract classes)，Control的每个子类都应实现自己的DrawWindow()方法，但没有硬性要求。要指定子类必须实现基类的方法，则应指明该方法是抽象(Abstract)的.
  + <font color=red>抽象方法没有实现</font>，它只创建派生类都要实现的方法名和签名，使类的一个或多个方法成为抽象的，会使得类也变为抽象的。抽象类是派生类的基，无法进行实例化.
  + 通过在方法定义前加abstract来指定其为抽象：`abstract public void DrawWindow();`（因方法无实现，无需大括号）
  + 抽象的局限：虽然在Control中指定DrawWindow()为抽象会去爱你固执要求所有派生类实现此方法，但如果从Listbox再派生一个类，就无法再强制二次派生的类实现DrawWindow()方法
  + 抽象类创建了一个合同(contract)，描述了要实现该抽象的所有类的公共方法
  + 抽象类Control应反映所有Control的共性，即永远不用实例化抽象的Control的本身
+ 密封类(sealed)：与抽象类相对应，密封类完全不允许被派生，使用关键字sealed声明
+ 万类之根：Object. c#中的所有类都可看成最终由System.Object派生而来。Object提供了子类可重定义的大量方法

| Methods           | Function      |
| ----------------- | ------------- |
| Equals()          | Determines whether the specified object is equal to the current object |
| GetHashCode()     | Serves as the default hash function.  |
| GetType()         | Gets the Type of the current instance.      |
| ToString()        | Returns a string that represents the current object      |
| Finalize()        | Allows an object to try to free resources and perform other cleanup operations before it is reclaimed by garbage collection.|
| MemberwiseClone() | Creates a shallow copy of the current Object      |
| ReferenceEquals() | Determines whether the specified Object instances are the same instance      |

# Chapter 06 操作符重载

# Chapter 07 结构体
+ 结构体是一种简单的用户定义类型，也是类的一种轻量级替代品，与类相似，也有构造方法、属性、方法、字段、操作符、嵌套类型和索引器
+ 结构体与类也有显著差异，结构体不支持继承或析构函数.特别的,<font color=red>类是一种引用类型，结构体是**值**类型</font>.
+ 结构体数组在内存使用方面效率较高，但在非类型一致的集合中效率较差，集合的元素应该是引用类型，所以结构体需要进行装箱处理，装箱和拆箱是有开销的，类在大集合中更具效率
+ 如果一个类，有10个结构体而不是10个对象作为其成员变量，当这个类在堆中被创建，一个大的对象被创建（具有10个结构体的类），而不是11个对象被创建
+ 结构体没有析构函数或自定义的默认构造方法：结构体无自定义的无参构造方法，若不显式提供构造方法，CLR将初始化结构体，将所有数据成员置零
  ```
  using System;

  namespace ConsoleApp1
  {
      public struct Location
      {
          public int X { get; set; }
          public int Y { get; set; }
          public override string ToString()
          {
              return (string.Format("{0}, {1}",X, Y));
          }
          public Location(int x, int y)
          {
              this.X = x;
              this.Y = y;
          }
      }
      public class Tester
      {
          public void myFunc(Location loc)
          {
              loc.X = 50;
              loc.Y = 100;
              Console.WriteLine("In myFunc loc: {0}",loc);
          }
          static void Main(string[] args)
          {
              Location loc1 = new Location(200,300);
              /*
              loc1.X = 200;
              loc1.Y = 300;
              */
              Console.WriteLine("loc1 location: {0}", loc1);
              Tester t = new Tester();
              t.myFunc(loc1);
              Console.WriteLine("loc1 location: {0}", loc1);
          }
      }
  }
  //output：
  loc1 location:200,300
  In myFunc loc:50,100
  loc1 location:200,300
  ```
+ 结构体不能初始化，如在上例中，`private int X = 150;`是非法的
+ 结构体是值类型，若将上例中`public struct Location`改为`public class Location`，则输出为：
  ```
  loc1 location:200,300
  In myFunc loc:50,100
  loc1 location:200,300
  ```
  按值传递(struct)变为按引用传递(class)

# Chapter 08 接口
