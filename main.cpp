#include "MainWindow.h"
#include "KeyboardHandlerAccess.h"

int main(int argc, char* argv[]) {
  QApplication QEnvironment(argc, argv);
  MainWindow Widget;
  Widget.show();

  try {
    CKeyboardHandlerInitializer InitNow;
  } catch (...) {

  }
  return QEnvironment.exec();
}
