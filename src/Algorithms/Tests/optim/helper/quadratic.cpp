#include <Algos/optim/helper/quadratic.h>

#include <gtest/gtest.h>

#include <unsupported/Eigen/CXX11/Tensor>

namespace
{
class Function
{
public:
    std::vector<Eigen::VectorXd> operator()(const Eigen::MatrixXd& X, const Eigen::VectorXd& parameters) const
    {
        std::vector<Eigen::VectorXd> result;
        for(auto row : X.colwise())
        {
            result.emplace_back(row.transpose() * parameters);
        }
        return result;
    }

    /*std::vector<Eigen::VectorXd> gradient(const Eigen::MatrixXd& X, const Eigen::VectorXd& parameters) const
    {
    }

    std::vector<Eigen::MatrixXd> hessian(const Eigen::MatrixXd& X, const Eigen::VectorXd& parameters) const
    {
    }*/
};
} // namespace

TEST(Quadratic, HelperCreation)
{
    Eigen::VectorXd params(2);
    params << 3, 4;

    Eigen::MatrixXd X(2, 40);
    X << -10, 39, 7, -30, 28, 13, 15, 11, -30, -11, 41, -42, -25, 41, 24, -11, -18, 20, -26, -42, 44, 12, 46, -39, 13,
            -34, -43, -40, -2, 20, -41, -16, -13, 26, 3, -37, -8, 10, -27, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1;
    Eigen::MatrixXd Y(1, 40);
    Y << -26., 121., 25., -86., 88., 43., 49., 37., -86., -29., 127., -122., -71., 127., 76., -29., -50., 64., -74.,
            -122., 136., 40., 142., -113., 43., -98., -125., -116., -2., 64., -119., -44., -35., 82., 13., -107., -20.,
            34., -77., 7.;

    Function fun;
    auto result = fun(X, params);
    for(size_t i = 0; i < result.size(); ++i)
    {
        ASSERT_EQ(Y.col(i), result[i]);
    }
}

TEST(Quadratic, Use)
{
    Eigen::VectorXd params(2);
    params << 3, 4;

    Eigen::MatrixXd X(2, 40);
    X << -10, 39, 7, -30, 28, 13, 15, 11, -30, -11, 41, -42, -25, 41, 24, -11, -18, 20, -26, -42, 44, 12, 46, -39, 13,
            -34, -43, -40, -2, 20, -41, -16, -13, 26, 3, -37, -8, 10, -27, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
            1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1;
    Eigen::MatrixXd Y(1, 40);
    Y << -26., 121., 25., -86., 88., 43., 49., 37., -86., -29., 127., -122., -71., 127., 76., -29., -50., 64., -74.,
            -122., 136., 40., 142., -113., 43., -98., -125., -116., -2., 64., -119., -44., -35., 82., 13., -107., -20.,
            34., -77., 7.;

    Function fun;

    optim::helper::Quadratic<Function> helper(fun, X, Y);

    ASSERT_EQ(0, helper(params));
    //ASSERT_TRUE(helper.gradient(params).isMuchSmallerThan(Eigen::VectorXd(2)));

    Eigen::VectorXd zeros(2);

    ASSERT_NE(0, helper(zeros));
    //ASSERT_TRUE((helper.hessian(zeros).colPivHouseholderQr().solve(helper.gradient(zeros)) + params)
    //                    .isMuchSmallerThan(0.1));
}
