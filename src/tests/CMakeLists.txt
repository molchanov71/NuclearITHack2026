add_executable(test_protocol
        test_protocol.cpp
)
target_link_libraries(test_protocol PRIVATE
        infrastructure
        Qt6::Core
        Qt6::Test
)
add_test(NAME test_protocol COMMAND test_protocol)

add_executable(test_identity
        test_identity.cpp
)
target_link_libraries(test_identity PRIVATE
        infrastructure
        Qt6::Core
        Qt6::Test
)
add_test(NAME test_identity COMMAND test_identity)

add_executable(test_runtime_smoke
        test_runtime_smoke.cpp
)
target_link_libraries(test_runtime_smoke PRIVATE
        app
        infrastructure
        model
        Qt6::Core
        Qt6::Sql
        Qt6::Test
)
add_test(NAME test_runtime_smoke COMMAND test_runtime_smoke)

add_executable(test_model_controller
        test_model_controller.cpp
)
target_link_libraries(test_model_controller PRIVATE
        controller
        model
        app
        Qt6::Core
        Qt6::Test
)
add_test(NAME test_model_controller COMMAND test_model_controller)

add_executable(test_infrastructure_support
        test_infrastructure_support.cpp
)
target_link_libraries(test_infrastructure_support PRIVATE
        infrastructure
        Qt6::Core
        Qt6::Network
        Qt6::Sql
        Qt6::Test
)
add_test(NAME test_infrastructure_support COMMAND test_infrastructure_support)

add_executable(test_view_smoke
        test_view_smoke.cpp
)
target_link_libraries(test_view_smoke PRIVATE
        view
        app
        Qt6::Core
        Qt6::Widgets
        Qt6::Test
)
add_test(NAME test_view_smoke COMMAND test_view_smoke)
