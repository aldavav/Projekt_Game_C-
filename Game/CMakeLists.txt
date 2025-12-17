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
    MainWindow/MainWindow.h
    MainWindow/mainwindow.ui

    Logger/Logger.cpp
    Logger/Logger.h

    Player/Player.cpp
    Player/Player.h

    Entity/Entity.cpp
    Entity/Entity.h

    Map/Map.cpp
    Map/Map.h
    Map/Tile.cpp
    Map/Tile.h
    Map/Location.cpp
    Map/Location.h
    Map/LocationDirector.cpp
    Map/LocationDirector.h
    Map/DefaultLocationBuilder.cpp
    Map/DefaultLocationBuilder.h

    GameEngine/GameEngine.cpp
    GameEngine/GameEngine.h

    ResourceManager/ResourceManager.cpp
    ResourceManager/ResourceManager.h

    InputManager/InputManager.cpp
    InputManager/InputManager.h

    MenuManager/MenuManager.cpp
    MenuManager/MenuManager.h

    Screens/GameScreen.cpp
    Screens/GameScreen.h
    Screens/MainMenuScreen.cpp
    Screens/MainMenuScreen.h

    resource.qrc
)


add_executable(Game ${PROJECT_SOURCES}
    unit.h unit.cpp
    unitbuilder.h unitbuilder.cpp
    infantry.h infantry.cpp
    tank.h tank.cpp
    plane.h plane.cpp
    tankbuilder.h tankbuilder.cpp
    infantrybuilder.h infantrybuilder.cpp
    plane.h plane.cpp
    planebuilder.h planebuilder.cpp
    mapwidget.h mapwidget.cpp
    unitbuydialog.h unitbuydialog.cpp)

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
