# asio
boost asio学习实践记录

## 为什么开这个repo?
- 在学习的过程中，发现使用`asio`很多的细节需要注意，因此萌生了专门记录这些细节的想法
- 在自己尝试使用异步方式编写网络程序时，对于很多次的实验，得出了自己的一些编程总结，
并不限于asio这一个库，还有boost相关组件如`bind`和`function`,以及智能指针的使用
- 培养自己从无到有建立一个完整的Linux项目，涉及`cmake`编写,`log4cplus`的配置和使用

## 结构

```
- asio
  - src
    - tcp         //TCP socket.
    - local       //UNIX domain socket.
    ...
  - test          //regular or benchmark test.
  ...
```
