cmake_minimum_required(VERSION 4.0)
project(lab1 C)

set(CMAKE_C_STANDARD 11)

add_executable(lab1 movdino.c
        module1.h
        module1.c
        field_module.c
        field_module.h
        concole_render.h
        console_render.c
        trees_and_rocks.c
)
