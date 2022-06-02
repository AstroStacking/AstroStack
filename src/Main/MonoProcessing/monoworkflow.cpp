#include "monoworkflow.h"

#include <Explorer/explorer.h>
#include <MonoProcessing/monoprocessing.h>
#include <Plugin/pluginfactory.h>

#include <QtCore/QJsonDocument>

namespace astro
{
MonoWorkflow::MonoWorkflow(QString name, Explorer* explorer)
    : m_explorer(explorer)
{
    setObjectName(name);
}

void MonoWorkflow::openProcess()
{
    QString file = m_explorer->getSelectedFile();

    MonoProcessing* widget = new MonoProcessing(file);
    m_explorer->addSubWindow(widget);
    widget->setupWorkflow();
}

std::vector<std::unique_ptr<MonoWorkflow>> MonoWorkflow::getMonoWorkflows(Explorer* explorer)
{
    std::vector<std::unique_ptr<MonoWorkflow>> workflows;

    QDir directory = PluginFactory::getRootPath();
    directory.cd("workflows");

    QStringList jsons = directory.entryList(QStringList() << "*.json", QDir::Files);
    for (QString filename : jsons)
    {
        QFile loadFile(directory.filePath(filename));
        loadFile.open(QIODevice::ReadOnly);
        QByteArray jsonData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));
        workflows.push_back(std::make_unique<MonoWorkflow>(loadDoc["Name"].toString(), explorer));
    }

    return workflows;
}

} // namespace astro
