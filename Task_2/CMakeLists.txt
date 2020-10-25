cmake_minimum_required(VERSION 3.5)

project(geoAlg2 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

set(SOURCES main.cpp mainwindow.cpp logger.cpp canvas.cpp point.cpp fileloader.cpp guilogs.cpp)
set(HEADERS mainwindow.h logger.h canvas.h point.h fileloader.h guilogs.h)
set(UI mainwindow.ui)

set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/icon.rc")

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if(ANDROID)
  add_library(geoAlg2 SHARED
    ${SOURCES}
    ${HEADERS}
    ${UI}
    ${APP_ICON_RESOURCE_WINDOWS}
  )
else()
  add_executable(geoAlg2
      ${SOURCES}
      ${HEADERS}
      ${UI}
      ${APP_ICON_RESOURCE_WINDOWS}
  )
endif()

target_link_libraries(geoAlg2 PRIVATE Qt5::Widgets)
