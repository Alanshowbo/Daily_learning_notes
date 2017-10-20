# C# KeyWords
## abstract
abstract 修饰符指示被修改内容的实现已丢失或不完整。
```csharp
abstract class ShapesClass
{
    abstract public int Area();
}
class Square : ShapesClass
{
    int side = 0;

    public Square(int n)
    {
        side = n;
    }
    // Area method is required to avoid
    // a compile-time error.
    public override int Area()
    {
        return side * side;
    }

    static void Main()
    {
        Square sq = new Square(12);
        Console.WriteLine("Area of the square = {0}", sq.Area());
    }

    interface I
    {
        void M();
    }
    abstract class C : I
    {
        public abstract void M();
    }

}
// Output: Area of the square = 144
```

## internal
internal 关键字是类型和类型成员的访问修饰符。 只有在同一程序集的文件中，内部类型或成员才可访问.
```csharp

```

## namespace
namespace 关键字用于声明包含一组相关对象的作用域。 可以使用命名空间来组织代码元素并创建全局唯一类型。
```csharp
//可以在两个或多个声明中定义一个命名空间。 例如，以下示例将两个类定义为 MyCompany 命名空间的一部分：
namespace MyCompany.Proj1
{
    class MyClass
    {
    }
}

namespace MyCompany.Proj1
{
    class MyClass1
    {
    }
}
```
---
```csharp
//以下示例显示如何在嵌套命名空间中调用静态方法。
namespace SomeNameSpace
{
    public class MyClass
    {
        static void Main()
        {
            Nested.NestedNameSpaceClass.SayHello();
        }
    }

    // a nested namespace
    namespace Nested   
    {
        public class NestedNameSpaceClass
        {
            public static void SayHello()
            {
                Console.WriteLine("Hello");
            }
        }
    }
}
// Output: Hello
```

## operator
使用 operator 关键字重载内置运算符，或在类或结构声明中提供用户定义的转换。
```csharp
//下面是针对分数的一个非常简单的类。 此类重载 + 和 * 运算符以执行分数加法和乘法，并提供转换运算符将分数类型转换为双精度类型。
class Fraction
{
    int num, den;
    public Fraction(int num, int den)
    {
        this.num = num;
        this.den = den;
    }

    // overload operator +
    public static Fraction operator +(Fraction a, Fraction b)
    {
        return new Fraction(a.num * b.den + b.num * a.den,
           a.den * b.den);
    }

    // overload operator *
    public static Fraction operator *(Fraction a, Fraction b)
    {
        return new Fraction(a.num * b.num, a.den * b.den);
    }

    // user-defined conversion from Fraction to double
    public static implicit operator double(Fraction f)
    {
        return (double)f.num / f.den;
    }
}

class Test
{
    static void Main()
    {
        Fraction a = new Fraction(1, 2);
        Fraction b = new Fraction(3, 7);
        Fraction c = new Fraction(2, 3);
        Console.WriteLine((double)(a * b + c));
    }
}
/*
Output
0.880952380952381
*/
```

## params
使用 params 关键字可以指定采用数目可变的参数的方法参数。
可以发送参数声明中所指定类型的逗号分隔的参数列表或指定类型的参数数组。 还可以不发送参数。 如果未发送任何参数，则 params 列表的长度为零。
在方法声明中的 params 关键字之后不允许有任何其他参数，并且在方法声明中只允许有一个 params 关键字。
```csharp
public class MyClass
{
    public static void UseParams(params int[] list)
    {
        for (int i = 0; i < list.Length; i++)
        {
            Console.Write(list[i] + " ");
        }
        Console.WriteLine();
    }

    public static void UseParams2(params object[] list)
    {
        for (int i = 0; i < list.Length; i++)
        {
            Console.Write(list[i] + " ");
        }
        Console.WriteLine();
    }

    static void Main()
    {
        // You can send a comma-separated list of arguments of the
        // specified type.
        UseParams(1, 2, 3, 4);
        UseParams2(1, 'a', "test");

        // A params parameter accepts zero or more arguments.
        // The following calling statement displays only a blank line.
        UseParams2();

        // An array argument can be passed, as long as the array
        // type matches the parameter type of the method being called.
        int[] myIntArray = { 5, 6, 7, 8, 9 };
        UseParams(myIntArray);

        object[] myObjArray = { 2, 'b', "test", "again" };
        UseParams2(myObjArray);

        // The following call causes a compiler error because the object
        // array cannot be converted into an integer array.
        //UseParams(myObjArray);

        // The following call does not cause an error, but the entire
        // integer array becomes the first element of the params array.
        UseParams2(myIntArray);
    }
}
/*
Output:
    1 2 3 4
    1 a test

    5 6 7 8 9
    2 b test again
    System.Int32[]
*/
```

