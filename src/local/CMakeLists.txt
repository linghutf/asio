set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(local_serv server.cc)
