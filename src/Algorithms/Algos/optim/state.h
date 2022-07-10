#pragma once

#include <Eigen/Dense>

#include <cstddef>
#include <limits>
#include <ostream>
#include <unordered_map>

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

template<int Size = Eigen::Dynamic>
class State
{
public:
    using Matrix = Eigen::Matrix<double, Size, Size>;
    using Vector = Eigen::Matrix<double, Size, 1>;
private:
    size_t m_iteration{};
    Status m_status{Status::NOT_STOPPED};

    double m_prevValue{std::numeric_limits<double>::max()};
    double m_currentValue{std::numeric_limits<double>::max()};
    double m_step{std::numeric_limits<double>::quiet_NaN()};
    double m_initialStep{std::numeric_limits<double>::quiet_NaN()};

    Vector m_direction;
    Vector m_gradient;
    Vector m_prevPoint;
    Vector m_currentPoint;

    std::unordered_map<std::string, double> m_miscellaneousData;

public:
    void increaseIteration() { ++m_iteration; }
    size_t getCurrentIteration() const { return m_iteration; }
    Status getStatus() const { return m_status; }
    double getPreviousValue() const { return m_prevValue; }
    double getCurrentValue() const { return m_currentValue; }
    double getStep() const { return m_step; }
    double getInitialStep() const { return m_initialStep; }
    Vector getDirection() const { return m_direction; }
    Vector getGradient() const { return m_gradient; }
    Vector getPreviousPoint() const { return m_prevPoint; }
    Vector getCurrentPoint() const { return m_currentPoint; }
    double getData(const std::string& name) const { return m_miscellaneousData.at(name); }

    void setStatus(Status status) { m_status = status; }
    void setCurrentValue(double value)
    {
        m_prevValue = m_currentValue;
        m_currentValue = value;
    }
    void setCurrentPoint(const Vector& point)
    {
        m_prevPoint = m_currentPoint;
        m_currentPoint = point;
    }
    void setStep(double step) { m_step = step; }
    void setInitialStep(double step) { m_initialStep = step; }
    void resetInitialStep() { m_initialStep = std::numeric_limits<double>::quiet_NaN(); }
    void setDirection(const Vector& direction) { m_direction = direction; }
    void setGradient(const Vector& gradient) { m_gradient = gradient; }
    void setData(const std::string& name, double value) { m_miscellaneousData[name] = value; }
};

template<int Size>
inline std::ostream& operator<<(std::ostream& stream, const State<Size>& state)
{
    stream << "Status " << state.getStatus();
    stream << "\n\tIteration: " << state.getCurrentIteration();
    stream << "\n\tValue: " << state.getCurrentValue();
    stream << "\n\tPoint: " << state.getCurrentPoint().transpose();
    stream << "\n\tGradient: " << state.getGradient().transpose();
    stream << "\n\tDirection: " << state.getDirection().transpose();

    return stream;
}
} // namespace optim
