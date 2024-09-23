set(QRCODE_SOURCE
	MoneroUri.cpp
)
set(QRCODE_HEADERS
	MoneroUri.h
    MoneroTxData.h
    MoneroWalletData.h
)

if (QT_VERSION_MAJOR EQUAL 6)
    find_package(Qt6 COMPONENTS Core REQUIRED)
else()
    find_package(Qt5 COMPONENTS Core REQUIRED)
endif()
message("qrcode Qt version: " ${QT_VERSION} "(Qt" ${QT_VERSION_MAJOR} ")")

add_library(otsuri STATIC ${QRCODE_SOURCE} ${QRCODE_HEADERS})
target_include_directories(otsuri PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(otsuri PRIVATE
    Qt${QT_VERSION_MAJOR}::Core
)
