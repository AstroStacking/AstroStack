FILE(GLOB ASTRO_MAIN_SRC
    explorer.ui
    explorer.cpp
    graphicsview.cpp
    imagedata.ui
    imagedata.cpp
    imagedisplay.ui
    imagedisplay.cpp
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    pluginfactory.cpp
)

FILE(GLOB ASTRO_MAIN_HEADERS
    explorer.h
    graphicsview.h
    imagedata.h
    imagedisplay.h
    mainwindow.h
    pluginfactory.h
)

ASTRO_add_executable(
    AstroStack
    NAME AstroStack
    FOLDER Main
    SRC ${ASTRO_MAIN_SRC}
    HEADERS ${ASTRO_MAIN_HEADERS}
    LIBRARIES ITKInputPlugin_init ITKInputPlugin AstroIO Qt6::Widgets Qt6::Charts
)
