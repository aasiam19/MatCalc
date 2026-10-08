cmake_minimum_required(VERSION 3.16)

project(matcalc)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)
set(CMAKE_AUTOMOC ON)
add_executable(matcalc
    src/main.cpp
    src/ui/ui_helpers.cpp
    src/ui/main_window.cpp
    src/ui/matrix_dialog.cpp
    src/matrix/matrix.cpp
)

target_include_directories(matcalc PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(matcalc
    Qt6::Widgets
)
