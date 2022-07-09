#include <Algos/optim/line_search/golden_section.h>

#include <optim/simple.h>

#include <gtest/gtest.h>

TEST(LineSearchGoldenSection, GoldenSection)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::GoldenSection lineSearch(0.0001, 1.);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 0, 0;
    lineSearch(origin, fun, state);
    ASSERT_LT(state.getStep(), 0.0001);
}

TEST(LineSearchGoldenSection, GoldenSectionWithInit)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::GoldenSection lineSearch(0.0001, 1.);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);
    state.setInitialStep(1);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    lineSearch(origin, fun, state);
    ASSERT_LT(state.getStep(), 0.0001);
}

TEST(LineSearchGoldenSection, Gradient)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::GoldenSection lineSearch(0.0001, 1.);
    Eigen::VectorXd origin(2);
    origin << 0, 0;
    Eigen::VectorXd gradient(2);
    gradient << 4, -8;
    state.setDirection(gradient);

    Eigen::VectorXd direction(2);
    direction << 1.0588, -2.1176;
    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(direction, 0.001));
    ASSERT_NEAR(state.getStep(), 1.0588/4, 0.0001);
}
