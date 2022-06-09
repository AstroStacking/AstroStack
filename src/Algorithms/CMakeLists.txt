FILE(GLOB ASTRO_ALGORITHMS_SRC
    Algos/affinemodel.cpp
    Algos/model.cpp
)

FILE(GLOB ASTRO_ALGORITHMS_HEADERS
    Algos/affinemodel.h
    Algos/model.h
    Algos/ransac.h
)

ASTRO_add_library(
    AstroAlgorithms
    NAME AstroAlgorithms
    FOLDER Algorithms
    SRC ${ASTRO_ALGORITHMS_SRC}
    HEADERS ${ASTRO_ALGORITHMS_HEADERS}
    LIBRARIES ${ITK_LIBRARIES}
)

add_subdirectory(Tests)
