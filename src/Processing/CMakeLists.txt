FILE(GLOB ASTRO_PROCESSING_SRC
    Processing/exponential.cpp
    Processing/exponential.ui
    Processing/histostretch.cpp
    Processing/histostretch.ui
    Processing/monointerface.cpp
    Processing/rldeconvolution.cpp
    Processing/rldeconvolution.ui
)

FILE(GLOB ASTRO_PROCESSING_HEADERS
    Processing/config.h
    Processing/exponential.h
    Processing/histostretch.h
    Processing/monointerface.h
    Processing/rldeconvolution.h
    Processing/ui_monointerface.h
)

FILE(GLOB ASTRO_PROCESSING_DOC
    Processing/exponential.md
    Processing/histostretch.md
    Processing/rldeconvolution.md
)

ASTRO_add_library(
    AstroProcessing
    NAME AstroProcessing
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_SRC}
    HEADERS ${ASTRO_PROCESSING_HEADERS}
    DOC ${ASTRO_PROCESSING_DOC}
    LIBRARIES AstroAlgorithms AstroPlugin AstroIO Qt6::Core Qt6::Widgets ${ITK_LIBRARIES}
)
