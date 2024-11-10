cmake_minimum_required(VERSION 3.29)
project(transport)

set(CMAKE_CXX_STANDARD 20)

add_executable(transport main.cpp
        input_reader.cpp
        input_reader.hpp
        transport_catalogue.cpp
        transport_catalogue.hpp
        geo.hpp
        stat_reader.cpp
        stat_reader.hpp)
