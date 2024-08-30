cmake_minimum_required(VERSION 3.16)

project(os VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# SourceTail
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    main.cpp
    bootloader.h
    bootloader.cpp
    resources.qrc
    processmanager.h
    processmanager.cpp
    mainwindow.h
    mainwindow.cpp
    mainwindow.ui
    cmdwindow.h
    cmdwindow.cpp
    cmdwindow.ui
)

include_directories(${CMAKE_SOURCE_DIR}/C_lib/include)
include_directories(${CMAKE_SOURCE_DIR}/C_lib/include/kernel_asm.h)
include_directories(${CMAKE_SOURCE_DIR}/C_lib/include_printf)

link_directories(${CMAKE_SOURCE_DIR}/C_lib)

# Common executable target
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(os
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
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

# Link the common C libraries and Qt Widgets
target_link_libraries(os PRIVATE Qt${QT_VERSION_MAJOR}::Widgets
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_lib.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_printf.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_1.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_2.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_3.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_4.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_5.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_6.a
                      ${CMAKE_SOURCE_DIR}/C_lib/kernel_asm_7.a)


# Set target properties
set_target_properties(os PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# Install target
install(TARGETS os
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize the executable for Qt 6
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(os)
endif()
