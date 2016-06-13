#ifndef __LOG__H__
#define __LOG__H__

#include <log4cplus/logger.h>
#include <log4cplus/loggingmacros.h>
#include <log4cplus/configurator.h>
#include <log4cplus/layout.h>
#include <log4cplus/helpers/loglog.h>

#include <boost/smart_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/thread/mutex.hpp>
namespace xlog{
    class Log:boost::noncopyable{
        public:
            static log4cplus::Logger get_instance()
            {
                return log_;
            }
        private:
            Log()
            {
                log4cplus::PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log.cfg"));
                log_ = log4cplus::Logger::getRoot();
            }
            static const Log *instance_;
            static log4cplus::Logger log_;
    };

}


#endif
