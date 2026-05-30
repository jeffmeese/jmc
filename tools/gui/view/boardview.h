#pragma once

#include <memory>
#include <vector>

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

class BoardView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit BoardView(QWidget * parent = nullptr);

protected:
  void resizeEvent(QResizeEvent * event) override;

signals:

private:
  std::unique_ptr<QGraphicsScene> mScene;
  std::vector<QGraphicsRectItem*> mSquares;
};
