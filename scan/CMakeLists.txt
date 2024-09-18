set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
message(STATUS "Qt VERSION: ${QT_VERSION}")
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)
# find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets MultimediaWidgets REQUIRED)
find_package(ZXing REQUIRED)

set(URSCAN_SOURCES
    InfoFrame.cpp
    ScanThread.cpp
    ScanWidget.ui
    ScanWidget.cpp
)

set(URSCAN_HEADERS
    InfoFrame.h
    ScanResult.h
    ScanThread.h
    ScanWidget.h
)

set(URSCAN_UI
    ScanWidget.ui
)
add_library(urscan STATIC ${URSCAN_SOURCES} ${URSCAN_HEADERS} ${URSCAN_UI})
target_include_directories(urscan PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(urscan PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::MultimediaWidgets
    ZXing::ZXing
    bcur
)