## readonly
```csharp
//在此示例中，即使在类构造函数中给字段 year 赋了值，它的值仍无法在 ChangeYear 方法中更改：
class Age
{
    readonly int _year;
    Age(int year)
    {
        _year = year;
    }
    void ChangeYear()
    {
        //_year = 1967; // Compile error if uncommented.
    }
}
```

## sealed
应用于某个类时，sealed 修饰符可阻止其他类继承自该类。 在下面的示例中，类 B 继承自类 A，但没有类可以继承自类 B。
```csharp
class A {}      
sealed class B : A {}
```

## static
使用 static 修饰符可声明属于类型本身而不是属于特定对象的静态成员。 static 修饰符可用于类、字段、方法、属性、运算符、事件和构造函数，但不能用于索引器、终结器或类以外的类型。
```csharp
public class MyBaseC
{
    public struct MyStruct
    {
        public static int x = 100;
    }
}
//若要引用静态成员 x，除非可从相同范围访问该成员，否则请使用完全限定的名称 MyBaseC.MyStruct.x：
Console.WriteLine(MyBaseC.MyStruct.x);  
```

## this

## unsafe

## virtual

## as
可以使用 as 运算符在符合的引用类型或可以为 null 的类型之间执行某些类型的转换。
as 运算符类似于转换运算。 但是，如果无法进行转换，则 as 会返回 null，而不是引发异常。
```csharp
class ClassA { }
class ClassB { }

class MainClass
{
    static void Main()
    {
        object[] objArray = new object[6];
        objArray[0] = new ClassA();
        objArray[1] = new ClassB();
        objArray[2] = "hello";
        objArray[3] = 123;
        objArray[4] = 123.4;
        objArray[5] = null;

        for (int i = 0; i < objArray.Length; ++i)
        {
            string s = objArray[i] as string;
            Console.Write("{0}:", i);
            if (s != null)
            {
                Console.WriteLine("'" + s + "'");
            }
            else
            {
                Console.WriteLine("not a string");
            }
        }
    }
}
/*
Output:
0:not a string
1:not a string
2:'hello'
3:not a string
4:not a string
5:not a string
*/
```

## checked
checked 关键字用于对整型类型算术运算和转换显式启用溢出检查。
默认情况下，如果表达式仅包含常量值，且产生的值在目标类型范围之外，则会导致编译器错误。 如果表达式包含一个或多个非常量值，则编译器不检测溢出。 在下面的示例中，计算赋给 i2 的表达式不会导致编译器错误。
```csharp
// The following example causes compiler error CS0220 because 2147483647
// is the maximum value for integers.
//int i1 = 2147483647 + 10;

// The following example, which includes variable ten, does not cause
// a compiler error.
int ten = 10;
int i2 = 2147483647 + ten;

// By default, the overflow in the previous statement also does
// not cause a run-time exception. The following line displays
// -2,147,483,639 as the sum of 2,147,483,647 and 10.
Console.WriteLine(i2);
```

## explicit

## fixed

## in

## is
is 关键字在运行时评估类型兼容性。 它确定对象实例或表达式结果是否可转换为指定类型。
```csharp
using System;

public class Class1 : IFormatProvider
{
   public object GetFormat(Type t)
   {
      if (t.Equals(this.GetType()))      
         return this;
      return null;
   }
}

public class Class2 : Class1
{
   public int Value { get; set; }
}

public class Example
{
   public static void Main()
   {
      var cl1 = new Class1();
      Console.WriteLine(cl1 is IFormatProvider);
      Console.WriteLine(cl1 is Object);
      Console.WriteLine(cl1 is Class1);
      Console.WriteLine(cl1 is Class2);
      Console.WriteLine();

      var cl2 = new Class2();
      Console.WriteLine(cl2 is IFormatProvider);
      Console.WriteLine(cl2 is Class2);
      Console.WriteLine(cl2 is Class1);
      Console.WriteLine();

      Class1 cl = cl2;
      Console.WriteLine(cl is Class1);
      Console.WriteLine(cl is Class2);
   }
}
// The example displays the following output:
//     True
//     True
//     True
//     False
//     
//     True
//     True
//     True
//     
//     True
//     True
```

