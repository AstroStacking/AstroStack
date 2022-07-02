#pragma once

#include <Eigen/Dense>

#include <cstddef>
#include <limits>
#include <ostream>

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

inline std::ostream& operator<<(std::ostream& stream, const Status& status)
{
    switch (status)
    {
        case Status::NOT_STOPPED:
            stream << "NOT STOPPED";
            break;
        case Status::ITERATION_MAX_REACHED:
            stream << "ITERATION MAX REACHED";
            break;
        case Status::NON_MONOTONOUS:
            stream << "NON MONOTONOUS";
            break;
        case Status::SMALL_DELTA_F:
            stream << "SMALL DELTA F";
            break;
        case Status::SMALL_DELTA_F_F:
            stream << "SMALL DELTA F F";
            break;
    }
    return stream;
}

class State
{
    size_t m_iteration{};
    Status m_status{Status::NOT_STOPPED};

    double m_prevValue{std::numeric_limits<double>::max()};
    double m_currentValue{std::numeric_limits<double>::max()};
    double m_step{std::numeric_limits<double>::quiet_NaN()};
    double m_initialStep{std::numeric_limits<double>::quiet_NaN()};

    Eigen::VectorXd m_direction;
    Eigen::VectorXd m_gradient;
    Eigen::VectorXd m_prevPoint;
    Eigen::VectorXd m_currentPoint;

public:
    void increaseIteration() { ++m_iteration; }
    size_t getCurrentIteration() const { return m_iteration; }
    Status getStatus() const { return m_status; }
    double getPreviousValue() const { return m_prevValue; }
    double getCurrentValue() const { return m_currentValue; }
    double getStep() const { return m_step; }
    double getInitialStep() const { return m_initialStep; }
    Eigen::VectorXd getDirection() const { return m_direction; }
    Eigen::VectorXd getGradient() const { return m_gradient; }
    Eigen::VectorXd getPreviousPoint() const { return m_prevPoint; }
    Eigen::VectorXd getCurrentPoint() const { return m_currentPoint; }

    void setStatus(Status status) { m_status = status; }
    void setCurrentValue(double value)
    {
        m_prevValue = m_currentValue;
        m_currentValue = value;
    }
    void setCurrentPoint(const Eigen::VectorXd& point)
    {
        m_prevPoint = m_currentPoint;
        m_currentPoint = point;
    }
    void setStep(double step) { m_step = step; }
    void setInitialStep(double step) { m_initialStep = step; }
    void resetInitialStep() { m_initialStep = std::numeric_limits<double>::quiet_NaN(); }
    void setDirection(const Eigen::VectorXd& direction) { m_direction = direction; }
    void setGradient(const Eigen::VectorXd& gradient) { m_gradient = gradient; }
};

inline std::ostream& operator<<(std::ostream& stream, const State& state)
{
    stream << "Status " << state.getStatus();
    stream << "\n\tIteration: " << state.getCurrentIteration();
    stream << "\n\tValue: " << state.getCurrentValue();
    stream << "\n\tPoint: " << state.getCurrentPoint().transpose();

    return stream;
}
} // namespace optim
