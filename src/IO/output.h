#include <io.h>

#include <QtWidgets/QWidget>

namespace astro
{

class OutputInterface
{
public:
    virtual ~OutputInterface();
    
    virtual QStringList filters() const = 0;
    virtual void save(const ImageType& image, QString filename, QWidget* parent) = 0;
};
}
