#pragma once

#include <Eigen/Dense>

class Rosenbrock
{
public:
    static constexpr int Size = 2;
    double operator()(const Eigen::VectorXd& x) const;
    Eigen::VectorXd gradient(const Eigen::VectorXd& x) const;
    Eigen::MatrixXd hessian(const Eigen::VectorXd& x) const;
};
