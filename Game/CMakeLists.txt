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
    src/Main/main.cpp
    src/Main/MainWindow.cpp
    resource.qrc
    
    src/Core/Logger/Logger.cpp
    src/Core/Resources/ResourceManager.cpp
    src/Core/Input/InputManager.cpp
    
    src/Game/Actions/CameraActions.cpp
    src/Game/Actions/UnitActions.cpp
    
    src/Game/Camera/Camera.cpp
    src/Game/Engine/GameEngine.cpp
    src/Game/Entities/Entity.cpp
    src/Game/Map/Map.cpp
    
    src/UI/Manager/MenuManager.cpp
    src/UI/Manager/ControlsSettingsManager.cpp
    src/UI/Manager/ConfigManager.cpp
    src/UI/Screens/SettingsScreen.cpp
    src/UI/Screens/GameScreen.cpp
    src/UI/Screens/GameOverScreen.cpp
    src/UI/Screens/MenuScreen.cpp
    src/UI/Components/MenuButton.cpp
    src/UI/Widgets/KeyCaptureDialog.cpp
    src/UI/Widgets/TacticalDialog.cpp
    src/UI/Manager/GameSettingsManager.cpp
    src/UI/Manager/AudioSettingsManager.cpp
    src/UI/Manager/DisplaySettingsManager.cpp
    src/UI/Manager/GraphicsSettingsManager.cpp
    src/UI/Screens/NewGameScreen.cpp
    src/UI/Screens/LoadingScreen.cpp
    src/UI/Screens/LoadGameScreen.cpp
    src/UI/HUD/TacticalHUD.cpp
    src/UI/Manager/GameManager.cpp
    src/UI/Widgets/InformationDialog.cpp
)

add_executable(Game ${PROJECT_SOURCES})

target_include_directories(Game PRIVATE
    ${CMAKE_SOURCE_DIR}/src
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
