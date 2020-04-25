#ifndef MAINWINDOW_H
#define MAINWINDOW_H

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  MainWindow(const MainWindow&) = delete;
  MainWindow(MainWindow&&) noexcept = delete;
  MainWindow& operator=(const MainWindow&) = delete;
  MainWindow& operator=(MainWindow&&) noexcept = delete;

private:
  std::unique_ptr<Ui::MainWindow> ui;
};
#endif // MAINWINDOW_H
