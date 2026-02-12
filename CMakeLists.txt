cmake_minimum_required(VERSION 3.18)
project(nexus LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(CTest)
enable_testing()

add_library(math
    source/math/bessel.cpp
    source/math/linalg.cpp
)

target_include_directories(math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(math PRIVATE Eigen3::Eigen)

find_package(Boost 1.70 REQUIRED)
target_link_libraries(math PRIVATE Boost::boost)

add_executable(app source/main.cpp)
target_link_libraries(app PRIVATE math)

add_executable(tests source/tests.cpp)
target_link_libraries(tests PRIVATE math)

add_test(NAME unit_tests COMMAND tests)
