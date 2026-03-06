cmake_minimum_required(VERSION 3.28)
project(NuclearITHack2026 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Network Sql Test OPTIONAL_COMPONENTS Multimedia)
find_package(spdlog CONFIG REQUIRED)

include(CTest)
include(FindPkgConfig)
pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)
pkg_check_modules(OPUS REQUIRED IMPORTED_TARGET opus)

add_compile_options(-Wall -Wextra -Wpedantic -ggdb)

add_subdirectory(src)

add_executable(NuclearITHack2026 main.cpp)
target_link_libraries(NuclearITHack2026 PRIVATE
        app
        controller
        infrastructure
        model
        view
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
