set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} )

find_package(Protobuf REQUIRED)
link_libraries(
    protobuf
    pthread
    )
file(GLOB ProtoFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.proto)
message(STATUS "ProtoFiles = ${ProtoFiles}")
PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS ${ProtoFiles} )


add_executable(msg_client client.cc ${PROTO_SRCS} )
add_executable(msg_server server.cc ${PROTO_SRCS} )
add_executable(buffer_array_test buffer_array.cc)
