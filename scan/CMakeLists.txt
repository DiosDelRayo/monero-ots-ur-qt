set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

if (QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 COMPONENTS Core Gui Widgets Multimedia MultimediaWidgets REQUIRED)
else()
    find_package(Qt5 COMPONENTS Core Gui Widgets MultimediaWidgets REQUIRED)
endif()
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
    Qt${QT_VERSION_MAJOR}::Core
    Qt::Gui
    Qt::Widgets
    Qt${QT_VERSION_MAJOR}::MultimediaWidgets
    ZXing::ZXing
    bcur
)
if (QT_VERSION_MAJOR EQUAL 6)
	target_link_libraries(urscan PUBLIC Qt6::Multimedia)
endif()
