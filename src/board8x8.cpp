#include "board8x8.h"

#include <cmath>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "fen.h"
#include "piece.h"
#include "piece_type.h"
#include "square.h"

namespace jmchess
{

constexpr std::int8_t getIndex(
  std::int8_t row,
  std::int8_t col)
{
  return (row << 3) | col;
}

constexpr std::int8_t getRow(
  std::int8_t index)
{
  return (index >> 3);
}

constexpr std::int8_t getCol(
  std::int8_t index)
{
  return (index & 7);
}

static constexpr std::int8_t NORTH = 8;
static constexpr std::int8_t SOUTH = -8;
static constexpr std::int8_t EAST  = 1;
static constexpr std::int8_t WEST  = -1;
static constexpr std::int8_t A1    = 0;
static constexpr std::int8_t B1    = 1;
static constexpr std::int8_t C1    = 2;
static constexpr std::int8_t D1    = 3;
static constexpr std::int8_t E1    = 4;
static constexpr std::int8_t F1    = 5;
static constexpr std::int8_t G1    = 6;
static constexpr std::int8_t H1    = 7;
static constexpr std::int8_t A8    = 56;
static constexpr std::int8_t B8    = 57;
static constexpr std::int8_t C8    = 58;
static constexpr std::int8_t D8    = 59;
static constexpr std::int8_t E8    = 60;
static constexpr std::int8_t F8    = 61;
static constexpr std::int8_t G8    = 62;
static constexpr std::int8_t H8    = 63;

Board8x8::Board8x8()
{
  reset();
}

bool Board8x8::checkJumpAttacks(
  std::int8_t index,
  std::int8_t rowIncrement,
  std::int8_t columnIncrement,
  Piece piece,
  Color attackColor) const
{
  std::int8_t row          = getRow(index);
  std::int8_t column       = getCol(index);
  std::int8_t attackRow    = row + rowIncrement;
  std::int8_t attackColumn = column + columnIncrement;

  if (attackRow < 0 || attackRow > 7 || attackColumn < 0 || attackColumn > 7)
  {
    return false;
  }

  std::int8_t attackIndex = getIndex(attackRow, attackColumn);
  Piece attackPiece       = mCells[attackIndex].piece;
  Color attackPieceColor  = mCells[attackIndex].color;

  return (attackPiece == piece && attackPieceColor == attackColor);
}

bool Board8x8::checkSliderAttacks(
  std::int8_t index,
  std::int8_t rowIncrement,
  std::int8_t columnIncrement,
  Piece piece1,
  Piece piece2,
  Color attackColor) const
{
  std::int8_t row          = getRow(index);
  std::int8_t column       = getCol(index);
  std::int8_t attackRow    = row + rowIncrement;
  std::int8_t attackColumn = column + columnIncrement;

  while (true)
  {
    // Check for off board
    if (attackRow < 0 || attackRow > 7 || attackColumn < 0 || attackColumn > 7)
    {
      break;
    }

    std::int8_t attackIndex = getIndex(attackRow, attackColumn);
    Piece attackPiece       = mCells[attackIndex].piece;
    Color attackPieceColor  = mCells[attackIndex].color;

    if (attackPiece != Piece::None)
    {
      if (attackPieceColor == attackColor && (attackPiece == piece1 || attackPiece == piece2))
      {
        return true;
      }

      break;
    }

    attackRow += rowIncrement;
    attackColumn += columnIncrement;
  }

  return false;
}

void Board8x8::generateCastlingMoves(
  Color sideToMove,
  MoveList & moveList) const
{
  std::int8_t castlingRights = mBoardState.castlingRights;
  if (sideToMove == Color::White)
  {
    Color attackColor = Color::Black;
    if (castlingRights & CASTLE_WHITE_KING)
    {
      bool f1Empty = (mCells[F1].piece == Piece::None);
      bool g1Empty = (mCells[G1].piece == Piece::None);
      if (f1Empty && g1Empty)
      {
        if (!isCellAttacked(E1, attackColor) && !isCellAttacked(F1, attackColor) && !isCellAttacked(G1, attackColor))
        {
          pushMove(E1, G1, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }

    if (castlingRights & CASTLE_WHITE_QUEEN)
    {
      bool d1Empty = (mCells[D1].piece == Piece::None);
      bool c1Empty = (mCells[C1].piece == Piece::None);
      bool b1Empty = (mCells[B1].piece == Piece::None);
      if (d1Empty && c1Empty && b1Empty)
      {
        if (!isCellAttacked(E1, attackColor) && !isCellAttacked(D1, attackColor) && !isCellAttacked(C1, attackColor))
        {
          pushMove(E1, C1, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }
  }
  else
  {
    Color attackColor = Color::White;
    if (castlingRights & CASTLE_BLACK_KING)
    {
      bool f8Empty = (mCells[F8].piece == Piece::None);
      bool g8Empty = (mCells[G8].piece == Piece::None);
      if (f8Empty && g8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(F8, attackColor) && !isCellAttacked(G8, attackColor))
        {
          pushMove(E8, G8, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }

    if (castlingRights & CASTLE_BLACK_QUEEN)
    {
      bool d8Empty = (mCells[D8].piece == Piece::None);
      bool c8Empty = (mCells[C8].piece == Piece::None);
      bool b8Empty = (mCells[B8].piece == Piece::None);
      if (d8Empty && c8Empty && b8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(D8, attackColor) && !isCellAttacked(C8, attackColor))
        {
          pushMove(E8, C8, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }
  }
}

void Board8x8::generateJumpMoves(
  std::int8_t index,
  std::int8_t rowIncrement,
  std::int8_t columnIncrement,
  Piece piece,
  Color pieceColor,
  MoveList & moveList) const
{
  std::int8_t row        = getRow(index);
  std::int8_t column     = getCol(index);
  std::int8_t destRow    = row + rowIncrement;
  std::int8_t destColumn = column + columnIncrement;

  // Check for off board moves
  if (destRow < 0 || destColumn < 0 || destRow > 7 || destColumn > 7)
  {
    return;
  }

  std::int8_t destIndex = getIndex(destRow, destColumn);
  Piece otherPiece      = mCells[destIndex].piece;
  Color otherColor      = mCells[destIndex].color;
  if (otherColor == Color::None)
  {
    pushMove(index, destIndex, piece, Piece::None, Piece::None, Move::Type::Quiet, moveList);
    return;
  }

  if (isOppositeColor(pieceColor, otherColor))
  {
    pushMove(index, destIndex, piece, otherPiece, Piece::None, Move::Type::Capture, moveList);
  }
}

void Board8x8::generateMoves(
  MoveList & moveList) const
{
  Color sideToMove = mBoardState.sideToMove;
  generateCastlingMoves(sideToMove, moveList);

  for (std::int8_t index = 0; index < 64; index++)
  {
    generateMoves(index, moveList);
  }
}

void Board8x8::generateMoves(
  std::int8_t index,
  MoveList & moveList) const
{
  if (index < 0 || index > 63)
  {
    std::ostringstream oss;
    oss << "Invalid index value in generateMoves: " << index;
    throw std::invalid_argument(oss.str());
  }

  Color sideToMove = mBoardState.sideToMove;
  Piece piece      = mCells[index].piece;
  Color color      = mCells[index].color;

  if (piece == Piece::None || (color != sideToMove))
  {
    return;
  }

  switch (piece)
  {
  case Piece::Pawn:
    generatePawnMoves(index, sideToMove, moveList);
    break;
  case Piece::Knight:
    generateJumpMoves(index, 1, -2, piece, sideToMove, moveList);
    generateJumpMoves(index, 2, -1, piece, sideToMove, moveList);
    generateJumpMoves(index, 2, 1, piece, sideToMove, moveList);
    generateJumpMoves(index, 1, 2, piece, sideToMove, moveList);
    generateJumpMoves(index, -1, 2, piece, sideToMove, moveList);
    generateJumpMoves(index, -2, 1, piece, sideToMove, moveList);
    generateJumpMoves(index, -2, -1, piece, sideToMove, moveList);
    generateJumpMoves(index, -1, -2, piece, sideToMove, moveList);
    break;
  case Piece::Bishop:
    generateSlidingMoves(index, 1, -1, piece, sideToMove, moveList);
    generateSlidingMoves(index, 1, 1, piece, sideToMove, moveList);
    generateSlidingMoves(index, -1, 1, piece, sideToMove, moveList);
    generateSlidingMoves(index, -1, -1, piece, sideToMove, moveList);
    break;
  case Piece::Rook:
    generateSlidingMoves(index, 1, 0, piece, sideToMove, moveList);
    generateSlidingMoves(index, -1, 0, piece, sideToMove, moveList);
    generateSlidingMoves(index, 0, 1, piece, sideToMove, moveList);
    generateSlidingMoves(index, 0, -1, piece, sideToMove, moveList);
    break;
  case Piece::Queen:
    generateSlidingMoves(index, 1, -1, piece, sideToMove, moveList);
    generateSlidingMoves(index, 1, 1, piece, sideToMove, moveList);
    generateSlidingMoves(index, -1, 1, piece, sideToMove, moveList);
    generateSlidingMoves(index, -1, -1, piece, sideToMove, moveList);
    generateSlidingMoves(index, 1, 0, piece, sideToMove, moveList);
    generateSlidingMoves(index, -1, 0, piece, sideToMove, moveList);
    generateSlidingMoves(index, 0, 1, piece, sideToMove, moveList);
    generateSlidingMoves(index, 0, -1, piece, sideToMove, moveList);
    break;
  case Piece::King:
    generateJumpMoves(index, 1, 0, piece, sideToMove, moveList);
    generateJumpMoves(index, -1, 0, piece, sideToMove, moveList);
    generateJumpMoves(index, 0, 1, piece, sideToMove, moveList);
    generateJumpMoves(index, 0, -1, piece, sideToMove, moveList);
    generateJumpMoves(index, 1, -1, piece, sideToMove, moveList);
    generateJumpMoves(index, 1, 1, piece, sideToMove, moveList);
    generateJumpMoves(index, -1, 1, piece, sideToMove, moveList);
    generateJumpMoves(index, -1, -1, piece, sideToMove, moveList);
    break;
  }
}

void Board8x8::generateMoves(
  std::int8_t row,
  std::int8_t column,
  MoveList & moveList) const
{
  if (row < 0 || row > 7)
  {
    std::ostringstream oss;
    oss << "Invalid row value in generateMoves: " << row;
    throw std::invalid_argument(oss.str());
  }

  if (column < 0 || column > 7)
  {
    std::ostringstream oss;
    oss << "Invalid column value in generateMoves: " << column;
    throw std::invalid_argument(oss.str());
  }

  std::int8_t index = getIndex(row, column);
  Color sideToMove  = mBoardState.sideToMove;
  Piece piece       = mCells[index].piece;
  Color color       = mCells[index].color;
  if (piece == Piece::None || (color != sideToMove))
  {
    return;
  }

  if (piece == Piece::King)
  {
    generateCastlingMoves(sideToMove, moveList);
  }

  return generateMoves(index, moveList);
}

void Board8x8::generatePawnMoves(
  std::int8_t index,
  Color pieceColor,
  MoveList & moveList) const
{
  std::int8_t sourceRow       = getRow(index);
  std::int8_t sourceColumn    = getCol(index);
  std::int8_t enPassantColumn = mBoardState.enpassantColumn;

  // Compute some values based on the piece color
  std::int8_t startRow       = 1;
  std::int8_t rowIncrement   = 1;
  std::int8_t indexIncrement = NORTH;
  std::int8_t enPassantRow   = 4;
  Color otherColor           = Color::Black;
  if (pieceColor == Color::Black)
  {
    startRow       = 6;
    rowIncrement   = -1;
    indexIncrement = SOUTH;
    enPassantRow   = 3;
    otherColor     = Color::White;
  }

  std::int8_t pushSquare       = index + indexIncrement;
  std::int8_t doublePushSquare = index + indexIncrement + indexIncrement;

  if (pushSquare >= 0 && pushSquare <= 63)
  {
    // Generate non-capture moves
    if (mCells[pushSquare].piece == Piece::None)
    {
      std::int8_t destRow = getRow(pushSquare);
      if (destRow == WHITE_PROMOTION_ROW || destRow == BLACK_PROMOTION_ROW)
      {
        pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Bishop, Move::Type::Promotion, moveList);
        pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Knight, Move::Type::Promotion, moveList);
        pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Rook, Move::Type::Promotion, moveList);
        pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Queen, Move::Type::Promotion, moveList);
      }
      else
      {
        pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::Quiet, moveList);
      }

      if (doublePushSquare >= 0 && doublePushSquare <= 63)
      {
        if (sourceRow == startRow && mCells[doublePushSquare].piece == Piece::None)
        {
          pushMove(index, doublePushSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::EnpassantPush, moveList);
        }
      }
    }
  }

  // Generate capture moves except en-passant captures
  constexpr std::int8_t columnIncrements[2] = {-1, +1};
  for (int i = 0; i < 2; i++)
  {
    std::int8_t destColumn = sourceColumn + columnIncrements[i];
    if (destColumn >= 0 && destColumn < 8)
    {
      std::int8_t destRow = sourceRow + rowIncrement;
      if (destRow >= 0 && destRow < 8)
      {
        std::int8_t destIndex = getIndex(destRow, destColumn);
        Piece capturePiece    = mCells[destIndex].piece;
        Color otherColor      = mCells[destIndex].color;
        if (capturePiece != Piece::None && isOppositeColor(pieceColor, otherColor))
        {
          if (pieceColor == Color::White && destRow == WHITE_PROMOTION_ROW)
          {
            pushMove(
              index, destIndex, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
            pushMove(
              index, destIndex, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
            pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
            pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
          }
          else if (pieceColor == Color::Black && destRow == BLACK_PROMOTION_ROW)
          {
            pushMove(
              index, destIndex, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
            pushMove(
              index, destIndex, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
            pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
            pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
          }
          else
          {
            pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::None, Move::Type::Capture, moveList);
          }
        }
      }
    }
  }

  // Generate en-passant captures moves
  if (enPassantColumn != INVALID_ENPASSANT_COLUMN)
  {
    if (abs(sourceColumn - enPassantColumn) == 1 && sourceRow == enPassantRow)
    {
      std::int8_t destRow    = sourceRow + rowIncrement;
      std::int8_t destColumn = enPassantColumn;
      if (destRow >= 0 && destRow <= 7 && destColumn >= 0 && destColumn <= 7)
      {
        std::int8_t destIndex    = getIndex(destRow, destColumn);
        std::int8_t captureIndex = destIndex - indexIncrement;
        if (mCells[captureIndex].piece == Piece::Pawn && mCells[captureIndex].color == otherColor)
        {
          pushMove(index, destIndex, Piece::Pawn, Piece::Pawn, Piece::None, Move::Type::EnpassantCapture, moveList);
        }
      }
    }
  }
}

void Board8x8::generateSlidingMoves(
  std::int8_t index,
  std::int8_t rowIncrement,
  std::int8_t columnIncrement,
  Piece piece,
  Color pieceColor,
  MoveList & moveList) const
{
  std::int8_t row        = getRow(index);
  std::int8_t column     = getCol(index);
  std::int8_t destRow    = row + rowIncrement;
  std::int8_t destColumn = column + columnIncrement;
  while (true)
  {
    // Check to make sure we haven't gone off the board
    if (destRow < 0 || destRow > 7 || destColumn < 0 || destColumn > 7)
    {
      break;
    }

    std::int8_t destIndex = getIndex(destRow, destColumn);
    Piece otherPiece      = mCells[destIndex].piece;
    Color otherColor      = mCells[destIndex].color;

    // Check for friendly pieces
    if (isSameColor(pieceColor, otherColor))
    {
      break;
    }

    // Check for quiet moves
    if (otherColor == Color::None)
    {
      pushMove(index, destIndex, piece, Piece::None, Piece::None, Move::Type::Quiet, moveList);
    }

    // Check for capture moves
    if (isOppositeColor(pieceColor, otherColor))
    {
      pushMove(index, destIndex, piece, otherPiece, Piece::None, Move::Type::Capture, moveList);
      break;
    }

    destRow += rowIncrement;
    destColumn += columnIncrement;
  }
}

BoardState Board8x8::getBoardState() const
{
  return mBoardState;
}

std::int8_t Board8x8::getKingColumn(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
}

std::int8_t Board8x8::getKingRow(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
}

PieceType Board8x8::getPieceType(
  std::int8_t row,
  std::int8_t col) const
{
  std::int8_t index = getIndex(row, col);

  Piece piece = mCells[index].piece;
  Color color = mCells[index].color;

  PieceType pieceType = PieceType::None;
  if (piece != Piece::None)
  {
    if (color == Color::White)
    {
      switch (piece)
      {
      case Piece::Pawn:
        pieceType = PieceType::WhitePawn;
        break;
      case Piece::Rook:
        pieceType = PieceType::WhiteRook;
        break;
      case Piece::Knight:
        pieceType = PieceType::WhiteKnight;
        break;
      case Piece::Bishop:
        pieceType = PieceType::WhiteBishop;
        break;
      case Piece::Queen:
        pieceType = PieceType::WhiteQueen;
        break;
      case Piece::King:
        pieceType = PieceType::WhiteKing;
        break;
      default:
        break;
      }
    }
    else
    {
      switch (piece)
      {
      case Piece::Pawn:
        pieceType = PieceType::BlackPawn;
        break;
      case Piece::Rook:
        pieceType = PieceType::BlackRook;
        break;
      case Piece::Knight:
        pieceType = PieceType::BlackKnight;
        break;
      case Piece::Bishop:
        pieceType = PieceType::BlackBishop;
        break;
      case Piece::Queen:
        pieceType = PieceType::BlackQueen;
        break;
      case Piece::King:
        pieceType = PieceType::BlackKing;
        break;
      default:
        break;
      }
    }
  }

  return pieceType;
}

bool Board8x8::isCellAttacked(
  std::int8_t row,
  std::int8_t col,
  Color attackingColor) const
{
  return isCellAttacked(getIndex(row, col), attackingColor);
}

bool Board8x8::isCellAttacked(
  std::int8_t index,
  Color attackingColor) const
{
  // Check for pawn attacks
  for (std::int8_t i = 0; i < 2; i++)
  {
    std::int8_t rowIncrement    = (attackingColor == Color::Black) ? PAWN_ROW_INCREMENTS[i] : -PAWN_ROW_INCREMENTS[i];
    std::int8_t columnIncrement = PAWN_COLUMN_INCREMENTS[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::Pawn, attackingColor))
    {
      return true;
    }
  }

  // Check for knight attacks
  for (std::int8_t i = 0; i < 8; i++)
  {
    std::int8_t rowIncrement    = KNIGHT_ROW_INCREMENTS[i];
    std::int8_t columnIncrement = KNIGHT_COLUMN_INCREMENTS[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::Knight, attackingColor))
    {
      return true;
    }
  }

  // Check for diagonal sliding attacks
  for (std::int8_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = DIAGONAL_ROW_INCREMENTS[i];
    std::int8_t columnIncrement = DIAGONAL_COLUMN_INCREMENTS[i];

    if (checkSliderAttacks(index, rowIncrement, columnIncrement, Piece::Bishop, Piece::Queen, attackingColor))
    {
      return true;
    }
  }

  // Check for straight sliding attacks
  for (std::int8_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = STRAIGHT_ROW_INCREMENTS[i];
    std::int8_t columnIncrement = STRAIGHT_COLUMN_INCREMENTS[i];

    if (checkSliderAttacks(index, rowIncrement, columnIncrement, Piece::Rook, Piece::Queen, attackingColor))
    {
      return true;
    }
  }

  // Check for king diagonal attacks
  for (std::int8_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = DIAGONAL_ROW_INCREMENTS[i];
    std::int8_t columnIncrement = DIAGONAL_COLUMN_INCREMENTS[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::King, attackingColor))
    {
      return true;
    }
  }

  // Check for king straight attacks
  for (std::int8_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = STRAIGHT_ROW_INCREMENTS[i];
    std::int8_t columnIncrement = STRAIGHT_COLUMN_INCREMENTS[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::King, attackingColor))
    {
      return true;
    }
  }

  return false;
}

bool Board8x8::isKingInCheck(
  Color color) const
{
  std::int8_t row    = (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
  std::int8_t column = (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
  Color attackColor  = (color == Color::White) ? Color::Black : Color::White;
  std::int8_t index  = getIndex(row, column);

  return isCellAttacked(index, attackColor);
}

void Board8x8::makeMove(
  const Move & move)
{
  BoardState boardState   = move.getBoardState();
  Color sideToMove        = boardState.sideToMove;
  Square sourceSquare     = move.getSourceSquare();
  Square destSquare       = move.getDestinationSquare();
  Piece movedPiece        = move.getPiece();
  std::int8_t sourceIndex = getIndex(sourceSquare.row, sourceSquare.col);
  std::int8_t destIndex   = getIndex(destSquare.row, destSquare.col);

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;

  // Do basic update
  mCells[sourceIndex].piece = Piece::None;
  mCells[sourceIndex].color = Color::None;
  mCells[destIndex].piece   = movedPiece;
  mCells[destIndex].color   = sideToMove;

  // Handle double pawn pushes
  if (move.isEnpassantPush())
  {
    mBoardState.enpassantColumn = sourceSquare.col;
  }

  // Handle promotion moves
  if (move.isPromotion())
  {
    Piece promotedPiece     = move.getPromotedPiece();
    mCells[destIndex].piece = promotedPiece;
  }

  // Handle castle moves
  if (move.isCastle())
  {
    switch (destIndex)
    {
    case C1:
      mCells[A1].piece = Piece::None;
      mCells[A1].color = Color::None;
      mCells[D1].piece = Piece::Rook;
      mCells[D1].color = Color::White;
      break;
    case G1:
      mCells[F1].piece = Piece::Rook;
      mCells[F1].color = Color::White;
      mCells[H1].piece = Piece::None;
      mCells[H1].color = Color::None;
      break;
    case C8:
      mCells[A8].piece = Piece::None;
      mCells[A8].color = Color::None;
      mCells[D8].piece = Piece::Rook;
      mCells[D8].color = Color::Black;
      break;
    case G8:
      mCells[F8].piece = Piece::Rook;
      mCells[F8].color = Color::Black;
      mCells[H8].piece = Piece::None;
      mCells[H8].color = Color::None;
      break;
    }
  }

  // Handle king moves
  if (movedPiece == Piece::King)
  {
    if (sideToMove == Color::White)
    {
      mWhiteKingSquare = destSquare;
    }
    else
    {
      mBlackKingSquare = destSquare;
    }
  }

  // Handle en-passant captures
  if (move.isEnpassantCapture())
  {
    std::int8_t dir              = (sideToMove == Color::White ? -1 : +1);
    std::int8_t enPassantIndex   = getIndex(destSquare.row + dir, boardState.enpassantColumn);
    mCells[enPassantIndex].piece = Piece::None;
    mCells[enPassantIndex].color = Color::None;
  }

  // Update castling rights
  // If the square associated with a king or rook is
  // modified in any way remove the castling ability
  // for that piece
  switch (sourceIndex)
  {
  case H1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_KING;
    break;
  case E1:
    mBoardState.castlingRights &= ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
    break;
  case A1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_QUEEN;
    break;
  case H8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_KING;
    break;
  case E8:
    mBoardState.castlingRights &= ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
    break;
  case A8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_QUEEN;
    break;
  }

  switch (destIndex)
  {
  case H1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_KING;
    break;
  case E1:
    mBoardState.castlingRights &= ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
    break;
  case A1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_QUEEN;
    break;
  case H8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_KING;
    break;
  case E8:
    mBoardState.castlingRights &= ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
    break;
  case A8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_QUEEN;
    break;
  }

  // Update half move clock
  mBoardState.halfMoveClock++;
  if (movedPiece == Piece::Pawn || move.isCapture())
  {
    mBoardState.halfMoveClock = 0;
  }

  // Update full move clock
  if (sideToMove == Color::Black)
  {
    mBoardState.fullMoveClock++;
  }

  // Update side to move
  mBoardState.sideToMove = (sideToMove == Color::White) ? Color::Black : Color::White;
}

void Board8x8::pushMove(
  std::int8_t fromSquare,
  std::int8_t toSquare,
  Piece piece,
  Piece capturePiece,
  Piece promotionPiece,
  Move::Type type,
  MoveList & moveList) const
{
  std::int8_t sourceRow = getRow(fromSquare);
  std::int8_t sourceCol = getCol(fromSquare);
  std::int8_t destRow   = getRow(toSquare);
  std::int8_t destCol   = getCol(toSquare);

  Square sourceSquare = {sourceRow, sourceCol};
  Square destSquare   = {destRow, destCol};
  Move move(sourceSquare, destSquare, piece, mBoardState, type, capturePiece, promotionPiece);
  moveList.addMove(move);
}

void Board8x8::reset()
{
  static const Piece pieces[] = {Piece::Rook, Piece::Knight, Piece::Bishop, Piece::Queen,
                                 Piece::King, Piece::Bishop, Piece::Knight, Piece::Rook};

  for (std::int8_t i = 0; i < 64; i++)
  {
    mCells[i].piece = Piece::None;
    mCells[i].color = Color::None;
  }

  for (std::int8_t col = 0; col < 8; col++)
  {
    mCells[getIndex(1, col)].piece = Piece::Pawn;
    mCells[getIndex(1, col)].color = Color::White;
    mCells[getIndex(0, col)].piece = pieces[col];
    mCells[getIndex(0, col)].color = Color::White;
    mCells[getIndex(6, col)].piece = Piece::Pawn;
    mCells[getIndex(6, col)].color = Color::Black;
    mCells[getIndex(7, col)].piece = pieces[col];
    mCells[getIndex(7, col)].color = Color::Black;
  }

  mWhiteKingSquare = {0, 4};
  mBlackKingSquare = {7, 4};

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;
  mBoardState.sideToMove      = Color::White;
  mBoardState.castlingRights  = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN | CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN;
  mBoardState.fullMoveClock   = 1;
  mBoardState.halfMoveClock   = 0;
}

void Board8x8::setPosition(
  const std::string & fenString)
{
  reset();

  Fen fenData;
  fenData.setFromString(fenString);

  mBoardState = fenData.getBoardState();

  for (std::int8_t i = 0; i < 8; i++)
  {
    for (std::int8_t j = 0; j < 8; j++)
    {
      std::int8_t index   = getIndex(i, j);
      mCells[index].piece = Piece::None;
      mCells[index].color = Color::None;

      PieceType pieceType = fenData.getPieceType(i, j);
      switch (pieceType)
      {
      case PieceType::WhitePawn:
        mCells[index].piece = Piece::Pawn;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteRook:
        mCells[index].piece = Piece::Rook;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteKnight:
        mCells[index].piece = Piece::Knight;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteBishop:
        mCells[index].piece = Piece::Bishop;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteQueen:
        mCells[index].piece = Piece::Queen;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteKing:
        mCells[index].piece = Piece::King;
        mCells[index].color = Color::White;
        mWhiteKingSquare    = Square{i, j};
        break;
      case PieceType::BlackPawn:
        mCells[index].piece = Piece::Pawn;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackRook:
        mCells[index].piece = Piece::Rook;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackKnight:
        mCells[index].piece = Piece::Knight;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackBishop:
        mCells[index].piece = Piece::Bishop;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackQueen:
        mCells[index].piece = Piece::Queen;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackKing:
        mCells[index].piece = Piece::King;
        mCells[index].color = Color::Black;
        mBlackKingSquare    = Square{i, j};
        break;
      default:
        mCells[index].piece = Piece::None;
        mCells[index].color = Color::None;
        break;
      }
    }
  }
}

void Board8x8::unmakeMove(
  const Move & move)
{
  // Set the previous board state
  BoardState boardState   = move.getBoardState();
  Color sideThatMoved     = boardState.sideToMove;
  Color otherSide         = (sideThatMoved == Color::White) ? Color::Black : Color::White;
  Square sourceSquare     = move.getSourceSquare();
  Square destSquare       = move.getDestinationSquare();
  Piece movedPiece        = move.getPiece();
  std::int8_t sourceIndex = getIndex(sourceSquare.row, sourceSquare.col);
  std::int8_t destIndex   = getIndex(destSquare.row, destSquare.col);

  // Do basic update
  mBoardState               = boardState;
  mCells[sourceIndex].piece = movedPiece;
  mCells[sourceIndex].color = sideThatMoved;
  mCells[destIndex].piece   = Piece::None;
  mCells[destIndex].color   = Color::None;

  // Handle capture moves (except enpassant captures)
  if (move.isStandardCapture() || move.isPromotionCapture())
  {
    Piece capturePiece      = move.getCapturedPiece();
    mCells[destIndex].piece = capturePiece;
    mCells[destIndex].color = otherSide;
  }

  // Handle enpassant capture
  if (move.isEnpassantCapture())
  {
    std::int8_t dir            = (sideThatMoved == Color::White ? -1 : +1);
    std::int8_t captureIndex   = getIndex(destSquare.row + dir, boardState.enpassantColumn);
    Piece capturePiece         = move.getCapturedPiece();
    mCells[captureIndex].piece = capturePiece;
    mCells[captureIndex].color = otherSide;
  }

  // Update king moves
  if (movedPiece == Piece::King)
  {
    if (sideThatMoved == Color::White)
    {
      mWhiteKingSquare = sourceSquare;
    }

    if (sideThatMoved == Color::Black)
    {
      mBlackKingSquare = sourceSquare;
    }
  }

  // Handle castling moves
  if (move.isCastle())
  {
    switch (destIndex)
    {
    case C1:
      mCells[A1].piece = Piece::Rook;
      mCells[A1].color = Color::White;
      mCells[D1].piece = Piece::None;
      mCells[D1].color = Color::None;
      break;
    case G1:
      mCells[F1].piece = Piece::None;
      mCells[F1].color = Color::None;
      mCells[H1].piece = Piece::Rook;
      mCells[H1].color = Color::White;
      break;
    case C8:
      mCells[A8].piece = Piece::Rook;
      mCells[A8].color = Color::Black;
      mCells[D8].piece = Piece::None;
      mCells[D8].color = Color::None;
      break;
    case G8:
      mCells[F8].piece = Piece::None;
      mCells[F8].color = Color::None;
      mCells[H8].piece = Piece::Rook;
      mCells[H8].color = Color::Black;
      break;
    }
  }
}

} // namespace jmchess
