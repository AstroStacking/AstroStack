#pragma once
#include <Processing/mono.h>

namespace Ui
{
class Exponential;
}

namespace astro
{
/**
  Raises the image values to the power of a parameter
 */
class Exponential
    : public QObject
    , public MonoInterface
{
    Q_OBJECT
public:
    ~Exponential() override;

    /**
     QWidgets that will be displayed in the stack
     */
    class GUI : public MonoInterface::GUI
    {
    public:
        GUI(QWidget* parent);
        ~GUI() override;

        ImageTypePtr process(ImageTypePtr img, QPromise<void>& promise) override;

    private:
        std::unique_ptr<Ui::Exponential> m_ui;
    };

    QString name() const override;

    QString explanation() const override;

    GUI* generateGUI(QWidget* parent) const override;
};

} // namespace astro
