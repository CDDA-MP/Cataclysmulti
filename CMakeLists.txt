cmake_minimum_required(VERSION 2.8)

# Cataclysmulti Client
project(cataclysmulti_client)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

link_libraries(uv ncursesw pthread json11)
aux_source_directory(./client/ CLIENT_SOURCE_FILES)

add_executable(cmulti ${CLIENT_SOURCE_FILES})

# Cataclysmulti Server
project(cataclysmulti_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall")

link_libraries(uv json11)
aux_source_directory(./server/ SERVER_SOURCE_FILES)

add_executable(cmultis ${SERVER_SOURCE_FILES})

