add_library(infrastructure STATIC
        database.cpp
        database.hpp
        identity_service.cpp
        identity_service.hpp
        log_service.cpp
        log_service.hpp
        network_services.cpp
        network_services.hpp
        protocol.cpp
        protocol.hpp
        repositories.cpp
        repositories.hpp
        runtime_paths.cpp
        runtime_paths.hpp
)

target_link_libraries(infrastructure PRIVATE
        model
        spdlog::spdlog
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        PkgConfig::SODIUM
        PkgConfig::OPUS
)

if(TARGET Qt6::Multimedia)
    target_link_libraries(infrastructure PRIVATE Qt6::Multimedia)
    target_compile_definitions(infrastructure PRIVATE NUCLEARIT_HAS_QT_MULTIMEDIA=1)
endif()
