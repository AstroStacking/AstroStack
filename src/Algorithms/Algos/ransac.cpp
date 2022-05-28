#include "ransac.h"

namespace astro
{
RANSAC::RANSAC(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
               const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y, int nbSamples)
    : m_X(X)
    , m_Y(Y)
    , m_nbSamples(nbSamples)
{
}

RANSAC::~RANSAC() = default;

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
RANSAC::fit(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
            const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y)
{
    return Y * X.transpose() * (X * X.transpose()).inverse();
}

bool RANSAC::run()
{
    return true;
}

} // namespace astro
