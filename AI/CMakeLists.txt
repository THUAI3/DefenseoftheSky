CMAKE_MINIMUM_REQUIRED(VERSION 3.15.4)

PROJECT(sample_ai)

INCLUDE_DIRECTORIES(include)

AUX_SOURCE_DIRECTORY(src DIR_SRCS)

set(CMAKE_CXX_STANDARD 11)

SET(SAMPLE_AI ${DIR_SRCS}
        src/main.cpp
        src/player.hpp
        include/client/client.h
        include/client/client.cpp
        include/jsoncpp/json/json-forwards.h
        include/jsoncpp/json/json.h
        include/jsoncpp/jsoncpp.cpp)

ADD_EXECUTABLE(sample_ai ${SAMPLE_AI})
