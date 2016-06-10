#include "netcommon.h"

class AsyncConnection:public boost::enable_shared_from_this<AsyncConnection>{
    public:
        AsyncConnection(boost::asio::io_service& service):
            socket_(service),bufsize_(),filesize_(0)
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

        boost::asio::local::stream_protocol& socket()
        {
            return socket_;
        }

        virtual ~AsyncConnection()
        {}

    private:

        void handle_read_request(const boost::system::error_code& ec,size_t bytes_transferred)
        {
            if(!ec){
                return;
            }
        }

    private:

        boost::asio::local::stream_protocol socket_;
        size_t bufsize_;
        size_t filesize_;

};

int main(int argc, char *argv[])
{
    boost::asio::io_service service;

    ::unlink(SOCKNAME);

    boost::

    return 0;
}
