FILE(GLOB TESTS_ALGORITHMS_SRC
    affinemodel.cpp
    gradient_estimation.cpp
    ransac.cpp
    Filters/Convertors/HSL2RGB.cpp
    Filters/Convertors/RGB2HSL.cpp
    Graph/distance.cpp
    Graph/full_matching.cpp
    Graph/partial_matching.cpp
    ImageStats/center.cpp
    ImageStats/image_statistics.cpp
    ImageStats/mean.cpp
    ImageStats/minmax_position.cpp
    ImageStats/size.cpp
    Optim/powell.cpp
    Optim/quadratic.cpp
    Optim/rosenbrock.cpp
    Optim/state.cpp
    Optim/Criteria/composite.cpp
    Optim/Criteria/criteria.cpp
    Optim/Helper/quadratic.cpp
    Optim/LineSearch/backtracking.cpp
    Optim/LineSearch/damped.cpp
    Optim/LineSearch/golden_section.cpp
    Optim/LineSearch/simple.cpp
    Optim/Optimizer/standard.cpp
    Optim/Step/conjugate_gradient.cpp
    Optim/Step/gradient.cpp
    Optim/Step/newton.cpp
)

FILE(GLOB TESTS_ALGORITHMS_HEADERS
    affinemodel.h
    Optim/powell.h
    Optim/quadratic.h
    Optim/rosenbrock.h
    Optim/simple.h
)

ASTRO_add_test(
    TestsAlgorithms
    NAME TestsAlgorithms
    TESTNAME TestsAlgorithms
    FOLDER Algorithms
    SRC ${TESTS_ALGORITHMS_SRC}
    HEADERS ${TESTS_ALGORITHMS_HEADERS}
    LIBRARIES AstroAlgorithms ${ITK_LIBRARIES}
)
