FILE(GLOB ASTRO_MAIN_SRC
    Common/graphicsview.cpp
    Common/imagedata.ui
    Common/imagedata.cpp
    Explorer/explorer.ui
    Explorer/explorer.cpp
    Explorer/imagedisplay.ui
    Explorer/imagedisplay.cpp
    HistoAdjust/histoadjust.ui
    HistoAdjust/histoadjust.cpp
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

FILE(GLOB ASTRO_MAIN_HEADERS
    Common/graphicsview.h
    Common/imagedata.h
    Explorer/explorer.h
    Explorer/imagedisplay.h
    HistoAdjust/histoadjust.h
    mainwindow.h
)

ASTRO_add_executable(
    AstroStack
    NAME AstroStack
    FOLDER Main
    SRC ${ASTRO_MAIN_SRC}
    HEADERS ${ASTRO_MAIN_HEADERS}
    LIBRARIES ITKInputPlugin_init ITKInputPlugin AstroIO AstroProcessing Qt6::Widgets Qt6::Charts
)
