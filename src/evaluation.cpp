#include "evaluation.h"

static constexpr int PAWN_WEIGHT   = 100;
static constexpr int KNIGHT_WEIGHT = 300;
static constexpr int BISHOP_WEIGHT = 350;
static constexpr int ROOK_WEIGHT   = 500;
static constexpr int QUEEN_WEIGHT  = 950;
static constexpr int KING_WEIGHT   = 60000;

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

constexpr std::int8_t flip(
  std::int8_t index)
{
  return (index ^ 56);
}

// clang-format off
static const double MiddleGamePawnSquareValue[64] = 
{ 
  0,  0,  0,  0,  0,  0,  0,  0,
 50, 50, 50, 50, 50, 50, 50, 50,
 10, 10, 20, 30, 30, 20, 10, 10,
  5,  5, 10, 25, 25, 10,  5,  5,
  0,  0,  0, 20, 20,  0,  0,  0,
  5, -5,-10,  0,  0,-10, -5,  5,
  5, 10, 10,-20,-20, 10, 10,  5,
  0,  0,  0,  0,  0,  0,  0,  0
};

static const double MiddleGameKnightSquareValue[64] = 
{ 
  -50,-40,-30,-30,-30,-30,-40,-50,
  -40,-20,  0,  0,  0,  0,-20,-40,
  -30,  0, 10, 15, 15, 10,  0,-30,
  -30,  5, 15, 20, 20, 15,  5,-30,
  -30,  0, 15, 20, 20, 15,  0,-30,
  -30,  5, 10, 15, 15, 10,  5,-30,
  -40,-20,  0,  5,  5,  0,-20,-40,
  -50,-40,-30,-30,-30,-30,-40,-50,
};

static const double MiddleGameBishopSquareValue[64] = 
{ 
  -20,-10,-10,-10,-10,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5, 10, 10,  5,  0,-10,
  -10,  5,  5, 10, 10,  5,  5,-10,
  -10,  0, 10, 10, 10, 10,  0,-10,
  -10, 10, 10, 10, 10, 10, 10,-10,
  -10,  5,  0,  0,  0,  0,  5,-10,
  -20,-10,-10,-10,-10,-10,-10,-20,
};

static const double MiddleGameRookSquareValue[64] = 
{ 
  0,  0,  0,  0,  0,  0,  0,  0,
  5, 10, 10, 10, 10, 10, 10,  5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
 -5,  0,  0,  0,  0,  0,  0, -5,
  0,  0,  0,  5,  5,  0,  0,  0
};

static const double MiddleGameQueenSquareValue[64] = 
{ 
  -20,-10,-10, -5, -5,-10,-10,-20,
  -10,  0,  0,  0,  0,  0,  0,-10,
  -10,  0,  5,  5,  5,  5,  0,-10,
   -5,  0,  5,  5,  5,  5,  0, -5,
    0,  0,  5,  5,  5,  5,  0, -5,
  -10,  5,  5,  5,  5,  5,  0,-10,
  -10,  0,  5,  0,  0,  0,  0,-10,
  -20,-10,-10, -5, -5,-10,-10,-20
};

static const double MiddleGameKingSquareValue[64] = 
{ 
  -50,-40,-30,-20,-20,-30,-40,-50,
  -30,-20,-10,  0,  0,-10,-20,-30,
  -30,-10, 20, 30, 30, 20,-10,-30,
  -30,-10, 30, 40, 40, 30,-10,-30,
  -30,-10, 30, 40, 40, 30,-10,-30,
  -30,-10, 20, 30, 30, 20,-10,-30,
  -30,-30,  0,  0,  0,  0,-30,-30,
  -50,-30,-30,-30,-30,-30,-30,-50
};

// clang-format on

namespace jmchess
{

double Evaluation::evaluate(
  const Board * board)
{
  double pieceWeight = 0.0;
  double mobilityWeight = 0.0;

  const double * pawnSquareTable = MiddleGamePawnSquareValue;
  const double * knightSquareTable = MiddleGameKnightSquareValue;
  const double * bishopSquareTable = MiddleGameBishopSquareValue;
  const double * rookSquareTable = MiddleGameRookSquareValue;
  const double * queenSquareTable = MiddleGameQueenSquareValue;
  const double * kingSquareTable = MiddleGameKingSquareValue;

  for (std::int8_t i = 0; i < 8; i++)
  {
    for (std::int8_t j = 0; j < 8; j++)
    {
      std::int8_t index = getIndex(i, j);
      PieceType pieceType = board->getPieceType(i, j);
      switch (pieceType)
      {
      case PieceType::WhitePawn:
        pieceWeight += PAWN_WEIGHT;
        mobilityWeight += pawnSquareTable[index];
        break;
      case PieceType::BlackPawn:
        pieceWeight -= PAWN_WEIGHT;
        mobilityWeight -= pawnSquareTable[flip(index)];
        break;
      case PieceType::WhiteKnight:
        pieceWeight += KNIGHT_WEIGHT;
        mobilityWeight += knightSquareTable[index];
        break;
      case PieceType::BlackKnight:
        pieceWeight -= KNIGHT_WEIGHT;
        mobilityWeight -= knightSquareTable[flip(index)];
        break;
      case PieceType::WhiteBishop:
        pieceWeight += BISHOP_WEIGHT;
        mobilityWeight += bishopSquareTable[index];
        break;
      case PieceType::BlackBishop:
        pieceWeight -= BISHOP_WEIGHT;
        mobilityWeight -= bishopSquareTable[flip(index)];
        break;
      case PieceType::WhiteRook:
        pieceWeight += ROOK_WEIGHT;
        mobilityWeight += rookSquareTable[index];
        break;
      case PieceType::BlackRook:
        pieceWeight -= ROOK_WEIGHT;
        mobilityWeight -= rookSquareTable[flip(index)];
        break;
      case PieceType::WhiteQueen:
        pieceWeight += QUEEN_WEIGHT;
        mobilityWeight += queenSquareTable[index];
        break;
      case PieceType::BlackQueen:
        pieceWeight -= QUEEN_WEIGHT;
        mobilityWeight -= queenSquareTable[flip(index)];
        break;
      case PieceType::WhiteKing:
        pieceWeight += KING_WEIGHT;
        mobilityWeight += kingSquareTable[index];
        break;
      case PieceType::BlackKing:
        pieceWeight -= KING_WEIGHT;
        mobilityWeight -= kingSquareTable[flip(index)];
        break;
      default:
        break;
      }
    }
  }

  double score = pieceWeight;
  score += mobilityWeight * 0.01;

  return (board->getBoardState().sideToMove == Color::White) ? score : -score;
}

} // namespace jmchess
