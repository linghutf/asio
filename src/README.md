# Asio相关细节

## Asio运行机制
1. io_service 可以当做线程池使用,在事件循环run之前post工作handler

## 异步调用参数
1. 使用时不能在异步调用中bind局部变量,以防止资源在回调处理时已释放
2. 变量类似`deadline_timer t`,不能`bind(func,&t)`,而是`bind(func,boost::ref(t))`,以引用方式而不是指针方式传参
3. 作为参数的`boost::system::error_code& `必须是`const`类型，否则在bind参数的情况下无法通过编译

## Buffer
1. `async_read`等待传递的buffer大小被填满才会触发回调函数
2. `async_write`传递buffer不带上大小参数，默认全发送，如果数据没有填满，会导致接收方数不到数据
3. 通过判断`error_code==boost::asio::error::eof`来判断对端是否关闭了写端,也可以通过判断`byte_transfered`是否大于0

## 流程设计
1. 设计安全的网络程序需要小心设计socket的断开操作,而不是依赖系统超时回收socket。一般流程如下:

  ```c++
  //参考serial/server client 关闭处理

  void handler(...)
  {
    //read or write handle
    ...

    //close write
    socket.shutdown(boost::asio::socket_base::shutdown_send);
    
    //waiting peer FIO signal(sync or async call)
    finish_conn_handler(...)
    {
        //closed socket.
        if(error_code==EOF)
        {
            socket.shutdown(boost::asio::socket_base::shutdown_receive);
            socket.close();
        }
    }
  }
  ```
