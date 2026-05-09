#ifndef TYPES_H
#define TYPES_H

#include <cstdint>

namespace jmchess
{

enum class Color : uint8_t
{
  None = 0,
  White,
  Black
};

enum class Piece : uint8_t
{
  None = 0,
  Pawn,
  Knight,
  Bishop,
  Rook,
  Queen,
  King
};

enum class PieceType : uint8_t
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

}

#endif // #ifndef TYPES_H
