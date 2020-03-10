cmake_minimum_required(VERSION 3.5)

project(ucwqt VERSION "1.0.0" LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(QtCommon)

fix_project_version()
set(RES_FILES)
add_project_meta(RES_FILES)
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

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES ucwqt_zh_CN.ts)

if(ANDROID)
  add_library(ucwqt SHARED
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    ${TS_FILES}
    ${RES_FILES}
  )
else()
  add_executable(ucwqt
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    ${TS_FILES}
    ${RES_FILES}
  )
endif()

target_link_libraries(ucwqt PRIVATE Qt5::Widgets)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
