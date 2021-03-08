include(CTest)

# Path for corkami dataset
set(CORKAMI_SUBMODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/assets/corkami-poc-dataset")
set(CORKAMI_PE_PATH "${CORKAMI_SUBMODULE_PATH}/PE/bin")

if (NOT EXISTS "${CORKAMI_PE_PATH}")
  message(WARNING "Could not find Corkami dataset for testing.\nUse 'git submodule update --init'")
endif()

# To reconfigure and rebuild when submodule status changes
# See https://gitlab.kitware.com/cmake/cmake/-/issues/18755 to read why using
# (potentially non-existant) CORKAMI_PE_PATH doesn't work
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${CORKAMI_SUBMODULE_PATH}")

### Catch2
if (NOT USE_EXTERNAL_CATCH2)
  include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/catch.cmake.in")
else()
  find_package(Catch2 REQUIRED)
endif()

add_executable(tests
  catch_main.cpp
  simple_test.cpp
  corkami_test.cpp
  )
target_compile_definitions(tests PRIVATE ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")
target_link_libraries(tests PRIVATE pe-parse Catch2::Catch2)

if (EXISTS "${CORKAMI_PE_PATH}")
  target_compile_definitions(tests PRIVATE CORKAMI_PE_PATH="${CORKAMI_PE_PATH}")
endif()

include(Catch)
catch_discover_tests(tests)
## END CATCH

## GTEST
add_executable(gtests
  simple_gtest.cpp
  corkami_gtest.cpp
  )
target_include_directories(gtests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_compile_definitions(gtests PRIVATE ASSETS_DIR="${CMAKE_CURRENT_SOURCE_DIR}/assets")

# Look for corkami dataset
if (EXISTS "${CORKAMI_PE_PATH}")
  target_compile_definitions(gtests PRIVATE CORKAMI_PE_PATH="${CORKAMI_PE_PATH}")
endif()

# Setup GTest
if (NOT USE_EXTERNAL_GTEST)
  set(gtest_cmake_in "${CMAKE_CURRENT_SOURCE_DIR}/cmake/gtest.cmake.in")
  set(gtest_cmake
      "${CMAKE_CURRENT_BINARY_DIR}/googletest-download/CMakeLists.txt")
  configure_file("${gtest_cmake_in}" "${gtest_cmake}")

  execute_process(
    COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)

  if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
  endif()

  execute_process(
    COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download)

  if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
  endif()

  # Prevent overriding the parent project's compiler/linker settings on Windows
  set(gtest_force_shared_crt
      ON
      CACHE BOOL "" FORCE)

  # Add googletest directly to our build. This defines the gtest and gtest_main
  # targets.
  add_subdirectory(
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-src
    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build EXCLUDE_FROM_ALL)
  target_link_libraries(gtests PRIVATE gtest)
else()
  find_package(GTest CONFIG REQUIRED)
  target_link_libraries(gtests PRIVATE GTest::gtest)
endif()

include(GoogleTest)
target_link_libraries(gtests PRIVATE gtest_main pe-parse)
gtest_discover_tests(gtests)
## END GTEST
