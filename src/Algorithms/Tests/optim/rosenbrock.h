#pragma once

#include <Eigen/Dense>

class Rosenbrock
{
public:
    double operator()(const Eigen::VectorXd& x);
    Eigen::VectorXd gradient(const Eigen::VectorXd& x);
    Eigen::MatrixXd hessian(const Eigen::VectorXd& x);
};
