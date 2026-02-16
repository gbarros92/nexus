add_executable(test_vector test_vector.cpp)

# Preferred: link via the alias exported by the top-level
target_link_libraries(test_vector PRIVATE nexus::core)

add_test(NAME test_vector COMMAND test_vector)
