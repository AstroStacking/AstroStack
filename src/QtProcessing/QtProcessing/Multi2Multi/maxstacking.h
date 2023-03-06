#pragma once
#include <QtProcessing/multi2multiinterface.h>

namespace Ui
{
class MaxStacking;
}

namespace astro
{
/**
 * Stacks the images, taking maximum of each pixel
 */
class MaxStacking
    : public QObject
    , public Multi2MultiInterface
{
    Q_OBJECT
public:
    ~MaxStacking() override;

    QString name() const override;

    QString explanation() const override;

    Multi2MultiInterfaceGUI* generateGUI(QWidget* parent) const override;
};

/**
 QWidgets that will be displayed in the stack
 */
class MaxStackingGUI : public Multi2MultiInterfaceGUI
{
    Q_OBJECT
public:
    MaxStackingGUI(QWidget* parent);
    ~MaxStackingGUI() override;

    void process(H5::Group group, QPromise<void>& promise) override;
    bool check() override;

signals:
    void save(const AstroImage& img);

private slots:
    void outputFileBoxOpen();
    void saveImg(const AstroImage& img);

private:
    void setupSlots();
    void setNextFilename(QFileInfo info, QString basename, long inc);

    std::unique_ptr<Ui::MaxStacking> m_ui;
    bool m_overwriteIfExists{true};
};

} // namespace astro
