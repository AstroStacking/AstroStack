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

class LightModel
{
    Eigen::Matrix<double, 9, 1> m_A;

public:
    LightModel() = default;
    LightModel(const LightModel&) = default;
    ~LightModel() = default;
    LightModel& operator=(const LightModel&) = default;

    Eigen::Matrix<double, 9, 1> getData() const { return m_A; }

    void fit(const Eigen::Matrix<double, 3, Eigen::Dynamic>& X, const Eigen::Matrix<double, 3, Eigen::Dynamic>& Y)
    {
        Gradient fun;
        auto optimizer = optim::optimizer::makeStandard(optim::helper::Quadratic(fun, X, Y),
                                                        optim::criteria::RelativeValue(0.00001),
                                                        optim::line_search::GoldenSection(0.0000001, 1),
                                                        optim::step::ConjugateGradient<optim::step::FRConjugate>());

        m_A = optimizer.getState().getCurrentPoint();
    }

    Eigen::Matrix<double, 3, Eigen::Dynamic> predict(const Eigen::Matrix<double, 3, Eigen::Dynamic>& X) const
    {
        Gradient fun;
        return {};
    }
};

void copyData(Eigen::Matrix3Xd& X, Eigen::Matrix<double, astro::PixelDimension, Eigen::Dynamic>& Y,
              const astro::ImageTypePtr& img)
{
    auto size = img->GetRequestedRegion().GetSize();

    Y.resize(astro::PixelDimension, size.at(0) * size.at(1));
    X = Eigen::Matrix3Xd::Ones(3, size.at(0) * size.at(1));

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        auto index = it.GetIndex();
        for (unsigned int j = 0; j < astro::PixelDimension; ++j)
        {
            Y(j, i) = value.GetElement(j);
        }
        X(0, i) = index.at(0);
        X(1, i) = index.at(1);
        ++it;
        ++i;
    }
}

void copyData(Eigen::Matrix3Xd& X, Eigen::Matrix<double, astro::PixelDimension, Eigen::Dynamic>& Y,
              const astro::ImageTypePtr& img, const astro::ScalarImageTypePtr& mask)
{
    auto size = img->GetRequestedRegion().GetSize();
    auto maskSize = mask->GetRequestedRegion().GetSize();
    if (size != maskSize)
    {
        throw std::range_error("Image and mask must have the same dimension");
    }

    Y.resize(astro::PixelDimension, size.at(0) * size.at(1));
    X = Eigen::Matrix3Xd::Ones(3, size.at(0) * size.at(1));

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<astro::ImageType>;
    using MaskedIteratorType = itk::ImageRegionIterator<astro::ScalarImageType>;
    IteratorType it(img, img->GetRequestedRegion());
    MaskedIteratorType maskedIt(mask, mask->GetRequestedRegion());
    it.GoToBegin();
    maskedIt.GoToBegin();

    while (!it.IsAtEnd())
    {
        if (maskedIt.Get() != 0)
        {
            auto value = it.Get();
            auto index = it.GetIndex();
            for (unsigned int j = 0; j < astro::PixelDimension; ++j)
            {
                Y(j, i) = value.GetElement(j);
            }
            X(0, i) = index.at(0);
            X(1, i) = index.at(1);
            ++i;
        }
        ++it;
        ++maskedIt;
    }
}
} // namespace

namespace astro
{
ImageTypePtr estimateGradient(const ImageTypePtr& input, const ScalarImageTypePtr& mask = nullptr)
{
    ImageTypePtr img = input;

    using DataMatrix = Eigen::Matrix<double, PixelDimension, Eigen::Dynamic>;
    DataMatrix Y;
    Eigen::Matrix3Xd X;

    if (mask)
    {
        copyData(X, Y, img, mask);
    }
    else
    {
        copyData(X, Y, img);
    }


    astro::RANSAC ransac(LightModel(), X, Y, 10, 2000, 0);
    ransac.run();

    return img;
}
} // namespace astro
