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

FILE(GLOB ASTRO_PROCESSING_CHROMA
    Standalone/chromasmoothing.cpp
)

ASTRO_add_executable(
    ChromaSmoothing
    NAME ChromaSmoothing
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_CHROMA}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_EXPONENTIAL
    Standalone/exponential.cpp
)

ASTRO_add_executable(
    Exponential
    NAME Exponential
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_EXPONENTIAL}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_HISTOSTRETCH
    Standalone/histostretch.cpp
)

ASTRO_add_executable(
    HistoStretch
    NAME HistoStretch
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_HISTOSTRETCH}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_LIGHTPOLLUTION
    Standalone/lightpollution.cpp
)

ASTRO_add_executable(
    LightPollution
    NAME LightPollution
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_LIGHTPOLLUTION}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_DECONVOLUTION
    Standalone/rldeconvolution.cpp
)

ASTRO_add_executable(
    Deconvolution
    NAME Deconvolution
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_DECONVOLUTION}
    LIBRARIES AstroProcessing Qt6::Core
)
