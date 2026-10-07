
cmake_minimum_required(VERSION 3.16)

project(matcalc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(matcalc
    src/main.cpp
    src/ui/ui_helpers.cpp
    src/matrix/matrix.cpp
)

target_link_libraries(matcalc  Qt6::Widgets)
