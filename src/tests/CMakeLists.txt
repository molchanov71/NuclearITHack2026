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
