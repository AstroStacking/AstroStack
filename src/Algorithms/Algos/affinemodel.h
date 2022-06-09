#pragma once

#include <eigen3/Eigen/Dense>

namespace astro
{
class AffineModel
{
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m_A;

public:
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> getData() const { return m_A; }

    void fit(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
             const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y);

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
    predict(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X) const;
};
} // namespace astro
