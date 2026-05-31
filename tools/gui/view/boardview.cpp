#include "boardview.h"

#include <memory>
#include <vector>

#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMouseEvent>
#include <QPoint>
#include <QWidget>

BoardView::BoardView(
  QWidget * parent)
  : QGraphicsView{parent}
{
  mScene.reset(new QGraphicsScene);
  setScene(mScene.get());
  // setAlignment(Qt::AlignTop | Qt::AlignLeft);

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

  std::vector<QString> resourceNames;
  resourceNames.push_back(":/white-pawn.png");
  resourceNames.push_back(":/white-knight.png");
  resourceNames.push_back(":/white-bishop.png");
  resourceNames.push_back(":/white-rook.png");
  resourceNames.push_back(":/white-queen.png");
  resourceNames.push_back(":/white-king.png");
  resourceNames.push_back(":/black-pawn.png");
  resourceNames.push_back(":/black-knight.png");
  resourceNames.push_back(":/black-bishop.png");
  resourceNames.push_back(":/black-rook.png");
  resourceNames.push_back(":/black-queen.png");
  resourceNames.push_back(":/black-king.png");

  mPixmaps.resize(12);
  for (std::int32_t i = 0; i < 12; i++)
  {
    mPixmaps[i] = QPixmap::fromImage(QImage(resourceNames[i]));
  }

  mPieceToPixmap[jmchess::PieceType::WhitePawn]   = mPixmaps[0];
  mPieceToPixmap[jmchess::PieceType::WhiteKnight] = mPixmaps[1];
  mPieceToPixmap[jmchess::PieceType::WhiteBishop] = mPixmaps[2];
  mPieceToPixmap[jmchess::PieceType::WhiteRook]   = mPixmaps[3];
  mPieceToPixmap[jmchess::PieceType::WhiteQueen]  = mPixmaps[4];
  mPieceToPixmap[jmchess::PieceType::WhiteKing]   = mPixmaps[5];
  mPieceToPixmap[jmchess::PieceType::BlackPawn]   = mPixmaps[6];
  mPieceToPixmap[jmchess::PieceType::BlackKnight] = mPixmaps[7];
  mPieceToPixmap[jmchess::PieceType::BlackBishop] = mPixmaps[8];
  mPieceToPixmap[jmchess::PieceType::BlackRook]   = mPixmaps[9];
  mPieceToPixmap[jmchess::PieceType::BlackQueen]  = mPixmaps[10];
  mPieceToPixmap[jmchess::PieceType::BlackKing]   = mPixmaps[11];

  mPixmapItems.resize(64);
  for (std::int32_t i = 0; i < 64; i++)
  {
    mPixmapItems[i] = new QGraphicsPixmapItem;
    mPixmapItems[i]->setVisible(false);
    mScene->addItem(mPixmapItems[i]);
  }

  for (std::int32_t i = 0; i < 8; i++)
  {
    mPixmapItems[i]->setPixmap(mPieceToPixmap[jmchess::PieceType::WhitePawn]);
  }
}

void BoardView::mousePressEvent(
  QMouseEvent * event)
{
  QGraphicsView::mousePressEvent(event);

  QRect rect = viewport()->rect();
  QPoint pos = event->pos();

  std::int32_t row = 7 - static_cast<std::int32_t>(static_cast<double>(pos.y()) / mSquareHeight);
  std::int32_t col = static_cast<std::int32_t>(static_cast<double>(pos.x()) / mSquareWidth);
  qDebug() << row << " " << col;

  jmchess::Board * board = mGame->getBoard();
  jmchess::MoveList moveList;
  board->generateMoves(row, col, moveList);
  qDebug() << moveList.totalMoves();
}

void BoardView::resizeEvent(
  QResizeEvent * event)
{
  QGraphicsView::resizeEvent(event);

  QRect rect       = viewport()->rect();
  QRectF sceneRect = rect;

  int width     = rect.width();
  int height    = rect.height();
  mSquareWidth  = width / 8;
  mSquareHeight = height / 8;

  int32_t totalHeight = mSquareHeight * 8;
  int32_t totalWidth  = mSquareWidth * 8;
  mScene->setSceneRect(0, 0, totalWidth, totalHeight);

  int32_t imageWidth  = mPixmaps[0].width();
  int32_t imageHeight = mPixmaps[0].height();

  double scaleWidth  = (static_cast<double>(imageWidth) / mSquareWidth);
  double scaleHeight = (static_cast<double>(imageHeight) / mSquareHeight);
  double scale       = scaleHeight;
  if (scaleWidth > scale)
  {
    scale = scaleWidth;
  }

  int32_t pieceWidth  = static_cast<int32_t>(imageWidth / scale);
  int32_t pieceHeight = static_cast<int32_t>(imageHeight / scale);

  mPieceToPixmap[jmchess::PieceType::WhitePawn]   = mPixmaps[0].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::WhiteKnight] = mPixmaps[1].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::WhiteBishop] = mPixmaps[2].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::WhiteRook]   = mPixmaps[3].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::WhiteQueen]  = mPixmaps[4].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::WhiteKing]   = mPixmaps[5].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::BlackPawn]   = mPixmaps[6].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::BlackKnight] = mPixmaps[7].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::BlackBishop] = mPixmaps[8].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::BlackRook]   = mPixmaps[9].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::BlackQueen]  = mPixmaps[10].scaled(pieceWidth, pieceHeight);
  mPieceToPixmap[jmchess::PieceType::BlackKing]   = mPixmaps[11].scaled(pieceWidth, pieceHeight);

  std::int32_t yPiece = height - mSquareHeight;
  std::int32_t yPos   = 0;
  for (std::int32_t i = 0; i < 8; i++)
  {
    std::int32_t xPos = 0;
    for (std::int32_t j = 0; j < 8; j++)
    {
      std::int32_t index = (i * 8) + j;

      QPoint pt = {xPos, yPos};
      QRectF squareRect;
      squareRect.setRect(xPos, yPos, mSquareWidth, mSquareHeight);
      mSquares[index]->setRect(squareRect);
      mPixmapItems[index]->setPos(xPos, yPiece);

      if (mGame != nullptr)
      {
        const jmchess::Board * board = mGame->getBoard();
        jmchess::PieceType pieceType = board->getPieceType(i, j);
        mPixmapItems[index]->setPixmap(mPieceToPixmap[pieceType]);
      }

      xPos += mSquareWidth;
    }
    yPos += mSquareHeight;
    yPiece -= mSquareHeight;
  }
}

void BoardView::setGame(
  GuiGame * game)
{
  mGame = game;

  for (std::int32_t i = 0; i < 64; i++)
  {
    mPixmapItems[i]->setVisible(false);
  }

  if (mGame != nullptr)
  {
    const jmchess::Board * board = mGame->getBoard();
    for (std::int8_t i = 0; i < 8; i++)
    {
      for (std::int8_t j = 0; j < 8; j++)
      {
        std::int8_t index            = (i * 8) + j;
        jmchess::PieceType pieceType = board->getPieceType(i, j);
        mPixmapItems[index]->setPixmap(mPieceToPixmap[pieceType]);
        mPixmapItems[index]->setVisible(true);
      }
    }
  }
}
