# Tests CMakeLists.txt

set(CMAKE_AUTOMOC ON)

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

# Function to create QML tests
function(add_qml_test TEST_NAME QML_SOURCE_DIR)
    set(TEST_EXECUTABLE ${TEST_NAME}_runner)

    add_executable(${TEST_EXECUTABLE}
        qml/test_runner.cpp
    )

    target_link_libraries(${TEST_EXECUTABLE}
        TaskManagerLib
        Qt6::Test
        Qt6::Quick
        Qt6::QuickTest
    )

    target_include_directories(${TEST_EXECUTABLE} PRIVATE
        ${CMAKE_SOURCE_DIR}/src/cpp
    )

    target_compile_definitions(${TEST_EXECUTABLE} PRIVATE
        QUICK_TEST_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}/${QML_SOURCE_DIR}"
        QML_IMPORT_PATH="${CMAKE_SOURCE_DIR}/src/qml"
    )

    add_test(NAME ${TEST_NAME} COMMAND ${TEST_EXECUTABLE})

    set_tests_properties(${TEST_NAME} PROPERTIES
        TIMEOUT 60
        ENVIRONMENT "QT_QPA_PLATFORM=offscreen;QML_IMPORT_PATH=${CMAKE_SOURCE_DIR}/src/qml"
    )
endfunction()


# Add C++ tests
add_cpp_unit_test(test_task unit/cpp/test_models/test_task.cpp)


# Add integration tests
add_cpp_unit_test(test_integration integration/test_integration.cpp)

# Add QML tests
add_qml_test(qml_components_test unit/qml/test_components/TestTaskItem.qml)

# Custom target to run all tests
add_custom_target(run_all_tests
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMENT "Running all tests"
)
