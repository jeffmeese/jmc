#include "board8x8.h"

#include <cstdint>
#include <iostream>
#include <sstream>
#include <stack>
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

#define getIndex(row, col) (((row) << 3) + (col))
#define getRow(index)      ((index >> 3))
#define getCol(index)      ((index % 8))

constexpr int8_t NORTH = 8;
constexpr int8_t SOUTH = -8;
constexpr int8_t EAST  = 1;
constexpr int8_t WEST  = -1;

enum BoardSquare
{
  A1 = 0,
  B1,
  C1,
  D1,
  E1,
  F1,
  G1,
  H1,
  A2 = 8,
  B2,
  C2,
  D2,
  E2,
  F2,
  G2,
  H2,
  A3 = 16,
  B3,
  C3,
  D3,
  E3,
  F3,
  G3,
  H3,
  A4 = 24,
  B4,
  C4,
  D4,
  E4,
  F4,
  G4,
  H4,
  A5 = 32,
  B5,
  C5,
  D5,
  E5,
  F5,
  G5,
  H5,
  A6 = 40,
  B6,
  C6,
  D6,
  E6,
  F6,
  G6,
  H6,
  A7 = 48,
  B7,
  C7,
  D7,
  E7,
  F7,
  G7,
  H7,
  A8 = 56,
  B8,
  C8,
  D8,
  E8,
  F8,
  G8,
  H8
};

static constexpr std::int32_t NORTH_ROW_INCREMENT               = +1;
static constexpr std::int32_t NORTH_COLUMN_INCREMENT            = +0;
static constexpr std::int32_t SOUTH_ROW_INCREMENT               = -1;
static constexpr std::int32_t SOUTH_COLUMN_INCREMENT            = +0;
static constexpr std::int32_t EAST_ROW_INCREMENT                = +0;
static constexpr std::int32_t EAST_COLUMN_INCREMENT             = +1;
static constexpr std::int32_t WEST_ROW_INCREMENT                = +0;
static constexpr std::int32_t WEST_COLUMN_INCREMENT             = -1;
static constexpr std::int32_t NORTH_EAST_ROW_INCREMENT          = +1;
static constexpr std::int32_t NORTH_EAST_COLUMN_INCREMENT       = +1;
static constexpr std::int32_t NORTH_WEST_ROW_INCREMENT          = +1;
static constexpr std::int32_t NORTH_WEST_COLUMN_INCREMENT       = -1;
static constexpr std::int32_t SOUTH_EAST_ROW_INCREMENT          = -1;
static constexpr std::int32_t SOUTH_EAST_COLUMN_INCREMENT       = +1;
static constexpr std::int32_t SOUTH_WEST_ROW_INCREMENT          = -1;
static constexpr std::int32_t SOUTH_WEST_COLUMN_INCREMENT       = -1;
static constexpr std::int32_t NORTH_WEST_WEST_ROW_INCREMENT     = +1;
static constexpr std::int32_t NORTH_WEST_WEST_COLUMN_INCREMENT  = -2;
static constexpr std::int32_t NORTH_NORTH_WEST_ROW_INCREMENT    = +2;
static constexpr std::int32_t NORTH_NORTH_WEST_COLUMN_INCREMENT = -1;
static constexpr std::int32_t NORTH_NORTH_EAST_ROW_INCREMENT    = +2;
static constexpr std::int32_t NORTH_NORTH_EAST_COLUMN_INCREMENT = +1;
static constexpr std::int32_t NORTH_EAST_EAST_ROW_INCREMENT     = +1;
static constexpr std::int32_t NORTH_EAST_EAST_COLUMN_INCREMENT  = +2;
static constexpr std::int32_t SOUTH_EAST_EAST_ROW_INCREMENT     = -1;
static constexpr std::int32_t SOUTH_EAST_EAST_COLUMN_INCREMENT  = +2;
static constexpr std::int32_t SOUTH_SOUTH_EAST_ROW_INCREMENT    = -2;
static constexpr std::int32_t SOUTH_SOUTH_EAST_COLUMN_INCREMENT = +1;
static constexpr std::int32_t SOUTH_SOUTH_WEST_ROW_INCREMENT    = -2;
static constexpr std::int32_t SOUTH_SOUTH_WEST_COLUMN_INCREMENT = -1;
static constexpr std::int32_t SOUTH_WEST_WEST_ROW_INCREMENT     = -1;
static constexpr std::int32_t SOUTH_WEST_WEST_COLUMN_INCREMENT  = -2;

