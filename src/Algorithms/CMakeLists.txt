FILE(GLOB ASTRO_ALGORITHMS_SRC
    Algos/ransac.cpp
)

FILE(GLOB ASTRO_ALGORITHMS_HEADERS
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
