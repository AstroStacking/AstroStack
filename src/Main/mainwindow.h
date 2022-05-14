#include <QtWidgets/QMainWindow>

class Ui_MainWindow;
class QMdiArea;
class QMdiSubWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  protected:
    void closeEvent(QCloseEvent* event) override;

    std::unique_ptr<Ui_MainWindow> m_ui;
    QMdiArea* m_mdiArea{};
    QMdiSubWindow* m_treeWindow{};
};
