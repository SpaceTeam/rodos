# All tests are assumed to be contained in a single file.
file(GLOB test_files
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  core-slow/*.cpp
  core-fast/*.cpp
  embedded-tests/*.cpp
  middleware-tests/*.cpp)

# These tests are known to currently fail / hang, so do not use them.
list(FILTER test_files
  EXCLUDE REGEX "core-fast/make-errors-asserts.cpp")
list(FILTER test_files
  EXCLUDE REGEX "core-fast/timepoints-errlog.cpp")


# Fixture for the tests, making sure that all test executables are
# build before attempting to run them.
add_custom_target(test-suite-build ALL)
add_test(test-suite-build-run
  "${CMAKE_COMMAND}"
  --build "${CMAKE_BINARY_DIR}"
  --config $<CONFIG>
  --target test-suite-build)
set_tests_properties(test-suite-build-run
  PROPERTIES FIXTURES_SETUP test-suite-build-fixture)


# Create a test for each test file: Create an executable from the test
# file and a test case which uses the test-runner.sh to run that
# executable.  Setup test dependencies (using the fixture) to ensure
# test exectuables are built before testing.
foreach(test_file ${test_files})
  string(REGEX REPLACE "[^A-Za-z0-9_]" "_" test_name ${test_file})
  add_executable("${test_name}-bin" EXCLUDE_FROM_ALL
    ${test_file} terminate-test.cpp)
  target_link_libraries("${test_name}-bin" PUBLIC rodos)
  add_dependencies(test-suite-build "${test_name}-bin")
  add_test(NAME "${test_name}-run"
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/test-runner.sh
    $<TARGET_FILE:${test_name}-bin>)
  set_tests_properties("${test_name}-run"
    PROPERTIES FIXTURES_REQUIRED test-suite-build-fixture)
endforeach()
