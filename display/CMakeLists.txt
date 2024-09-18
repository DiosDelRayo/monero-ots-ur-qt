set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets REQUIRED)

set(URDISPLAY_SOURCES
    QrWidget.cpp
    UrWidget.cpp
    UrSettingsDialog.cpp
)

set(URDISPLAY_HEADERS
    QrWidget.h
    UrWidget.h
    UrSettingsDialog.h
)

set(URDISPLAY_UI
  UrWidget.ui
  UrSettingsDialog.ui
)

add_library(urdisplay STATIC
    ${URDISPLAY_SOURCES} 
    ${URDISPLAY_HEADERS} 
    ${URDISPLAY_UI}
)

target_sources(urdisplay PRIVATE ${UI_HEADERS})

target_include_directories(urdisplay PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(urdisplay PUBLIC
    Qt::Core
    Qt::Gui
    Qt::Widgets
    qrcode
)
