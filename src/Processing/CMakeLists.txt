FILE(GLOB ASTRO_PROCESSING_SRC
    Processing/chromasmoothing.cpp
    Processing/exponential.cpp
    Processing/histostretch.cpp
    Processing/lightpollution.cpp
    Processing/rldeconvolution.cpp
)

FILE(GLOB ASTRO_PROCESSING_HEADERS
    Processing/config.h
    Processing/chromasmoothing.h
    Processing/exponential.h
    Processing/histostretch.h
    Processing/lightpollution.h
    Processing/rldeconvolution.h
)

ASTRO_add_library(
    AstroProcessing
    NAME AstroProcessing
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_SRC}
    HEADERS ${ASTRO_PROCESSING_HEADERS}
    LIBRARIES AstroAlgorithms AstroIO ${ITK_LIBRARIES}
)
