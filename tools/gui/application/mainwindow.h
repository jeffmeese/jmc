#pragma once

#include <memory>

#include <QMainWindow>

#include "gui_game.h"
#include "move_model.h"

namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget * parent = nullptr);

signals:

private:
  Ui::MainWindow * mUi = nullptr;
  std::unique_ptr<GuiGame> mGuiGame;
  std::unique_ptr<MoveModel> mMoveModel;
};
