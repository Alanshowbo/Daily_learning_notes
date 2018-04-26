
g++是 linux下c++的编译器,在执行编译工作的时候，总共需要4步

1. 预处理,生成.i的文件
2. 将预处理后的文件不转换成汇编语言,生成文件.s
3. 有汇编变为目标代码(机器代码)生成.o的文件
4. 连接目标代码,生成可执行程序

g++ 编译c++经常使用的参数：

-c
```
只编译，不连接。例如： g++ -c helloworld.cpp
只生成helloworld.o不连接
```
-o
```
指定输出文件名。例如：g++ -c helloworld.cpp -o abc.o
默认是生成helloworld.o，用-o abc.o以后，就生成的是abc.o
```
-I
```
附加一个包函头文件的路径。例如：g++ helloworld.cpp -I"/usr/helloworld/include"
```
-l
```
小的L， 附一个库，例如要使用libabc.so 就g++ helloworld.cpp -labc
```
-L
```
添加一个库的路径，例如 g++ hello.cpp -L"/usr/hello/lib" -labc
```
-shared
```
生成动态库文件，例如： g++ -shared hellp.cpp -o libhello.so
```

调用动态库的时候有几个问题会经常碰到，有时，明明已经将库的头文件所在目录通过include进来了，库所在文件通过 “-L”参数引导，并指定了“-l”的库名，但通过ldd命令察看时，就是死活找不到你指定链接的so文件。其实编译链接上了共享库不代表执行时可以找到。所以“-L”什么的对执行没有用，你需要指明共享库的路径。方法有三个：
+ 修改 LD_LIBRARY_PATH，指明共享库的路径。LD_LIBRARY_PATH：这个环境变量指示动态连接器可以装载动态库的路径。在终端下使用如下命令：
[root@localhost sharelib]# export LD_LIBRARY_PATH = .
[root@localhost sharelib]# export LD_LIBRARY_PATH = your lib dir export
+ 通过/etc/ld.so.conf文件来指定动态库的目录。然后运行ldconfig命令更新搜索共享库的路径。通常这个做法就可以解决库无法链接的问题并且一劳永逸。
+ 或者把库文件拷贝到/lib下，然后ldconfig就ok了。这个方法有的取巧，且破坏原库文件的纯洁性，不应是首选方法。修改/etc/ld.so.conf文件，然后调用 /sbin/ldconfig需要有root权限，如果没有root权限，那么只能采用输出LD_LIBRARY_PATH的方法了。

linux下文件的类型是不依赖于其后缀名的，但一般来讲：

    .o 是目标文件,相当于windows中的.obj文件
    .so 为共享库,是shared object,用于动态连接的,和dll差不多
    .a 为静态库,是好多个.o合在一起,用于静态连接
    .la 为libtool自动生成的一些共享库，主要记录了一些配置信息。