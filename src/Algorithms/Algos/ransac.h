#pragma once

#include <eigen3/Eigen/Dense>

namespace astro
{
class RANSAC
{
    const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& m_X;
    const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& m_Y;

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m_A;

    const int m_nbSamples;

public:
    static constexpr int DEFAULT_NB_SAMPLES = 100;

    static Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
    fit(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
        const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y);

    RANSAC(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
           const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y, int nbSamples = DEFAULT_NB_SAMPLES);
    ~RANSAC();

    bool run();
};
} // namespace astro
