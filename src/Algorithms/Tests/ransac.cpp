#include <affinemodel.h>

#include <Algos/ransac.h>

#include <gtest/gtest.h>

TEST(RANSAC, Constructor)
{
    Eigen::MatrixXd X(2, 50);
    X << -10, 39, 7, -30, 28, 13, 15, 11, -30, -11, 41, -42, -25, 41, 24, -11, -18, 20, -26, -42, 44, 12, 46, -39, 13,
            -34, -43, -40, -2, 20, -41, -16, -13, 26, 3, -37, -8, 10, -27, 1, -41, 1, 49, 12, 27, 39, 34, -15, 10, -21,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1;
    Eigen::MatrixXd Y(1, 50);
    Y << -26., 121., 25., -86., 88., 43., 49., 37., -86., -29., 127., -122., -71., 127., 76., -29., -50., 64., -74.,
            -122., 136., 40., 142., -113., 43., -98., -125., -116., -2., 64., -119., -44., -35., 82., 13., -107., -20.,
            34., -77., 7., -11., 70., 15., 4., 8., 1., 10., -4., 3., -5.;

    astro::RANSAC ransac(AffineModel(), X, Y, 20);
}

TEST(RANSAC, SimpleTest)
{
    Eigen::MatrixXd X(2, 40);
    X << -10, 39, 7, -30, 28, 13, 15, 11, -30, -11, 41, -42, -25, 41, 24, -11, -18, 20, -26, -42, 44, 12, 46, -39, 13,
            -34, -43, -40, -2, 20, -41, -16, -13, 26, 3, -37, -8, 10, -27, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1;
    Eigen::MatrixXd Y(1, 40);
    Y << -26., 121., 25., -86., 88., 43., 49., 37., -86., -29., 127., -122., -71., 127., 76., -29., -50., 64., -74.,
            -122., 136., 40., 142., -113., 43., -98., -125., -116., -2., 64., -119., -44., -35., 82., 13., -107., -20.,
            34., -77., 7.;

    astro::RANSAC ransac(AffineModel(), X, Y, 10, 2000);

    ransac.run();

    Eigen::MatrixXd X2(2, 1);
    X2 << -10, 1;
    auto Y2 = ransac.predict(X2);

    ASSERT_NEAR(-26, Y2(0), 0.0001);
}
