#include "net.h"
#include "log.h"

log4cplus::Logger logger = log4cplus::Logger::getRoot();

void handle_close(boost::asio::ip::tcp::socket& s,const boost::system::error_code& ec);


int main(int argc, char *argv[])
{
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.cfg"));

    boost::asio::io_service io;
    boost::asio::ip::tcp::socket s(io);
    s.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),7832));

    s.send(boost::asio::buffer("1234"));
    boost::asio::streambuf buf;
    boost::asio::read_until(s,buf,'\n');

    //std::ostringstream out(buf);
    std::cout<<std::string(boost::asio::buffers_begin(buf.data()),
            boost::asio::buffers_end(buf.data()));

    s.shutdown(boost::asio::socket_base::shutdown_both);
    s.close();
    /*
    s.async_receive(boost::asio::null_buffers(),
            boost::bind(handle_close,
                boost::ref(s),
                boost::asio::placeholders::error));
    */
    io.run();

    return 0;
}

void handle_close(boost::asio::ip::tcp::socket& s,const boost::system::error_code& ec)
{
    if(!ec){
        //LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("waiting for close."));
        //boost::this_thread::sleep(boost::posix_time::microseconds(100));
        boost::asio::async_read(s,boost::asio::null_buffers(),
                boost::bind(handle_close,
                    boost::ref(s),
                    boost::asio::placeholders::error));

    }else if(ec==boost::asio::error::eof){
        s.shutdown(boost::asio::socket_base::shutdown_receive);
        s.close();
        LOG4CPLUS_INFO(logger,LOG4CPLUS_TEXT("closed connection."));
    }else{
        LOG4CPLUS_ERROR(logger,LOG4CPLUS_TEXT("close:"<<ec.message()));
    }
}
