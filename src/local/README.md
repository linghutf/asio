# UNIX domain socket

## 程序说明
1. server 和 client 通信，发送文件
2. server为发送方,client首先发送文件信息请求
3. server应答，查找文件，发送文件信息
4. client收到信息，做好文件接收准备
5. server传输文件完成,关闭写端,client读到shutdown信号,关闭连接,server随后关闭,终止会话.

## 关键点
1. tcp保证数据按序到达,可靠连接
2. tcp对数据的分片传输不提供有序性，需要自身应用保证
