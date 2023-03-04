#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QObject>

namespace astro
{
class Explorer;
class Mono2MonoInterface;

class Mono2MonoWorkflow : public QObject
{
    Q_OBJECT
public:
    Mono2MonoWorkflow(QString name, Explorer* explorer);
    void addStep(Mono2MonoInterface* plugin, QJsonObject object);

public slots:
    void openProcess();

private:
    Explorer* m_explorer{};
    std::vector<std::pair<Mono2MonoInterface*, QJsonObject>> m_steps;

public:
    static std::vector<std::unique_ptr<Mono2MonoWorkflow>> getWorkflows(Explorer* explorer);
};
} // namespace astro
