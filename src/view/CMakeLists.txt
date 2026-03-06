add_library(view SHARED
        main_window.cpp
        main_window.hpp
)

target_link_libraries(view PRIVATE
        app
        Qt6::Core
        Qt6::Gui
        Qt6::Widgets
)
