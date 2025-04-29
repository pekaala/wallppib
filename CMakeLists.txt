cmake_minimum_required(VERSION 3.10)

project(Wallppib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/main.cpp
    src/wallpapermanager.cpp
)

if(DEFINED ENV{SNAPCRAFT})
    message(STATUS "Building inside snapcraft environment")
else()
    message(STATUS "Building locally, using vcpkg")
    set(CMAKE_TOOLCHAIN_FILE /home/ibrahim/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")
    include_directories(/home/ibrahim/vcpkg/installed/x64-linux/include)
endif()

add_executable(wallppib ${SOURCES})

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

    target_link_libraries(wallppib
        ${GTKMM_LIBRARIES}
        ${OpenCV_LIBS}
        ${X11_LIBRARIES}
        ${MAGICK++_LIBRARIES}
    )

    message(STATUS "Finished building for Linux")
endif()

install(TARGETS wallppib
    RUNTIME DESTINATION bin
)
