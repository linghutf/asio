/**
 * 由于asio::buffer使用array,因此对array测试是否能取到end
 */

#include <boost/array.hpp>
#include <iostream>

int main(int argc, char *argv[])
{
    boost::array<int,12> a ={1,2};
    //是最后一个元素并非2,所以array不能记录写入数量的多少
    std::cout<<*(a.end()-1)<<'\n';
    //无法取到填充的大小
    std::cout<<"size:"<<a.size()<<",max_size:"<<a.max_size()<<'\n';
    return 0;
}
