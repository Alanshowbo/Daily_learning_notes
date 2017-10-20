# WordPress 个人博客搭建
---
> Created in 20170718 by YunShuiXin

## Step 1 域名购买、服务器deploy
+ 域名可直接从阿里云购买，同时可购买域名解析服务
+ 服务器可选择搬瓦工、Vultr、阿里云等。阿里云针对学生有优惠，但对于普通用户价格较贵。Vultr速度一般，有亚洲节点（日本、新加坡）一个月5美元的配置应当能够满足个人博客要求；搬瓦工最便宜，低配一个月3美元，但没有亚洲节点，延迟较低的服务器是洛杉矶，延迟大概在200ms。
+ 在阿里云的控制台中将域名和服务器IP地址绑定
> [万网域名解析设置方法](https://help.aliyun.com/knowledge_detail/39783.html)

## Step 2 服务器端配置

### 更新
+ 使用putty/xshell远程ssh连接
+ `sudo apt-get update`
+ `sudo apt-get upgrade`
+ `sudo apt-get install python-pip`

### [安装LNMP](https://lnmp.org/install.html)
        apt-get install screen
        screen -S lnmp
        wget -c http://soft.vpser.net/lnmp/lnmp1.4.tar.gz && tar zxf lnmp1.4.tar.gz && cd lnmp1.4 && ./install.sh lnmp

+ 目前提供了较多的MySQL、MariaDB版本和不安装数据库的选项，需要注意的是MySQL 5.6,5.7及MariaDB 10必须在1G以上内存的更高配置上才能选择，不确定可直接选default
+ 之后设置password（设置过程中删除需按住ctrl加backspace）
+ InnoDB引擎默认为开启，一般建议开启
+ PHP版本选择
+ 是否安装内存优化，默认不安装
+ install开始，视情况需要几十分钟
+ 如果显示Nginx: OK，MySQL: OK，PHP: OK，并且Nginx、MySQL、PHP都是running，80和3306端口都存在，并提示安装使用的时间及Install lnmp V1.4 completed! enjoy it.的话，说明已经安装成功。

### [安装虚拟主机](https://lnmp.org/faq/lnmp-vhost-add-howto.html)

+ `lnmp vhost add`
+ 输入域名，添加更多域名（带www和不带表示不同的域名，可将这两个加入）
+ 设置网站目录，回车采用默认：/home/wwwroot/域名
+ Allow rewrite rule:Y
+ enter the rewrite of programme:wordpress
+ Allow access log:Y
+ enter ascess log filename:回车，默认日志目录为：/home/wwwlogs/ 默认文件名为：域名.log
+ Creat database and MySQL user with same name? 选Y或N均可
+ add ssl certificate? 选择N，
+ 虚拟主机配置文件在：/usr/local/nginx/conf/vhost/域名.conf，nano打开，在root/home/wwwroot/www.vpser.net;这一行下面添加：`include wordpress.conf;`
+ 主机删除：
        lnmp vhost del
        chattr -i /网站目录/.user.ini
        sudo rm -rf 目录名

### [安装WordPress](https://codex.wordpress.org/zh-cn:%E5%AE%89%E8%A3%85WordPress)
        cd ~
        wget http://wordpress.org/latest.tar.gz
        tar -xzvf latest.tar.gz
+ 在一键安装LNMP的时候已经安装了phpMyAdmin，通过[服务器ip]/phpmyadmin可以访问数据库后台。为了安全起见要改一下phpmyadmin这个文件夹的名字，它位于路径/home/wwwroot/default/
+ 使用phpMyAdmin建立wordpress数据库和权限。
        cd ~ wordpress
        mv wp-config-sample.php wp-config.php
        nano wp-config.php
    + DB_NAME：在第二步中为WordPress创建的数据库名称
    + DB_USER：在第二步中创建的WordPress用户名
    + DB_PASSWORD：第二步中为WordPress用户名设定的密码
    + DB_HOST：第二步中设定的hostname（通常是localhost）
+ 将wordpress文件夹里的所有文件（不包括wordpress这个文件夹本身）移动到网站的根目录下，例如/home/wwwroot/你的域名文件夹。`mv * /home/wwwroot/域名`
+ wordpress切换中文：通过FTP、SSH等方式打开并编辑站点根目录下的wp-config.php文件，查找define('WPLANG', '');一行，在第二个参数处填入zh_CN，变成define('WPLANG', 'zh_CN');并保存文件。进入站点控制板（dashboard），看到更新提示后进行升级即可。WordPress会自动从官方网站下载中文语言包并安装
+ 访问域名，ENJOY ~

## Step 3 网站显示优化
### Font Family:[如何优雅的选择字体(font-family)](https://segmentfault.com/a/1190000006110417)
+ 在不同操作系统、不同游览器里面默认显示的字体是不一样的，并且相同字体在不同操作系统里面渲染的效果也不尽相同。
+ 各平台的默认字体情况：
  + Window下：宋体（SimSun），微软雅黑（"Microsoft Yahei"），Arial，Tahoma，Verdana，其它。
  + Mac OS下：华文黑体（STHeiti）、华文细黑（STXihei），黑体-简（Heiti SC），冬青黑体（ Hiragino Sans GB ），Times New Roman，Helvetica、Helvetica Neue，苹方（PingFang SC），San Francisco，其它。
  + Android系统：Droid Sans、Droid Sans Fallback，Droid Sans为安卓系统中默认的西文字体，是一款人文主义无衬线字体，而Droid Sans Fallback则是包含汉字、日文假名、韩文的文字扩展支持。
  + iOS系统：同Mac。
  + 绝大部分中文字体里都包含英文字母和数字，不进行英文字体声明是没有问题的，但是大多数中文字体中的英文和数字的部分都不是特别漂亮，所以建议也对英文字体进行声明。
  + 照顾不同的操作系统：
    + 英文、数字部分：在默认的操作系统中，Mac和Win都会带有Arial, Verdana, Tahoma等几个预装字体，从显示效果来看，Tahoma要比Arial更加清晰一些，因此字体设置Tahoma最好放到前面，当找不到Tahoma时再使用Arial；而在Mac中，还拥有一款更加漂亮的Helvetica字体，所以为了照顾Mac用户有更好的体验，应该更优先设置Helvetica字体；Android系统下默认的无衬线字体就可以接受，因此无需单独设置。最后，英文、数字字体的最佳写法如下：
            font-family: Helvetica, Tahoma, Arial;
    + 中文部分：在Win下，微软雅黑为大部分人最常使用的中文字体，由于很多人安装Office的缘故，Mac电脑中也会出现微软雅黑字体，因此把显示效果不错的微软雅黑加入到字体列表是个不错的选择；同样，为了保证Mac中更为优雅字体苹方（PingFang SC）、黑体-简（Heiti SC）、冬青黑体（ Hiragino Sans GB ）的优先显示，需要把这些字体放到中文字体列表的最前面；同时为了照顾到Linux操作系统的体验，还需要添加文泉驿微米黑字体。最后，中文字体部分最佳写法如下：
            font-family: "PingFang SC", "Hiragino Sans GB", "Heiti SC", "Microsoft YaHei", "WenQuanYi Micro Hei";
    + 中英文整合写法：
            font-family: Helvetica, Tahoma, Arial, "Heiti SC", "Microsoft YaHei", "WenQuanYi Micro Hei";
            font-family: Helvetica, Tahoma, Arial, "PingFang SC", "Hiragino Sans GB", "Heiti SC", "Microsoft YaHei", "WenQuanYi Micro Hei";
    + 补充字体族名称：字体族大体上分为两类：sans-serif（无衬线体）和serif（衬线体），当所有的字体都找不到时，我们可以使用字体族名称作为操作系统最后选择字体的方向。一般非衬线字体在显示器中的显示效果会比较好，因此我们需要在最后添加 sans-serif，写法如下：
            font-family: Helvetica, Tahoma, Arial, "华文中宋", "PingFang SC", "Hiragino Sans GB", "Heiti SC", "Microsoft YaHei", "WenQuanYi Micro Hei", sans-serif;
+ 大公司的常见写法：
  + 淘宝：
        font: 12px/1.5 tahoma,arial,'Hiragino Sans GB','\5b8b\4f53',sans-serif;
  + 小米：
        font: 14px/1.5 "Helvetica Neue",Helvetica,Arial,"Microsoft Yahei","Hiragino Sans GB","Heiti SC","WenQuanYi Micro Hei",sans-serif;
  + 简书：
        font-family: "lucida grande", "lucida sans unicode", lucida, helvetica, "Hiragino Sans GB", "Microsoft YaHei", "WenQuanYi Micro Hei", sans-serif;
+ 其他的一些注意点
  + 字体何时需要添加引号？当字体具体某个取值中若有一种样式名称包含空格，则需要用双引号或单引号表示，如：
        font-family: "Microsoft YaHei", "Arial Narrow", sans-serif;
  + 如果书写中文字体名称为了保证兼容性也会添加引号，如：
        font-family: "黑体-简", "微软雅黑", "文泉驿微米黑";
+ 个人觉得优秀的西文字体：Adobe Hebrew，Times New Roman，Minion Pro
        font-family: "Minion Pro", "Adobe Hebrew", Helvetica, Tahoma, Arial, "PingFang SC", "Hiragino Sans GB", "Heiti SC", "Microsoft YaHei", "WenQuanYi Micro Hei", sans-serif;

## Step 4 网站速度优化

  + 可使用七牛插件实现静态资源加速；对于备案的网站，还可实现CDN加速；个人试用感觉效果一般，加载图片不如原网站快，可能带宽有限，对于文字网页可能效果较好。
  + 图片资源可使用压缩插件，个人尝试效果一般；
  + 开启缓存功能，安装插件WP Fastest Cache/WP Super Cache
