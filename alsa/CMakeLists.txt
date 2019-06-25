
find_package(PkgConfig REQUIRED)

# Depenedencies
pkg_check_modules(DEPS REQUIRED alsa)
pkg_get_variable(ALSA_LIBDIR alsa libdir)

# Plugin has to be installed in a specific directory to be found by ALSA
set(ALSA_LIBDIR ${ALSA_LIBDIR}/alsa-lib)

# Plugin target
add_library(asound_module_ctl_ev3dev SHARED ctl_ev3dev.c)
target_include_directories(asound_module_ctl_ev3dev PRIVATE ${DEPS_INCLUDE_DIRS})
target_compile_options(asound_module_ctl_ev3dev PRIVATE ${DEPS_CFLAGS_OTHER})
target_compile_definitions(asound_module_ctl_ev3dev PRIVATE PIC)
target_link_libraries(asound_module_ctl_ev3dev PRIVATE ${DEPS_LDFLAGS} -Wl,--no-undefined)
install(TARGETS asound_module_ctl_ev3dev DESTINATION ${ALSA_LIBDIR})

# Default conf file
install(FILES ev3dev.alsa.conf DESTINATION ${CMAKE_INSTALL_PKGDATADIR})
