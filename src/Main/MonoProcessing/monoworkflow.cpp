#include "monoworkflow.h"

#include <Explorer/explorer.h>
#include <MonoProcessing/monoprocessing.h>
#include <Plugin/pluginfactory.h>
#include <QtProcessing/monointerface.h>

#include <QtCore/QJsonArray>
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
    widget->setupWorkflow(m_steps);
}

void MonoWorkflow::addStep(MonoInterface* plugin, QJsonObject object)
{
    m_steps.emplace_back(plugin, object);
}

std::vector<std::unique_ptr<MonoWorkflow>> MonoWorkflow::getMonoWorkflows(Explorer* explorer)
{
    std::vector<std::unique_ptr<MonoWorkflow>> workflows;

    const std::map<QString, MonoInterface*>& plugins = MonoInterface::getPlugins();

    QDir directory = PluginFactory::getRootPath();
    directory.cd("workflows");

    QStringList jsons = directory.entryList(QStringList() << "*.json", QDir::Files);
    for (QString filename : jsons)
    {
        QFile loadFile(directory.filePath(filename));
        loadFile.open(QIODevice::ReadOnly);
        QByteArray jsonData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));
        if (loadDoc["Type"].toString() != "Mono")
        {
            continue;
        }
        workflows.push_back(std::make_unique<MonoWorkflow>(loadDoc["Name"].toString(), explorer));
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
