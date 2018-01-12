cmake_minimum_required(VERSION 3.9)
project(udp_randevo)

set(CMAKE_CXX_STANDARD 11)

add_executable(udp_randevo_server main.cpp Request.cpp Request.h headers.h User.cpp User.h Server.cpp Server.h)

add_executable(udp_randevo_client main_client.cpp Request.cpp Request.h headers.h Client.cpp Client.h)

target_link_libraries(udp_randevo_server pthread)
target_link_libraries(udp_randevo_client pthread)