cmake_minimum_required(VERSION 3.10)
project(remote-joy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
find_package(Protobuf REQUIRED)
find_package(SDL2 REQUIRED)

add_subdirectory(third-party/enet)

# protobuf
set(PROTO_FILES ${CMAKE_SOURCE_DIR}/protobuf/joystick-data.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# remote-joy executable
set(EXEC_SOURCE_FILES ${CMAKE_SOURCE_DIR}/remote-joy-exec/src/main.cpp
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/include/joystick_data.h
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/src/joystick_data.cpp
)
add_executable(remote-joy ${EXEC_SOURCE_FILES} ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(remote-joy PRIVATE ${CMAKE_SOURCE_DIR}/third-party/CLI11 ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/third-party/tabulate ${CMAKE_SOURCE_DIR}/remote-joy-exec/include ${CMAKE_SOURCE_DIR}/third-party/enet/include)
target_link_libraries(remote-joy PRIVATE protobuf::libprotobuf SDL2::SDL2 enet)