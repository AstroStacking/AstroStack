#pragma once

#include <Algos/optim/state.h>

#include <unsupported/Eigen/CXX11/Tensor>

#include <iostream>

namespace optim
{
namespace helper
{
/// Helper class to do a least square optimization
template<typename Function>
class Quadratic
{
    const Function& m_fun;
    const Eigen::MatrixXd& m_X;
    const Eigen::MatrixXd& m_Y;

public:
    Quadratic(const Function& fun, const Eigen::MatrixXd& X, const Eigen::MatrixXd& Y)
        : m_fun(fun)
        , m_X(X)
        , m_Y(Y)
    {
    }

    double operator()(const Eigen::VectorXd& parameters) const
    {
        return Eigen::pow((m_fun(m_X, parameters) - m_Y).array(), 2).sum();
    }

    Eigen::VectorXd gradient(const Eigen::VectorXd& parameters) const
    {
        return 2 * (m_fun.gradient(m_X, parameters) * (m_fun(m_X, parameters) - m_Y).transpose());
    }

    Eigen::MatrixXd hessian(const Eigen::VectorXd& parameters) const
    {
        Eigen::MatrixXd diff = m_Y - m_fun(m_X, parameters);
        Eigen::TensorMap<Eigen::Tensor<double, 4>> diffView(diff.data(), m_Y.cols(), m_Y.rows(), 1, 1);
        Eigen::array<long, 4> bcast = {1, 1, parameters.size(), parameters.size()};
        Eigen::Tensor<double, 4> full = diffView.broadcast(bcast);

        Eigen::array<Eigen::IndexPair<int>, 2> contractionDims = {Eigen::IndexPair<int>(0, 0), Eigen::IndexPair<int>(1, 1)};
        std::array<long, 2> offset = {0, 0};
        std::array<long, 2> extent = {parameters.size(), parameters.size()};
        Eigen::Tensor<double, 2> first =
                -2 * m_fun.hessian(m_X, parameters).contract(full, contractionDims).slice(offset, extent);
        Eigen::Map<Eigen::MatrixXd> extr(first.data(), parameters.size(), parameters.size());

        Eigen::MatrixXd gradient = m_fun.gradient(m_X, parameters);
        std::cout << m_Y - m_fun(m_X, parameters) << std::endl;
        std::cout << extr << std::endl;
        std::cout << 2 * gradient * gradient.transpose() << std::endl;
        std::cout << (extr + 2 * gradient * gradient.transpose()) << std::endl;
        return extr + 2 * gradient * gradient.transpose();
    }
};
} // namespace helper
} // namespace optim
