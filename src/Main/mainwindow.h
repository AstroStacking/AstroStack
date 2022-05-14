#include <QMainWindow>

class Ui_MainWindow;
class QMdiArea;

class MainWindow : public QMainWindow
{
    Q_OBJECT

  public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

  protected:
    std::unique_ptr<Ui_MainWindow> m_ui;
    QMdiArea* m_mdiArea{};
};
