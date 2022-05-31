FILE(GLOB ASTRO_MAIN_SRC
    Common/doubleprogressbar.cpp
    Common/doubleprogressbar.ui
    Common/graphicsview.cpp
    Common/imagedata.cpp
    Common/imagedata.ui
    Explorer/explorer.cpp
    Explorer/explorer.ui
    Explorer/imagedisplay.cpp
    Explorer/imagedisplay.ui
    MonoProcessing/monoprocessing.cpp
    MonoProcessing/monoprocessing.ui
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

FILE(GLOB ASTRO_MAIN_HEADERS
    Common/doubleprogressbar.h
    Common/graphicsview.h
    Common/imagedata.h
    Explorer/explorer.h
    Explorer/imagedisplay.h
    MonoProcessing/monoprocessing.h
    mainwindow.h
)

ASTRO_add_executable(
    AstroStack
    NAME AstroStack
    FOLDER Main
    SRC ${ASTRO_MAIN_SRC}
    HEADERS ${ASTRO_MAIN_HEADERS}
    LIBRARIES AstroAlgorithms AstroIO AstroProcessing Qt6::Widgets Qt6::Charts
)
