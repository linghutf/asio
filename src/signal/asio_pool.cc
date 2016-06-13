#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/timer.hpp>

void work(int i)
{
    std::cout<<i<<std::endl;
}

void async_timer(const boost::system::error_code& ec,boost::asio::deadline_timer& t,int &count)
{
    if(!ec){
        if(count<10){
            std::cout<<"loop:"<<count++<<'\n';
            t.expires_from_now(boost::posix_time::seconds(1));
            t.async_wait(boost::bind(async_timer,boost::asio::placeholders::error,boost::ref(t),boost::ref(count)));
        }
    }else{
        std::cerr<<ec.message()<<'\n';
    }
}

/*
void fib_async(boost::asio::io_service& io,unsigned int n,unsigned int& sum){
    if(n==1||n==2){
        sum = 1;
        std::cout<<"id:"<<boost::this_thread::get_id()<<",val="<<sum<<'\n';
    }else{
        sum += n-1;
        io.post(boost::bind(boost::ref(io),n-1,))

    }
}
*/
int main(int argc, char *argv[])
{
    boost::asio::io_service io;
    int i = 0;
    io.post(boost::bind(work,i));
    i=1;
    io.post(boost::bind(work,i));
    i=2;
    io.post(boost::bind(work,i));


    boost::asio::deadline_timer t(io,boost::posix_time::seconds(1));
    int count = 0;
    t.async_wait(boost::bind(async_timer,boost::asio::placeholders::error,boost::ref(t),boost::ref(count)));
    //io.post(boost::bind(async_timer,boost::asio::placeholders::error,boost::ref(t),boost::ref(count)));
    io.run();

    return 0;
}
