#include "application.h"
#include "mainwindow.h"

int main(int argc, char ** argv)
{
  Application application(argc, argv);
  MainWindow mainWindow;
  mainWindow.show();
  return application.exec();
}