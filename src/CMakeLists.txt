cmake_minimum_required(VERSION 2.6)

set(CMAKE_CXX_FLAGS "-O2")

find_package( Boost REQUIRED thread system chrono )
if(Boost_FOUND )
    message(STATUS "founded boost-lib.")
endif()

link_libraries( ${Boost_LIBRARIES} )

add_subdirectory(timer)
add_subdirectory(tcp)
add_subdirectory(local)
