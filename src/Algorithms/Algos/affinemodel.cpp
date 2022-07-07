#include "affinemodel.h"

namespace astro
{
AffineModel::AffineModel() = default;
AffineModel::AffineModel(const AffineModel&) = default;
AffineModel::~AffineModel() = default;
AffineModel& AffineModel::operator=(const AffineModel&) = default;

Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> AffineModel::getData() const
{
    return m_A;
}

void AffineModel::fit(const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& X,
                      const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& Y)
{
    m_A = Y * X.transpose() * (X * X.transpose()).inverse();
}

Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>
AffineModel::predict(const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& X) const
{
    return m_A * X;
}
} // namespace astro
