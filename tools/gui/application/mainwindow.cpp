#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "gui_game.h"

MainWindow::MainWindow(QWidget *parent)
  : QMainWindow{parent}
  , mUi(new Ui::MainWindow)
  , mGuiGame(new GuiGame)
  , mMoveModel(new MoveModel)
{
  mUi->setupUi(this);
  mUi->cBoardView->setGame(mGuiGame.get());
  mMoveModel->setGame(mGuiGame.get());
  mUi->cMoveView->setModel(mMoveModel.get());

  connect(mUi->cBoardView, &BoardView::moveMade, mMoveModel.get(), &MoveModel::slotMoveMade);
  connect(mUi->cBoardView, &BoardView::moveUndone, mMoveModel.get(), &MoveModel::slotMoveUndone);
}
