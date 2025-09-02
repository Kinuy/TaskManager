cmake_minimum_required(VERSION 3.16)

project(TaskManager VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS
    Core
    Quick
    QuickControls2
    Test
    QuickTest
)

# Qt6 automatic tools
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Collect source files
file(GLOB_RECURSE CPP_SOURCES
    "src/cpp/*.cpp"
    "src/cpp/*.h"
)

# Create library for easier testing
add_library(TaskManagerLib STATIC
    ${CPP_SOURCES}
#    ${QML_RESOURCES}
)

target_link_libraries(TaskManagerLib
    PUBLIC
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
)

target_include_directories(TaskManagerLib PUBLIC
    src/cpp
    src/cpp/models
    src/cpp/controllers
    src/cpp/utils
)

# Main executable with different name to avoid conflicts
add_executable(TaskManagerExe
    src/main.cpp
)

# Set output name for final executable
set_target_properties(TaskManagerExe PROPERTIES
    OUTPUT_NAME "TaskManager"
)

# macOS Bundle properties
if(APPLE)
    set_target_properties(TaskManagerExe PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_GUI_IDENTIFIER com.example.TaskManager
        MACOSX_BUNDLE_BUNDLE_NAME "TaskManager"
        MACOSX_BUNDLE_DISPLAY_NAME "TaskManager"
        MACOSX_BUNDLE_INFO_STRING "TaskManager ${PROJECT_VERSION}"
        MACOSX_BUNDLE_BUNDLE_VERSION "${PROJECT_VERSION}"
        MACOSX_BUNDLE_SHORT_VERSION_STRING "${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}"
    )
endif()

# QML Module (must come BEFORE target_link_libraries)
qt6_add_qml_module(TaskManagerExe
    URI TaskManager
    VERSION 1.0
    RESOURCE_PREFIX "/qt/qml"
    QML_FILES
        src/Main.qml
        #src/qml/components/TaskItem.qml
        #src/qml/components/CustomButton.qml
        #src/qml/pages/TaskListPage.qml
        #src/qml/dialogs/AddTaskDialog.qml
        #src/qml/styles/AppTheme.qml
        #RESOURCES  # ← Icon hier hinzufügen
        #    src/resources/images/icon.png
)

# Link libraries
target_link_libraries(TaskManagerExe
    PRIVATE
        TaskManagerLib
        Qt6::Core
        Qt6::Quick
        Qt6::QuickControls2
)

# Tests
#option(BUILD_TESTS "Build tests" ON)
#if(BUILD_TESTS)
#    enable_testing()
#    add_subdirectory(tests)
#endif()

# Installation
if(APPLE)
   install(TARGETS TaskManagerExe
       BUNDLE DESTINATION .
       RUNTIME DESTINATION bin
   )
else()
   install(TARGETS TaskManagerExe
       RUNTIME DESTINATION bin
   )
endif()
