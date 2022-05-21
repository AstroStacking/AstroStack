FILE(GLOB ASTRO_MAIN_SRC
    graphicsview.cpp
    imagedata.ui
    imagedata.cpp
    mainwindow.ui
    mainwindow.cpp
    main.cpp
    Explorer/explorer.ui
    Explorer/explorer.cpp
    Explorer/imagedisplay.ui
    Explorer/imagedisplay.cpp
    HistoAdjust/histoadjust.ui
    HistoAdjust/histoadjust.cpp
)

FILE(GLOB ASTRO_MAIN_HEADERS
    graphicsview.h
    imagedata.h
    mainwindow.h
    Explorer/explorer.h
    Explorer/imagedisplay.h
    HistoAdjust/histoadjust.h
)

ASTRO_add_executable(
    AstroStack
    NAME AstroStack
    FOLDER Main
    SRC ${ASTRO_MAIN_SRC}
    HEADERS ${ASTRO_MAIN_HEADERS}
    LIBRARIES ITKInputPlugin_init ITKInputPlugin AstroIO Qt6::Widgets Qt6::Charts
)
