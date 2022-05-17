#include <io.h>

#include <QtCore/QStringList>
#include <QtWidgets/QWidget>

namespace astro
{

class InputInterface
{
public:
    virtual ~InputInterface();

    virtual QStringList filters() const = 0;
    virtual ImageType open(QString filename, QWidget* parent) = 0;
};
}

