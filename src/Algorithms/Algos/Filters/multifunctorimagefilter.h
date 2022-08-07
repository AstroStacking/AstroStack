/**
 * \file multifunctorimagefilter.h
 */

#pragma once

#include <Algos/config.h>

#include <itkImageScanlineIterator.h>
#include <itkInPlaceImageFilter.h>
#include <itkSimpleDataObjectDecorator.h>
#include <itkTotalProgressReporter.h>

namespace astro
{
namespace filters
{
template<typename TInputImage, typename TOutputImage, typename TFunction>
class MultiFunctorImageFilter : public itk::InPlaceImageFilter<TInputImage, TOutputImage>
{
public:
    ITK_DISALLOW_COPY_AND_MOVE(MultiFunctorImageFilter);

    using Self = MultiFunctorImageFilter;
    using Superclass = itk::InPlaceImageFilter<TInputImage, TOutputImage>;

    using Pointer = itk::SmartPointer<Self>;
    using ConstPointer = itk::SmartPointer<const Self>;

    using FunctorType = TFunction;
    using InputImageType = TInputImage;
    using InputImagePointer = typename InputImageType::ConstPointer;
    using InputImageRegionType = typename InputImageType::RegionType;
    using InputImagePixelType = typename InputImageType::PixelType;
    using DecoratedInputImagePixelType = itk::SimpleDataObjectDecorator<InputImagePixelType>;

    using OutputImageType = TOutputImage;
    using OutputImagePointer = typename OutputImageType::Pointer;
    using OutputImageRegionType = typename OutputImageType::RegionType;
    using OutputImagePixelType = typename OutputImageType::PixelType;

    itkNewMacro(Self);

    itkTypeMacro(MultiFunctorImageFilter, InPlaceImageFilter);

    FunctorType& getFunctor() { return m_functor; }

    const FunctorType& getFunctor() const { return m_functor; }

    void setFunctor(const FunctorType& functor)
    {
        if (m_functor != functor)
        {
            m_functor = functor;
            this->Modified();
        }
    }

    using Superclass::SetNthInput;

protected:
    MultiFunctorImageFilter() { setFunctor(FunctorType()); }

    ~MultiFunctorImageFilter() override = default;

    void DynamicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread) override
    {
        std::vector<const TInputImage*> images(this->GetNumberOfInputs());
        std::vector<itk::ImageScanlineConstIterator<TInputImage>> iterators(this->GetNumberOfInputs());
        for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
        {
            images[idx] = dynamic_cast<const TInputImage*>(itk::ProcessObject::GetInput(idx));
            iterators[idx] = itk::ImageScanlineConstIterator<TInputImage>(images[idx], outputRegionForThread);
        }
        TOutputImage* outputPtr = this->GetOutput(0);
        itk::ImageScanlineIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

        itk::TotalProgressReporter progress(this, outputPtr->GetRequestedRegion().GetNumberOfPixels());

        std::vector<typename TInputImage::PixelType> values(this->GetNumberOfInputs());

        while (!iterators[0].IsAtEnd())
        {
            while (!iterators[0].IsAtEndOfLine())
            {
                for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
                {
                    values[idx] = iterators[idx].Get();
                }
                outputIt.Set(m_functor(values));
                for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
                {
                    ++iterators[idx];
                }
                ++outputIt;
            }

            for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
            {
                iterators[idx].NextLine();
            }
            outputIt.NextLine();
            progress.Completed(outputRegionForThread.GetSize()[0]);
        }
    }

    void GenerateOutputInformation() override
    {
        const itk::DataObject* input = nullptr;
        InputImagePointer inputPtr = dynamic_cast<const TInputImage*>(itk::ProcessObject::GetInput(0));

        for (unsigned int idx = 0; idx < this->GetNumberOfOutputs(); ++idx)
        {
            itk::DataObject* output = this->GetOutput(idx);
            if (output)
            {
                output->CopyInformation(input);
            }
        }
    }

private:
    FunctorType m_functor;
};
} // namespace filters

} // namespace astro
