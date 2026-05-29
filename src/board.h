#pragma once

#include <cstdint>

#include "board_state.h"
#include "color.h"
#include "move.h"
#include "move_list.h"
#include "piece_type.h"

namespace jmchess
{

// Constants for board state
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

// Constant for board squares
constexpr std::int8_t INVALID_INDEX = -1;
constexpr std::int8_t A1            = 0;
constexpr std::int8_t B1            = 1;
constexpr std::int8_t C1            = 2;
constexpr std::int8_t D1            = 3;
constexpr std::int8_t E1            = 4;
constexpr std::int8_t F1            = 5;
constexpr std::int8_t G1            = 6;
constexpr std::int8_t H1            = 7;
constexpr std::int8_t A2            = 8;
constexpr std::int8_t B2            = 9;
constexpr std::int8_t C2            = 10;
constexpr std::int8_t D2            = 11;
constexpr std::int8_t E2            = 12;
constexpr std::int8_t F2            = 13;
constexpr std::int8_t G2            = 14;
constexpr std::int8_t H2            = 15;
constexpr std::int8_t A3            = 16;
constexpr std::int8_t B3            = 17;
constexpr std::int8_t C3            = 18;
constexpr std::int8_t D3            = 19;
constexpr std::int8_t E3            = 20;
constexpr std::int8_t F3            = 21;
constexpr std::int8_t G3            = 22;
constexpr std::int8_t H3            = 23;
constexpr std::int8_t A4            = 24;
constexpr std::int8_t B4            = 25;
constexpr std::int8_t C4            = 26;
constexpr std::int8_t D4            = 27;
constexpr std::int8_t E4            = 28;
constexpr std::int8_t F4            = 29;
constexpr std::int8_t G4            = 30;
constexpr std::int8_t H4            = 31;
constexpr std::int8_t A5            = 32;
constexpr std::int8_t B5            = 33;
constexpr std::int8_t C5            = 34;
constexpr std::int8_t D5            = 35;
constexpr std::int8_t E5            = 36;
constexpr std::int8_t F5            = 37;
constexpr std::int8_t G5            = 38;
constexpr std::int8_t H5            = 39;
constexpr std::int8_t A6            = 40;
constexpr std::int8_t B6            = 41;
constexpr std::int8_t C6            = 42;
constexpr std::int8_t D6            = 43;
constexpr std::int8_t E6            = 44;
constexpr std::int8_t F6            = 45;
constexpr std::int8_t G6            = 46;
constexpr std::int8_t H6            = 47;
constexpr std::int8_t A7            = 48;
constexpr std::int8_t B7            = 49;
constexpr std::int8_t C7            = 50;
constexpr std::int8_t D7            = 51;
constexpr std::int8_t E7            = 52;
constexpr std::int8_t F7            = 53;
constexpr std::int8_t G7            = 54;
constexpr std::int8_t H7            = 55;
constexpr std::int8_t A8            = 56;
constexpr std::int8_t B8            = 57;
constexpr std::int8_t C8            = 58;
constexpr std::int8_t D8            = 59;
constexpr std::int8_t E8            = 60;
constexpr std::int8_t F8            = 61;
constexpr std::int8_t G8            = 62;
constexpr std::int8_t H8            = 63;

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
