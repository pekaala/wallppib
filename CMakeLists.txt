cmake_minimum_required(VERSION 3.10)

project(Wallppib)

set(CMAKE_CXX_STANDARD 17)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(SOURCES
    src/main.cpp
    src/domain/image_info.cpp
    src/services/wallpaper_manager.cpp
    src/services/image_manager.cpp
    src/ui/alerts/info_bar_message.cpp
    src/ui/indicators/app_indicator_impression.cpp
    src/ui/about_window.cpp
    src/ui/app_bar_window.cpp
    src/ui/home_window.cpp
    src/ui/settings_window.cpp
    src/utils/localization/localization_manager.cpp
    src/utils/directory_path.cpp
    src/utils/log.cpp
    src/utils/settings.cpp
    src/utils/system_info.cpp   
    src/utils/file_type.cpp
)

add_executable(wallppib ${SOURCES})

target_include_directories(wallppib PRIVATE ${PROJECT_SOURCE_DIR}/src/include)

if(DEFINED ENV{SNAPCRAFT})
    message(STATUS "Snapcraft build environment detected")
    include_directories($ENV{SNAPCRAFT_STAGE}/include)
else()
    message(STATUS "Building locally, using vcpkg")
    set(CMAKE_TOOLCHAIN_FILE /home/ibrahim/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
    include_directories(/home/ibrahim/vcpkg/installed/x64-linux/include)
endif()

if(UNIX)
    message(STATUS "Building for Linux")

    include_directories(${CMAKE_SOURCE_DIR}/include)

    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM gtkmm-3.0)
    pkg_check_modules(MAGICK++ REQUIRED Magick++)
    pkg_check_modules(APPINDICATOR3 REQUIRED ayatana-appindicator3-0.1)

    find_package(OpenCV REQUIRED)
    find_package(X11 REQUIRED)

    include_directories(
        ${GTKMM_INCLUDE_DIRS}
        ${MAGICK++_INCLUDE_DIRS}
        ${APPINDICATOR3_INCLUDE_DIRS}
        ${OpenCV_INCLUDE_DIRS}
        ${X11_INCLUDE_DIR}
    )

    link_directories(
        ${GTKMM_LIBRARY_DIRS}
        ${MAGICK++_LIBRARY_DIRS}
        ${APPINDICATOR3_LIBRARY_DIRS}
        ${OpenCV_LIBRARY_DIRS}
        ${X11_LIBRARY_DIRS}
    )

    target_link_libraries(wallppib
        ${GTKMM_LIBRARIES}
        ${MAGICK++_LIBRARIES}
        ${APPINDICATOR3_LIBRARIES}
        ${OpenCV_LIBS}
        ${X11_LIBRARIES}
        Threads::Threads
        -lpthread
    )

    set_target_properties(wallppib PROPERTIES
        INSTALL_RPATH "$ORIGIN/../lib"
        BUILD_WITH_INSTALL_RPATH TRUE
    )

    message(STATUS "Finished building for Linux")
endif()

install(TARGETS wallppib
    RUNTIME DESTINATION bin
)
