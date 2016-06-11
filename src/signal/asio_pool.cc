#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

void work(int i)
{
    std::cout<<i<<std::endl;
}

int main(int argc, char *argv[])
{
    boost::asio::io_service io;
    int i = 0;
    io.post(boost::bind(work,i));
    i=1;
    io.post(boost::bind(work,i));
    i=2;
    io.post(boost::bind(work,i));
    io.run();

    return 0;
}
