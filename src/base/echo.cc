#include "net.h"
#include "log.h"

typedef boost::asio::ip::tcp socket_t;
typedef boost::system::error_code error_type;

log4cplus::Logger logger = log4cplus::Logger::getRoot();

class Conn:public boost::enable_shared_from_this<Conn>,boost::noncopyable{
    public:
        Conn(boost::asio::io_service& io):socket_(io)
    {
    }

        void start()
        {
            //改用read_some防止不触发回调
            socket_.async_read_some(
            /*boost::asio::async_read(socket_,*/boost::asio::buffer(buf_),
                    boost::bind(&Conn::handle_read,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
        }

        socket_t::socket& socket()
        {
            return socket_;
        }

    private:
        void handle_read(const error_type& ec,size_t bytes_transferred)
        {
            if(!ec){
                LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("recving size:"<<bytes_transferred));
                boost::shared_ptr<std::string> pstr(
                        new std::string(/*
                            boost::asio::buffers_begin(buf_.data()),
                            boost::asio::buffers_end(buf_.data())));
                            */
                            buf_.begin(),buf_.begin()+bytes_transferred));
                //buf_.consume(pstr->size());
                pstr->push_back('\n');
                LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("sending ["<<*pstr<<']'));

                boost::asio::async_write(socket_,
                        boost::asio::buffer(*pstr),
                        boost::bind(&Conn::handle_write,
                            shared_from_this(),
                            pstr,
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

            }else if(ec == boost::asio::error::eof){
                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("waiting for close."));
                socket_.shutdown(boost::asio::socket_base::shutdown_send);
                socket_.close();
                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("closed."));
                /*
                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("closed."));

                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("waiting for FIN-ACK."));
                //socket_.shutdown(boost::asio::socket_base::shutdown_send);
                //等待FIN的ACK
                boost::asio::async_read(socket_,
                        boost::asio::null_buffers(),
                        boost::bind(&Conn::handle_close,
                            shared_from_this(),
                            boost::asio::placeholders::error));
                */
            }else{
                LOG4CPLUS_ERROR(logger,LOG4CPLUS_TEXT("read:"<<ec.message()));
            }
        }

        void handle_write(boost::shared_ptr<std::string> pstr,const error_type& ec,size_t bytes_transferred)
        {
            if(!ec){
                LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("send bytes:"<<bytes_transferred));
                //continue
                socket_.async_read_some(boost::asio::buffer(buf_),
                        boost::bind(&Conn::handle_read,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));

            }else if(ec == boost::asio::error::eof){
                //说明对方发起了主动关闭
                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("waiting for close."));
                socket_.shutdown(boost::asio::socket_base::shutdown_send);
                socket_.close();
                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("closed."));
                /*
                //等待FIN的ACK
                boost::asio::async_read(socket_,
                        boost::asio::null_buffers(),
                        boost::bind(&Conn::handle_close,
                            shared_from_this(),
                            boost::asio::placeholders::error));
                */
            }else{
                LOG4CPLUS_ERROR(logger,LOG4CPLUS_TEXT("read:"<<ec.message()));
            }
        }

        //not use
        void handle_close(const boost::system::error_code& ec){
            if(!ec){
                boost::asio::async_read(socket_,
                        boost::asio::null_buffers(),
                        boost::bind(&Conn::handle_close,
                            shared_from_this(),
                            boost::asio::placeholders::error));

            }else if(ec == boost::asio::error::eof){
                LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("closed successfully."));
                socket_.shutdown(boost::asio::socket_base::shutdown_receive);
                socket_.close();
            }else{
                LOG4CPLUS_ERROR(logger,LOG4CPLUS_TEXT("close:"<<ec.message()));
            }
        }
    private:
        socket_t::socket socket_;
        boost::array<char,4096> buf_;
        //boost::asio::streambuf buf_;
};

class Server:boost::noncopyable{
    public:
        Server(boost::asio::io_service& io,unsigned short port):io_(io),acceptor_(io,socket_t::endpoint(socket_t::v4(),port))
    {
        start_accept();
    }

    private:
        void start_accept()
        {
            boost::shared_ptr<Conn> new_conn(new Conn(io_));
            acceptor_.async_accept(new_conn->socket(),
                    boost::bind(&Server::handle_accept,
                        this,
                        new_conn,
                        boost::asio::placeholders::error));
        }

        void handle_accept(boost::shared_ptr<Conn> conn,const error_type& ec)
        {
            if(!ec){
                LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("connection established."));
                conn->start();
            }else{
                LOG4CPLUS_ERROR(logger,LOG4CPLUS_TEXT("accept:"<<ec.message()));
            }
            start_accept();
        }
    private:
        boost::asio::io_service& io_;
        socket_t::acceptor acceptor_;
};

int main(int argc, char *argv[])
{
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.cfg"));

    boost::asio::io_service io;
    Server s(io,7832);
    io.run();

    logger.shutdown();


    return 0;
}
