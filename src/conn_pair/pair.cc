#include "common.h"

#include <boost/thread.hpp>
#include <boost/array.hpp>
#include <vector>

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

using domain = boost::asio::local::stream_protocol;

std::shared_ptr<spdlog::logger> LOGGER = spdlog::stdout_logger_mt("test",true);

class UpperCaseFilter:public boost::enable_shared_from_this<UpperCaseFilter>{
    public:
        UpperCaseFilter(boost::asio::io_service& service)
            :socket_(service)
        {
        }

        domain::socket& socket()
        {
            return socket_;
        }

        void start()
        {
            LOGGER->debug("start listening...");
            //waiting for request
            socket_.async_read_some(boost::asio::buffer(data_),
                    boost::bind(&UpperCaseFilter::read_handler,
                        shared_from_this(),
                        boost::asio::placeholders::error,
                        boost::asio::placeholders::bytes_transferred));

        }

    private:
        void read_handler(const boost::system::error_code& ec, size_t size)
        {
            if(!ec){
                LOGGER->debug("recving data --- {1} size;{0}",size,data_[0]);
                for(size_t i = 0;i < size;++i){
                    data_[i] = std::toupper(data_[i]);
                }

                LOGGER->debug("handles data --- {0},{1} ",data_[0],data_[size-1]);

                //send result
                //需要带上长度，否则默认array全发送,然而数据没有填满,导致客户端收不到数据
                boost::asio::async_write(socket_,boost::asio::buffer(data_,size),
                        boost::bind(&UpperCaseFilter::write_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error));
            }else{
                std::cerr<<__FUNCTION__<<":"<<ec.message()<<'\n';
            }
        }

        void write_handler(const boost::system::error_code& ec)
        {
            if(!ec){
                LOGGER->debug("resp sended.waiting Request...");
                socket_.async_read_some(boost::asio::buffer(data_),
                        boost::bind(&UpperCaseFilter::read_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error,
                            boost::asio::placeholders::bytes_transferred));
            }else{
                std::cerr<<__FUNCTION__<<":"<<ec.message()<<'\n';
            }
        }

        void close_handler(const boost::system::error_code& ec)
        {
            if(!ec){

                //close write
                socket_.shutdown(boost::asio::socket_base::shutdown_send);

                //waiting for close read
                boost::asio::async_read(socket_,boost::asio::buffer(data_),
                        boost::bind(&UpperCaseFilter::close_handler,
                            shared_from_this(),
                            boost::asio::placeholders::error));
            }else if(ec == boost::asio::error::eof){
                //close read
                socket_.shutdown(boost::asio::socket_base::shutdown_receive);
                socket_.close();

                std::cout<<"connection closed.\n";
            }else{
                std::cerr<<__FUNCTION__<<":"<<ec.message()<<'\n';
            }
        }

    private:
        domain::socket socket_;
        boost::array<char,512> data_;
        //boost::asio::streambuf buf_;
};


void run_service(boost::asio::io_service* service)
{
    try{
        LOGGER->debug("service running...");
        service->run();
    }catch(std::exception& e){
        std::cerr<<e.what()<<'\n';
    }
}


int main(int argc, char *argv[])
{

    try{
        LOGGER->set_level(spdlog::level::debug);

        boost::asio::io_service service;

        boost::shared_ptr<UpperCaseFilter> serv(new UpperCaseFilter(service));
        domain::socket socket(service);
        boost::asio::local::connect_pair(socket,serv->socket());

        serv->start();

        //run
        boost::thread t(boost::bind(run_service,&service));

        //request
        std::string request;
        while(true){

            std::cin>>request;
            //send request
            boost::asio::write(socket,boost::asio::buffer(request));
            LOGGER->debug("send --- {0}",request);

            //waiting for response
            std::vector<char> response(request.size());
            boost::asio::read(socket,boost::asio::buffer(response));
            LOGGER->debug("recv --- {0}",std::string(response.begin(),response.end()));
            //show response
            std::cout<<"resp:";
            std::cout.write(&response[0],request.size());
            std::cout<<'\n';

        }

    }catch(std::exception& e){
        std::cerr<<e.what()<<'\n';
        return 1;
    }

    return 0;
}

#else
#error UNIX domain socket not supported by the platform.
#endif
