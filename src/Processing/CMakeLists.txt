FILE(GLOB ASTRO_PROCESSING_SRC
    Processing/chromasmoothing.cpp
    Processing/exponential.cpp
    Processing/grey.cpp
    Processing/histostretch.cpp
    Processing/lightpollution.cpp
    Processing/maxstacking.cpp
    Processing/rldeconvolution.cpp
    Processing/stardetection.cpp
)

FILE(GLOB ASTRO_PROCESSING_HEADERS
    Processing/config.h
    Processing/chromasmoothing.h
    Processing/exponential.h
    Processing/grey.h
    Processing/histostretch.h
    Processing/lightpollution.h
    Processing/maxstacking.h
    Processing/rldeconvolution.h
    Processing/stardetection.h
)

ASTRO_add_library(
    AstroProcessing
    NAME AstroProcessing
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_SRC}
    HEADERS ${ASTRO_PROCESSING_HEADERS}
    LIBRARIES AstroAlgorithms AstroIO ${ITK_LIBRARIES} hdf5::hdf5_cpp hdf5::hdf5
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

FILE(GLOB ASTRO_PROCESSING_DECONVOLUTION
    Standalone/deconvolution.cpp
)

ASTRO_add_executable(
    Deconvolution
    NAME Deconvolution
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_DECONVOLUTION}
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

FILE(GLOB ASTRO_PROCESSING_GREY
    Standalone/grey.cpp
)

ASTRO_add_executable(
    Grey
    NAME Grey
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_GREY}
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

FILE(GLOB ASTRO_PROCESSING_READ
    Standalone/read.cpp
)

ASTRO_add_executable(
    Read
    NAME Read
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_READ}
    LIBRARIES AstroProcessing Qt6::Core
)

FILE(GLOB ASTRO_PROCESSING_REGISTER
    Standalone/register.cpp
)

#ASTRO_add_executable(
#    Register
#    NAME Register
#    FOLDER Processing
#    SRC ${ASTRO_PROCESSING_REGISTER}
#    LIBRARIES AstroProcessing Qt6::Core
#)

FILE(GLOB ASTRO_PROCESSING_STACKING
    Standalone/stacking.cpp
)

ASTRO_add_executable(
    Stacking
    NAME Stacking
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_STACKING}
    LIBRARIES AstroProcessing Qt6::Core
)
