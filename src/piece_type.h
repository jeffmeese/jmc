#pragma once

#include "color.h"

namespace jmchess
{

enum class PieceType
{
  WhitePawn   = 0,
  WhiteKnight = 1,
  WhiteBishop = 2,
  WhiteRook   = 3,
  WhiteQueen  = 4,
  WhiteKing   = 5,
  BlackPawn   = 6,
  BlackKnight = 7,
  BlackBishop = 8,
  BlackRook   = 9,
  BlackQueen  = 10,
  BlackKing   = 11,
  None        = 12
};

inline bool isBlack(
  PieceType pieceType)
{
  if (pieceType == PieceType::BlackPawn)
  {
    return true;
  }

  if (pieceType == PieceType::BlackRook)
  {
    return true;
  }

  if (pieceType == PieceType::BlackBishop)
  {
    return true;
  }

  if (pieceType == PieceType::BlackKnight)
  {
    return true;
  }

  if (pieceType == PieceType::BlackQueen)
  {
    return true;
  }

  if (pieceType == PieceType::BlackKing)
  {
    return true;
  }

  return false;
}

inline bool isNone(
  PieceType pieceType)
{
  return pieceType == PieceType::None;
}

inline bool isWhite(
  PieceType pieceType)
{
  if (pieceType == PieceType::WhitePawn)
  {
    return true;
  }

  if (pieceType == PieceType::WhiteRook)
  {
    return true;
  }

  if (pieceType == PieceType::WhiteBishop)
  {
    return true;
  }

  if (pieceType == PieceType::WhiteKnight)
  {
    return true;
  }

  if (pieceType == PieceType::WhiteQueen)
  {
    return true;
  }

  if (pieceType == PieceType::WhiteKing)
  {
    return true;
  }

  return false;
}

inline bool isColor(
  PieceType pieceType,
  Color color)
{
  return (color == Color::White) ? isWhite(pieceType) : isBlack(pieceType);
}

} // namespace jmchess
