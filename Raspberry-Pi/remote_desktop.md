1. 必须先安装tightvncserver

sudo apt-get install tightvncserver

2. 再安装xrdp服务。

sudo apt-get install xrdp

3. 如果开着防火墙ufw ， 那么打开服务器上的远程桌面访问端口

sudo ufw allow 3389

4. 重启两个服务

sudo service ufw restart
sudo service xrdp restart

