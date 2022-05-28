#include "ransac.h"

namespace astro
{
RANSAC::RANSAC(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
               const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y, int nbSamples, int nbIterations)
    : m_X(X)
    , m_Y(Y)
    , m_generator(std::random_device()())
    , m_nbSamples(nbSamples)
    , m_nbIterations(nbIterations)
{
    if (m_X.cols() >= m_nbSamples)
    {
        throw std::range_error("X need to have more samples than the number of samples per iterations");
    }
    if (m_Y.cols() != m_X.cols())
    {
        throw std::range_error("X and Y need to have the same number of columns");
    }
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
    for (int i = 0; i < m_nbIterations; ++i)
    {
        iterate();
    }
}

void RANSAC::iterate()
{
    std::vector<int> v(m_X.cols());
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), m_generator);

    v.resize(m_nbSamples);

    auto A = fit(m_X(Eigen::all, v), m_Y(Eigen::all, v));
}

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
RANSAC::predict(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X) const
{
    return m_A * X;
}
} // namespace astro
