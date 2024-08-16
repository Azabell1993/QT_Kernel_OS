cmake_minimum_required(VERSION 3.16)

project(os VERSION 0.1 LANGUAGES C CXX)  # C 언어 추가

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    bootloader.h
    bootloader.cpp
    resources.qrc
)

# Include C library headers
include_directories(${CMAKE_SOURCE_DIR}/C_lib/include)

# Add the directory where kernel_lib.a is located
link_directories(${CMAKE_SOURCE_DIR}/C_lib)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(os
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        resources.qrc
    )
else()
    if(ANDROID)
        add_library(os SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(os
            ${PROJECT_SOURCES}
        )
    endif()
endif()

# Link with C library and Qt Widgets
target_link_libraries(os PRIVATE Qt${QT_VERSION_MAJOR}::Widgets ${CMAKE_SOURCE_DIR}/C_lib/kernel_lib.a)

set_target_properties(os PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

install(TARGETS os
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(os)
endif()
