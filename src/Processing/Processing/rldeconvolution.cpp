#include "rldeconvolution.h"
#include "ui_rldeconvolution.h"
#include "ui_monointerface.h"

#include <itkImageDuplicator.h>
#include <itkImageRegionIterator.h>
#include <itkImageToHistogramFilter.h>
#include <itkMinimumMaximumImageCalculator.h>
#include <itkMultiplyImageFilter.h>
#include <itkVectorIndexSelectionCastImageFilter.h>

namespace astro
{
RLDeconvolution::~RLDeconvolution() = default;

QString RLDeconvolution::name() const
{
    return "RLDeconvolution";
}

QString RLDeconvolution::explanation() const
{
    return tr("Raises the image values to the power of a parameter");
}

MonoInterfaceGUI* RLDeconvolution::generateGUI(QWidget* parent) const
{
    return new RLDeconvolutionGUI(parent);
}

RLDeconvolutionGUI::RLDeconvolutionGUI(QWidget* parent)
    : MonoInterfaceGUI(parent)
    , m_ui(std::make_unique<Ui::RLDeconvolution>())
{
    QWidget* child = new QWidget(this);
    m_ui->setupUi(child);
    m_monoUi->setupUi(this, child);
    setTitle(tr("Richardson Lucy Deconvolution"));

    setupSlots();
}

RLDeconvolutionGUI::~RLDeconvolutionGUI() = default;

ImageTypePtr RLDeconvolutionGUI::process(ImageTypePtr img, QPromise<void>& promise)
{
    emit save(img);

    return img;
}
} // namespace astro
