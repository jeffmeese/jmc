#include "boardview.h"

#include <memory>
#include <vector>

#include <QDebug>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMessageBox>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QWidget>

BoardView::BoardView(
  QWidget * parent)
  : QWidget{parent}
{
  mBoardPixmap       = QPixmap(":/1280px-Chessboard480-wood.svg.png");
  mWhitePawnPixmap   = QPixmap(":/white-pawn.png");
  mWhiteKnightPixmap = QPixmap(":/white-knight.png");
  mWhiteBishopPixmap = QPixmap(":/white-bishop.png");
  mWhiteRookPixmap   = QPixmap(":/white-rook.png");
  mWhiteQueenPixmap  = QPixmap(":/white-queen.png");
  mWhiteKingPixmap   = QPixmap(":/white-king.png");
  mBlackPawnPixmap   = QPixmap(":/black-pawn.png");
  mBlackKnightPixmap = QPixmap(":/black-knight.png");
  mBlackBishopPixmap = QPixmap(":/black-bishop.png");
  mBlackRookPixmap   = QPixmap(":/black-rook.png");
  mBlackQueenPixmap  = QPixmap(":/black-queen.png");
  mBlackKingPixmap   = QPixmap(":/black-king.png");
}

void BoardView::mousePressEvent(
  QMouseEvent * event)
{
  // QGraphicsView::mousePressEvent(event);

  if (mGame == nullptr)
  {
    return;
  }

  QRect rect       = this->rect();
  int width        = rect.width();
  int height       = rect.height();
  int squareWidth  = width / 8;
  int squareHeight = height / 8;

  QPoint pos       = event->pos();
  std::int32_t row = 7 - static_cast<std::int32_t>(static_cast<double>(pos.y()) / squareHeight);
  std::int32_t col = static_cast<std::int32_t>(static_cast<double>(pos.x()) / squareWidth);

  jmchess::Board * board       = mGame->getBoard();
  jmchess::PieceType pieceType = board->getPieceType(row, col);

  // If the user selected the same piece, deselect it
  if (row == mSelectedRow && col == mSelectedCol)
  {
    mSelectedRow = -1;
    mSelectedCol = -1;
    mLegalMoves.clear();
    update();
    return;
  }

  // If the user selects a moveable piece, show the legal moves for that piece
  jmchess::MoveList moveList;
  board->generateMoves(row, col, moveList);
  if (moveList.totalMoves() > 0)
  {
    mSelectedRow = row;
    mSelectedCol = col;
    mLegalMoves.clear();
    for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
    {
      const jmchess::Move & move = moveList.getMove(i);
      if (board->makeMove(move))
      {
        mLegalMoves.push_back(move);
        board->unmakeMove(move);
      }
    }
    update();
    return;
  }

  if (mLegalMoves.size() > 0)
  {
    // Check if the user selected a legal move
    int selectedRow = mSelectedRow;
    int selectedCol = mSelectedCol;
    auto it         = std::find_if(
      mLegalMoves.begin(), mLegalMoves.end(), [selectedRow, selectedCol, row, col](const jmchess::Move & move) {
        bool source      = move.getSourceRow() == selectedRow && move.getSourceColumn() == selectedCol;
        bool destination = move.getDestinationRow() == row && move.getDestinationColumn() == col;
        return source && destination;
      });

    if (it == mLegalMoves.end())
    {
      // Handle illegal move
      QMessageBox::warning(this, "Illegal Move", "That move is not legal.");
      mLegalMoves.clear();
      mSelectedRow = -1;
      mSelectedCol = -1;
      update();
      return;
    }

    if (it != mLegalMoves.end())
    {
      // Handle legal move
      mGame->makeMove(*it);
      emit moveMade(*it);
      mSelectedRow = -1;
      mSelectedCol = -1;
      mLegalMoves.clear();
      update();
    }
  }
}

void BoardView::paintEvent(
  QPaintEvent * event)
{
  QPainter painter(this);

  QRect rect       = this->rect();
  int width        = rect.width();
  int height       = rect.height();
  int squareWidth  = width / 8;
  int squareHeight = height / 8;

  for (std::int8_t i = 0; i < 8; i++)
  {
    for (std::int8_t j = 0; j < 8; j++)
    {
      if ((i + j) % 2 == 0)
      {
        painter.fillRect(j * squareWidth, i * squareHeight, squareWidth, squareHeight, mLightSquareColor);
      }
      else
      {
        painter.fillRect(j * squareWidth, i * squareHeight, squareWidth, squareHeight, mDarkSquareColor);
      }
    }
  }

  // painter.drawPixmap(0, 0, width, height, mBoardPixmap);

  if (mGame != nullptr)
  {
    const jmchess::Board * board = mGame->getBoard();
    for (std::int8_t i = 0; i < 8; i++)
    {
      for (std::int8_t j = 0; j < 8; j++)
      {
        std::int8_t index            = (i * 8) + j;
        jmchess::PieceType pieceType = board->getPieceType(i, j);
        switch (pieceType)
        {
        case jmchess::PieceType::WhitePawn:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mWhitePawnPixmap);
          break;
        case jmchess::PieceType::WhiteKnight:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mWhiteKnightPixmap);
          break;
        case jmchess::PieceType::WhiteBishop:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mWhiteBishopPixmap);
          break;
        case jmchess::PieceType::WhiteRook:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mWhiteRookPixmap);
          break;
        case jmchess::PieceType::WhiteQueen:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mWhiteQueenPixmap);
          break;
        case jmchess::PieceType::WhiteKing:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mWhiteKingPixmap);
          break;
        case jmchess::PieceType::BlackPawn:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mBlackPawnPixmap);
          break;
        case jmchess::PieceType::BlackKnight:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mBlackKnightPixmap);
          break;
        case jmchess::PieceType::BlackBishop:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mBlackBishopPixmap);
          break;
        case jmchess::PieceType::BlackRook:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mBlackRookPixmap);
          break;
        case jmchess::PieceType::BlackQueen:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mBlackQueenPixmap);
          break;
        case jmchess::PieceType::BlackKing:
          painter.drawPixmap(j * squareWidth, (7 - i) * squareHeight, squareWidth, squareHeight, mBlackKingPixmap);
          break;
        }
      }
    }

    if (mLegalMoves.size() > 0)
    {
      QBrush brush(QColor(255, 255, 0, 128));
      for (const jmchess::Move & move : mLegalMoves)
      {
        painter.fillRect(
          move.getDestinationColumn() * squareWidth, (7 - move.getDestinationRow()) * squareHeight, squareWidth,
          squareHeight, brush);
      }
    }
  }
}

void BoardView::resizeEvent(
  QResizeEvent * event)
{
}

void BoardView::setGame(
  GuiGame * game)
{
  mGame = game;
  update();

  // for (std::int32_t i = 0; i < 64; i++)
  // {
  //   mPixmapItems[i]->setVisible(false);
  // }

  // if (mGame != nullptr)
  // {
  //   const jmchess::Board * board = mGame->getBoard();
  //   for (std::int8_t i = 0; i < 8; i++)
  //   {
  //     for (std::int8_t j = 0; j < 8; j++)
  //     {
  //       std::int8_t index            = (i * 8) + j;
  //       jmchess::PieceType pieceType = board->getPieceType(i, j);
  //       mPixmapItems[index]->setPixmap(mPieceToPixmap[pieceType]);
  //       mPixmapItems[index]->setVisible(true);
  //     }
  //   }
  // }
}
