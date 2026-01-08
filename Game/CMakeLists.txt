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
    src/main.cpp
    resource.qrc
    src/MainWindow.cpp
    src/Core/Logger/Logger.cpp
    src/Core/Resources/ResourceManager.cpp
    src/Core/Input/InputManager.cpp
    src/Core/Camera/Camera.cpp
    src/Game/Engine/GameEngine.cpp
    src/Game/Entities/Entity.cpp
    src/Game/Map/map.cpp
    src/Game/Map/MapManager.cpp
    src/Game/Player/player.cpp
    src/UI/Manager/MenuManager.cpp
    src/UI/Screens/GameScreen.cpp
    src/UI/Screens/MainMenuScreen.cpp
)

add_executable(Game ${PROJECT_SOURCES})

target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/src/Core
    ${CMAKE_SOURCE_DIR}/src/Game
    ${CMAKE_SOURCE_DIR}/src/UI
)

target_link_libraries(Game PRIVATE
    Qt${QT_VERSION_MAJOR}::Widgets
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Multimedia
)

set_target_properties(Game PROPERTIES
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)
