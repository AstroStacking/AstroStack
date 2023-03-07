#include "multi2multiworkflow.h"
#include <Explorer/explorer.h>
#include <Multi2MultiProcessing/multi2multiprocessing.h>
#include <Plugin/pluginfactory.h>
#include <QtProcessing/multi2multiinterface.h>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

namespace astro
{
Multi2MultiWorkflow::Multi2MultiWorkflow(QString name, Explorer* explorer)
    : m_explorer(explorer)
{
    setObjectName(name);
}

void Multi2MultiWorkflow::openProcess()
{
    Multi2MultiProcessing* widget = new Multi2MultiProcessing(objectName());
    m_explorer->addSubWindow(widget);
    widget->restore();
    widget->setupWorkflow(m_steps);
}

void Multi2MultiWorkflow::addStep(Multi2MultiInterface* plugin, QJsonObject object)
{
    m_steps.emplace_back(plugin, object);
}

std::vector<std::unique_ptr<Multi2MultiWorkflow>> Multi2MultiWorkflow::getWorkflows(Explorer* explorer)
{
    std::vector<std::unique_ptr<Multi2MultiWorkflow>> workflows;

    const std::map<QString, Multi2MultiInterface*>& plugins = Multi2MultiInterface::getPlugins();

    QDir directory = PluginFactory::getRootPath();
    directory.cd("workflows");

    QStringList jsons = directory.entryList(QStringList() << "*.json", QDir::Files);
    for (QString filename : jsons)
    {
        QFile loadFile(directory.filePath(filename));
        loadFile.open(QIODevice::ReadOnly);
        QByteArray jsonData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));
        if (loadDoc["Type"].toString() != "Multi2Multi")
        {
            continue;
        }
        workflows.push_back(std::make_unique<Multi2MultiWorkflow>(loadDoc["Name"].toString(), explorer));
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
