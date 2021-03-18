# v1.0

1. tcp流程图

   ![image-20210317103347149](https://gitee.com/xsm970228/images2020.9.5/raw/master/20210317103352.png)

2. server端

   - accept到一个链接就创建一个子进程处理，父进程一直accept
   - 子进程不停的接受数据放在msg中，收到数据就打印出来
   - 为了防止server端先断开连接而使端口处在TIME_WAIT状态，使用延时关闭（0s）和端口复用

3. client端

   建立tcp链接后不停的发送数据

# v2.0

将v1.0代码进行封装

- head：放头文件
- src：放源文件
- bin：放编译好的可执行文件
- makefile：使用make进行编译