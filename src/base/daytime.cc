#include "net.h"
#include "log.h"

#include <time.h>
#include <stdlib.h>

#include <string>
#include <boost/move/move.hpp>
log4cplus::Logger LOG = log4cplus::Logger::getRoot();

std::string toFormattedString(struct timeval *t)
{
    char buf[32]={0};
    time_t sec = static_cast<time_t>(t->tv_sec);
    struct tm tm_time;
    gmtime_r(&sec,&tm_time);

    snprintf(buf,sizeof(buf),"%4d-%02d-%02d %02d:%02d:%02d",
            tm_time.tm_year+1900,
            tm_time.tm_mon+1,
            tm_time.tm_mday,
            tm_time.tm_hour,
            tm_time.tm_min,
            tm_time.tm_sec);
    return buf;
}

class Connection:public boost::enable_shared_from_this<Connection>{
    public:
        //new_connection socket bind to io_service
        Connection(boost::asio::io_service& io):socket_(io){}

        void start()
        {
            struct timeval t;
            gettimeofday(&t,NULL);
            //localtime string
            str_ = boost::move(toFormattedString(&t));
            LOG4CPLUS_DEBUG(LOG,LOG4CPLUS_TEXT("sending "<<str_));

            boost::asio::async_write(socket_,boost::asio::buffer(str_),
                        boost::bind(&Connection::handle_close,
                            shared_from_this(),
                            boost::asio::placeholders::error
                        ));
        }

        boost::asio::ip::tcp::socket& socket()
        {
            return socket_;
        }

    private:

        void handle_close(const boost::system::error_code& ec)
        {
            if(!ec){
                LOG4CPLUS_DEBUG(LOG,LOG4CPLUS_TEXT("close connection..."));
                socket_.shutdown(boost::asio::socket_base::shutdown_both);
                socket_.close();
/*
                boost::asio::async_read(socket_,boost::asio::null_buffers(),
                        boost::bind(&Connection::handle_close,
                            shared_from_this(),
                            boost::asio::placeholders::error));
*/
            }else if(ec == boost::asio::error::eof){
                socket_.shutdown(boost::asio::socket_base::shutdown_receive);
                socket_.close();
                LOG4CPLUS_DEBUG(LOG,LOG4CPLUS_TEXT("connection closed."));
            }else{
                LOG4CPLUS_ERROR(LOG,LOG4CPLUS_TEXT("error:"<<ec.message()));
            }
        }

    private:
        std::string str_;
        boost::asio::ip::tcp::socket socket_;

};

class Server:boost::noncopyable{
    public:
        Server(boost::asio::io_service& io,short port):
            io_(io),acceptor_(io,boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),port))
    {
        start_accept();
    }

    private:

        void start_accept()
        {
            boost::shared_ptr<Connection> new_conn(new Connection(io_));
            acceptor_.async_accept(new_conn->socket(),
                    boost::bind(&Server::handle_accept,
                        this,
                        new_conn,
                        boost::asio::placeholders::error)
                    );
        }

        void handle_accept(boost::shared_ptr<Connection> new_conn,const boost::system::error_code& ec)
        {
            if(!ec){
                LOG4CPLUS_DEBUG(LOG,LOG4CPLUS_TEXT("connection established."));
                new_conn->start();
            }
            //无需判断
            /*
            }else if(ec == boost::asio::error::eof){
                acceptor_.close();
            }
            */
            start_accept();
        }

    private:
        boost::asio::io_service& io_;
        boost::asio::ip::tcp::acceptor acceptor_;
};
int main(int argc, char *argv[])
{
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.cfg"));
//    LOG = log4cplus::Logger::getRoot();

    boost::asio::io_service io;

    Server serv(io,8087);

    io.run();

    return 0;
}
