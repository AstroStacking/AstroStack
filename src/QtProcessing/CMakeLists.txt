FILE(GLOB ASTRO_QTPROCESSING_SRC
    QtProcessing/Mono2Mono/chromasmoothing.cpp
    QtProcessing/Mono2Mono/chromasmoothing.ui
    QtProcessing/Mono2Mono/exponential.cpp
    QtProcessing/Mono2Mono/exponential.ui
    QtProcessing/Mono2Mono/histostretch.cpp
    QtProcessing/Mono2Mono/histostretch.ui
    QtProcessing/Mono2Mono/lightpollution.cpp
    QtProcessing/Mono2Mono/lightpollution.ui
    QtProcessing/Mono2Mono/rldeconvolution.cpp
    QtProcessing/Mono2Mono/rldeconvolution.ui
    QtProcessing/mono2monointerface.cpp
    QtProcessing/multi2multiinterface.cpp
)

FILE(GLOB ASTRO_QTPROCESSING_HEADERS
    QtProcessing/Mono2Mono/chromasmoothing.h
    QtProcessing/Mono2Mono/exponential.h
    QtProcessing/Mono2Mono/histostretch.h
    QtProcessing/Mono2Mono/lightpollution.h
    QtProcessing/Mono2Mono/rldeconvolution.h
    QtProcessing/config.h
    QtProcessing/mono2monointerface.h
    QtProcessing/multi2multiinterface.h
    QtProcessing/ui_mono2monointerface.h
    QtProcessing/ui_multi2multiinterface.h
)

FILE(GLOB ASTRO_QTPROCESSING_DOC
    QtProcessing/Mono2Mono/chromasmoothing.md
    QtProcessing/Mono2Mono/exponential.md
    QtProcessing/Mono2Mono/histostretch.md
    QtProcessing/Mono2Mono/lightpollution.md
    QtProcessing/Mono2Mono/rldeconvolution.md
    QtProcessing/Multi2Mono/maxstacking.md
    QtProcessing/Multi2Mono/medianstacking.md
    QtProcessing/Multi2Mono/robustmeanstacking.md
)

ASTRO_add_library(
    AstroQtProcessing
    NAME AstroQtProcessing
    FOLDER Processing
    SRC ${ASTRO_QTPROCESSING_SRC}
    HEADERS ${ASTRO_QTPROCESSING_HEADERS}
    DOC ${ASTRO_QTPROCESSING_DOC}
    LIBRARIES AstroAlgorithms AstroPlugin AstroProcessing AstroQtIO Qt6::Core Qt6::Widgets ${ITK_LIBRARIES}
)
