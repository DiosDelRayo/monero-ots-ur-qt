set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

set(FRAMESTATE_SOURCE
    FrameState.cpp
)
set(FRAMESTATE_HEADERS
    FrameState.h
)

add_library(framestate STATIC ${FRAMESTATE_SOURCE} ${FRAMESTATE_HEADERS})
target_include_directories(framestate PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(framestate PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
)
