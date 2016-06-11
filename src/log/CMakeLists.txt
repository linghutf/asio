set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories(
    ~/spdlog/include
)

link_libraries(
    log4cplus
    )
set(CMAKE_CXX_FLAGS "-std=c++11")

add_executable(spdlog_test spdlog_test.cc)
add_executable(log4_test log4cplus.cc)
