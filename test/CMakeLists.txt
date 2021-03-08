include(CTest)

# Collect all the paths for the corkami files we want to test
# This is easier than doing the path munging in C++
file(GLOB CORKAMI_PES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/assets/corkami-poc-dataset/PE/bin/*.exe"
  "${CMAKE_CURRENT_SOURCE_DIR}/assets/corkami-poc-dataset/PE/bin/*.dll"
  "${CMAKE_CURRENT_SOURCE_DIR}/assets/corkami-poc-dataset/PE/bin/*.sys")

set(CORKAMI_FILES "")
set(sep "")
foreach(filename ${CORKAMI_PES})
  string(APPEND CORKAMI_FILES "${sep}\n\"${filename}\"")
  set(sep ",")
endforeach()
list(LENGTH CORKAMI_PES HAS_CORKAMI_PES)

if (NOT HAS_CORKAMI_PES)
  message(WARNING "Not testing on Corkami dataset. Use 'git submodule update --init'")
endif()

### Catch2
# Needed for asset directories
configure_file(peparse_test_paths.h.in peparse_test_paths.h @ONLY)
configure_file(corkami_test.cpp.in corkami_test.cpp @ONLY)

if (NOT USE_EXTERNAL_CATCH2)
  include("${CMAKE_CURRENT_SOURCE_DIR}/cmake/catch.cmake.in")
else()
  find_package(Catch2 REQUIRED)
endif()

add_executable(tests
  catch_main.cpp
  simple_test.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/corkami_test.cpp
  )
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(tests PRIVATE pe-parse Catch2::Catch2)

include(Catch)
catch_discover_tests(tests)
## END CATCH

## GTEST
configure_file(corkami_gtest.cpp.in corkami_gtest.cpp @ONLY)
add_executable(gtests
  simple_gtest.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/corkami_gtest.cpp
  )
target_include_directories(gtests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
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
