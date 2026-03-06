add_library(app STATIC
        app_runtime.cpp
        app_runtime.hpp
        app_config.cpp
        app_config.hpp
        dependency_container.cpp
        dependency_container.hpp
)

target_link_libraries(app PUBLIC
        infrastructure
        model
        controller
        spdlog::spdlog_header_only
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        Qt6::Widgets
)