## new
new 运算符:
- 用于创建对象和调用构造函数。
```csharp
Class1 obj  = new Class1();  
```
- 用于创建匿名类型的实例：
```csharp
var query = from cust in customers  
            select new {Name = cust.Name, Address = cust.PrimaryAddress};
```
- 不能重载 new 运算符。

new 修饰符:
在用作声明修饰符时，new 关键字可以显式隐藏从基类继承的成员。 隐藏继承的成员时，该成员的派生版本将替换基类版本。 虽然可以不使用 new 修饰符来隐藏成员，但将收到编译器警告。 如果使用 new 来显式隐藏成员，将禁止此警告。
```csharp
public class BaseC
{
    public int x;
    public void Invoke() { }
}
public class DerivedC : BaseC
{
    new public void Invoke() { }
}
```
---
```csharp
public class BaseC
{
    public static int x = 55;
    public static int y = 22;
}

public class DerivedC : BaseC
{
    // Hide field 'x'.
    new public static int x = 100;

    static void Main()
    {
        // Display the new value of x:
        Console.WriteLine(x);

        // Display the hidden value of x:
        Console.WriteLine(BaseC.x);

        // Display the unhidden member y:
        Console.WriteLine(y);
    }
}
/*
Output:
100
55
22
*/
```
new 约束

## out
out 关键字通过引用传递参数。 它与 ref 关键字相似，只不过 ref 要求在传递之前初始化变量。 若要使用 out 参数，方法定义和调用方法均必须显式使用 out 关键字。
```csharp
using System;

class OutExample
{
   static void Method(out int i)
   {
      i = 44;
   }

   static void Main()
   {
      int value;
      Method(out value);
      Console.WriteLine(value);     // value is now 44
   }
}
```

## ref

## short
可以通过为其分配十进制文本、十六进制文本或（从 C# 7 开始）二进制文本来声明和初始化 short 变量。 如果整数文本在 short 范围之外（即，如果它小于 Int16.MinValue 或大于 Int16.MaxValue），会发生编译错误。
```csharp
short shortValue1 = 1034;
Console.WriteLine(shortValue1);

short shortValue2 = 0x040A;
Console.WriteLine(shortValue2);

short shortValue3 = 0b0100_00001010;
Console.WriteLine(shortValue3);
// The example displays the following output:
//          1034
//          1034
//          1034
```

## throw
发出程序执行期间出现异常的信号。
```csharp
using System;

public class NumberGenerator
{
   int[] numbers = { 2, 4, 6, 8, 10, 12, 14, 16, 18, 20 };

   public int GetNumber(int index)
   {
      if (index < 0 || index >= numbers.Length) {
         throw new IndexOutOfRangeException();
      }
      return numbers[index];
   }
}
//然后方法调用方使用 try-catch 或 try-catch-finally 块来处理引发的异常。 下例处理由 GetNumber 方法引发的异常。
public class Example
{
   public static void Main()
   {
      var gen = new NumberGenerator();
      int index = 10;
      try {
          int value = gen.GetNumber(index);
          Console.WriteLine($"Retrieved {value}");
      }
      catch (IndexOutOfRangeException e)
      {
         Console.WriteLine($"{e.GetType().Name}: {index} is outside the bounds of the array");
      }
   }
}
// The example displays the following output:
//        IndexOutOfRangeException: 10 is outside the bounds of the array
```

## base
base 关键字用于从派生类中访问基类的成员：
+ 调用基类上已被其他方法重写的方法。
+ 指定创建派生类实例时应调用的基类构造函数。
```csharp
public class Person
{
    protected string ssn = "444-55-6666";
    protected string name = "John L. Malgraine";

    public virtual void GetInfo()
    {
        Console.WriteLine("Name: {0}", name);
        Console.WriteLine("SSN: {0}", ssn);
    }
}
class Employee : Person
{
    public string id = "ABC567EFG";
    public override void GetInfo()
    {
        // Calling the base class GetInfo method:
        base.GetInfo();
        Console.WriteLine("Employee ID: "+string(id));
    }
}

class TestClass
{
    static void Main()
    {
        Employee E = new Employee();
        E.GetInfo();
    }
}
/*
Output
Name: John L. Malgraine
SSN: 444-55-6666
Employee ID: ABC567EFG
*/
```
---
//本示例显示如何指定在创建派生类实例时调用的基类构造函数。
public class BaseClass
{
    int num;