Board8x8::Board8x8()
{
  reset();
}

bool Board8x8::checkJumpAttacks(
  std::uint8_t index,
  std::int8_t rowIncrement,
  std::int8_t columnIncrement,
  Piece piece,
  Color attackColor) const
{
  std::int8_t row          = getRow(index);
  std::int8_t column       = getCol(index);
  std::int8_t attackRow    = row + rowIncrement;
  std::int8_t attackColumn = column + columnIncrement;
  std::int8_t attackIndex  = getIndex(attackRow, attackColumn);

  if (attackRow < 0 || attackRow > 7 || attackColumn < 0 || attackColumn > 7)
  {
    return false;
  }

  Piece attackPiece      = mPieces[attackIndex];
  Color attackPieceColor = mColors[attackIndex];
  if (attackPiece == piece && attackPieceColor == attackColor)
  {
    return true;
  }

  return false;
}

bool Board8x8::checkSliderAttacks(
  std::uint8_t index,
  std::int8_t rowIncrement,
  std::int8_t columnIncrement,
  Piece piece,
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
    Piece attackPiece       = mPieces[attackIndex];
    Color attackPieceColor  = mColors[attackIndex];

    // Check for friendly pieces
    if (piece != Piece::None && isOppositeColor(attackColor, attackPieceColor))
    {
      break;
    }

    // Check for enemy piece
    if (piece == attackPiece && isSameColor(attackPieceColor, attackColor))
    {
      return true;
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
  std::uint8_t castlingRights = mBoardState.castlingRights;
  if (sideToMove == Color::White)
  {
    Color attackColor = Color::Black;
    if (castlingRights & CASTLE_WHITE_KING)
    {
      bool f1Empty = (mPieces[F1] == Piece::None);
      bool g1Empty = (mPieces[G1] == Piece::None);
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
      bool d1Empty = (mPieces[D1] == Piece::None);
      bool c1Empty = (mPieces[C1] == Piece::None);
      if (d1Empty && c1Empty)
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
      bool f8Empty = (mPieces[F8] == Piece::None);
      bool g8Empty = (mPieces[G8] == Piece::None);
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
      bool d8Empty = (mPieces[D8] == Piece::None);
      bool c8Empty = (mPieces[C8] == Piece::None);
      if (d8Empty && c8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(D8, attackColor) && !isCellAttacked(C8, attackColor))
        {
          pushMove(E8, B8, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }
  }
}

void Board8x8::generateJumpMoves(
  std::uint8_t index,
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
  std::int8_t destIndex  = getIndex(destRow, destColumn);

  // Check for off board moves
  if (destRow < 0 || destColumn < 0 || destRow > 7 || destColumn > 7)
  {
    return;
  }

  Piece otherPiece = mPieces[destIndex];
  Color otherColor = mColors[destIndex];
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
  for (uint8_t index = 0; index < 64; index++)
  {
    generateMoves(index, moveList);
  }
}

void Board8x8::generateMoves(
  uint8_t index,
  MoveList & moveList) const
{
  Color sideToMove = mBoardState.sideToMove;
  Piece piece      = mPieces[index];
  Color color      = mColors[index];

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
    generateJumpMoves(
      index, NORTH_WEST_WEST_ROW_INCREMENT, NORTH_WEST_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, NORTH_NORTH_WEST_ROW_INCREMENT, NORTH_NORTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, NORTH_NORTH_EAST_ROW_INCREMENT, NORTH_NORTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, NORTH_EAST_EAST_ROW_INCREMENT, NORTH_EAST_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, SOUTH_EAST_EAST_ROW_INCREMENT, SOUTH_EAST_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, SOUTH_SOUTH_EAST_ROW_INCREMENT, SOUTH_SOUTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, SOUTH_SOUTH_WEST_ROW_INCREMENT, SOUTH_SOUTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(
      index, SOUTH_WEST_WEST_ROW_INCREMENT, SOUTH_WEST_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    break;
  case Piece::Bishop:
    generateSlidingMoves(index, NORTH_WEST_ROW_INCREMENT, NORTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, NORTH_EAST_ROW_INCREMENT, NORTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, SOUTH_EAST_ROW_INCREMENT, SOUTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, SOUTH_WEST_ROW_INCREMENT, SOUTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    break;
  case Piece::Rook:
    generateSlidingMoves(index, NORTH_ROW_INCREMENT, NORTH_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, SOUTH_ROW_INCREMENT, SOUTH_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, EAST_ROW_INCREMENT, EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, WEST_ROW_INCREMENT, WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    break;
  case Piece::Queen:
    generateSlidingMoves(index, NORTH_WEST_ROW_INCREMENT, NORTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, NORTH_EAST_ROW_INCREMENT, NORTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, SOUTH_EAST_ROW_INCREMENT, SOUTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, SOUTH_WEST_ROW_INCREMENT, SOUTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, NORTH_ROW_INCREMENT, NORTH_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, SOUTH_ROW_INCREMENT, SOUTH_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, EAST_ROW_INCREMENT, EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateSlidingMoves(index, WEST_ROW_INCREMENT, WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    break;
  case Piece::King:
    generateCastlingMoves(sideToMove, moveList);
    generateJumpMoves(index, NORTH_ROW_INCREMENT, NORTH_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, SOUTH_ROW_INCREMENT, SOUTH_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, EAST_ROW_INCREMENT, EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, WEST_ROW_INCREMENT, WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, NORTH_WEST_ROW_INCREMENT, NORTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, NORTH_EAST_ROW_INCREMENT, NORTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, SOUTH_EAST_ROW_INCREMENT, SOUTH_EAST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    generateJumpMoves(index, SOUTH_WEST_ROW_INCREMENT, SOUTH_WEST_COLUMN_INCREMENT, piece, sideToMove, moveList);
    break;
  }
}

void Board8x8::generateMoves(
  std::uint8_t row,
  std::uint8_t col,
  MoveList & moveList) const
{
  uint8_t index    = getIndex(row, col);
  Color sideToMove = mBoardState.sideToMove;
  Piece piece      = mPieces[index];
  Color color      = mColors[index];
  if (piece == Piece::None || (color != sideToMove))
  {
    return;
  }

  return generateMoves(index, moveList);
}

void Board8x8::generatePawnMoves(
  std::uint8_t index,
  Color pieceColor,
  MoveList & moveList) const
{
  std::int8_t sourceRow        = getRow(index);
  std::int8_t sourceColumn     = getCol(index);
  std::uint8_t enPassantColumn = mBoardState.enpassantColumn;

  // Compute some values based on the piece color
  std::int8_t startRow       = 1;
  std::int8_t rowIncrement   = 1;
  std::int8_t indexIncrement = NORTH;
  std::int8_t enPassantRow   = 5;
  Color otherColor           = Color::Black;
  if (pieceColor == Color::Black)
  {
    startRow       = 6;
    rowIncrement   = -1;
    indexIncrement = SOUTH;
    enPassantRow   = 2;
    otherColor     = Color::Black;
  }

  std::uint8_t pushSquare       = index + indexIncrement;
  std::uint8_t doublePushSquare = index + indexIncrement + indexIncrement;

  // Generate non-capture moves
  if (mPieces[pushSquare] == Piece::None)
  {
    std::uint8_t destRow = getRow(pushSquare);
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

    if (sourceRow == startRow && mPieces[doublePushSquare] == Piece::None)
    {
      pushMove(index, doublePushSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::EnpassantPush, moveList);
    }
  }

  // Generate capture moves except en-passant captures
  constexpr std::int8_t columnIncrements[2] = {-1, +1};
  for (int i = 0; i < 2; i++)
  {
    std::int8_t destColumn = sourceColumn + columnIncrements[i];
    if (destColumn >= 0 && destColumn < 8)
    {
      std::int8_t destRow   = sourceRow + rowIncrement;
      std::int8_t destIndex = getIndex(destRow, destColumn);
      Piece capturePiece    = mPieces[destIndex];
      Color otherColor      = mColors[destIndex];
      if (capturePiece != Piece::None && isOppositeColor(pieceColor, otherColor))
      {
        if (pieceColor == Color::White && destRow == WHITE_PROMOTION_ROW)
        {
          pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
          pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
          pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
          pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
        }
        else if (pieceColor == Color::Black && destRow == BLACK_PROMOTION_ROW)
        {
          pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
          pushMove(index, destIndex, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
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

  // Generate en-passant captures moves
  if (enPassantColumn != INVALID_ENPASSANT_COLUMN)
  {
    if (abs(sourceColumn - enPassantColumn) == 1 && sourceRow == enPassantRow)
    {
      std::int8_t destRow      = sourceRow + rowIncrement;
      std::int8_t destColumn   = enPassantColumn;
      std::int8_t destIndex    = getIndex(destRow, destColumn);
      std::int8_t captureIndex = destIndex + indexIncrement;
      if (mPieces[captureIndex] == Piece::Pawn && mColors[captureIndex] == otherColor)
      {
        pushMove(index, destIndex, Piece::Pawn, Piece::Pawn, Piece::None, Move::Type::EnpassantCapture, moveList);
      }
    }

    // constexpr std::int8_t dirs[] = {EAST, WEST};
    // std::int8_t enPassantIndex   = getIndex(enPassantRow, enPassantColumn);

    // std::int8_t destRow = sourceRow + rowIncrement;
    // for (int8_t i = 0; i < 2; i++)
    // {
    //   std::int8_t destColumn = sourceColumn + dirs[i];

    //   if (destRow >= 0 && destRow <= 7 && destColumn >= 0 && destColumn <= 7)
    //   {
    //     std::int8_t captureIndex = getIndex(destRow, destColumn);
    //     if (enPassantIndex == captureIndex)
    //     {
    //       pushMove(
    //         row, col, destRow, destCol, Piece::Pawn, mPieces[destIndex - (8 * rowIncr)], Piece::Pawn,
    //         Move::Type::EpCapture, moveList);
    //     }
    //   }
    // }
  }
}

void Board8x8::generateSlidingMoves(
  std::uint8_t index,
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
    Piece otherPiece      = mPieces[destIndex];
    Color otherColor      = mColors[destIndex];

    // Check for friedly pieces
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

std::uint8_t Board8x8::getKingColumn(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
}

std::uint8_t Board8x8::getKingRow(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
}

PieceType Board8x8::getPieceType(
  std::uint8_t row,
  std::uint8_t col) const
{
  uint8_t index = getIndex(row, col);

  Piece piece = mPieces[index];
  Color color = mColors[index];

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
  std::uint8_t row,
  std::uint8_t col,
  Color attackingColor) const
{
  return isCellAttacked(getIndex(row, col), attackingColor);
}

bool Board8x8::isCellAttacked(
  std::uint8_t index,
  Color attackingColor) const
{
  static std::vector<std::int8_t> pawnRowIncrements;
  static std::vector<std::int8_t> pawnColumnIncrements;
  static std::vector<std::int8_t> knightRowIncrements;
  static std::vector<std::int8_t> knightColumnIncrements;
  static std::vector<std::int8_t> straightRowIncrements;
  static std::vector<std::int8_t> straightColumnIncrements;
  static std::vector<std::int8_t> diagonalRowIncrements;
  static std::vector<std::int8_t> diagonalColumnIncrements;

  // Initialize on first call
  if (pawnRowIncrements.size() == 0)
  {
    pawnRowIncrements.push_back(NORTH_EAST_ROW_INCREMENT);
    pawnRowIncrements.push_back(NORTH_WEST_ROW_INCREMENT);
    pawnColumnIncrements.push_back(NORTH_EAST_COLUMN_INCREMENT);
    pawnColumnIncrements.push_back(NORTH_WEST_COLUMN_INCREMENT);

    knightRowIncrements.push_back(NORTH_WEST_WEST_ROW_INCREMENT);
    knightRowIncrements.push_back(NORTH_NORTH_WEST_ROW_INCREMENT);
    knightRowIncrements.push_back(NORTH_NORTH_EAST_ROW_INCREMENT);
    knightRowIncrements.push_back(NORTH_EAST_EAST_ROW_INCREMENT);
    knightRowIncrements.push_back(SOUTH_EAST_EAST_ROW_INCREMENT);
    knightRowIncrements.push_back(SOUTH_SOUTH_EAST_ROW_INCREMENT);
    knightRowIncrements.push_back(SOUTH_SOUTH_WEST_ROW_INCREMENT);
    knightRowIncrements.push_back(SOUTH_WEST_WEST_ROW_INCREMENT);
    knightColumnIncrements.push_back(NORTH_WEST_WEST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(NORTH_NORTH_WEST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(NORTH_NORTH_EAST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(NORTH_EAST_EAST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(SOUTH_EAST_EAST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(SOUTH_SOUTH_EAST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(SOUTH_SOUTH_WEST_COLUMN_INCREMENT);
    knightColumnIncrements.push_back(SOUTH_WEST_WEST_COLUMN_INCREMENT);

    straightRowIncrements.push_back(NORTH_ROW_INCREMENT);
    straightRowIncrements.push_back(SOUTH_ROW_INCREMENT);
    straightRowIncrements.push_back(EAST_ROW_INCREMENT);
    straightRowIncrements.push_back(WEST_ROW_INCREMENT);
    straightColumnIncrements.push_back(NORTH_COLUMN_INCREMENT);
    straightColumnIncrements.push_back(SOUTH_COLUMN_INCREMENT);
    straightColumnIncrements.push_back(EAST_COLUMN_INCREMENT);
    straightColumnIncrements.push_back(WEST_COLUMN_INCREMENT);

    diagonalRowIncrements.push_back(NORTH_WEST_ROW_INCREMENT);
    diagonalRowIncrements.push_back(NORTH_EAST_ROW_INCREMENT);
    diagonalRowIncrements.push_back(SOUTH_EAST_ROW_INCREMENT);
    diagonalRowIncrements.push_back(SOUTH_WEST_ROW_INCREMENT);
    diagonalColumnIncrements.push_back(NORTH_WEST_COLUMN_INCREMENT);
    diagonalColumnIncrements.push_back(NORTH_EAST_COLUMN_INCREMENT);
    diagonalColumnIncrements.push_back(SOUTH_EAST_COLUMN_INCREMENT);
    diagonalColumnIncrements.push_back(SOUTH_WEST_COLUMN_INCREMENT);
  }

  // Check for pawn attacks
  for (std::int32_t i = 0; i < 2; i++)
  {
    std::int8_t rowIncrement    = pawnRowIncrements[i];
    std::int8_t columnIncrement = pawnColumnIncrements[i];

    // White attacks in the opposite row direction
    if (attackingColor == Color::White)
    {
      rowIncrement = -rowIncrement;
      // std::cout << rowIncrement << std::endl;
    }

    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::Pawn, attackingColor))
    {
      return true;
    }
  }

  // Check for knight attacks
  for (std::int32_t i = 0; i < 8; i++)
  {
    std::int8_t rowIncrement    = knightRowIncrements[i];
    std::int8_t columnIncrement = knightColumnIncrements[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::Knight, attackingColor))
    {
      return true;
    }
  }

  // Check for diagonal sliding attacks
  for (std::int32_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = diagonalRowIncrements[i];
    std::int8_t columnIncrement = diagonalColumnIncrements[i];

    if (checkSliderAttacks(index, rowIncrement, columnIncrement, Piece::Bishop, attackingColor))
    {
      return true;
    }

    if (checkSliderAttacks(index, rowIncrement, columnIncrement, Piece::Queen, attackingColor))
    {
      return true;
    }
  }

  // Check for straight sliding attacks
  for (std::int32_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = straightRowIncrements[i];
    std::int8_t columnIncrement = straightColumnIncrements[i];

    if (checkSliderAttacks(index, rowIncrement, columnIncrement, Piece::Rook, attackingColor))
    {
      return true;
    }

    if (checkSliderAttacks(index, rowIncrement, columnIncrement, Piece::Queen, attackingColor))
    {
      return true;
    }
  }

  // Check for king diagonal attacks
  for (std::int32_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = diagonalRowIncrements[i];
    std::int8_t columnIncrement = diagonalColumnIncrements[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::King, attackingColor))
    {
      return true;
    }
  }

  // Check for king straight attacks
  for (std::int32_t i = 0; i < 4; i++)
  {
    std::int8_t rowIncrement    = straightRowIncrements[i];
    std::int8_t columnIncrement = straightColumnIncrements[i];
    if (checkJumpAttacks(index, rowIncrement, columnIncrement, Piece::King, attackingColor))
    {
      return true;
    }
  }

  return false;
}

void Board8x8::makeMove(
  const Move & move)
{
  // Push a copy of the current board state to the stack
  mStateStack.push(mBoardState);

  Color sideToMove         = mBoardState.sideToMove;
  Square sourceSquare      = move.getSourceSquare();
  Square destSquare        = move.getDestinationSquare();
  Piece movedPiece         = move.getPiece();
  std::uint8_t sourceIndex = getIndex(sourceSquare.row, sourceSquare.col);
  std::uint8_t destIndex   = getIndex(destSquare.row, destSquare.col);

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;

  // Handle quiet moves
  if (move.isQuiet())
  {
    mPieces[sourceIndex] = Piece::None;
    mColors[sourceIndex] = Color::None;
    mPieces[destIndex]   = movedPiece;
    mColors[destIndex]   = sideToMove;
  }

  // Handle double pawn pushes
  if (move.isEnpassantPush())
  {
    mBoardState.enpassantColumn = sourceSquare.col;
    mPieces[sourceIndex]        = Piece::None;
    mColors[sourceIndex]        = Color::None;
    mPieces[destIndex]          = movedPiece;
    mColors[destIndex]          = sideToMove;
  }

  // Handle promotion moves
  if (move.isPromotion())
  {
    Piece promotedPiece = move.getPromotedPiece();

    mPieces[sourceIndex] = Piece::None;
    mColors[sourceIndex] = Color::None;
    mPieces[destIndex]   = promotedPiece;
    mColors[destIndex]   = sideToMove;
  }

  // Handle castle moves
  if (move.isCastle())
  {
    switch (destIndex)
    {
    case C1:
      mPieces[A1] = Piece::None;
      mColors[A1] = Color::None;
      mPieces[C1] = Piece::King;
      mColors[C1] = Color::White;
      mPieces[D1] = Piece::Rook;
      mColors[D1] = Color::White;
      mPieces[E1] = Piece::None;
      mColors[E1] = Color::None;
      break;
    case G1:
      mPieces[E1] = Piece::None;
      mColors[E1] = Color::None;
      mPieces[F1] = Piece::Rook;
      mColors[F1] = Color::White;
      mPieces[G1] = Piece::King;
      mColors[G1] = Color::White;
      mPieces[H1] = Piece::None;
      mColors[H1] = Color::None;
      break;
    case C8:
      mPieces[A8] = Piece::None;
      mColors[A8] = Color::None;
      mPieces[C8] = Piece::King;
      mColors[C8] = Color::Black;
      mPieces[D8] = Piece::Rook;
      mColors[D8] = Color::Black;
      mPieces[E8] = Piece::None;
      mColors[E8] = Color::None;
      break;
    case G8:
      mPieces[E8] = Piece::None;
      mColors[E8] = Color::None;
      mPieces[F8] = Piece::Rook;
      mColors[F8] = Color::Black;
      mPieces[G8] = Piece::King;
      mColors[G8] = Color::Black;
      mPieces[H8] = Piece::None;
      mColors[H8] = Color::None;
      break;
    }
  }

  // Handle king moves
  if (movedPiece == Piece::King)
  {
    mPieces[sourceIndex] = Piece::None;
    mColors[sourceIndex] = Color::None;
    mPieces[destIndex]   = Piece::King;
    mColors[destIndex]   = sideToMove;

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
    std::uint8_t enPassantIndex = getIndex(sourceSquare.row, mBoardState.enpassantColumn);
    mPieces[sourceIndex]        = Piece::None;
    mColors[sourceIndex]        = Color::None;
    mPieces[enPassantIndex]     = Piece::None;
    mColors[enPassantIndex]     = Color::None;
    mPieces[destIndex]          = movedPiece;
    mColors[destIndex]          = sideToMove;
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
  uint8_t fromSquare,
  uint8_t toSquare,
  Piece piece,
  Piece capturePiece,
  Piece promotionPiece,
  Move::Type type,
  MoveList & moveList) const
{
  uint8_t sourceRow = getRow(fromSquare);
  uint8_t sourceCol = getCol(fromSquare);
  uint8_t destRow   = getRow(toSquare);
  uint8_t destCol   = getCol(toSquare);

  Square sourceSquare = {sourceRow, sourceCol};
  Square destSquare   = {destRow, destCol};
  std::unique_ptr<Move> move(new Move(sourceSquare, destSquare, piece, type, capturePiece, promotionPiece));
  moveList.addMove(std::move(move));
}

void Board8x8::reset()
{
  static const Piece pieces[] = {Piece::Rook, Piece::Knight, Piece::Bishop, Piece::Queen,
                                 Piece::King, Piece::Bishop, Piece::Knight, Piece::Rook};

  for (std::uint8_t i = 0; i < 64; i++)
  {
    mPieces[i] = Piece::None;
    mColors[i] = Color::None;
  }

  for (uint8_t col = 0; col < 8; col++)
  {
    mPieces[getIndex(1, col)] = Piece::Pawn;
    mColors[getIndex(1, col)] = Color::White;

    mPieces[getIndex(0, col)] = pieces[col];
    mColors[getIndex(0, col)] = Color::White;

    mPieces[getIndex(6, col)] = Piece::Pawn;
    mColors[getIndex(6, col)] = Color::Black;

    mPieces[getIndex(7, col)] = pieces[col];
    mColors[getIndex(7, col)] = Color::Black;
  }

  mWhiteKingSquare = {0, 4};
  mBlackKingSquare = {7, 4};

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;
  mBoardState.sideToMove      = Color::White;
  mBoardState.castlingRights  = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN | CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN;
  mBoardState.fullMoveClock   = 0;
  mBoardState.halfMoveClock   = 1;
}

void Board8x8::setPosition(
  const std::string & fenString)
{
  reset();

  Fen fenData;
  fenData.setFromString(fenString);

  mBoardState = fenData.getBoardState();

  for (std::uint8_t i = 0; i < 8; i++)
  {
    for (std::uint8_t j = 0; j < 8; j++)
    {
      std::uint8_t index = getIndex(i, j);
      mPieces[index]     = Piece::None;
      mColors[index]     = Color::None;

      PieceType pieceType = fenData.getPieceType(i, j);
      switch (pieceType)
      {
      case PieceType::WhitePawn:
        mPieces[index] = Piece::Pawn;
        mColors[index] = Color::White;
        break;
      case PieceType::WhiteRook:
        mPieces[index] = Piece::Rook;
        mColors[index] = Color::White;
        break;
      case PieceType::WhiteKnight:
        mPieces[index] = Piece::Knight;
        mColors[index] = Color::White;
        break;
      case PieceType::WhiteBishop:
        mPieces[index] = Piece::Bishop;
        mColors[index] = Color::White;
        break;
      case PieceType::WhiteQueen:
        mPieces[index] = Piece::Queen;
        mColors[index] = Color::White;
        break;
      case PieceType::WhiteKing:
        mPieces[index]   = Piece::King;
        mColors[index]   = Color::White;
        mWhiteKingSquare = Square{i, j};
        break;
      case PieceType::BlackPawn:
        mPieces[index] = Piece::Pawn;
        mColors[index] = Color::Black;
        break;
      case PieceType::BlackRook:
        mPieces[index] = Piece::Rook;
        mColors[index] = Color::Black;
        break;
      case PieceType::BlackKnight:
        mPieces[index] = Piece::Knight;
        mColors[index] = Color::Black;
        break;
      case PieceType::BlackBishop:
        mPieces[index] = Piece::Bishop;
        mColors[index] = Color::Black;
        break;
      case PieceType::BlackQueen:
        mPieces[index] = Piece::Queen;
        mColors[index] = Color::Black;
        break;
      case PieceType::BlackKing:
        mPieces[index]   = Piece::King;
        mColors[index]   = Color::Black;
        mBlackKingSquare = Square{i, j};
        break;
      default:
        mPieces[index] = Piece::None;
        mColors[index] = Color::None;
        break;
      }
    }
  }
}

void Board8x8::unmakeMove(
  const Move & move)
{
  // Set the previous board state
  mBoardState = mStateStack.top();
  mStateStack.pop();

  Color sideThatMoved      = mBoardState.sideToMove;
  Color otherSide          = (sideThatMoved == Color::White) ? Color::Black : Color::White;
  Square sourceSquare      = move.getSourceSquare();
  Square destSquare        = move.getDestinationSquare();
  Piece movedPiece         = move.getPiece();
  std::uint8_t sourceIndex = getIndex(sourceSquare.row, sourceSquare.col);
  std::uint8_t destIndex   = getIndex(destSquare.row, destSquare.col);

  // Handle quiet moves
  if (move.isQuiet())
  {
    mPieces[sourceIndex] = movedPiece;
    mColors[sourceIndex] = sideThatMoved;
    mPieces[destIndex]   = Piece::None;
    mColors[destIndex]   = Color::None;
  }

  // Handle double pawn pushes
  if (move.isEnpassantPush())
  {
    mPieces[sourceIndex] = movedPiece;
    mColors[sourceIndex] = sideThatMoved;
    mPieces[destIndex]   = Piece::None;
    mColors[destIndex]   = Color::None;
  }

  // Handle capture moves
  if (move.isCapture())
  {
    Piece capturePiece        = move.getCapturedPiece();
    std::uint8_t captureIndex = destIndex;
    if (move.isEnpassantCapture())
    {
      captureIndex = getIndex(sourceSquare.row, mBoardState.enpassantColumn);
    }

    mPieces[sourceIndex]  = movedPiece;
    mColors[sourceIndex]  = sideThatMoved;
    mPieces[captureIndex] = capturePiece;
    mColors[captureIndex] = otherSide;
  }

  // Handle promotion moves
  if (move.isPromotion())
  {
    mPieces[sourceIndex] = Piece::Pawn;
    mColors[sourceIndex] = sideThatMoved;
    mPieces[destIndex]   = Piece::None;
    mColors[destIndex]   = Color::None;

    if (move.isPromotionCapture())
    {
      Piece capturePiece = move.getCapturedPiece();
      mPieces[destIndex] = capturePiece;
      mColors[destIndex] = otherSide;
    }
  }

  // Update king moves
  if (movedPiece == Piece::King)
  {
    mPieces[sourceIndex] = movedPiece;
    mColors[sourceIndex] = sideThatMoved;
    mPieces[destIndex]   = Piece::None;
    mColors[destIndex]   = Color::None;

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
      mPieces[A1] = Piece::Rook;
      mColors[A1] = Color::White;
      mPieces[C1] = Piece::None;
      mColors[C1] = Color::None;
      mPieces[D1] = Piece::None;
      mColors[D1] = Color::None;
      mPieces[E1] = Piece::King;
      mColors[E1] = Color::White;
      break;
    case G1:
      mPieces[E1] = Piece::King;
      mColors[E1] = Color::White;
      mPieces[F1] = Piece::None;
      mColors[F1] = Color::None;
      mPieces[G1] = Piece::None;
      mColors[G1] = Color::None;
      mPieces[H1] = Piece::Rook;
      mColors[H1] = Color::White;
      break;
    case C8:
      mPieces[A8] = Piece::Rook;
      mColors[A8] = Color::Black;
      mPieces[C8] = Piece::None;
      mColors[C8] = Color::None;
      mPieces[D8] = Piece::None;
      mColors[D8] = Color::None;
      mPieces[E8] = Piece::King;
      mColors[E8] = Color::Black;
      break;
    case G8:
      mPieces[E8] = Piece::King;
      mColors[E8] = Color::Black;
      mPieces[F8] = Piece::None;
      mColors[F8] = Color::None;
      mPieces[G8] = Piece::None;
      mColors[G8] = Color::None;
      mPieces[H8] = Piece::Rook;
      mColors[H8] = Color::Black;
      break;
    }
  }
}

} // namespace jmchess
