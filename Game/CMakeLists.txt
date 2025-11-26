cmake_minimum_required(VERSION 3.16)

project(Game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

# --- 1. DEFINE ALL SOURCE FILES (.cpp and .qrc) ---
set(PROJECT_SOURCES
    main.cpp
    mainwindow/mainwindow.cpp
    logger/Logger.cpp
    resource.qrc
    player/player.cpp
    unit/unit.cpp
    map/map.cpp
)

# --- 2. USE STANDARD ADD_EXECUTABLE FOR COMPATIBILITY ---
# This avoids the previous 'undefined reference to qMain' linker error.
# We no longer need the 'if/else' block for Qt versions unless there are specific, non-standard needs.
add_executable(Game ${PROJECT_SOURCES})

# --- 3. SPECIFY INCLUDE DIRECTORIES FOR HEADERS ---
# This is how CMake finds your header (.h) and UI (.ui) files in subfolders
# and ensures CMAKE_AUTOUIC/AUTOMOC work correctly.
target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}              # For main.cpp and top-level files
    ${CMAKE_SOURCE_DIR}/mainwindow
    ${CMAKE_SOURCE_DIR}/logger
    ${CMAKE_SOURCE_DIR}/player
    ${CMAKE_SOURCE_DIR}/unit
    ${CMAKE_SOURCE_DIR}/map
)

# --- 4. LINK LIBRARIES ---
target_link_libraries(Game PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# --- 5. TARGET PROPERTIES (Standard Qt/CMake settings) ---

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
    set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Game)
endif()
set_target_properties(Game PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

# --- 6. INSTALLATION ---
include(GNUInstallDirs)
install(TARGETS Game
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
