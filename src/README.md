# Asio相关细节

## 异步调用参数
1. 使用时不能在异步调用中bind局部变量,以防止资源在回调处理时已释放
2. 变量类似`deadline_timer t`,不能`bind(func,&t)`,而是`bind(func,boost::ref(t))`,以引用方式而不是指针方式传参

## Buffer
1. `async_read`等待传递的buffer大小被填满才会触发回调函数
2. 通过判断`error_code==boost::asio::error::eof`来判断对端是否关闭了写端,也可以通过判断`byte_transfered`是否大于0

## 流程设计
1. 设计安全的网络程序需要小心设计socket的断开操作,而不是依赖系统超时回收socket。一般流程如下:

  ```c++
  void handler(...)
  {
    //read or write handle finished
    ...

    //close write
    socket.shutdown(shutdown_type::shutdown_write);
    
    //waiting peer FIO signal(sync or async call)
    async_read(...)
    {
        //closed socket.
        if(error_code==EOF)
        {
            socket.close();
        }
    }
  }
  ```
