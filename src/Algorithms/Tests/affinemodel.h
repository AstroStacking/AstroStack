#pragma once

#include <Eigen/Dense>

class AffineModel
{
    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> m_A;

public:
    AffineModel();
    AffineModel(const AffineModel&);
    ~AffineModel();
    AffineModel& operator=(const AffineModel&);

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic> getData() const;

    void fit(const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& X,
             const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& Y);

    Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>
    predict(const Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic>& X) const;
};
