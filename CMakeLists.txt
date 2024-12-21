cmake_minimum_required(VERSION 3.10)
project(remote-joy)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# dependencies
find_package(Protobuf REQUIRED)
find_package(SDL2 REQUIRED)
find_package(absl REQUIRED)

# protobuf
set(PROTO_FILES ${CMAKE_SOURCE_DIR}/protobuf/joystick-data.proto)
protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${PROTO_FILES})

# remote-joy executable
set(EXEC_SOURCE_FILES ${CMAKE_SOURCE_DIR}/remote-joy-exec/src/main.cpp
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/include/joystick_data.h
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/src/joystick_data.cpp
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/include/udp-sender.h
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/src/udp-sender.cpp
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/include/sdl-manager.h
                      ${CMAKE_SOURCE_DIR}/remote-joy-exec/src/sdl-manager.cpp
)

add_executable(remote-joy ${EXEC_SOURCE_FILES} ${PROTO_SRCS} ${PROTO_HDRS})
target_include_directories(remote-joy PRIVATE ${CMAKE_SOURCE_DIR}/third-party/CLI11 ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR}/third-party/tabulate ${CMAKE_SOURCE_DIR}/remote-joy-exec/include)
target_link_libraries(remote-joy PRIVATE protobuf::libprotobuf absl::log_internal_check_op SDL2::SDL2)