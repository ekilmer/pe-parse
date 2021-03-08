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

# Needed for asset directories
configure_file(peparse_test_paths.h.in peparse_test_paths.h @ONLY)
configure_file(corkami_test.cpp.in corkami_test.cpp @ONLY)

find_package(Catch2 REQUIRED)

add_executable(tests
  catch_main.cpp
  simple_test.cpp
  ${CMAKE_CURRENT_BINARY_DIR}/corkami_test.cpp
  )
target_include_directories(tests PRIVATE ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(tests PRIVATE pe-parse Catch2::Catch2)

include(Catch)
catch_discover_tests(tests)
