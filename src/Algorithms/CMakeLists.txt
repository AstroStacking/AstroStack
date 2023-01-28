FILE(GLOB ASTRO_ALGORITHMS_SRC
    Algos/ImageStats/center.cpp
    Algos/ImageStats/mean.cpp
    Algos/ImageStats/minmax_position.cpp
    Algos/ImageStats/size.cpp
    Algos/model.cpp
)

FILE(GLOB ASTRO_ALGORITHMS_HEADERS
    Algos/config.h
    Algos/gradient_estimation.h
    Algos/ransac.h
    Algos/Filters/Convertors/HSL2RGB.h
    Algos/Filters/Convertors/RGB2HSL.h
    Algos/Filters/Stackers/max.h
    Algos/Filters/mergehslfilter.h
    Algos/Filters/multifunctorimagefilter.h
    Algos/ImageStats/center.h
    Algos/ImageStats/image_statistics.h
    Algos/ImageStats/mean.h
    Algos/ImageStats/minmax_position.h
    Algos/ImageStats/size.h
    Algos/ImageStats/traits.h
    Algos/Optim/state.h
    Algos/Optim/Criteria/composite.h
    Algos/Optim/Criteria/criteria.h
    Algos/Optim/LineSearch/backtracking.h
    Algos/Optim/LineSearch/damped.h
    Algos/Optim/LineSearch/golden_section.h
    Algos/Optim/LineSearch/simple.h
    Algos/Optim/Helper/quadratic.h
    Algos/Optim/Optimizer/standard.h
    Algos/Optim/Step/conjugate_gradient.h
    Algos/Optim/Step/gradient.h
    Algos/Optim/Step/newton.h
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
