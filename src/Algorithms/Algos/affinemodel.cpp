#include "affinemodel.h"

namespace astro
{
void AffineModel::fit(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X,
                      const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& Y)
{
    m_A = Y * X.transpose() * (X * X.transpose()).inverse();
}

Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>
AffineModel::predict(const Eigen::Matrix<float, Eigen::Dynamic, Eigen::Dynamic>& X) const
{
    return m_A * X;
}
} // namespace astro
