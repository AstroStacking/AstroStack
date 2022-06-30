FILE(GLOB ASTRO_ALGORITHMS_SRC
    Algos/affinemodel.cpp
    Algos/model.cpp
)

FILE(GLOB ASTRO_ALGORITHMS_HEADERS
    Algos/affinemodel.h
    Algos/config.h
    Algos/model.h
    Algos/ransac.h
    Algos/optim/state.h
    Algos/optim/criteria/composite.h
    Algos/optim/criteria/criteria.h
)

ASTRO_add_library(
    AstroAlgorithms
    NAME AstroAlgorithms
    FOLDER Algorithms
    SRC ${ASTRO_ALGORITHMS_SRC}
    HEADERS ${ASTRO_ALGORITHMS_HEADERS}
    LIBRARIES ${ITK_LIBRARIES} Eigen3::Eigen
)

add_subdirectory(Tests)
