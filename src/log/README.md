# log库总结
1. log4cplus基于文件配置能够作为单例使用，而spdlog在重复初始化一个log时，会抛出异常
2. 经测试，log4cplus在普通磁盘上，多线程异步写日志是spdlog的2倍左右
3. SSD上，log4cplus性能反而不如spdlog,需要仔细重新测试,也可能是配置参数的问题
4. log4cplus异步方式v1.2.0+提供，但是在GCC新版本会使用C++11模式编译，导致异步log时失败(`queue failed.`),猜测是auto_ptr的问题,需要在编译时加上`CXXFLAGS=-std=c++03`来避免使用新标准
