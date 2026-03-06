add_library(app SHARED
        app_config.cpp
        app_config.hpp
        dependency_container.cpp
        dependency_container.hpp
)

target_link_libraries(app PRIVATE
        spdlog::spdlog_header_only
        Qt6::Core
        Qt6::Widgets
)
