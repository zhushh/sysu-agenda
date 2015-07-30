sysu-agenda
===========

##说明
project主要包含headers，src,bin,temp文件夹;其中headers用来存放头文件,src用来存放头文件的定义,bin目录保存生成的可执行文件以及保存数据的两个文件user.csv和meeting.csv,
temp目录存放的是一些自己进行阶段测试的代码拷贝;

##下载
git clone https://github.com/zhushh/sysu-agenda

##使用
先进行编译,编译需要编译出两个可执行程序,分别是server和client,执行下面命令即可:

<pre>$ make server</pre>

<pre>$ make client</pre>

执行完之后在bin目录下就有两个可执行程序server和client,分别表示服务器程序和客户端程序,然后运行客户端程序之前需要先运行服务器程序.运行后即可操作.目前代码还在完善当中.

