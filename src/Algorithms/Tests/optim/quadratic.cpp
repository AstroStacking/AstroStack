#include "quadratic.h"

#include <gtest/gtest.h>

double Quadratic::operator()(const Eigen::VectorXd& x)
{
    return std::pow(x(0) + 2 * x(1) - 7, 2) + std::pow(2 * x(0) + x(1) - 5, 2);
}

Eigen::VectorXd Quadratic::gradient(const Eigen::VectorXd& x)
{
    Eigen::VectorXd grad(x.size());
    grad << 2 * (x(0) + 2 * x(1) - 7) + 4 * (2 * x(0) + x(1) - 5),
            4 * (x(0) + 2 * x(1) - 7) + 2 * (2 * x(0) + x(1) - 5);
    return grad;
}

Eigen::MatrixXd Quadratic::hessian(const Eigen::VectorXd& x)
{
    Eigen::MatrixXd mat(x.size(), x.size());

    mat << 10, 8, 8, 10;

    return mat;
}

TEST(Quadratic, Direct)
{
    Quadratic function;
    Eigen::VectorXd x(2);
    x << 1, 3;
    ASSERT_EQ(function(x), 0);
}

TEST(Quadratic, Gradient)
{
    Quadratic function;
    Eigen::VectorXd x(2);
    x << 1, 3;

    Eigen::VectorXd result(2);
    result << 0, 0;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 0, 3;
    result << -10, -8;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 1, 0;
    result << -24, -30;
    ASSERT_TRUE(function.gradient(x).isApprox(result));
}

TEST(Quadratic, Hessian)
{
    Quadratic function;
    Eigen::VectorXd x(2);
    x << 0, 0;

    Eigen::MatrixXd result(2, 2);
    result << 10, 8, 8, 10;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 1, 0;
    ASSERT_TRUE(function.hessian(x).isApprox(result));
}
