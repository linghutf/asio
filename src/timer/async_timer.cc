#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/bind.hpp>
#include <iostream>

//使用timer指针传参出错,引用传参正常
void work(const boost::system::error_code& ec,boost::asio::deadline_timer& t,int *count)
{
    if(!ec){
        if((*count)<5){
            std::cout<<"working..."<<std::endl;
            ++*count;
            t.expires_from_now(boost::posix_time::seconds(1));
            t.async_wait(boost::bind(&work,boost::asio::placeholders::error,boost::ref(t),count));
        }
    }else{
        std::cout<<ec.message()<<std::endl;
    }
}

int main(int argc, char *argv[])
{
    int count = 0;
    boost::asio::io_service ios;
    boost::asio::deadline_timer t(ios,boost::posix_time::seconds(2));
    t.async_wait(boost::bind(&work,boost::asio::placeholders::error,boost::ref(t),&count));
    //t.async_wait(boost::bind(&work,&t,boost::asio::placeholders::error,count));//after 2s call work
    ios.run();
    std::cout<<"exit."<<std::endl;

    return 0;
}
