cmake_minimum_required(VERSION 3.5)
project(Cataclysmulti)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")
link_libraries(uv ncursesw pthread json11)
set(SOURCE_FILES main.cpp input.cpp network.cpp protocol.cpp game.cpp debug.cpp output.cpp catacharset.cpp color.cpp)
add_executable(Cataclysmulti ${SOURCE_FILES})
