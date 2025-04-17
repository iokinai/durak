cmake_minimum_required(VERSION 3.15)

include(cmake/proj.cmake)

project(${PROJECT_NAME})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 23)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/pages/gamepage.cpp
    src/game/fsm/state.cpp
    src/game/fsm/fsm.cpp
    src/game/cards/card.cpp
    src/game/player/playerAI.cpp
    src/game/player/playerHuman.cpp
    src/widgets/cardwidget/cardwidget.cpp
    src/widgets/cardwidget/clickablecardwidget.cpp
    src/widgets/playerwidget/hostplayerwidget.cpp
)

set(PROJECT_HEADERS
    src/mainwindow.hpp
    src/pages/gamepage.hpp
    src/game/fsm/state.hpp
    src/game/fsm/fsm.hpp
    src/game/cards/card.hpp
    src/game/player/playerAI.hpp
    src/game/player/playerHuman.hpp
    src/game/player/player.hpp
    src/game/fsm/action.hpp
    src/game/fsm/event.hpp
    src/widgets/cardwidget/cardwidget.hpp
    src/widgets/playerwidget/playerwidget.hpp
    src/widgets/cardwidget/clickablecardwidget.hpp
    src/widgets/playerwidget/hostplayerwidget.hpp
    src/widgets/playerwidget/playerwidget.hpp
)

set(PROJECT_UIS
    src/mainwindow.ui
    src/pages/gamepage.ui
    src/widgets/cardwidget/cardwidget.ui
    src/widgets/playerwidget/hostplayerwidget.ui
)

set(PROJECT_RESOURCES
    durak.qrc
)

qt_add_resources(RESOURCES "${PROJECT_RESOURCES}")

qt_add_executable(
    ${PROJECT_NAME}
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_UIS}
    ${RESOURCES}
)

set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE TRUE)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/src)