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


foreach(test_file ${test_files})
  # filename: Used to get name of expected output
  # test_name: replaced_problematic_symbols
  get_filename_component(filename ${test_file} NAME_WE)
  string(REGEX REPLACE "[^A-Za-z0-9_]" "_" test_name ${test_file})

  # Compile rodos test application
  add_executable("${test_name}-bin" EXCLUDE_FROM_ALL ${test_file})
  target_include_directories("${test_name}-bin"
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries("${test_name}-bin" PUBLIC rodos)

  # Command to run rodos test application, generating output files
  # including diff
  add_custom_command(
    OUTPUT
    "${test_name}-bin.output"
    "${test_name}-bin.output_trimmed"
    "${test_name}-bin.expected_trimmed"
    "${test_name}-bin.diff"
    COMMAND
    ${CMAKE_CURRENT_SOURCE_DIR}/test-runner.sh
    $<TARGET_FILE:${test_name}-bin>
    ${CMAKE_CURRENT_SOURCE_DIR}/expected-outputs/${filename}.txt
    DEPENDS
    ${test_name}-bin
    coverage_prepare)
  list(APPEND diff_files "${test_name}-bin.diff")

  add_custom_target(${test_name}-clean
    COMMAND
    ${CMAKE_COMMAND} -E remove -f
    "${test_name}-bin.output"
    "${test_name}-bin.output_trimmed"
    "${test_name}-bin.expected_trimmed"
    "${test_name}-bin.diff")

  add_custom_target(${test_name}-run DEPENDS "${test_name}-bin.diff")
  add_dependencies(coverage_collect ${test_name}-run)
  add_dependencies(coverage_prepare ${test_name}-bin)
  add_dependencies(coverage_reset ${test_name}-clean)
endforeach()

add_custom_target(generate-diffs DEPENDS "${diff_files}")
