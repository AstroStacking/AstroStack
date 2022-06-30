#pragma once

#include <cstddef>

namespace optim
{
enum class Status
{
    NOT_STOPPED,
    ITERATION_MAX_REACHED
};

class State
{
    size_t m_iteration{};
    Status m_status{Status::NOT_STOPPED};

public:
    void increaseIteration() { ++m_iteration; }
    size_t getCurrentIteration() const { return m_iteration; }
    Status getStatus() const { return m_status; }

    void setStatus(Status status) { m_status = status; }
};
} // namespace optim