    public BaseClass()
    {
        Console.WriteLine("in BaseClass()");
    }

    public BaseClass(int i)
    {
        num = i;
        Console.WriteLine("in BaseClass(int i)");
    }

    public int GetNum()
    {
        return num;
    }
}

public class DerivedClass : BaseClass
{
    // This constructor will call BaseClass.BaseClass()
    public DerivedClass() : base()
    {
    }

    // This constructor will call BaseClass.BaseClass(int i)
    public DerivedClass(int i) : base(i)
    {
    }

    static void Main()
    {
        DerivedClass md = new DerivedClass();
        DerivedClass md1 = new DerivedClass(1);
    }
}
/*
Output:
in BaseClass()
in BaseClass(int i)
*/

## case
//switch 是一个选择语句，它根据与匹配表达式匹配的模式，从候选列表中选择单个开关部分进行执行。
```csharp
using System;

public class Example
{
   public static void Main()
   {
      int caseSwitch = 1;

      switch (caseSwitch)
      {
          case 1:
              Console.WriteLine("Case 1");
              break;
          case 2:
              Console.WriteLine("Case 2");
              break;
          default:
              Console.WriteLine("Default case");
              break;
      }
   }
}
// The example displays the following output:
//       Case 1
```

## extern
//extern 修饰符用于声明在外部实现的方法。 extern 修饰符的常见用法是在使用 Interop 服务调入非托管代码时与 DllImport 特性一起使用。 在这种情况下，还必须将方法声明为 static
```csharp
//using System.Runtime.InteropServices; 在此示例中，程序接收来自用户的字符串并将该字符串显示在消息框中。 程序使用从 User32.dll 库导入的 MessageBox 方法。
class ExternTest
{
    [DllImport("User32.dll", CharSet=CharSet.Unicode)]
    public static extern int MessageBox(IntPtr h, string m, string c, int type);

    static int Main()
    {
        string myString;
        Console.Write("Enter your message: ");
        myString = Console.ReadLine();
        return MessageBox((IntPtr)0, myString, "My Message Box", 0);
    }

}
```

## lock

## sizeof
用于获取非托管类型的大小（以字节为单位）。 非托管类型包括下表列出的内置类型以及以下类型：
+ 枚举类型
+ 指针类型
+ 用户定义的结构，不包含任何属于引用类型的字段或属性
Expression	  常量值
+ sizeof(sbyte)	1
+ sizeof(byte)	1
+ sizeof(short)	2
+ sizeof(ushort)	2
+ sizeof(int)	    4
+ sizeof(uint)	4
+ sizeof(long)	8
+ sizeof(ulong)	8
+ sizeof(char)	2 (Unicode)
+ sizeof(float)	4
+ sizeof(double)	8
+ sizeof(decimal)	16
+ sizeof(bool)	1

## struct
struct 类型是一种值类型，通常用来封装小型相关变量组，例如，矩形的坐标或库存商品的特征。 下面的示例显示了一个简单的结构声明：
```csharp
public struct Book  
{  
    public decimal price;  
    public string title;  
    public string author;  
}
```

## volatile

## delegate
委托类型的声明与方法签名相似。 它有一个返回值和任意数目任意类型的参数：

## enum
enum 关键字用于声明枚举，一种包含一组被称为枚举数列表的已命名常数的不同类型.每个枚举类型都有一个基础类型，该基础类型可以是除 char 外的任何整型类型。 枚举元素的默认基础类型是 int。
```csharp
// Add the attribute Flags or FlagsAttribute.
[Flags]
public enum CarOptions
{
    // The flag for SunRoof is 0001.
    SunRoof = 0x01,
    // The flag for Spoiler is 0010.
    Spoiler = 0x02,
    // The flag for FogLights is 0100.
    FogLights = 0x04,
    // The flag for TintedWindows is 1000.
    TintedWindows = 0x08,
}

class FlagTest
{
    static void Main()
    {
        // The bitwise OR of 0001 and 0100 is 0101.
        CarOptions options = CarOptions.SunRoof | CarOptions.FogLights;

        // Because the Flags attribute is specified, Console.WriteLine displays
        // the name of each enum element that corresponds to a flag that has
        // the value 1 in variable options.
        Console.WriteLine(options);
        // The integer value of 0101 is 5.
        Console.WriteLine((int)options);
    }
}
/* Output:
   SunRoof, FogLights
   5
*/
```

