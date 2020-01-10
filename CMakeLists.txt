cmake_minimum_required(VERSION 3.5)
PROJECT(ev3dev-mocks VERSION 1.4.0 LANGUAGES C)

include(GNUInstallDirs)
set(CMAKE_INSTALL_PKGDATADIR ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME})
set(CMAKE_INSTALL_PKGLIBDIR ${CMAKE_INSTALL_LIBDIR}/${PROJECT_NAME})

add_subdirectory(alsa)
add_subdirectory(script)
add_subdirectory(umockdev)
