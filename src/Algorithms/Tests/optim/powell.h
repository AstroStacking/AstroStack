#pragma once

#include <Eigen/Dense>

class Powell
{
public:
    double operator()(const Eigen::VectorXd& x) const;
    Eigen::VectorXd gradient(const Eigen::VectorXd& x) const;
    Eigen::MatrixXd hessian(const Eigen::VectorXd& x) const;
};
