cmake_minimum_required(VERSION 3.15)

project(linuxdesktopui LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)
find_library(PAM_LIBRARY pam REQUIRED)
add_executable(linuxdesktopui
    main.c
    login.c
    login.h
    disstack.c
    disstack.h
)

target_link_libraries(linuxdesktopui PRIVATE
    PkgConfig::GTK4
    ${PAM_LIBRARY}
)