#pragma once

#include "color.h"

namespace jmchess
{

enum class PieceType
{
  None = 0,
  WhitePawn,
  WhiteKnight,
  WhiteBishop,
  WhiteRook,
  WhiteQueen,
  WhiteKing,
  BlackPawn,
  BlackKnight,
  BlackBishop,
  BlackRook,
  BlackQueen,
  BlackKing
};

inline bool isBlack(PieceType pieceType)
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

inline bool isNone(PieceType pieceType)
{
  return pieceType == PieceType::None;
}

inline bool isWhite(PieceType pieceType)
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

inline bool isColor(PieceType pieceType, Color color)
{
  return (color == Color::White) ? isWhite(pieceType) : isBlack(pieceType);
}

}