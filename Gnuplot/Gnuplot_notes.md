# Gnuplot notes

1. 运行脚本
 + 把代码写在一个文件file中
  `gnuplot file`

2. 设置输出格式

    + PS格式：
        彩色（推荐使用）：
            set term postscript enhanced color
            set output "xx.ps"
    + EPS格式：
     黑白（曲线类型控制未知）：
            set term postscript eps enhanced
            set output "xx.eps"
    + PNG格式：
            set terminal png size 1500,1000 enhanced font "Helvetica,20"
            set output 'xx.png'
    + PDF格式需要装有ps2pdf，然后：
            set term postscript enhanced color
            set output "| ps2pdf - xx.pdf"。

3. 图片设置
    + 设置图片/字体大小
            set size 0.6,0.6
      字体大小设置比较复杂，此命令是按比例缩放图片大小，字体大小不变，相当于字体相对变大。
    + 设置/取消命名（位于图上方）
            set title "figurename""
            unset title
      注：在多图设置（见8小节）中，命名命令对其后所有图生效，要在每个图中重新命名。之后不需要命名时要记得取消。

4. 设置/取消背景网格
        set grid
        unset grid
   如果网格为极坐标系，则
        set grid polar

5. 设置图注
    + 图注在图中的位置
        set key left/right top/bottom/center
    + 图注增加/取消外框
            set/unset key box
    +  图注间距
            set key spacing 1.5
    +  图注位于图外
            set key lmargin/rmargin/tmargin/bmargin(below)
    +  取消图注
            unset key
    +  图注设置参数一览：
            set key {on|off} {default}
                         {{inside | outside} | {lmargin | rmargin | tmargin | bmargin}
                           | {at <position>}}
                         {left | right | center} {top | bottom | center}
                         {vertical | horizontal} {Left | Right}
                         {{no}reverse} {{no}invert}
                         {samplen <sample_length>} {spacing <vertical_spacing>}
                         {width <width_increment>}
                         {height <height_increment>}
                         {{no}autotitle {columnheader}}
                         {title "<text>"} {{no}enhanced}
                         {{no}box { {linestyle | ls <line_style>}
                                    | {linetype | lt <line_type>}
                                      {linewidth | lw <line_width>}}}
                 unset key
                 show key

6. 设置坐标轴
    + 设置坐标轴范围
        + 设置给定坐标轴范围
                set xrange/yrange [1:100]
          也可以设置负向坐标
                set xrange/yrange [100:1]
          只设置一端坐标（另一端将自动调整）
                set xrange/yrange [:100]
        + 恢复自动坐标轴范围（此时人工给定设置依然保留）
                set auto x/y
        + 恢复给定坐标轴范围
                set noauto x/y
    + 设置/取消坐标名称
            set xlabel/ylabel "name"
            unset xlabel/ylabel
    + 设置坐标轴标识
        + 设置坐标轴最大间隔（显示数值）
            set xtics/ytics 0.5
        + 设置坐标轴小间隔
            set mxtics/mytics 2
            说明：将每个大间隔平分为2份
        + 设置标识文本显示倾斜度
            set xtics rotate by -45
            说明：标识右倾45度
    + 设置指数坐标轴
            set log x/y
            set xtics 1.5
            说明：设置指数坐标轴的间隔时，数字代表10的指数（待明）。
            注意此时坐标的值域必须大于0。
    + 移动坐标标记
            set x/ytics [out] offset 1,1
            "1,1" 为对应的移动向量的x,y值.        
7. 作图－曲线
    + 从数据作图
        + 从不同数据文件作图
                plot "1p.dat" u 1:2 w lp lt 1 pt 1 lc 1 lw 3 title "1", \
                "2p.dat" u 1:2 w lp lt 1 pt 2 lc 7 lw 3 title "2", \
                "3p.dat" u 1:2 w lp lt 1 pt 3 lc 3 lw 3 title "3", \
                "4p.dat" u 1:2 w lp lt 1 pt 4 lc 4 lw 3 title "4"
                说明：
                u 1:2 ＝使用*.dat文件中的1，2列作为x，y作图；
                w lp ＝with line and points，作曲线并标出数据点；
                w l ＝with line，只作出曲线；
                w p ＝with points，只作数据点；
                这些似乎可以在plot之前通过一行命令整体设置：
                set style data linespoints（待测）
                ls＝linestyle 可自己设置，通过：
                set linestyle 1 lt 1 lc 2 lw 3
                lt ＝line type, 曲线类型。已定义，见图片；
                pt = point type, 描点类型。已定义，见⋯⋯；
                lc ＝line color, 曲线颜色。已定义，见图片；也可使用rgb数值：lc rgb "#00FF00"
                lw ＝line width, 曲线宽度。按数值依次增大；
                title 可简写做 ti ；
                使用数据所在列的抬头做title：title columnheader(n)
                n 代表第n列；
                如果不需要曲线名称，使用 notitle；
                plot "xxx.dat" u 1:2 notitle
                使用"\"分行，使代码更工整。
        + 从同一个数据文件作图
                plot "1p.dat" u 1:2 w l ti "col2", \
                "1p.dat" u 1:3 w l ti "col3"
                可简写为：
                plot "1p.dat" u 1:2 w l ti "col2", \
                " " u 1:3 w l ti "col3"
        + 作图命令内部循环 （待亲测！）
                plot 'dat.dat' u 2:xtic(1) title columnheader(2), \
                for [i=3:22] ' ' u i title columnheader(i)
        + 读取数据
                gnuplot默认数据分割符号是空格。如果文件中用逗号分割数据，可在gnuplot中使用如下命令：
                set datafile separator ","
8. 在特定位置放置标签
    + 位置坐标依照坐标轴
            set label "(0,0) first" at first 0, first 0
    + 位置坐标依照画图区域（图域左下角为（0，0））
            set label "(0,0) graph" at graph 0, graph 0
    + 位置坐标依照整个图片（图片左下角为（0，0））
            set label "(0,0) screen" at screen 0, screen 0
    + 画任意线段（依照坐标轴）
            set arrow from x0,y0 to x1,y1 nohead
9. 多图设置
    使用范例说明如下：
    此为四小图横向排列。
            set multiplot
    说明：多图设置开始
            set size 0.27,1
    说明：定义每张小图的尺寸。可在每张小图开始前分别设置。
            set origin -0.005,0
    说明：设置图1的起始位置。以左下角坐标为准。
            plot "xxxline1.dat" u 1:2 w lp title "line1"
            set origin 0.24,0
    说明：设置图2的起始位置。
            plot "line2.dat" u 1:2 w lp title "line2"
                set origin 0.485,0
    说明：设置图3的起始位置。
            plot "line3.dat" u 1:2 w lp title "line3"
            set origin 0.73,0
    说明：设置图4的起始位置。
            plot "line4.dat" u 1:2 w lp title "line4"
            unset multiplot

10. 作直方图 （histogram）
        set style fill solid 1.00 border 0
        说明：或者border -1, 暂时不清楚作用……
        set boxwidth 0.9 absolute
        说明：设置方柱宽度
        set style histogram
        set style data histogram (or 'histograms')
        set style histogram clustered gap 1 title offset character 0, 0, 0
        说明：不清楚……
        plot 'dat.dat' u 2:xtic(1) ti col lc 1
        注意：此处x，y的顺序是相反的。
