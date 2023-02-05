#include "register.h"
#include <Processing/grey.h>

#include <itkAffineTransform.h>
#include <itkBSplineTransform.h>
#include <itkImageRegistrationMethodv4.h>
#include <itkLandmarkBasedTransformInitializer.h>
#include <itkMeanSquaresImageToImageMetricv4.h>
#include <itkRegularStepGradientDescentOptimizerv4.h>
#include <itkResampleImageFilter.h>
#include <itkTranslationTransform.h>

namespace astro
{
namespace processing
{
namespace
{
template<typename TransformType>
ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving,
                            const std::vector<std::pair<double, double>> fixStars,
                            const std::vector<std::pair<double, double>> movingStars, double defaultValue)
{
    using LandmarkBasedTransformInitializerType =
            itk::LandmarkBasedTransformInitializer<TransformType, ImageType, ImageType>;

    typename LandmarkBasedTransformInitializerType::Pointer landmarkBasedTransformInitializer =
            LandmarkBasedTransformInitializerType::New();
    //  Create source and target landmarks.
    using LandmarkContainerType = typename LandmarkBasedTransformInitializerType::LandmarkPointContainer;
    using LandmarkPointType = typename LandmarkBasedTransformInitializerType::LandmarkPointType;

    LandmarkContainerType fixedLandmarks;
    LandmarkContainerType movingLandmarks;

    for (auto pair : fixStars)
    {
        LandmarkPointType point;
        point[0] = pair.first;
        point[1] = pair.second;
        fixedLandmarks.push_back(point);
    }
    for (auto pair : movingStars)
    {
        LandmarkPointType point;
        point[0] = pair.first;
        point[1] = pair.second;
        movingLandmarks.push_back(point);
    }

    landmarkBasedTransformInitializer->SetFixedLandmarks(fixedLandmarks);
    landmarkBasedTransformInitializer->SetMovingLandmarks(movingLandmarks);

    auto transform = TransformType::New();
    transform->SetIdentity();
    landmarkBasedTransformInitializer->SetTransform(transform);
    landmarkBasedTransformInitializer->InitializeTransform();

    using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType, double>;
    auto resampleFilter = ResampleFilterType::New();
    resampleFilter->SetInput(moving);
    resampleFilter->SetTransform(transform);
    resampleFilter->SetUseReferenceImage(true);
    resampleFilter->SetReferenceImage(fix);
    resampleFilter->SetDefaultPixelValue(defaultValue);
    resampleFilter->Update();

    return resampleFilter->GetOutput();
}

template<typename TransformType>
ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving, double defaultValue)
{
    auto transform = TransformType::New();

    auto fixGrey = grey(fix);
    auto movingGrey = grey(moving);

    transform->SetIdentity();

    using OptimizerType = itk::RegularStepGradientDescentOptimizerv4<double>;
    auto optimizer = OptimizerType::New();
    optimizer->SetLearningRate(4);
    optimizer->SetMinimumStepLength(0.001);
    optimizer->SetRelaxationFactor(0.5);
    optimizer->SetNumberOfIterations(200);

    using MetricType = itk::MeanSquaresImageToImageMetricv4<ScalarImageType, ScalarImageType>;
    auto metric = MetricType::New();

    using RegistrationType = itk::ImageRegistrationMethodv4<ScalarImageType, ScalarImageType>;
    auto registration = RegistrationType::New();
    registration->SetInitialTransform(transform);
    registration->SetMetric(metric);
    registration->SetOptimizer(optimizer);
    registration->SetFixedImage(fixGrey);
    registration->SetMovingImage(movingGrey);
    registration->Update();
    
    std::cout << *registration->GetOutput()->Get() << std::endl;

    using ResampleFilterType = itk::ResampleImageFilter<ImageType, ImageType, double>;
    auto resampleFilter = ResampleFilterType::New();
    resampleFilter->SetInput(moving);
    resampleFilter->SetTransform(registration->GetOutput()->Get());
    resampleFilter->SetUseReferenceImage(true);
    resampleFilter->SetReferenceImage(fix);
    resampleFilter->SetDefaultPixelValue(defaultValue);
    resampleFilter->Update();

    return resampleFilter->GetOutput();
}
} // namespace

ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving,
                            const std::vector<std::pair<double, double>> fixStars,
                            const std::vector<std::pair<double, double>> movingStars, double defaultValue)
{
    return registerImages<itk::AffineTransform<double, Dimension>>(fix, moving, fixStars, movingStars, defaultValue);
}

ImageTypePtr registerImages(const ImageTypePtr& fix, const ImageTypePtr& moving, double defaultValue)
{
    return registerImages<itk::AffineTransform<double, Dimension>>(fix, moving, defaultValue);
}

ImageTypePtr registerImagesHighQuality(const ImageTypePtr& fix, const ImageTypePtr& moving, double defaultValue)
{
    return registerImages<itk::BSplineTransform<double, Dimension>>(fix, moving, defaultValue);
}

} // namespace processing
} // namespace astro
