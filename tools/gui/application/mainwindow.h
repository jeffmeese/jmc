#pragma once

#include <QMainWindow>

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
};
