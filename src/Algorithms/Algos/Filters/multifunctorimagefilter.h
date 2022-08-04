/**
 * \file multifunctorimagefilter.h
 */

#pragma once

#include <InPlaceImageFilter.h>

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
    using DecoratedInputImagePixelType = SimpleDataObjectDecorator<InputImagePixelType>;

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

protected:
    MultiFunctorImageFilter()
    {
#if !defined(ITK_WRAPPING_PARSER)
        Superclass::SetFunctor(FunctorType());
#endif
    }

    ~MultiFunctorImageFilter() override = default;

    void DynamicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread) override
    {
        std::vector<const TInputImage*> images(this->GetNumberOfInputs());
        std::vector<ImageScanlineConstIterator<TInputImage>> iterators(this->GetNumberOfInputs());
        for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
        {
            images[idx] = dynamic_cast<const TInputImage*>(ProcessObject::GetInput(idx));
            iterators[idx] = ImageScanlineConstIterator<TInputImage>(images[idx], outputRegionForThread);
        }
        TOutputImage* outputPtr = this->GetOutput(0);
        ImageScanlineIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

        TotalProgressReporter progress(this, outputPtr->GetRequestedRegion().GetNumberOfPixels());

        std::vector<typename TInputImage::PixelType> values(this->GetNumberOfInputs());

        while (!iterators[0].IsAtEnd())
        {
            while (!iterators[0].IsAtEndOfLine())
            {
                for (unsigned int idx = 0; idx < this->GetNumberOfInputs(); ++idx)
                {
                    values[idx] = iterators[idx].Get();
                }
                outputIt.Set(m_Functor(values));
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
        const DataObject* input = nullptr;
        InputImagePointer inputPtr = dynamic_cast<const TInputImage*>(ProcessObject::GetInput(0));

        for (unsigned int idx = 0; idx < this->GetNumberOfOutputs(); ++idx)
        {
            DataObject* output = this->GetOutput(idx);
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
