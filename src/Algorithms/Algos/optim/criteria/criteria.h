#pragma once

#include <cstddef>

#include <Algos/optim/state.h>

namespace optim
{
namespace criteria
{
class Iteration
{
    size_t m_iterationMax;

public:
    explicit Iteration(size_t iterationMax)
        : m_iterationMax(iterationMax)
    {
    }

    bool operator()(State& state) const
    {
        bool result = state.getCurrentIteration() >= m_iterationMax;
        if (result)
        {
            state.setStatus(Status::ITERATION_MAX_REACHED);
        }
        return result;
    }
};
} // namespace criteria
} // namespace optim
