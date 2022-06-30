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

class Monotony
{
    double m_ftol;

public:
    explicit Monotony(double ftol)
        : m_ftol(ftol)
    {
    }

    bool operator()(State& state) const
    {
        bool result = state.getCurrentValue() >= state.getPreviousValue() * (1 + m_ftol);
        if (result)
        {
            state.setStatus(Status::NON_MONOTONOUS);
        }
        return result;
    }
};
} // namespace criteria
} // namespace optim
