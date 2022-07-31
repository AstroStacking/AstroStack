#include <affinemodel.h>

#include <Algos/gradient_estimation.h>

#include <gtest/gtest.h>

TEST(gradient_estimation, Estimation)
{
    astro::gradient_estimation::LightModel model;
    astro::gradient_estimation::LightGradient truthmodel;

    astro::gradient_estimation::Vector9d params;
    params << 0, 1, 2, -1, 4, 2, 1, -5, 6;

    Eigen::Matrix2Xd X(2, 50);
    X << -25, 17, -47, -11, -4, -7, -13, 2, -9, -4, -42, -22, 29, -19, -13, -25, -22, -18, -44, 17, -34, -14, -27, -28,
            -29, -25, 37, 11, -49, 23, -13, -49, 40, 19, -20, 10, 20, -32, 21, -15, -29, 10, 21, 19, 26, -21, -43, -49,
            -39, 39, -10, 44, 43, -38, 5, -32, 22, -28, -9, -45, 6, -17, 16, -19, -25, 13, -39, 38, 21, -15, 25, -7, 46,
            11, 44, 44, 30, 19, -30, 45, 21, -30, 35, 1, -28, -6, 30, 4, -4, -8, -30, -44, 41, -7, 12, -33, 2, 17, 10,
            -15;
    X = X / 50;
    Eigen::Matrix3Xd Y = truthmodel.predict(X, params);
    model.fit(X, Y);

    ASSERT_TRUE(params.isApprox(model.getData()));
}
