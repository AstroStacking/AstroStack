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
    Mono2MonoProcessing/mono2monoprocessing.cpp
    Mono2MonoProcessing/mono2monoprocessing.ui
    Mono2MonoProcessing/mono2monoworkflow.cpp
    Multi2MultiProcessing/multi2multiprocessing.cpp
    Multi2MultiProcessing/multi2multiprocessing.ui
    Multi2MultiProcessing/multi2multiworkflow.cpp
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
    Mono2MonoProcessing/mono2monoprocessing.h
    Mono2MonoProcessing/mono2monoworkflow.h
    Multi2MultiProcessing/multi2multiprocessing.h
    Multi2MultiProcessing/multi2multiworkflow.h
    mainwindow.h
)

ASTRO_add_executable(
    AstroStack
    NAME AstroStack
    FOLDER Main
    SRC ${ASTRO_MAIN_SRC}
    HEADERS ${ASTRO_MAIN_HEADERS}
    LIBRARIES AstroAlgorithms AstroQtIO AstroQtProcessing hdf5::hdf5_cpp Qt6::Widgets Qt6::Charts
    BUNDLE TRUE
)
