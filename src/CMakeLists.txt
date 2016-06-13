cmake_minimum_required(VERSION 3.0)

set(CMAKE_CXX_FLAGS "-O2")

find_package( Boost REQUIRED thread system chrono )
if(Boost_FOUND )
    message(STATUS "founded boost-lib.")
endif()

link_libraries( ${Boost_LIBRARIES} )

add_subdirectory(timer)
add_subdirectory(tcp)
add_subdirectory(local)
add_subdirectory(serial)
add_subdirectory(signal)
add_subdirectory(conn_pair)
add_subdirectory(log)
add_subdirectory(base)
