/**
 * 对象存活测试
 */

#include <boost/smart_ptr.hpp>

#include <iostream>

void test_expire()
{
    boost::shared_ptr<int> p;
    boost::weak_ptr<int> w(p);//false
    std::cout<<"expired:"<<w.expired()<<'\n';

}

int main(int argc, char *argv[])
{
    boost::shared_ptr<int> q;
    boost::weak_ptr<int> p;//true
    std::cout<<"expired:"<<p.expired()<<"\n,unique:"<<q.unique()<<'\n';

    test_expire();
    return 0;
}
