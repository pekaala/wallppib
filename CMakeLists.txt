cmake_minimum_required(VERSION 3.10)

project(Wallppib)

set(CMAKE_CXX_STANDARD 17)

set(SOURCES
    src/main.cpp
    src/wallpaperengine.cpp
)

set(CMAKE_TOOLCHAIN_FILE /home/ibrahim/vcpkg/scripts/buildsystems/vcpkg.cmake CACHE STRING "")

include_directories(/home/ibrahim/vcpkg/installed/x64-linux/include)

add_executable(wallppib ${SOURCES})

if(WIN32)
    message(STATUS "Building for Windows")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM gtkmm-3.0)
    include_directories(${GTKMM_INCLUDE_DIRS})
    link_directories(${GTKMM_LIBRARY_DIRS})
    set(SYSTEM_LIBS user32 gdi32)
endif()

if(UNIX)
    message(STATUS "Building for Linux")
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM gtkmm-3.0)
    include_directories(${GTKMM_INCLUDE_DIRS})
    link_directories(${GTKMM_LIBRARY_DIRS})
endif()


if(UNIX)
    target_link_libraries(wallppib ${GTKMM_LIBRARIES})
endif()

if(WIN32)
    target_link_libraries(wallppib ${SYSTEM_LIBS})
endif()