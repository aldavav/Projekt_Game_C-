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
    src/Core/Common/GameTypes.h
    src/Core/Common/ICommand.h
    src/Core/Common/WorldData.h
    src/Core/Config/Configuration.h
    src/Core/Config/LoggerMacros.h
    src/Core/Input/InputManager.cpp
    src/Core/Input/InputManager.h
    src/Core/Logger/Logger.cpp
    src/Core/Logger/Logger.h
    src/Core/Settings/AudioSettingsManager.cpp
    src/Core/Settings/AudioSettingsManager.h
    src/Core/Settings/ControlsSettingsManager.cpp
    src/Core/Settings/ControlsSettingsManager.h
    src/Core/Settings/DisplaySettingsManager.cpp
    src/Core/Settings/DisplaySettingsManager.h
    src/Core/Settings/GameSettingsManager.cpp
    src/Core/Settings/GameSettingsManager.h
    src/Core/Settings/GraphicsSettingsManager.cpp
    src/Core/Settings/GraphicsSettingsManager.h
)

set(SRC_GAME
    src/Game/Actions/GuardUnitAction.cpp
    src/Game/Actions/GuardUnitAction.h
    src/Game/Actions/MoveUnitAction.cpp
    src/Game/Actions/MoveUnitAction.h
    src/Game/Actions/StopUnitAction.cpp
    src/Game/Actions/StopUnitAction.h
    src/Game/Actions/ZoomAction.cpp
    src/Game/Actions/ZoomAction.h
    src/Game/Camera/Camera.cpp
    src/Game/Camera/Camera.h
    src/Game/Engine/GameEngine.cpp
    src/Game/Engine/GameEngine.h
    src/Game/Engine/GameManager.cpp
    src/Game/Engine/GameManager.h
    src/Game/Entities/Entity.cpp
    src/Game/Entities/Entity.h
    src/Game/Map/Map.cpp
    src/Game/Map/Map.h
    src/Game/Resources/AssetManager.cpp
    src/Game/Resources/AssetManager.h
    src/Game/Resources/ConfigManager.cpp
    src/Game/Resources/ConfigManager.h
    src/Game/Resources/ResourceManager.cpp
    src/Game/Resources/ResourceManager.h
)

set(SRC_UI_COMPONENTS
    src/UI/Components/MenuButton.cpp
    src/UI/Components/MenuButton.h
)

set(SRC_UI_HUD
    src/UI/HUD/MinimapProvider.cpp
    src/UI/HUD/MinimapProvider.h
    src/UI/HUD/TacticalHUD.cpp
    src/UI/HUD/TacticalHUD.h
)

set(SRC_UI_MANAGERS
    src/UI/Manager/MenuManager.cpp
    src/UI/Manager/MenuManager.h
)

set(SRC_UI_SCREENS
    src/UI/Screens/AbstractScreen.cpp
    src/UI/Screens/AbstractScreen.h
    src/UI/Screens/GameOverScreen.cpp
    src/UI/Screens/GameOverScreen.h
    src/UI/Screens/GameScreen.cpp
    src/UI/Screens/GameScreen.h
    src/UI/Screens/LoadGameScreen.cpp
    src/UI/Screens/LoadGameScreen.h
    src/UI/Screens/LoadingScreen.cpp
    src/UI/Screens/LoadingScreen.h
    src/UI/Screens/MenuScreen.cpp
    src/UI/Screens/MenuScreen.h
    src/UI/Screens/NewGameScreen.cpp
    src/UI/Screens/NewGameScreen.h
    src/UI/Screens/SettingsScreen.cpp
    src/UI/Screens/SettingsScreen.h
)

set(SRC_UI_WIDGETS
    src/UI/Widgets/BaseTacticalDialog.cpp
    src/UI/Widgets/BaseTacticalDialog.h
    src/UI/Widgets/InformationDialog.cpp
    src/UI/Widgets/InformationDialog.h
    src/UI/Widgets/KeyCaptureDialog.cpp
    src/UI/Widgets/KeyCaptureDialog.h
    src/UI/Widgets/TacticalDialog.cpp
    src/UI/Widgets/TacticalDialog.h
)

add_executable(Game 
    ${SRC_MAIN}
    ${SRC_CORE}
    ${SRC_GAME}
    ${SRC_UI_MANAGERS}
    ${SRC_UI_SCREENS}
    ${SRC_UI_WIDGETS}
    ${SRC_UI_COMPONENTS}
    ${SRC_UI_HUD}
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
