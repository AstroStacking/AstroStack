#pragma once

#include <Algos/optim/state.h>

#include <cmath>
#include <cstddef>

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

class RelativeValue
{
    double m_ftol;

public:
    explicit RelativeValue(double ftol)
        : m_ftol(ftol)
    {
    }

    bool operator()(State& state) const
    {
        bool result = std::abs(state.getCurrentValue() - state.getPreviousValue()) /
                              std::abs(state.getCurrentValue() + state.getPreviousValue() +
                                       std::numeric_limits<double>::epsilon()) <=
                      m_ftol;
        if (result)
        {
            state.setStatus(Status::SMALL_DELTA_F_F);
        }
        return result;
    }
};

class AbsoluteValue
{
    double m_ftol;

public:
    explicit AbsoluteValue(double ftol)
        : m_ftol(ftol)
    {
    }

    bool operator()(State& state) const
    {
        bool result = std::abs(state.getCurrentValue() - state.getPreviousValue()) <= m_ftol;
        if (result)
        {
            state.setStatus(Status::SMALL_DELTA_F);
        }
        return result;
    }
};
} // namespace criteria
} // namespace optim
