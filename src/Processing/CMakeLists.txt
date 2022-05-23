FILE(GLOB ASTRO_PROCESSING_SRC
    Processing/mono.cpp
)

FILE(GLOB ASTRO_PROCESSING_HEADERS
    Processing/mono.h
)

ASTRO_add_library(
    AstroProcessing
    NAME AstroProcessing
    FOLDER Processing
    SRC ${ASTRO_PROCESSING_SRC}
    HEADERS ${ASTRO_PROCESSING_HEADERS}
    LIBRARIES AstroPlugin AstroIO Qt6::Core Qt6::Widgets ${ITK_LIBRARIES}
)