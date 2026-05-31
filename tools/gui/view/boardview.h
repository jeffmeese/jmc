#pragma once

#include <memory>
#include <vector>

#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QPixmap>
#include <QWidget>

#include "piece_type.h"

#include "application/gui_game.h"

class BoardView : public QGraphicsView
{
  Q_OBJECT

public:
  explicit BoardView(QWidget * parent = nullptr);

  void setGame(GuiGame * game);

protected:
  void mousePressEvent(QMouseEvent * event) override;
  void resizeEvent(QResizeEvent * event) override;

signals:

private:
  std::unique_ptr<QGraphicsScene> mScene;
  std::vector<QGraphicsRectItem *> mSquares;
  std::vector<QPixmap> mPixmaps;
  std::map<jmchess::PieceType, QPixmap> mPieceToPixmap;
  std::vector<QGraphicsPixmapItem *> mPixmapItems;
  std::int32_t mSquareWidth = 0;
  std::int32_t mSquareHeight = 0;
  GuiGame * mGame = nullptr;
};
