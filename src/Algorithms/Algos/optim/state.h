#pragma once

#include <cstddef>
#include <limits>

namespace optim
{
enum class Status
{
    /// Optimisation is not finished
    NOT_STOPPED,
    /// Max iteration has been reached
    ITERATION_MAX_REACHED,
    /// Optimisation led to an increase of the cost function
    NON_MONOTONOUS,
    /// Absolute delta is below give threshold
    SMALL_DELTA_F,
    /// Relative delta is below give threshold
    SMALL_DELTA_F_F
};

class State
{
    size_t m_iteration{};
    Status m_status{Status::NOT_STOPPED};

    double m_prevValue{std::numeric_limits<double>::max()};
    double m_currentValue{std::numeric_limits<double>::max()};

public:
    void increaseIteration() { ++m_iteration; }
    size_t getCurrentIteration() const { return m_iteration; }
    Status getStatus() const { return m_status; }
    double getPreviousValue() const { return m_prevValue; }
    double getCurrentValue() const { return m_currentValue; }

    void setStatus(Status status) { m_status = status; }
    void setCurrentValue(double value)
    {
        m_prevValue = m_currentValue;
        m_currentValue = value;
    }
};
} // namespace optim