## implicit
implicit 关键字用于声明隐式的用户定义类型转换运算符。 如果可以确保转换过程不会造成数据丢失，则可使用该关键字在用户定义类型和其他类型之间进行隐式转换。
```csharp
class Digit
{
    public Digit(double d) { val = d; }
    public double val;
    // ...other members

    // User-defined conversion from Digit to double
    public static implicit operator double(Digit d)
    {
        return d.val;
    }
    //  User-defined conversion from double to Digit
    public static implicit operator Digit(double d)
    {
        return new Digit(d);
    }
}

class Program
{
    static void Main(string[] args)
    {
        Digit dig = new Digit(7);
        //This call invokes the implicit "double" operator
        double num = dig;
        //This call invokes the implicit "Digit" operator
        Digit dig2 = 12;
        Console.WriteLine("num = {0} dig2 = {1}", num, dig2.val);
        Console.ReadLine();
    }
}
```

## interface

## object
object 类型是 .NET Framework 中 Object 的别名。 在 C# 的统一类型系统中，所有类型（预定义类型、用户定义类型、引用类型和值类型）都是直接或间接从 Object 继承的。 可以将任何类型的值赋给 object 类型的变量。
```csharp
class ObjectTest
{
   public int i = 10;
}

class MainClass2
{
   static void Main()
   {
      object a;
      a = 1;   // an example of boxing
      Console.WriteLine(a);
      Console.WriteLine(a.GetType());
      Console.WriteLine(a.ToString());

      a = new ObjectTest();
      ObjectTest classRef;
      classRef = (ObjectTest)a;
      Console.WriteLine(classRef.i);
   }
}
/* Output
    1
    System.Int32
    1
 * 10
*/
```

## override
扩展或修改继承的方法、属性、索引器或事件的抽象或虚拟实现需要 override 修饰符。不能重写非虚方法或静态方法。 重写基方法必须是 virtual、abstract 或 override。
```csharp
//在此示例中，Square 类必须提供 Area 的重写实现，因为 Area 继承自抽象 ShapesClass
abstract class ShapesClass
{
    abstract public int Area();
}
class Square : ShapesClass
{
    int side = 0;

    public Square(int n)
    {
        side = n;
    }
    // Area method is required to avoid
    // a compile-time error.
    public override int Area()
    {
        return side * side;
    }

    static void Main()
    {
        Square sq = new Square(12);
        Console.WriteLine("Area of the square = {0}", sq.Area());
    }

    interface I
    {
        void M();
    }
    abstract class C : I
    {
        public abstract void M();
    }

}
// Output: Area of the square = 144
``

## stackalloc

## unchecked
在未经检查的上下文中，如果表达式生成的值超出目标类型的范围，则不会标记溢出。 例如，由于以下示例中的计算在 unchecked 块或表达式中执行，因此将忽略计算结果对于整数而言过大的事实，并且向 int1 赋予值 -2,147,483,639。
```csharp
unchecked
{
    int1 = 2147483647 + 10;
}
int1 = unchecked(ConstantMax + 10);
```

## using static
using static 指令指定无需指定类型名称即可访问其静态成员的类型。
```csharp
using System;

public class Circle
{
   public Circle(double radius)
   {
      Radius = radius;
   }

   public double Radius { get; set; }

   public double Diameter
   {
      get { return 2 * Radius; }
   }

   public double Circumference
   {
      get { return 2 * Radius * Math.PI; }      
   }

   public double Area
   {
      get { return Math.PI * Math.Pow(Radius, 2); }
   }
}
```
---
通过消除每次引用成员时，显式引用 Math 类的需求，using static 指令将生成更简洁的代码：
```csharp
using System;
using static System.Math;

public class Circle
{
   public Circle(double radius)
   {
      Radius = radius;
   }

   public double Radius { get; set; }

   public double Diameter
   {
      get { return 2 * Radius; }
   }

   public double Circumference
   {
      get { return 2 * Radius * PI; }      
   }

   public double Area
   {
      get { return PI * Pow(Radius, 2); }
   }
}
```
