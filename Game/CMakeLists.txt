cmake_minimum_required(VERSION 3.16)

project(Game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find necessary Qt components
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Multimedia)

# --- 1. DEFINE ALL SOURCE FILES (.cpp and .qrc) ---
set(PROJECT_SOURCES
    main.cpp
    # Standardizing existing components to PascalCase
    MainWindow/MainWindow.cpp
    Logger/Logger.cpp
    resource.qrc
    Player/Player.cpp
    Unit/Unit.cpp
    Map/Map.cpp
    
    # NEW SYSTEM FILES (already in PascalCase)
    GameEngine/GameEngine.cpp
    ResourceManager/ResourceManager.cpp
    InputManager/InputManager.cpp
    MenuManager/MenuManager.cpp
)

# --- 2. USE STANDARD ADD_EXECUTABLE ---
add_executable(Game ${PROJECT_SOURCES})

# --- 3. SPECIFY INCLUDE DIRECTORIES FOR HEADERS ---
target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}              # For main.cpp and top-level files
    # Standardizing existing include directories to PascalCase
    ${CMAKE_SOURCE_DIR}/MainWindow
    ${CMAKE_SOURCE_DIR}/Logger
    ${CMAKE_SOURCE_DIR}/Player
    ${CMAKE_SOURCE_DIR}/Unit
    ${CMAKE_SOURCE_DIR}/Map
    
    # NEW SYSTEM DIRECTORIES
    ${CMAKE_SOURCE_DIR}/GameEngine
    ${CMAKE_SOURCE_DIR}/ResourceManager
    ${CMAKE_SOURCE_DIR}/InputManager
    ${CMAKE_SOURCE_DIR}/MenuManager
)

# --- 4. LINK LIBRARIES ---
# Linked libraries for Core and Multimedia are essential for your new managers
target_link_libraries(Game PRIVATE 
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia 
)

# --- 5. TARGET PROPERTIES (Standard Qt/CMake settings) ---
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