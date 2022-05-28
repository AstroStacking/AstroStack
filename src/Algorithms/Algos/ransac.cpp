#include "ransac.h"

namespace astro
{
RANSAC::RANSAC(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
               const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y, int nbSamples, int nbIterations)
    : m_X(X)
    , m_Y(Y)
    , m_nbSamples(nbSamples)
    , m_nbIterations(nbIterations)
{
}

RANSAC::~RANSAC() = default;

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
RANSAC::fit(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
            const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y)
{
    return Y * X.transpose() * (X * X.transpose()).inverse();
}

void RANSAC::run()
{
    m_nbInliers = 0;
    for(int i = 0; i < m_nbIterations; ++i)
    {
        iterate();
    }
}

void RANSAC::iterate()
{
    
}

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic> RANSAC::predict(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X) const
{
    return m_A * X;
}
} // namespace astro
