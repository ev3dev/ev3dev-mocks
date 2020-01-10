
foreach(MACHINE "ARM" "x86-64")
    file(READ ${MACHINE}/FBIOGET_FSCREENINFO FBIOGET_FSCREENINFO)

    configure_file(ev3-buttons.ioctl ${MACHINE}/ev3-buttons.ioctl COPYONLY)
    configure_file(ev3-fbdev.ioctl.in ${MACHINE}/ev3-fbdev.ioctl @ONLY)
    configure_file(ev3.umockdev.in ${MACHINE}/ev3.umockdev @ONLY)

    install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/${MACHINE} DESTINATION ${CMAKE_INSTALL_PKGDATADIR})
endforeach(MACHINE "ARM" "x86-64")

