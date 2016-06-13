set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(Boost COMPONENTS thread system chrono REQUIRED)

include_directories(
    /usr/local/include
)

link_directories(
    /usr/local/lib
)

link_libraries(
    log4cplus
    ${Boost_LIBRARIES}
    )

add_executable(daytime daytime.cc)
add_executable(echo echo.cc)
add_executable(test_echo test_echo.cc)
