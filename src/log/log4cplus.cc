/**
 * log4cplus确实是单例模式，多次获取都是同一对象
 *
 */

#include "common.h"

void foo(const char *log_name)
{
    log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT(log_name));
    log4cplus::Logger logger = log4cplus::Logger::getRoot();

    LOG4CPLUS_DEBUG(logger,LOG4CPLUS_TEXT("this is 1+1="<<1+1<<"."));

}

int main(int argc, char *argv[])
{
    foo("properties.cfg");
    foo("properties.cfg");
    return 0;
}
