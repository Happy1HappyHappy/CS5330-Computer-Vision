#include "mainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
  QApplication app(argc, argv);

  MainWindow window;
  window.setWindowTitle("CBIR Project 2 GUI");
  window.resize(1000, 800);
  window.show();

  return app.exec();
}
