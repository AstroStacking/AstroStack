#pragma once

#include <Algos/optim/state.h>

namespace optim
{
namespace line_search
{
class Simple
{
    double m_length;

public:
    Simple(double length)
        : m_length(length)
    {
    }

    Eigen::VectorXd operator()(const Eigen::VectorXd& x, State& state) const
    {
        return x + (std::isnan(state.getInitialStep()) ? m_length : state.getInitialStep()) * state.getDirection();
    }
};
} // namespace line_search
} // namespace optim
