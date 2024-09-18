set(QRCODE_SOURCE
    QrCode.cpp
)
set(QRCODE_HEADERS
    QrCode.h
    QrCode_p.h
)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Core Gui Widgets MultimediaWidgets Svg REQUIRED)

add_library(qrcode STATIC ${QRCODE_SOURCE} ${QRCODE_HEADERS})
target_include_directories(qrcode PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(qrcode PUBLIC
    Qt${QT_VERSION_MAJOR}::Svg
    qrencode
)
