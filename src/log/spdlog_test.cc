/*
 *
 * 查看spdlog是否是单例模式
 *
 */
#include <spdlog/spdlog.h>
#include <memory>

std::shared_ptr<spdlog::logger> foo()
{
    auto log = spdlog::stdout_logger_mt("test",true);
    log->info("this is Test.");
    return log;
}

int main(int argc, char *argv[])
{
    auto log = spdlog::stdout_logger_mt("test",true);
    log->info("this is Test.");
    auto log2 = foo();
    log2->info(log==log2);

    return 0;
}
