#pragma once

#include <cstdint>
#include <string>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "piece.h"
#include "piece_type.h"
#include "square.h"

namespace jmchess
{

class Board8x8 : public Board
{
public:
  Board8x8();

  void generateMoves(MoveList & moveList) const override;

  void generateMoves(
    std::int8_t row,
    std::int8_t col,
    MoveList & moveList) const override;

  BoardState getBoardState() const override;

  std::int8_t getKingColumn(Color color) const;

  std::int8_t getKingRow(Color color) const;

  PieceType getPieceType(
    std::int8_t row,
    std::int8_t col) const override;

  bool isCellAttacked(
    std::int8_t row,
    std::int8_t col,
    Color attackingColor) const override;

  bool isKingInCheck(Color color) const override;

  void makeMove(const Move & move) override;

  void reset() override;

  void setPosition(const std::string & fenString) override;

  void unmakeMove(const Move & move) override;

private:
  struct Cell
  {
    Piece piece;
    Color color;
  };

  bool checkJumpAttacks(
    std::int8_t index,
    std::int8_t rowIncrement,
    std::int8_t columnIncrement,
    Piece piece,
    Color attackColor) const;

  bool checkSliderAttacks(
    std::int8_t index,
    std::int8_t rowIncrement,
    std::int8_t columnIncrement,
    Piece piece1,
    Piece piece2,
    Color attackColor) const;

  void generateCastlingMoves(
    Color sideToMove,
    MoveList & moveList) const;

  void generateJumpMoves(
    std::int8_t index,
    std::int8_t rowIncrement,
    std::int8_t columnIncrement,
    Piece piece,
    Color sideToMove,
    MoveList & moveList) const;

  void generateMoves(
    std::int8_t index,
    MoveList & moveList) const;

  void generatePawnMoves(
    std::int8_t index,
    Color sideToMove,
    MoveList & moveList) const;

  void generateSlidingMoves(
    std::int8_t index,
    std::int8_t rowIncrement,
    std::int8_t columnIncrement,
    Piece piece,
    Color sideToMove,
    MoveList & moveList) const;

  bool isCellAttacked(
    std::int8_t index,
    Color attackingColor) const;

  void pushMove(
    std::int8_t fromIndex,
    std::int8_t toIndex,
    Piece piece,
    Piece capturePiece,
    Piece promotionPiece,
    Move::Type type,
    MoveList & moveList) const;

  static constexpr std::int8_t PAWN_ROW_INCREMENTS[2]        = {1, 1};
  static constexpr std::int8_t PAWN_COLUMN_INCREMENTS[2]     = {1, -1};
  static constexpr std::int8_t KNIGHT_ROW_INCREMENTS[8]      = {1, 2, 2, 1, -1, -2, -2, -1};
  static constexpr std::int8_t KNIGHT_COLUMN_INCREMENTS[8]   = {-2, -1, 1, 2, 2, 1, -1, -2};
  static constexpr std::int8_t STRAIGHT_ROW_INCREMENTS[4]    = {1, -1, 0, 0};
  static constexpr std::int8_t STRAIGHT_COLUMN_INCREMENTS[4] = {0, 0, 1, -1};
  static constexpr std::int8_t DIAGONAL_ROW_INCREMENTS[4]    = {1, 1, -1, -1};
  static constexpr std::int8_t DIAGONAL_COLUMN_INCREMENTS[4] = {-1, 1, 1, -1};

  BoardState mBoardState;
  Cell mCells[64];
  Square mBlackKingSquare;
  Square mWhiteKingSquare;
};

} // namespace jmchess
