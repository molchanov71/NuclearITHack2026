add_library(model STATIC
        domain_entities.hpp
        domain_enums.hpp
        stores.cpp
        stores.hpp
)

target_link_libraries(model PRIVATE
        Qt6::Core
)
