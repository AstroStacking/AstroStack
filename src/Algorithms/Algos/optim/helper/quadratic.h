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
        auto result = m_fun(m_X, parameters);
        Eigen::VectorXd accu(result.front().size());
        for(size_t i = 0; i < result.size(); ++i)
        {
            accu = accu + Eigen::pow((m_Y.col(i) - result[i]).array(), 2).matrix();
        }

        return accu.sum();
    }

    Eigen::VectorXd gradient(const Eigen::VectorXd& parameters) const
    {
        auto result = m_fun(m_X, parameters);
        auto resultGradient = m_fun.gradient(m_X, parameters);

        Eigen::VectorXd accu(parameters.size());
        for(size_t i = 0; i < result.size(); ++i)
        {
            accu = accu - 2 * (resultGradient[i] * (m_Y.col(i) - result[i]).transpose()).rowwise().sum();
        }
        return accu;
    }

    Eigen::MatrixXd firstHessian(const Eigen::VectorXd& parameters) const
    {
        Eigen::MatrixXd diff = m_Y - m_fun(m_X, parameters);
        Eigen::TensorMap<Eigen::Tensor<double, 4>> diffView(diff.data(), m_Y.rows(), m_Y.cols(), 1, 1);
        Eigen::array<long, 4> bcast = {1, 1, parameters.size(), parameters.size()};
        Eigen::Tensor<double, 4> full = diffView.broadcast(bcast);

        Eigen::array<Eigen::IndexPair<int>, 1> contractionDims = {Eigen::IndexPair<int>(0, 0)};
        Eigen::Tensor<double, 3> data =
                2 * m_fun.hessian(m_X, parameters).contract(full, contractionDims).sum(Eigen::array<long, 2>({0, 1}));
        return Eigen::Map<Eigen::MatrixXd>(data.data(), parameters.size(), parameters.size());
    }

    Eigen::MatrixXd secondHessian(const Eigen::VectorXd& parameters) const
    {
        Eigen::Tensor<double, 3> gradient = m_fun.gradient(m_X, parameters);
        Eigen::array<Eigen::IndexPair<int>, 2> contractionDims = {Eigen::IndexPair<int>(0, 0),
                                                                  Eigen::IndexPair<int>(1, 1)};

        Eigen::Tensor<double, 2> data = 2 * gradient.contract(gradient, contractionDims);
        std::cout << data << std::endl;
        return Eigen::Map<Eigen::MatrixXd>(data.data(), parameters.size(), parameters.size());
    }

    Eigen::MatrixXd hessian(const Eigen::VectorXd& parameters) const
    {
        return firstHessian(parameters) + secondHessian(parameters);
    }
};
} // namespace helper
} // namespace optim
