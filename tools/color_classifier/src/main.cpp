#include <QtGui>
#include <QApplication>
#include <color_classifier/main_window.hpp>

int main(int argc, char **argv) {

  QApplication app(argc, argv);
  color_classifier::MainWindow main;
  main.show();
  app.connect(&app, SIGNAL(lastWindowClosed()), &app, SLOT(quit()));
  int result = app.exec();

	return result;
}
