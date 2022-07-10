#pragma once

#include <Eigen/Dense>

class Simple
{
public:
    static constexpr int Size = 2;
    double operator()(const Eigen::VectorXd& X) const { return std::pow(X(0) - 2, 2) + std::pow(2 * X(1) + 4, 2); }

    Eigen::VectorXd gradient(const Eigen::VectorXd& X) const
    {
        Eigen::VectorXd result(2);
        result << 2. * (X(0) - 2), 4 * (2 * X(1) + 4);

        return result;
    }
};
