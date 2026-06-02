#pragma once

#include <cstdint>
#include <map>
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

class BoardView : public QWidget
{
  Q_OBJECT

public:
  explicit BoardView(QWidget * parent = nullptr);

  void setGame(GuiGame * game);

protected:
  void paintEvent(QPaintEvent * event) override;
  void mousePressEvent(QMouseEvent * event) override;
  void resizeEvent(QResizeEvent * event) override;

signals:
  void moveMade(const jmchess::Move & move);

  void moveUndone();

private:
  QPixmap mBoardPixmap;
  QPixmap mWhitePawnPixmap;
  QPixmap mWhiteKnightPixmap;
  QPixmap mWhiteBishopPixmap;
  QPixmap mWhiteRookPixmap;
  QPixmap mWhiteQueenPixmap;
  QPixmap mWhiteKingPixmap;
  QPixmap mBlackPawnPixmap;
  QPixmap mBlackKnightPixmap;
  QPixmap mBlackBishopPixmap;
  QPixmap mBlackRookPixmap;
  QPixmap mBlackQueenPixmap;
  QPixmap mBlackKingPixmap;
  std::vector<jmchess::Move> mLegalMoves;
  GuiGame * mGame           = nullptr;
  std::int32_t mSelectedRow = -1;
  std::int32_t mSelectedCol = -1;
  QColor mLightSquareColor  = QColor(240, 217, 181);
  QColor mDarkSquareColor   = QColor(181, 136, 99);
};
