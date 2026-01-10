cmake_minimum_required(VERSION 3.16)

project(Game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Multimedia)

set(PROJECT_SOURCES
main.cpp
MainWindow/MainWindow.cpp
Logger/Logger.cpp
resource.qrc
Player/Player.cpp
Entity/Entity.cpp
Map/Map.cpp
Map/Tile.cpp
Map/Location.cpp
Map/LocationDirector.cpp
Map/DefaultLocationBuilder.cpp
GameEngine/GameEngine.cpp
ResourceManager/ResourceManager.cpp
InputManager/InputManager.cpp
MenuManager/MenuManager.cpp
Screens/GameScreen.cpp
Screens/MainMenuScreen.cpp
)

add_executable(Game ${PROJECT_SOURCES}
    Screens/CreditsScreen.cpp
    Screens/CreditsScreen.h)

target_include_directories(Game PRIVATE
${CMAKE_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/MainWindow
${CMAKE_SOURCE_DIR}/Entity
${CMAKE_SOURCE_DIR}/Logger
${CMAKE_SOURCE_DIR}/Player
${CMAKE_SOURCE_DIR}/Map
${CMAKE_SOURCE_DIR}/GameEngine
${CMAKE_SOURCE_DIR}/ResourceManager
${CMAKE_SOURCE_DIR}/InputManager
${CMAKE_SOURCE_DIR}/MenuManager
${CMAKE_SOURCE_DIR}/Screens
)

target_link_libraries(Game PRIVATE
Qt${QT_VERSION_MAJOR}::Widgets
Qt${QT_VERSION_MAJOR}::Core
Qt${QT_VERSION_MAJOR}::Multimedia
)

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

include(GNUInstallDirs)
install(TARGETS Game
BUNDLE DESTINATION .
LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
