#include "netcommon.h"

using domain = boost::asio::local::stream_protocol;


class AsyncConnection:public boost::enable_shared_from_this<AsyncConnection>{
    public:
        AsyncConnection(boost::asio::io_service& service):
            socket_(service),filesize_(0)
    {
    }
        void start()
        {
            boost::asio::async_read(socket_,boost::asio::buffer(cache_),
                    boost::bind(&AsyncConnection::handle_read_request,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));
        }

        boost::asio::local::stream_protocol::socket& socket()
        {
            return socket_;
        }


    private:

        void handle_read_request(const boost::system::error_code& ec,size_t bytes_transferred)
        {
            if(!ec){
                return;
            }
        }

    private:

        boost::asio::local::stream_protocol::socket socket_;
        size_t filesize_;
        char cache_[BUFSIZ];

};

int main(int argc, char *argv[])
{
    boost::asio::io_service service;

    ::unlink(SOCKNAME);



    return 0;
}
