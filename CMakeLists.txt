cmake_minimum_required(VERSION 3.18)
project(nexus LANGUAGES CXX)

# -------------------------
# Global options / defaults
# -------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(NEXUS_BUILD_PYTHON "Build pybind11 extension module" ON)
option(NEXUS_BUILD_APP    "Build the C++ app target"        OFF)

include(GNUInstallDirs)

# CTest defines BUILD_TESTING option
include(CTest)

# --------------
# Core library
# --------------
add_library(nexus_core
  source/math/linalg.cpp
)
add_library(nexus::core ALIAS nexus_core)

target_include_directories(nexus_core
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)
target_link_libraries(nexus_core PUBLIC Eigen3::Eigen)

find_package(Boost REQUIRED)
# Prefer Boost::boost if present; otherwise headers.
if(TARGET Boost::boost)
  target_link_libraries(nexus_core PUBLIC Boost::boost)
elseif(TARGET Boost::headers)
  target_link_libraries(nexus_core PUBLIC Boost::headers)
else()
  message(FATAL_ERROR "Boost imported target not found (Boost::boost / Boost::headers).")
endif()

# -------------------
# Optional C++ app
# -------------------
if(NEXUS_BUILD_APP)
  add_executable(app source/main.cpp)
  target_link_libraries(app PRIVATE nexus::core)
endif()

# -----------------------
# Optional Python module
# -----------------------
if(NEXUS_BUILD_PYTHON)
  find_package(pybind11 CONFIG REQUIRED)

  # You can keep the module name as _core, and install into the 'nexus' package.
  pybind11_add_module(_core
    python/bindings/module.cpp
  )
  target_link_libraries(_core PRIVATE nexus::core)

  install(TARGETS _core
    LIBRARY DESTINATION nexus
    RUNTIME DESTINATION nexus
  )
endif()

# -----------------------
# Optional unit tests
# -----------------------
if(BUILD_TESTING)
  enable_testing()
  add_subdirectory(tests)
endif()
