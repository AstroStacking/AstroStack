#pragma once

#include <Algos/optim/state.h>

namespace optim
{
namespace step
{
class Gradient
{
public:
    Gradient() = default;

    template<typename Function>
    Eigen::VectorXd operator()(const Eigen::VectorXd& x, State& state, const Function& fun) const
    {
        Eigen::VectorXd gradient = fun.gradient(x);
        state.setGradient(gradient);
        return gradient;
    }
};
} // namespace step
} // namespace optim
