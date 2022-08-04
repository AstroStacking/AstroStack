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

    void DynamicThreadedGenerateData(const OutputImageRegionType& outputRegionForThread) override;

    void GenerateOutputInformation() override;

private:
    FunctorType m_functor;
};
} // namespace filters

} // namespace astro
