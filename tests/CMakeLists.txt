# Tests CMakeLists.txt

# Function to create C++ unit tests
function(add_cpp_unit_test TEST_NAME)
    add_executable(${TEST_NAME} ${ARGN})
    
    target_link_libraries(${TEST_NAME}
        TaskManagerLib
        Qt6::Test
        Qt6::Core
        Qt6::Quick
    )
    
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/src/cpp
    )
    
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
    
    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 30
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen"
    )
endfunction()



# Add C++ tests
add_cpp_unit_test(test_task unit/cpp/test_models/test_task.cpp)

# Custom target to run all tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running all tests"
)
