# All tests are assumed to be contained in a single file.
file(GLOB test_files
  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
  core-slow/*.cpp
  core-fast/*.cpp
  embedded-tests/*.cpp
  middleware-tests/*.cpp)

# These tests are known to currently fail / hang, so do not use them.
#
# TODO: Make these tests useable!
#
list(REMOVE_ITEM test_files
  "core-fast/make-errors-asserts.cpp"
  "core-fast/timepoints-errlog.cpp")
if (COVERAGE)
  list(REMOVE_ITEM test_files
    "core-fast/timeevents.cpp"
    "core-slow/preemptiontest-and-ceiler.cpp"
    "core-slow/priotest.cpp"
    "core-fast/gets-async.cpp"
    "core-fast/getsnowiat.cpp")
endif()

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

add_test(NAME "only-run-tests"
  COMMAND "${CMAKE_COMMAND}" -E echo
  "Only ran tests; didn't check output")

# Create a test for each test file: Create an executable from the test
# file and a test case which uses the test-runner.sh to run that
# executable.  Setup test dependencies (using the fixture) to ensure
# test exectuables are built before testing.
foreach(test_file ${test_files})
  get_filename_component(filename ${test_file} NAME_WE)
  string(REGEX REPLACE "[^A-Za-z0-9_]" "_" test_name ${test_file})
  add_executable("${test_name}-bin" EXCLUDE_FROM_ALL ${test_file})
  target_include_directories("${test_name}-bin"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries("${test_name}-bin" PUBLIC rodos)
  add_dependencies(test-suite-build "${test_name}-bin")
  add_test(NAME "${test_name}-run"
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/test-runner.sh
    $<TARGET_FILE:${test_name}-bin>
    ${CMAKE_CURRENT_SOURCE_DIR}/expected-outputs/${filename}.txt)
  add_test(NAME "${test_name}-cmp"
    COMMAND
    ${CMAKE_COMMAND} -E compare_files
    $<TARGET_FILE:${test_name}-bin>.output_trimmed
    $<TARGET_FILE:${test_name}-bin>.expected_trimmed)
  set_tests_properties("${test_name}-run"
    PROPERTIES FIXTURES_REQUIRED test-suite-build-fixture)
  set_tests_properties("${test_name}-run"
    PROPERTIES FIXTURES_SETUP "${test_name}-run-fixture")
  set_tests_properties("${test_name}-cmp"
    PROPERTIES FIXTURES_REQUIRED "${test_name}-run-fixture")
  list(APPEND only-run-tests-fixtures "${test_name}-run-fixture")
endforeach()

set_tests_properties("only-run-tests"
  PROPERTIES FIXTURES_REQUIRED "${only-run-tests-fixtures}")

