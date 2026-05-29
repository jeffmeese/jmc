#pragma once

#include <cstdint>

#include "board_state.h"
#include "color.h"
#include "move.h"
#include "move_list.h"
#include "piece_type.h"

namespace jmchess
{

constexpr std::int8_t CASTLE_NONE              = 0x00;
constexpr std::int8_t CASTLE_WHITE_KING        = 0x01;
constexpr std::int8_t CASTLE_WHITE_QUEEN       = 0x02;
constexpr std::int8_t CASTLE_BLACK_KING        = 0x04;
constexpr std::int8_t CASTLE_BLACK_QUEEN       = 0x08;
constexpr std::int8_t WHITE_PROMOTION_ROW      = 7;
constexpr std::int8_t BLACK_PROMOTION_ROW      = 0;
constexpr std::int8_t INVALID_ENPASSANT_COLUMN = 8;
constexpr std::int8_t WHITE_ENPASSANT_ROW      = 4;
constexpr std::int8_t BLACK_ENPASSANT_ROW      = 3;

class Board
{
public:
  virtual void generateMoves(MoveList & moveList) const = 0;

  virtual void generateMoves(
    std::int8_t row,
    std::int8_t col,
    MoveList & moveList) const = 0;

  virtual BoardState getBoardState() const = 0;

  virtual std::int8_t getKingColumn(Color color) const = 0;

  virtual std::int8_t getKingRow(Color color) const = 0;

  static std::int8_t getIndex(
    std::int8_t row,
    std::int8_t col);

  static std::int8_t getRow(std::int8_t index);

  static std::int8_t getCol(std::int8_t index);

  virtual PieceType getPieceType(
    std::int8_t row,
    std::int8_t col) const = 0;

  virtual bool isCellAttacked(
    std::int8_t row,
    std::int8_t col,
    Color attackingColor) const = 0;

  virtual bool isKingInCheck(Color color) const = 0;

  virtual bool makeMove(const Move & move) = 0;

  virtual void reset() = 0;

  virtual void setPosition(const std::string & fenString) = 0;

  virtual void unmakeMove(const Move & move) = 0;
};

} // namespace jmchess
