#include "rosenbrock.h"

#include <gtest/gtest.h>

double Rosenbrock::operator()(const Eigen::VectorXd& x) const
{
    return 100.0 * std::pow((x(1) - x(0) * x(0)), 2) + std::pow(1. - x(0), 2);
}

Eigen::VectorXd Rosenbrock::gradient(const Eigen::VectorXd& x) const
{
    Eigen::VectorXd grad(x.size());
    grad << -400 * x(0) * (x(1) - x(0) * x(0)) - 2 * (1 - x(0)), 200 * (x(1) - x(0) * x(0));
    return grad;
}

Eigen::MatrixXd Rosenbrock::hessian(const Eigen::VectorXd& x) const
{
    Eigen::MatrixXd mat(x.size(), x.size());

    mat << 1200. * x(0) * x(0) - 400. * x(1) + 2., -400 * x(1), -400 * x(1), 200;

    return mat;
}

TEST(Rosenbrock, Direct)
{
    Rosenbrock function;
    Eigen::VectorXd x(2);
    x << 1, 1;
    ASSERT_EQ(function(x), 0);
}

TEST(Rosenbrock, Gradient)
{
    Rosenbrock function;
    Eigen::VectorXd x(2);
    x << 1, 1;

    Eigen::VectorXd result(2);
    result << 0, 0;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 0, 1;
    result << -2, 200;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 1, 0;
    result << 400, -200;
    ASSERT_TRUE(function.gradient(x).isApprox(result));
}

TEST(Rosenbrock, Hessian)
{
    Rosenbrock function;
    Eigen::VectorXd x(2);
    x << 0, 0;

    Eigen::MatrixXd result(2, 2);
    result << 2, 0, 0, 200;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 1, 0;
    result << 1202, 0, 0, 200;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 0, 1;
    result << -398, -400, -400, 200;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 1, 1;
    result << 802, -400, -400, 200;
    ASSERT_TRUE(function.hessian(x).isApprox(result));
}
