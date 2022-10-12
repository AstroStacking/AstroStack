#include "multiworkflow.h"

#include <Explorer/explorer.h>
#include <MultiProcessing/multiprocessing.h>
#include <Plugin/pluginfactory.h>
#include <QtProcessing/multiinterface.h>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

namespace astro
{
MultiWorkflow::MultiWorkflow(QString name, Explorer* explorer)
    : m_explorer(explorer)
{
    setObjectName(name);
}

void MultiWorkflow::openProcess()
{
    QString file = m_explorer->getSelectedFile();

    MultiProcessing* widget = new MultiProcessing(file);
    m_explorer->addSubWindow(widget);
    widget->setupWorkflow(m_steps);
}

void MultiWorkflow::addStep(MultiInterface* plugin, QJsonObject object)
{
    m_steps.emplace_back(plugin, object);
}

std::vector<std::unique_ptr<MultiWorkflow>> MultiWorkflow::getMultiWorkflows(Explorer* explorer)
{
    std::vector<std::unique_ptr<MultiWorkflow>> workflows;

    const std::map<QString, MultiInterface*>& plugins = MultiInterface::getPlugins();

    QDir directory = PluginFactory::getRootPath();
    directory.cd("workflows");

    QStringList jsons = directory.entryList(QStringList() << "*.json", QDir::Files);
    for (QString filename : jsons)
    {
        QFile loadFile(directory.filePath(filename));
        loadFile.open(QIODevice::ReadOnly);
        QByteArray jsonData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));
        if (loadDoc["Type"].toString() != "Multi")
        {
            continue;
        }
        workflows.push_back(std::make_unique<MultiWorkflow>(loadDoc["Name"].toString(), explorer));
        for (auto step : loadDoc["Steps"].toArray())
        {
            auto object = step.toObject();
            auto plugin = plugins.find(object["Type"].toString());
            if (plugin == plugins.end())
            {
                continue;
            }
            workflows.back()->addStep(plugin->second, object);
        }
    }

    return workflows;
}

} // namespace astro
