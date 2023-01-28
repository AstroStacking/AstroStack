#include <Algos/Optim/LineSearch/backtracking.h>

#include <Optim/simple.h>

#include <gtest/gtest.h>

TEST(LineSearchBacktracking, Damped)
{
    Simple fun;

    const double size = .1;
    optim::State state;

    optim::line_search::Backtracking lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 0, 0;
    state.setGradient(fun.gradient(origin));

    ASSERT_TRUE(lineSearch(origin, fun, state).isMuchSmallerThan(direction));
    ASSERT_LT(state.getStep(), 0.00001);
}

TEST(LineSearchBacktracking, DampedWithInit)
{
    Simple fun;

    const double size = .1;
    optim::State state;

    optim::line_search::Backtracking lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 1, 1;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 0, 0;
    state.setGradient(fun.gradient(origin));
    state.setInitialStep(1);

    ASSERT_TRUE(lineSearch(origin, fun, state).isMuchSmallerThan(direction));
    ASSERT_LT(state.getStep(), 0.00001);
}

TEST(LineSearchBacktracking, Gradient)
{
    Simple fun;

    const double size = .1;
    optim::State state;

    optim::line_search::Backtracking lineSearch(0.0001, 0.1);
    Eigen::VectorXd direction(2);
    direction << 4, -8;
    state.setDirection(direction);

    Eigen::VectorXd origin(2);
    origin << 0, 0;
    state.setGradient(fun.gradient(origin));
    state.setInitialStep(1);

    ASSERT_LT(fun(lineSearch(origin, fun, state)), fun(origin) + 0.1 * state.getStep() * direction.squaredNorm());
    ASSERT_GT(state.getStep(), 0);
}
