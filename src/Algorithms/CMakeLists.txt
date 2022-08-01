FILE(GLOB ASTRO_ALGORITHMS_SRC
    Algos/model.cpp
)

FILE(GLOB ASTRO_ALGORITHMS_HEADERS
    Algos/config.h
    Algos/gradient_estimation.h
    Algos/ransac.h
    Algos/Filters/Convertors/HSL2RGB.h
    Algos/Filters/Convertors/RGB2HSL.h
    Algos/Filters/mergehslfilter.h
    Algos/optim/state.h
    Algos/optim/criteria/composite.h
    Algos/optim/criteria/criteria.h
    Algos/optim/line_search/backtracking.h
    Algos/optim/line_search/damped.h
    Algos/optim/line_search/golden_section.h
    Algos/optim/line_search/simple.h
    Algos/optim/helper/quadratic.h
    Algos/optim/optimizer/standard.h
    Algos/optim/step/conjugate_gradient.h
    Algos/optim/step/gradient.h
    Algos/optim/step/newton.h
)

ASTRO_add_library(
    AstroAlgorithms
    NAME AstroAlgorithms
    FOLDER Algorithms
    SRC ${ASTRO_ALGORITHMS_SRC}
    HEADERS ${ASTRO_ALGORITHMS_HEADERS}
    LIBRARIES AstroIO ${ITK_LIBRARIES} Eigen3::Eigen
)

add_subdirectory(Tests)
