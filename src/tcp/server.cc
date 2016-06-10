#include "netcommon.h"

#include <iostream>

typedef boost::array<char,40> Buf;

Buf stream_buf;

void connect_domain()
{
    boost::asio::io_service serv;
    boost::asio::ip::tcp::resolver resolver(serv);
    boost::asio::ip::tcp::resolver::query query("www.baidu.com","80");
    boost::asio::ip::tcp::resolver::iterator iter = resolver.resolve(query);

    boost::asio::ip::tcp::endpoint ep = *iter;

}

void read_handler(boost::asio::ip::tcp::socket& sock,const boost::system::error_code& ec,size_t bytes_transferred)
{
    if(!ec){
        if(bytes_transferred>0){
            std::cout<<"comming here.\tby:"<<bytes_transferred<<std::endl;

            for(int i=9;i<bytes_transferred;++i){
                std::cout<<stream_buf[i];
            }
            std::cout<<std::endl;

            sock.shutdown(boost::asio::socket_base::shutdown_send);

            //等待关闭连接的信号
            boost::asio::async_read(sock,boost::asio::buffer(stream_buf),boost::bind(
                        read_handler,boost::ref(sock),boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
        }
    }else if(ec == boost::asio::error::eof){
        sock.close();
        std::cout<<"successfully closed Server."<<std::endl;
    }else{
        std::cout<<ec.message()<<std::endl;
    }

}

void write_handler(boost::asio::ip::tcp::socket& sock,const boost::system::error_code& ec)
{
    if(!ec){
        std::cout<<"comming here..."<<std::endl;
        //buffer 大小决定读取的大小，一旦超过buffer的预设大小，则异步调用生效
        //之前没有调用成功，是因为cache的size太大，发送数据没有达到buffer大小
        boost::asio::async_read(sock,boost::asio::buffer(stream_buf),boost::bind(
                    read_handler,boost::ref(sock),boost::asio::placeholders::error,boost::asio::placeholders::bytes_transferred));
    }else{
        std::cout<<ec.message()<<std::endl;
    }
}

void accept_handler(boost::asio::ip::tcp::socket& sock,const boost::system::error_code& ec)
{
    if(!ec){
        boost::asio::async_write(sock,boost::asio::buffer("hello,world.\n"),boost::bind(write_handler,
                    boost::ref(sock),boost::asio::placeholders::error));

    }else{
        std::cout<<ec.message()<<std::endl;
    }
}

int main(int argc, char *argv[])
{
    boost::asio::io_service service;

    //endpoint
    //boost::asio::ip::tcp::endpoint ep1; //ok
    //boost::asio::ip::tcp::endpoint ep2(boost::asio::ip::tcp::v4(),8086); //ok
    boost::asio::ip::tcp::endpoint ep3(boost::asio::ip::address::from_string("127.0.0.1"),8087);

    //socket
    boost::asio::ip::tcp::socket sock(service);
    //acceptor
    boost::asio::ip::tcp::acceptor ap(service,ep3);
    ap.async_accept(sock,boost::bind(accept_handler,boost::ref(sock),boost::asio::placeholders::error));

    service.run();

    return 0;
}
