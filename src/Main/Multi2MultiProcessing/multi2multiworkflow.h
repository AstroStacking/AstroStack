#pragma once

#include <QtCore/QJsonObject>
#include <QtCore/QObject>

namespace astro
{
class Explorer;
class Multi2MultiInterface;

class Multi2MultiWorkflow : public QObject
{
    Q_OBJECT
public:
    Multi2MultiWorkflow(QString name, Explorer* explorer);
    void addStep(Multi2MultiInterface* plugin, QJsonObject object);

public slots:
    void openProcess();

private:
    Explorer* m_explorer{};
    std::vector<std::pair<Multi2MultiInterface*, QJsonObject>> m_steps;

public:
    static std::vector<std::unique_ptr<Multi2MultiWorkflow>> getWorkflows(Explorer* explorer);
};
} // namespace astro
