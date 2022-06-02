#pragma once

#include <QtCore/QObject>

namespace astro
{
class Explorer;

class MonoWorkflow : public QObject
{
    Q_OBJECT
public:
    MonoWorkflow(Explorer* explorer);
public slots:
    void openProcess();

private:
    Explorer* m_explorer{};
public:
    static std::vector<std::unique_ptr<MonoWorkflow>> getMonoWorkflows(Explorer* explorer);
};
} // namespace astro
