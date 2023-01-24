FILE(GLOB ASTRO_PROCESSING_CHROMA
    chromasmoothing.cpp
)

ASTRO_add_executable(
    ChromaSmoothing
    NAME ChromaSmoothing
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_CHROMA}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_DECONVOLUTION
    deconvolution.cpp
)

ASTRO_add_executable(
    Deconvolution
    NAME Deconvolution
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_DECONVOLUTION}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_EXPONENTIAL
    exponential.cpp
)

ASTRO_add_executable(
    Exponential
    NAME Exponential
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_EXPONENTIAL}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_HISTOSTRETCH
    histostretch.cpp
)

ASTRO_add_executable(
    HistoStretch
    NAME HistoStretch
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_HISTOSTRETCH}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_GREY
    grey.cpp
)

ASTRO_add_executable(
    Grey
    NAME Grey
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_GREY}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_LIGHTPOLLUTION
    lightpollution.cpp
)

ASTRO_add_executable(
    LightPollution
    NAME LightPollution
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_LIGHTPOLLUTION}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_REGISTER
    register.cpp
)

#ASTRO_add_executable(
#    Register
#    NAME Register
#    FOLDER Processing/Standalone
#    SRC ${ASTRO_PROCESSING_REGISTER}
#    LIBRARIES AstroProcessing Qt6::Core
#)

FILE(GLOB ASTRO_PROCESSING_STACKING
    stacking.cpp
)

ASTRO_add_executable(
    Stacking
    NAME Stacking
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_STACKING}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_STARDETECTION
    stardetection.cpp
)

ASTRO_add_executable(
    StarDetection
    NAME StarDetection
    FOLDER Processing/Standalone
    SRC ${ASTRO_PROCESSING_STARDETECTION}
    LIBRARIES AstroProcessing Qt6::Core
)
