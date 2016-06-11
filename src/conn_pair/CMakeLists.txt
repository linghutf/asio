set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

include_directories(
    ~/spdlog/include
)

set(CMAKE_CXX_FLAGS "-std=c++11")
add_executable(pair pair.cc)
