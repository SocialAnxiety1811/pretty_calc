cmake_minimum_required(VERSION 3.5)

project(pretty_Calc VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets StateMachine)  # Добавлен StateMachine
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets StateMachine)  # Добавлен StateMachine

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(pretty_Calc
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        styles.qrc
        dialog.ui
        SwitchWidget.h
        SwitchWidget.cpp
    )
else()
    if(ANDROID)
        add_library(pretty_Calc SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(pretty_Calc
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(pretty_Calc PRIVATE Qt${QT_VERSION_MAJOR}::Widgets Qt${QT_VERSION_MAJOR}::StateMachine)  # Добавлен StateMachine

if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.pretty_Calc)
endif()
set_target_properties(pretty_Calc PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS pretty_Calc
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(pretty_Calc)
endif()
