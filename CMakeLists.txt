cmake_minimum_required(VERSION 4.2)
project(NuclearITHack2026)

set(CMAKE_CXX_STANDARD 23)
find_package(Qt6 REQUIRED COMPONENTS Core Widgets Gui)

find_package(spdlog CONFIG REQUIRED)

include(FindPkgConfig)
pkg_check_modules(SODIUM REQUIRED IMPORTED_TARGET libsodium)

pkg_check_modules(OPUS REQUIRED IMPORTED_TARGET opus)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)
add_compile_options(-Wall -Wextra -Wpedantic -ggdb -fsanitize=leak)

add_subdirectory(src)

add_executable(NuclearITHack2026 main.cpp)
target_link_libraries(NuclearITHack2026 PRIVATE
        spdlog::spdlog_header_only    # или spdlog::spdlog для статической
        PkgConfig::SODIUM             # libsodium
        PkgConfig::OPUS               # libopus
)
target_link_libraries(NuclearITHack2026 PRIVATE view Qt6::Core Qt6::Gui Qt6::Widgets)
