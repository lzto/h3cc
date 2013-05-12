PROJECT(h3cc)
cmake_minimum_required(VERSION 2.6)

set (CMAKE_BUILD_TYPE Debug)

ADD_LIBRARY(h3c8021x SHARED h3c8021x.cpp md5_ctx.cpp)

SET_TARGET_PROPERTIES(h3c8021x PROPERTIES
    VERSION 1.0
    SOVERSION 2)
INSTALL(TARGETS h3c8021x
        DESTINATION /usr/lib)
INSTALL(FILES h3c8021x.h
    DESTINATION /usr/include
)

ADD_EXECUTABLE(h3cc main.cpp)
target_link_libraries(h3cc h3c8021x -lpcap)
INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/h3cc
    DESTINATION /usr/bin)

find_package(Qt4 REQUIRED)
include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})
set(h3cGui_SRC dot1xthread.cpp h3cconfig.cpp logwidget.cpp
	h3cmainwindow.cpp
	h3cgui.cpp
	mainh3c.cpp
	aboutwidget.cpp)
qt4_automoc(${h3cGui_SRC})

add_executable(h3cgui ${h3cGui_SRC})
target_link_libraries(h3cgui h3c8021x -lpcap ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY})

INSTALL(PROGRAMS ${CMAKE_CURRENT_BINARY_DIR}/h3cgui
    DESTINATION /usr/bin)

INSTALL(FILES icons/h3cc128x128.png  icons/h3cc.png
	DESTINATION /usr/share/h3cc/icons)

