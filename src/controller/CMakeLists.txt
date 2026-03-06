add_library(controller STATIC
        controllers.cpp
        controllers.hpp
)

target_link_libraries(controller PRIVATE
        model
        Qt6::Core
)
