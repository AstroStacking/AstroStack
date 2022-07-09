#include <Algos/optim/line_search/damped.h>

#include <optim/simple.h>

#include <gtest/gtest.h>

TEST(LineSearchDamped, Damped)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::Damped lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(0.125 * direction));
    ASSERT_EQ(state.getStep(), 0.125);
}

TEST(LineSearchDamped, DampedWithInit)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::Damped lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);
    state.setInitialStep(1);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(0.125 * direction));
    ASSERT_EQ(state.getStep(), 0.125);
}

TEST(LineSearchDamped, VeryDamped)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::Damped lineSearch(0.0001, 1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);
    state.setInitialStep(1);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(direction));
    ASSERT_EQ(state.getStep(), 1);
}

TEST(LineSearchDamped, GradientDirection)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::Damped lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 4, -8;
    state.setDirection(direction);
    state.setInitialStep(1);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(0.5 * direction));
    ASSERT_EQ(state.getStep(), .5);
}

TEST(LineSearchDamped, Fail)
{
    Simple fun;

    const double size = .1;
    optim::State state;
    optim::line_search::Damped lineSearch(0.2, 0.1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);
    state.setInitialStep(1);

    Eigen::VectorXd origin(2);
    origin << 0, 0;

    ASSERT_TRUE(lineSearch(origin, fun, state).isApprox(origin));
    ASSERT_EQ(state.getStep(), 0);
}
