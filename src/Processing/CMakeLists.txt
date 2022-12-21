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
    LIBRARIES AstroAlgorithms AstroIO ${ITK_LIBRARIES} hdf5::hdf5_cpp
)

add_subdirectory(Standalone)
add_subdirectory(Tests)
