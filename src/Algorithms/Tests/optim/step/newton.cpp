#include <Algos/optim/step/newton.h>

#include <gtest/gtest.h>

#include <optim/powell.h>

TEST(Newton, Powell)
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
