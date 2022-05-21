#include <QtWidgets/QMainWindow>

class Ui_MainWindow;
class QMdiArea;
class QMdiSubWindow;

namespace astro
{
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

    void addSubWindow(QWidget* window);

protected:
    void closeEvent(QCloseEvent* event) override;

    void createActions();
    void createMenus();
    void restore();

    std::unique_ptr<Ui_MainWindow> m_ui;
    QMdiArea* m_mdiArea{};
    QMdiSubWindow* m_treeWindow{};

    QMenu* m_fileMenu{};
    QAction* m_exitAct{};
};
} // namespace astro
