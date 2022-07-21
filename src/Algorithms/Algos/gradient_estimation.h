#pragma once

#include <Algos/optim/criteria/composite.h>
#include <Algos/optim/criteria/criteria.h>
#include <Algos/optim/helper/quadratic.h>
#include <Algos/optim/line_search/golden_section.h>
#include <Algos/optim/optimizer/standard.h>
#include <Algos/optim/step/conjugate_gradient.h>
#include <Algos/ransac.h>

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

#include <Eigen/Dense>

namespace
{
class Gradient
{
public:
    using Vector9d = Eigen::Vector<double, 9>;
    using Matrix93d = Eigen::Matrix<double, 9, 3>;
    std::vector<Eigen::Vector3d> operator()(const Eigen::Matrix2Xd& X, const Vector9d& parameters) const
    {
        std::vector<Eigen::Vector3d> result;
        for (auto col : X.colwise())
        {
            Eigen::Vector3d data = Eigen::Vector3d::Zero();
            data(0) = parameters(0) + parameters(1) * col(0) + parameters(2) * col(1);
            data(1) = parameters(3) + parameters(4) * col(0) + parameters(5) * col(1);
            data(2) = parameters(6) + parameters(7) * col(0) + parameters(8) * col(1);
            result.emplace_back(data);
        }
        return result;
    }

    std::vector<Matrix93d> gradient(const Eigen::Matrix2Xd& X, const Vector9d& parameters) const
    {
        std::vector<Matrix93d> result;
        for (auto col : X.colwise())
        {
            Matrix93d data = Matrix93d::Zero();
            data(0, 0) = 1;
            data(1, 0) = col(0);
            data(2, 0) = col(1);
            data(3, 1) = 1;
            data(4, 1) = col(0);
            data(5, 1) = col(1);
            data(6, 2) = 1;
            data(7, 2) = col(0);
            data(8, 2) = col(1);
            result.emplace_back(data);
        }
        return result;
    }
};
} // namespace

namespace astro
{
template<size_t nbDims>
ImageTypePtr estimateGradient(const ImageTypePtr& img, const ScalarImageTypePtr& mask = nullptr)
{
    using DataMatrix = Eigen::Matrix<double, nbDims, Eigen::Dynamic>;
    
    auto size = img->GetRequestedRegion().GetSize();
    DataMatrix Y(size.at(0) * size.at(1), nbDims);
    Eigen::Matrix3Xd X(Eigen::Matrix3Xd::Ones(size.at(0) * size.at(1), 3));

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        auto index = it.GetIndex();
        for (unsigned int j = 0; j < nbDims; ++j)
        {
            Y(i, j) = value.GetElement(j);
        }
        X(i, 0) = index.at(0);
        X(i, 1) = index.at(1);
        ++it;
        ++i;
    }

    /*
    using DuplicatorType = itk::ImageDuplicator<ImageType>;
    auto duplicator = DuplicatorType::New();
    duplicator->SetInputImage(img.getImg());
    duplicator->Update();

    img.setImg(duplicator->GetOutput());
    using IteratorType = itk::ImageRegionIterator<ImageType>;

    IteratorType it(img.getImg(), img.getImg()->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        for (unsigned int i = 0; i < nbDims; ++i)
        {
            value.SetElement(i, std::pow(value.GetElement(i), exponent));
        }
        it.Set(value);
        ++it;
    }*/

    return img;
}
} // namespace astro
