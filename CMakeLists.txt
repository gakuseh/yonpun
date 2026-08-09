cmake_minimum_required(VERSION 3.22)

project(Yonpun LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Quick QuickControls2)

qt_standard_project_setup()

qt_add_executable(Yonpun
    main.cpp
    
    todomodel.h
    todomodel.cpp
)

# Ensure the QML file is available next to the executable in the build dir
configure_file(${CMAKE_SOURCE_DIR}/main.qml ${CMAKE_CURRENT_BINARY_DIR}/main.qml COPYONLY)

target_link_libraries(Yonpun PRIVATE Qt6::Quick Qt6::QuickControls2)

set_target_properties(Yonpun PROPERTIES
    WIN32_EXECUTABLE ON
    MACOSX_BUNDLE ON
)