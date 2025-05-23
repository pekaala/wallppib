cmake_minimum_required(VERSION 3.10)

project(Wallppib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/main.cpp
    src/core/wallpaper_manager.cpp
    src/ui/alerts/message.cpp
    src/ui/indicators/app_indicator_impression.cpp
    src/ui/about_window.cpp
    src/ui/app_bar_window.cpp
    src/ui/home_window.cpp
    src/ui/settings_window.cpp
    src/utils/localization/localization_manager.cpp
    src/utils/directory_path.cpp
    src/utils/log.cpp
    src/utils/settings.cpp
)

add_executable(wallppib ${SOURCES})

target_include_directories(wallppib PRIVATE ${PROJECT_SOURCE_DIR}/src/include)


if(DEFINED ENV{SNAPCRAFT})
    message(STATUS "Building inside snapcraft environment")
else()
    message(STATUS "Building locally, using vcpkg")
    set(CMAKE_TOOLCHAIN_FILE /home/ibrahim/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
    include_directories(/home/ibrahim/vcpkg/installed/x64-linux/include)
endif()


if(UNIX)
    message(STATUS "Building for Linux")

    include_directories(${CMAKE_SOURCE_DIR}/include)

    # GTKMM
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM gtkmm-3.0)
    include_directories(${GTKMM_INCLUDE_DIRS})
    link_directories(${GTKMM_LIBRARY_DIRS})

    # OpenCV
    find_package(OpenCV REQUIRED)
    include_directories(${OpenCV_INCLUDE_DIRS})
    link_directories(${OpenCV_LIBRARY_DIRS})

    # X11
    find_package(X11 REQUIRED)
    include_directories(${X11_INCLUDE_DIR})
    link_directories(${X11_LIBRARY_DIRS})

    # Magick++
    pkg_check_modules(MAGICK++ REQUIRED Magick++)
    include_directories(${MAGICK++_INCLUDE_DIRS})
    link_directories(${MAGICK++_LIBRARY_DIRS})

    # AppIndicator
    pkg_check_modules(APPINDICATOR3 REQUIRED ayatana-appindicator3-0.1)
    include_directories(${APPINDICATOR3_INCLUDE_DIRS})
    link_directories(${APPINDICATOR3_LIBRARY_DIRS})

    target_link_libraries(wallppib
        ${GTKMM_LIBRARIES}
        ${OpenCV_LIBS}
        ${X11_LIBRARIES}
        ${MAGICK++_LIBRARIES}
        ${APPINDICATOR3_LIBRARIES}
    )

    message(STATUS "Finished building for Linux")
endif()

install(TARGETS wallppib
    RUNTIME DESTINATION bin
)
