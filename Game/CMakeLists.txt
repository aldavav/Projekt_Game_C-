cmake_minimum_required(VERSION 3.16)
project(Game VERSION 0.1 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets Core Multimedia)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Core Multimedia)


set(SRC_MAIN
    src/Main/main.cpp
    src/Main/MainWindow.cpp
    src/Main/MainWindow.h
    resource.qrc
)

set(SRC_CORE
    src/Core/Assets/AssetManager.cpp
    src/Core/Assets/AssetManager.h
    src/Core/Assets/CursorTypes.h
    src/Core/Common/AnsiCodes.h
    src/Core/Common/KeyCodes.h
    src/Core/Config/ConfigTypes.h
    src/Core/Config/ControlTypes.h
    src/Core/Config/GameConfig.h
    src/Core/Config/GraphicsTypes.h
    src/Core/Logger/Logger.cpp
    src/Core/Logger/Logger.h
    src/Core/Logger/LoggerMacros.h
    src/Core/Logger/LoggerTypes.h
    src/Core/Resources/ResourceManager.cpp
    src/Core/Resources/ResourceManager.h
    src/Core/Input/ICommand.h
    src/Core/Input/InputManager.cpp
    src/Core/Input/InputManager.h
    src/Core/Input/RawInputEvent.h
)

set(SRC_GAME
    src/Game/Actions/CameraActions.cpp
    src/Game/Actions/CameraActions.h
    src/Game/Actions/GameActions.h
    src/Game/Actions/GameCommands.h
    src/Game/Actions/UnitActions.cpp
    src/Game/Actions/UnitActions.h
    src/Game/Camera/Camera.cpp
    src/Game/Camera/Camera.h
    src/Game/Engine/GameEngine.cpp
    src/Game/Engine/GameEngine.h
    src/Game/Engine/GameSpeed.h
    src/Game/Entities/Entity.cpp
    src/Game/Entities/Entity.h
    src/Game/Entities/EntityTypes.h
    src/Game/Map/Chunk.h
    src/Game/Map/Difficulty.h
    src/Game/Map/Map.cpp
    src/Game/Map/Map.h
    src/Game/Map/MapStats.h
    src/Game/Map/Tile.h
)

set(SRC_UI_MANAGERS
    src/UI/Manager/MenuManager.cpp
    src/UI/Manager/MenuManager.h
    src/UI/Manager/ControlsSettingsManager.cpp
    src/UI/Manager/ControlsSettingsManager.h
    src/UI/Manager/ConfigManager.cpp
    src/UI/Manager/ConfigManager.h
    src/UI/Manager/GameSettingsManager.cpp
    src/UI/Manager/GameSettingsManager.h
    src/UI/Manager/AudioSettingsManager.cpp
    src/UI/Manager/AudioSettingsManager.h
    src/UI/Manager/DisplaySettingsManager.cpp
    src/UI/Manager/DisplaySettingsManager.h
    src/UI/Manager/GraphicsSettingsManager.cpp
    src/UI/Manager/GraphicsSettingsManager.h
    src/UI/Manager/GameManager.cpp
    src/UI/Manager/GameManager.h
)

set(SRC_UI_SCREENS
    src/UI/Screens/AbstractScreen.cpp
    src/UI/Screens/AbstractScreen.h
    src/UI/Screens/MenuScreen.cpp
    src/UI/Screens/MenuScreen.h
    src/UI/Screens/GameScreen.cpp
    src/UI/Screens/GameScreen.h
    src/UI/Screens/SettingsScreen.cpp
    src/UI/Screens/SettingsScreen.h
    src/UI/Screens/NewGameScreen.cpp
    src/UI/Screens/NewGameScreen.h
    src/UI/Screens/LoadingScreen.cpp
    src/UI/Screens/LoadingScreen.h
    src/UI/Screens/LoadGameScreen.cpp
    src/UI/Screens/LoadGameScreen.h
    src/UI/Screens/GameOverScreen.cpp
    src/UI/Screens/GameOverScreen.h
)

set(SRC_UI_WIDGETS
    src/UI/Components/MenuButton.cpp
    src/UI/Components/MenuButton.h
    src/UI/Widgets/KeyCaptureDialog.cpp
    src/UI/Widgets/KeyCaptureDialog.h
    src/UI/Widgets/TacticalDialog.cpp
    src/UI/Widgets/TacticalDialog.h
    src/UI/Widgets/InformationDialog.cpp
    src/UI/Widgets/InformationDialog.h
    src/UI/HUD/TacticalHUD.cpp
    src/UI/HUD/TacticalHUD.h
)

add_executable(Game 
    ${SRC_MAIN}
    ${SRC_CORE}
    ${SRC_GAME}
    ${SRC_UI_MANAGERS}
    ${SRC_UI_SCREENS}
    ${SRC_UI_WIDGETS}
)

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
