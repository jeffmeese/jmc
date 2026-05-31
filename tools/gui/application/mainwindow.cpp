#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gui_game.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{parent}
  , mUi(new Ui::MainWindow)
  , mGuiGame(new GuiGame)
{
  mUi->setupUi(this);
  mUi->cBoardView->setGame(mGuiGame.get());
}
