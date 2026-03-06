add_library(view STATIC
        main_window.cpp
        main_window.hpp
)

target_link_libraries(view PRIVATE
        app
        controller
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
