#ifndef __NET__COMMON__H__
#define __NET__COMMON__H__

#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/array.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>

#include <vector>
#include <fstream>
#include <string>
#include <iostream>
#include <stdio.h>

//typedef boost::asio::local::stream_protocol;

const char *SOCKNAME = "/tmp/test";

#define LOG_ERROR(FUNC_NAME,ERROR_CODE) do{\
    std::cerr<<FUNC_NAME<<":"<<ERROR_CODE.message()<<std::endl;\
    }while(0); \

#endif
