#pragma once

#include <cstdint>
#include <string>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "piece_type.h"

namespace jmchess
{

class Fen
{
public:
  Fen();

  BoardState getBoardState() const;

  PieceType getPieceType(
    std::uint8_t row,
    std::uint8_t col) const;

  void setPieceType(
    std::uint8_t row,
    std::uint8_t col,
    PieceType pieceType);

  void setBoardState(const BoardState & boardState);

  bool setFromBoard(const Board * board);

  bool setFromString(const std::string & fenString);

  std::string toString() const;

private:
  void init();
  
  BoardState mBoardState;
  PieceType mPieceTypes[8][8];
};

} // namespace jmchess
