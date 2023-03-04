#include "mono2monoworkflow.h"
#include <Explorer/explorer.h>
#include <Mono2MonoProcessing/mono2monoprocessing.h>
#include <Plugin/pluginfactory.h>
#include <QtProcessing/mono2monointerface.h>

#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

namespace astro
{
Mono2MonoWorkflow::Mono2MonoWorkflow(QString name, Explorer* explorer)
    : m_explorer(explorer)
{
    setObjectName(name);
}

void Mono2MonoWorkflow::openProcess()
{
    QString file = m_explorer->getSelectedFile();

    Mono2MonoProcessing* widget = new Mono2MonoProcessing(file);
    m_explorer->addSubWindow(widget);
    widget->setupWorkflow(m_steps);
}

void Mono2MonoWorkflow::addStep(Mono2MonoInterface* plugin, QJsonObject object)
{
    m_steps.emplace_back(plugin, object);
}

std::vector<std::unique_ptr<Mono2MonoWorkflow>> Mono2MonoWorkflow::getWorkflows(Explorer* explorer)
{
    std::vector<std::unique_ptr<Mono2MonoWorkflow>> workflows;

    const std::map<QString, Mono2MonoInterface*>& plugins = Mono2MonoInterface::getPlugins();

    QDir directory = PluginFactory::getRootPath();
    directory.cd("workflows");

    QStringList jsons = directory.entryList(QStringList() << "*.json", QDir::Files);
    for (QString filename : jsons)
    {
        QFile loadFile(directory.filePath(filename));
        loadFile.open(QIODevice::ReadOnly);
        QByteArray jsonData = loadFile.readAll();
        QJsonDocument loadDoc(QJsonDocument::fromJson(jsonData));
        if (loadDoc["Type"].toString() != "Mono2Mono")
        {
            continue;
        }
        workflows.push_back(std::make_unique<Mono2MonoWorkflow>(loadDoc["Name"].toString(), explorer));
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
