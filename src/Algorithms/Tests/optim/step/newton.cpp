#include <Algos/Optim/Step/newton.h>

#include <Optim/powell.h>

#include <gtest/gtest.h>

TEST(NewtonStep, Powell)
{
    optim::State state;
    optim::step::Newton newton;
    Powell fun;

    Eigen::VectorXd x(4);
    x << 0, 0, 0, 0;
    ASSERT_EQ(newton(x, state, fun), -fun.hessian(x).colPivHouseholderQr().solve(fun.gradient(x)));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));

    x << 1, 1, 0, 0;
    ASSERT_EQ(newton(x, state, fun), -fun.hessian(x).colPivHouseholderQr().solve(fun.gradient(x)));
    ASSERT_EQ(state.getGradient(), fun.gradient(x));
}
