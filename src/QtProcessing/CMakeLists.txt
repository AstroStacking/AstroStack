FILE(GLOB ASTRO_QTPROCESSING_SRC
    QtProcessing/chromasmoothing.cpp
    QtProcessing/chromasmoothing.ui
    QtProcessing/exponential.cpp
    QtProcessing/exponential.ui
    QtProcessing/histostretch.cpp
    QtProcessing/histostretch.ui
    QtProcessing/lightpollution.cpp
    QtProcessing/lightpollution.ui
    QtProcessing/monointerface.cpp
    QtProcessing/multiinterface.cpp
    QtProcessing/rldeconvolution.cpp
    QtProcessing/rldeconvolution.ui
)

FILE(GLOB ASTRO_QTPROCESSING_HEADERS
    QtProcessing/config.h
    QtProcessing/chromasmoothing.h
    QtProcessing/exponential.h
    QtProcessing/histostretch.h
    QtProcessing/lightpollution.h
    QtProcessing/monointerface.h
    QtProcessing/multiinterface.h
    QtProcessing/rldeconvolution.h
    QtProcessing/ui_monointerface.h
    QtProcessing/ui_multiinterface.h
)

FILE(GLOB ASTRO_QTPROCESSING_DOC
    QtProcessing/chromasmoothing.md
    QtProcessing/exponential.md
    QtProcessing/histostretch.md
    QtProcessing/lightpollution.md
    QtProcessing/rldeconvolution.md
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
