# 实验2：进程间通信

## 实验目的：

1. 了解管道通信的特点，掌握管道通信的使用方法
2. 了解消息队列通信机制及原理，掌握消息队列相关系统调用的使用方法及功能

## 实验环境

Linux操作系统

（P.S. 我没有装虚拟机，是在WSL上编译运行的，然后使用`<sys/msg.h>`里的`msgget`和`msgsnd`之类的函数都会报错"Function not implemented" (error code ENOSYS)）

## 使用方法

```bash
./scripts/run.sh
```

