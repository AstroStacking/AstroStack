#include "affinemodel.h"

#include <gtest/gtest.h>

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

TEST(AffineModel, Regression)
{
    Eigen::MatrixXd X(2, 40);
    X << -10, 39, 7, -30, 28, 13, 15, 11, -30, -11, 41, -42, -25, 41, 24, -11, -18, 20, -26, -42, 44, 12, 46, -39, 13,
            -34, -43, -40, -2, 20, -41, -16, -13, 26, 3, -37, -8, 10, -27, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1;
    Eigen::MatrixXd Y(1, 40);
    Y << -26., 121., 25., -86., 88., 43., 49., 37., -86., -29., 127., -122., -71., 127., 76., -29., -50., 64., -74.,
            -122., 136., 40., 142., -113., 43., -98., -125., -116., -2., 64., -119., -44., -35., 82., 13., -107., -20.,
            34., -77., 7.;

    AffineModel model;

    model.fit(X, Y);
    auto result = model.getData();

    ASSERT_NEAR(3, result(0), 0.0001);
    ASSERT_NEAR(4, result(1), 0.0001);
}
