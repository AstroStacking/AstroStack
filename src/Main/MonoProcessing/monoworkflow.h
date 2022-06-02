#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QObject>

namespace astro
{
class Explorer;
class MonoInterface;

class MonoWorkflow : public QObject
{
    Q_OBJECT
public:
    MonoWorkflow(QString name, Explorer* explorer);
    void addStep(MonoInterface* plugin, QJsonObject object);

public slots:
    void openProcess();

private:
    Explorer* m_explorer{};
    std::vector<std::pair<MonoInterface*, QJsonObject>> m_steps;

public:
    static std::vector<std::unique_ptr<MonoWorkflow>> getMonoWorkflows(Explorer* explorer);
};
} // namespace astro
