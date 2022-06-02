#include "monoworkflow.h"

#include <Explorer/explorer.h>
#include <MonoProcessing/monoprocessing.h>

namespace astro
{
MonoWorkflow::MonoWorkflow(Explorer* explorer)
:m_explorer(explorer)
{
    setObjectName("Process");
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
    
    workflows.push_back(std::make_unique<MonoWorkflow>(explorer));
    
    return workflows;
}

} // namespace astro
