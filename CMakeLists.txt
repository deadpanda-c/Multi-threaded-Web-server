cmake_minimum_required(VERSION 3.22)
project(WebServer)
set(CMAKE_CXX_STANDARD 20)

file(GLOB_RECURSE SOURCES src/*.cpp)

include_directories(
  include
  lib/server_logging/include
)

add_executable(server ${SOURCES})

add_subdirectory(lib/server_logging)
target_link_libraries(server server_logging)

