#pragma once

#include <Algos/optim/state.h>

namespace optim
{
namespace step
{
/// Computes a direction based on the Newton method
class Newton
{
public:
    Newton() = default;

    template<typename Function>
    Eigen::VectorXd operator()(const Eigen::VectorXd& x, State& state, const Function& fun) const
    {
        Eigen::VectorXd gradient = fun.gradient(x);
        state.setGradient(gradient);
        return -fun.hessian(x).colPivHouseholderQr().solve(gradient);
    }
};
} // namespace step
} // namespace optim
