cmake_minimum_required(VERSION 3.5)
project(Cataclysmulti)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
link_libraries("uv" "ncursesw")
set(SOURCE_FILES main.cpp input.cpp network.cpp protocol.cpp game.cpp debug.cpp)
add_executable(Cataclysmulti ${SOURCE_FILES})