FILE(GLOB ASTRO_MAIN_SRC
    explorer.ui
    explorer.cpp
    imagedata.ui
    imagedata.cpp
    mainwindow.ui
    mainwindow.cpp
    main.cpp
)

FILE(GLOB ASTRO_MAIN_HEADERS
    explorer.h
    imagedata.h
    mainwindow.h
)

ASTRO_add_executable(
    AstroStack
    NAME AstroStack
    FOLDER Main
    SRC ${ASTRO_MAIN_SRC}
    HEADERS ${ASTRO_MAIN_HEADERS}
    LIBRARIES Qt6::Widgets Qt6::Charts
)
