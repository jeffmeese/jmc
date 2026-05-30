#include "boardview.h"

#include <memory>
#include <vector>

#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QWidget>

BoardView::BoardView(
  QWidget * parent)
  : QGraphicsView{parent}
{
  mScene.reset(new QGraphicsScene);
  setScene(mScene.get());
  setAlignment(Qt::AlignTop | Qt::AlignLeft);

  mSquares.resize(64);
  mScene->setSceneRect(0, 0, 200, 200);

  QColor lightColor(240, 230, 140);
  QColor darkColor(139, 69, 19);
  QPen pen(Qt::black);

  QColor currentColor = lightColor;
  for (int32_t i = 0; i < 8; i++)
  {
    for (std::int32_t j = 0; j < 8; j++)
    {
      std::int32_t index = (i * 8) + j;

      QBrush brush(currentColor);
      QGraphicsRectItem * item = new QGraphicsRectItem;
      item->setPen(pen);
      item->setBrush(brush);
      mSquares[index] = item;
      mScene->addItem(item);

      currentColor = (currentColor == lightColor) ? darkColor : lightColor;
    }
    currentColor = (currentColor == lightColor) ? darkColor : lightColor;
  }
}

void BoardView::resizeEvent(
  QResizeEvent * event)
{
  //QGraphicsView::resizeEvent(event);

  QRect rect = this->rect();
  QRectF sceneRect = rect;
  mScene->setSceneRect(0, 0, 200, 200);
  //mScene->setSceneRect(sceneRect);

  int width  = sceneRect.width();
  int height = sceneRect.height();
  int squareWidth = width / 8;
  int squareHeight = height / 8;

  std::int32_t yPos = 0; 
  for (std::int32_t i = 0; i < 8; i++)
  {
    std::int32_t xPos = 0;
    for (std::int32_t j = 0; j < 8; j++)
    {
      std::int32_t index = (i * 8) + j;

      QPoint pt = {xPos, yPos};
      QRectF squareRect;
      squareRect.setRect(xPos, yPos, squareWidth, squareHeight);
      mSquares[index]->setRect(squareRect);

      xPos += squareWidth;
    }
    yPos += squareHeight;
  }
}
