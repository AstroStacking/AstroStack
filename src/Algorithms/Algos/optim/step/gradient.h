#pragma once

#include <Algos/Optim/state.h>

namespace optim
{
namespace step
{
/// Computes a direction based on the gradient
class Gradient
{
public:
    Gradient() = default;

    template<typename Function, typename State>
    typename State::Vector operator()(const typename State::Vector& x, State& state, const Function& fun) const
    {
        typename State::Vector gradient = fun.gradient(x);
        state.setGradient(gradient);
        return -gradient;
    }
};
} // namespace step
} // namespace optim
