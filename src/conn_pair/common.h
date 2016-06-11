#ifndef __COMMON__H__
#define __COMMON__H__

#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/ref.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/smart_ptr.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include <boost/noncopyable.hpp>
#include <boost/serialization/singleton.hpp>

#include <spdlog/spdlog.h>

#include <memory>
/*
 * convert std::shared_ptr to boost::shared_ptr
 *
 */
/*
template<typename T> boost::shared_ptr<T> to_boost(const std::shared_ptr<T>& p)
{
    return boost::shared_ptr<T>(p.get(),[p](...) mutable {p.reset();});
}
*/
/*
class Log:public boost::serialization::singleton<Log>{

    private:
        Log(const char* name,bool color=true)
        {
            logger_ = spdlog::stdout_logger_mt(name,color);
        }

    public:
        std::shared_ptr<spdlog::logger> get_mutable_instance(){
            if(initialized)
            return logger_;
        }

    private:
        std::shared_ptr<spdlog::logger> logger_;

        static boost::atomic_bool initialized;
};
*/
#endif
