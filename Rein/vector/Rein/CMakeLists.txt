cmake_minimum_required(VERSION 3.8)
project(Rein)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES
        chrono_time.h
        data_structure.h
        generator.cpp
        generator.h
        main.cpp
        oldrein.cpp
        oldrein.h
        printer.cpp
        printer.h
        rein.cpp
        rein.h
        util.cpp
        util.h)

add_executable(Rein ${SOURCE_FILES})