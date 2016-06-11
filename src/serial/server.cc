/*
 * unix domain socket 通讯
 * server 收到info请求，返回n个info的message,client打印出来，然后关闭连接
 */

#include "common.h"

class Session: public boost::enable_shared_from_this<Session>{
    public:
        Session(boost::asio::io_service& service):
            socket_(service)
    {
    }
        boost::asio::local::stream_protocol::socket& socket()
        {
            return socket_;
        }

        void start()
        {
            boost::asio::async_read(socket_,
                    boost::asio::buffer(&size_,sizeof(size_t)),
                    boost::bind(&Session::read_size_handler,
                        shared_from_this(),
                        boost::asio::placeholders::error));
        }


    private:
        void read_message_handler(const boost::system::error_code& ec)
        {
            if(!ec){
                //read messsage
                info::Info io;
                std::istream in(&buf_);
                io.ParseFromIstream(&in);
                std::cout<<io.id()<<","<<io.name()<<std::endl;

                //clear
                buf_.consume(buf_.size());

                //设置应答数据
                info::Message_t msgs;
                info::Info *p;
                for(int i=0;i<12;++i){
                    p = msgs.add_msg();
                    p->set_id(i);
                    p->set_name("response.");
                }

                //序列化
                std::ostream out(&buf_);
                msgs.SerializeToOstream(&out);

                //发送数据
                //首先发送length
                size_ = buf_.size();
                boost::asio::async_write(socket_,
                        boost::asio::buffer(&size_,sizeof(size_t)),
                        boost::bind(&Session::write_message_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error));

                return;
            }
        }

    private:

        void read_size_handler(const boost::system::error_code& ec)
        {
            if(!ec){
                boost::asio::async_read(socket_,
                        buf_,
                        boost::asio::transfer_exactly(size_),
                        boost::bind(&Session::read_message_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error));
            }else{
                std::cout<<ec.message()<<std::endl;
            }
        }

        void write_message_handler(const boost::system::error_code& ec)
        {
            if(!ec){
                //发送content
                boost::asio::async_write(socket_,
                        buf_,
                        boost::asio::transfer_exactly(size_),
                        boost::bind(&Session::finish_write_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error));
            }else{
                std::cerr<<ec.message()<<std::endl;
            }
        }

        //等待关闭
        void finish_write_handler(const boost::system::error_code& ec)
        {
            if(!ec){
                //关闭写端
                socket_.shutdown(boost::asio::socket_base::shutdown_send);
                //等待FIN信号
                boost::asio::async_read(socket_,
                        buf_,
                        boost::bind(&Session::finish_write_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error));
            }else if(ec == boost::asio::error::eof){
                socket_.shutdown(boost::asio::socket_base::shutdown_receive);
                socket_.close();
                std::cout<<"关闭连接."<<std::endl;
            }
        }

    private:
        boost::asio::local::stream_protocol::socket socket_;
        size_t size_;
        boost::asio::streambuf buf_;
};

class Server{
    public:
        Server(boost::asio::io_service& service,const char* pathname):
            service_(service),acceptor_(service,boost::asio::local::stream_protocol::endpoint(pathname))
    {
        start_accept();
    }

    private:

        void start_accept()
        {
            boost::shared_ptr<Session> new_session(new Session(service_));
            acceptor_.async_accept(new_session->socket(),
                    boost::bind(&Server::accept_handler,
                        this,new_session,
                        boost::asio::placeholders::error));
        }

        void accept_handler(boost::shared_ptr<Session> new_session,
                const boost::system::error_code& ec)
        {
            if(!ec){
                new_session->start();
            }else if((ec == boost::asio::error::eof)|(ec == boost::asio::error::connection_aborted)){
                acceptor_.close();
            }
            //listen another socket conn.
            start_accept();
        }

    private:
        boost::asio::io_service& service_;
        boost::asio::local::stream_protocol::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
    try{
        if(argc != 2){
            std::cerr<<"usage:"<<argv[0]<<" pathname."<<std::endl;
            return 1;
        }
        boost::asio::io_service service;

        ::unlink(argv[1]);
        Server serv(service,argv[1]);
        service.run();
    }catch(std::exception& e){
        std::cerr<<"except:"<<e.what()<<std::endl;
    }

    google::protobuf::ShutdownProtobufLibrary();
    return 0;
}
