#pragma once

#include <Algos/optim/criteria/composite.h>
#include <Algos/optim/criteria/criteria.h>
#include <Algos/optim/helper/quadratic.h>
#include <Algos/optim/line_search/golden_section.h>
#include <Algos/optim/optimizer/standard.h>
#include <Algos/optim/step/gradient.h>
#include <Algos/ransac.h>
#include <IO/io.h>

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>

#include <Eigen/Dense>

namespace astro
{
namespace gradient_estimation
{
using Vector9d = Eigen::Vector<double, 9>;
using Matrix93d = Eigen::Matrix<double, 9, 3>;
class LightGradient
{
public:
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

    Eigen::Matrix3Xd predict(const Eigen::Matrix2Xd& X, const Vector9d& parameters) const
    {
        Eigen::Matrix3Xd result(3, X.cols());
        for (Eigen::Index col = 0; col < X.cols(); ++col)
        {
            result(0, col) = parameters(0) + parameters(1) * X(0, col) + parameters(2) * X(1, col);
            result(1, col) = parameters(3) + parameters(4) * X(0, col) + parameters(5) * X(1, col);
            result(2, col) = parameters(6) + parameters(7) * X(0, col) + parameters(8) * X(1, col);
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
    Eigen::Matrix<double, 9, 1> m_A{};

public:
    LightModel() = default;
    LightModel(const LightModel&) = default;
    ~LightModel() = default;
    LightModel& operator=(const LightModel&) = default;

    Eigen::Matrix<double, 9, 1> getData() const { return m_A; }

    void fit(const Eigen::Matrix2Xd& X, const Eigen::Matrix3Xd& Y)
    {
        LightGradient fun;
        auto optimizer = optim::optimizer::makeStandard(
                optim::helper::Quadratic<LightGradient, 9, 2, 3>(fun, X, Y),
                optim::criteria::makeOr(optim::criteria::RelativeValue(0.00001), optim::criteria::Iteration(100)),
                optim::line_search::GoldenSection(0.0000001, 1), optim::step::Gradient());
        optimizer(m_A);
        m_A = optimizer.getState().getCurrentPoint();
    }

    Eigen::Matrix3Xd predict(const Eigen::Matrix2Xd& X) const
    {
        LightGradient fun;
        return fun.predict(X, m_A);
    }
};

void copyData(Eigen::Matrix2Xd& X, Eigen::Matrix<double, PixelDimension, Eigen::Dynamic>& Y, const ImageType& img)
{
    auto size = img.GetRequestedRegion().GetSize();

    Y.resize(PixelDimension, size.at(0) * size.at(1));
    X = Eigen::Matrix2Xd::Ones(2, size.at(0) * size.at(1));

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<const ImageType>;
    IteratorType it(&img, img.GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        auto index = it.GetIndex();
        for (unsigned int j = 0; j < PixelDimension; ++j)
        {
            Y(j, i) = value.GetElement(j);
        }
        X(0, i) = index.at(0) / size.at(0);
        X(1, i) = index.at(1) / size.at(1);
        ++it;
        ++i;
    }
}

void copyData(Eigen::Matrix2Xd& X, Eigen::Matrix<double, PixelDimension, Eigen::Dynamic>& Y, const ImageType& img,
              const ScalarImageType& mask)
{
    auto size = img.GetRequestedRegion().GetSize();
    auto maskSize = mask.GetRequestedRegion().GetSize();
    if (size != maskSize)
    {
        throw std::range_error("Image and mask must have the same dimension");
    }

    Y.resize(PixelDimension, size.at(0) * size.at(1));
    X = Eigen::Matrix2Xd::Ones(2, size.at(0) * size.at(1));

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<const ImageType>;
    using MaskedIteratorType = itk::ImageRegionIterator<const ScalarImageType>;
    IteratorType it(&img, img.GetRequestedRegion());
    MaskedIteratorType maskedIt(&mask, mask.GetRequestedRegion());
    it.GoToBegin();
    maskedIt.GoToBegin();

    while (!it.IsAtEnd())
    {
        if (maskedIt.Get() != 0)
        {
            auto value = it.Get();
            auto index = it.GetIndex();
            for (unsigned int j = 0; j < PixelDimension; ++j)
            {
                Y(j, i) = value.GetElement(j);
            }
            X(0, i) = index.at(0) / size.at(0);
            X(1, i) = index.at(1) / size.at(1);
            ++i;
        }
        ++it;
        ++maskedIt;
    }
}

void lightData(const Eigen::Matrix<double, PixelDimension, Eigen::Dynamic>& Y, ImageType& img)
{
    auto size = img.GetRequestedRegion().GetSize();

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<ImageType>;
    IteratorType it(&img, img.GetRequestedRegion());
    it.GoToBegin();

    while (!it.IsAtEnd())
    {
        auto value = it.Get();
        auto index = it.GetIndex();
        for (unsigned int j = 0; j < PixelDimension; ++j)
        {
            value.SetElement(j, Y(j, i));
        }
        ++it;
        ++i;
    }
}

void lightData(const Eigen::Matrix<double, PixelDimension, Eigen::Dynamic>& Y, ImageType& img,
               const ScalarImageType& mask)
{
    auto size = img.GetRequestedRegion().GetSize();
    auto maskSize = mask.GetRequestedRegion().GetSize();
    if (size != maskSize)
    {
        throw std::range_error("Image and mask must have the same dimension");
    }

    size_t i = 0;
    using IteratorType = itk::ImageRegionIterator<ImageType>;
    using MaskedIteratorType = itk::ImageRegionIterator<const ScalarImageType>;
    IteratorType it(&img, img.GetRequestedRegion());
    MaskedIteratorType maskedIt(&mask, mask.GetRequestedRegion());
    it.GoToBegin();
    maskedIt.GoToBegin();

    while (!it.IsAtEnd())
    {
        if (maskedIt.Get() != 0)
        {
            auto value = it.Get();
            auto index = it.GetIndex();
            for (unsigned int j = 0; j < PixelDimension; ++j)
            {
                value.SetElement(j, Y(j, i));
            }
            ++i;
        }
        ++it;
        ++maskedIt;
    }
}
} // namespace gradient_estimation

ImageTypePtr estimateGradient(const ImageTypePtr& input, const ScalarImageTypePtr& mask = nullptr)
{
    ImageTypePtr img = input;

    using DataMatrix = Eigen::Matrix<double, PixelDimension, Eigen::Dynamic>;
    DataMatrix Y;
    Eigen::Matrix2Xd X;

    if (mask)
    {
        gradient_estimation::copyData(X, Y, *img, *mask);
    }
    else
    {
        gradient_estimation::copyData(X, Y, *img);
    }

    // Found heuristic for these...
    RANSAC ransac(gradient_estimation::LightModel(), X, Y, 100, 200, 0);
    ransac.run();

    auto light = ransac.predict(X);

    auto output = ImageType::New();

    ImageType::IndexType start;
    start[0] = 0; // first index on X
    start[1] = 0; // first index on Y
    start[2] = 0; // first index on Z

    ImageType::SizeType size = img->GetLargestPossibleRegion().GetSize();

    ImageType::RegionType region;
    region.SetSize(size);
    region.SetIndex(start);

    output->SetRegions(region);
    output->Allocate();

    if (mask)
    {
        gradient_estimation::lightData(light, *output, *mask);
    }
    else
    {
        gradient_estimation::lightData(light, *output);
    }

    return output;
}
} // namespace astro
