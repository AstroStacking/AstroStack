#include "powell.h"

#include <gtest/gtest.h>

double Powell::operator()(const Eigen::VectorXd& x) const
{
    return std::pow(x(0) + 10 * x(1), 2) + 5 * std::pow(x(2) - x(3), 2) + std::pow(x(1) - 2 * x(2), 4) +
           10 * std::pow(x(0) - x(3), 4);
}

Eigen::VectorXd Powell::gradient(const Eigen::VectorXd& x) const
{
    Eigen::VectorXd grad(x.size());
    grad << 2 * (x(0) + 10 * x(1)) + 40 * std::pow(x(0) - x(3), 3),
            4 * std::pow(x(1) - 2 * x(2), 3) + 20 * (x(0) + 10 * x(1)),
            10 * (x(2) - x(3)) - 8 * std::pow(x(1) - 2 * x(2), 3), -10 * (x(2) - x(3)) - 40 * std::pow(x(0) - x(3), 3);
    return grad;
}

Eigen::MatrixXd Powell::hessian(const Eigen::VectorXd& x) const
{
    Eigen::MatrixXd mat(x.size(), x.size());

    mat << 2 + 120 * std::pow(x(0) - x(3), 2), 20, 0, -120 * std::pow(x(0) - x(3), 2), 20,
            12 * std::pow(x(1) - 2 * x(2), 2) + 200, -24 * std::pow(x(1) - 2 * x(2), 2), 0, 0,
            -24 * std::pow(x(1) - 2 * x(2), 2), 10 + 48 * std::pow(x(1) - 2 * x(2), 2), -10,
            -120 * std::pow(x(0) - x(3), 2), 0, -10, 10 + 120 * std::pow(x(0) - x(3), 2);

    return mat;
}

TEST(Powell, Direct)
{
    Powell function;
    Eigen::VectorXd x(4);
    x << 0, 0, 0, 0;
    ASSERT_EQ(function(x), 0);

    x << 1, 1, 0, 0;
    ASSERT_EQ(function(x), 132);
}

TEST(Powell, Gradient)
{
    Powell function;
    Eigen::VectorXd x(4);
    x << 0, 0, 0, 0;

    Eigen::VectorXd result(4);
    result << 0, 0, 0, 0;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 1, 0, 0, 0;
    result << 42, 20, 0, -40;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 0, 1, 0, 0;
    result << 20, 204, -8, 0;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 0, 0, 1, 0;
    result << 0, -32, 74, -10;
    ASSERT_TRUE(function.gradient(x).isApprox(result));

    x << 0, 0, 0, 1;
    result << -40, 0, -10, 50;
    ASSERT_TRUE(function.gradient(x).isApprox(result));
}

TEST(Powell, Hessian)
{
    Powell function;
    Eigen::VectorXd x(4);
    x << 0, 0, 0, 0;

    Eigen::MatrixXd result(4, 4);
    result << 2, 20, 0, 0, 20, 200, 0, 0, 0, 0, 10, -10, 0, 0, -10, 10;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 1, 0, 0, 0;
    result << 122, 20, 0, -120, 20, 200, 0, 0, 0, 0, 10, -10, -120, 0, -10, 130;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 0, 1, 0, 0;
    result << 2, 20, 0, 0, 20, 212, -24, 0, 0, -24, 58, -10, 0, 0, -10, 10;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 0, 0, 1, 0;
    result << 2, 20, 0, 0, 20, 248, -96, 0, 0, -96, 202, -10, 0, 0, -10, 10;
    ASSERT_TRUE(function.hessian(x).isApprox(result));

    x << 0, 0, 0, 1;
    result << 122, 20, 0, -120, 20, 200, 0, 0, 0, 0, 10, -10, -120, 0, -10, 130;
    ASSERT_TRUE(function.hessian(x).isApprox(result));
}
