#pragma once

#include <eigen3/Eigen/Dense>

namespace astro
{
class RANSAC
{
    const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& m_X;
    const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& m_Y;

    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> m_A{{3, 4}};
    int m_nbInliers{0};

    const int m_nbSamples;
    const int m_nbIterations;
    
    void iterate();

public:
    static constexpr int DEFAULT_NB_SAMPLES = 100;
    static constexpr int DEFAULT_NB_ITERATIONS = 100;

    static Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
    fit(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
        const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y);

    RANSAC(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
           const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y, int nbSamples = DEFAULT_NB_SAMPLES,
           int nbIterations = DEFAULT_NB_ITERATIONS);
    ~RANSAC();

    void run();
    
    Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> predict(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X) const;
};
} // namespace astro
