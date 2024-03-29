#pragma once

#include <Algos/Optim/state.h>

namespace optim
{
namespace line_search
{
/// Just takes a step in the direction computed by a step
class Simple
{
    double m_length;

public:
    Simple(double length)
        : m_length(length)
    {
    }

    template<typename Function, typename State>
    typename State::Vector operator()(const typename State::Vector& x, const Function& fun, State& state) const
    {
        return x + (std::isnan(state.getInitialStep()) ? m_length : state.getInitialStep()) * state.getDirection();
    }
};
} // namespace line_search
} // namespace optim
