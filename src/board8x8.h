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

  struct Attacks
  {
    std::int8_t diagonalAttacks[4][8];
    std::int8_t straightAttacks[4][8];
    std::int8_t knightAttacks[8];
    std::int8_t kingAttacks[8];
    std::int8_t whitePawnAttacks[2];
    std::int8_t blackPawnAttacks[2];
  };

  bool checkJumpAttacks(
    std::int8_t index,
    const std::int8_t * attacks,
    std::int8_t attacksSize,
    Piece piece,
    Color attackColor) const;

  bool checkSliderAttacks(
    std::int8_t index,
    const std::int8_t * attacks,
    std::int8_t attacksSize,
    Piece piece1,
    Piece piece2,
    Color attackColor) const;

  void generateCastlingMoves(
    Color sideToMove,
    MoveList & moveList) const;

  void generateJumpMoves(
    std::int8_t index,
    const std::int8_t * attacks,
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
    const std::int8_t * attacks,
    Piece piece,
    Color sideToMove,
    MoveList & moveList) const;

  bool isCellAttacked(
    std::int8_t index,
    Color attackingColor) const;

  void initAttacks();

  void pushMove(
    std::int8_t fromIndex,
    std::int8_t toIndex,
    Piece piece,
    Piece capturePiece,
    Piece promotionPiece,
    Move::Type type,
    MoveList & moveList) const;

  static constexpr std::int8_t PAWN_ROW_INCREMENTS[2]        = {+1, +1};
  static constexpr std::int8_t PAWN_COLUMN_INCREMENTS[2]     = {+1, -1};
  static constexpr std::int8_t KNIGHT_ROW_INCREMENTS[8]      = {+1, +2, +2, +1, -1, -2, -2, -1};
  static constexpr std::int8_t KNIGHT_COLUMN_INCREMENTS[8]   = {+2, +1, -1, -2, -2, -1, +1, +2};
  static constexpr std::int8_t KING_ROW_INCREMENTS[8]        = {+1, -1, +0, +0, +1, +1, -1, -1};
  static constexpr std::int8_t KING_COLUMN_INCREMENTS[8]     = {+0, +0, +1, -1, +1, -1, -1, +1};
  static constexpr std::int8_t STRAIGHT_ROW_INCREMENTS[4]    = {+1, -1, +0, +0};
  static constexpr std::int8_t STRAIGHT_COLUMN_INCREMENTS[4] = {+0, +0, +1, -1};
  static constexpr std::int8_t DIAGONAL_ROW_INCREMENTS[4]    = {+1, +1, -1, -1};
  static constexpr std::int8_t DIAGONAL_COLUMN_INCREMENTS[4] = {+1, -1, +1, -1};

  BoardState mBoardState;
  Square mBlackKingSquare;
  Square mWhiteKingSquare;
  Cell mCells[64];
  Attacks mAttacks[64];
};

} // namespace jmchess
