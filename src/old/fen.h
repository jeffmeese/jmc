#ifndef FEN_H
#define FEN_H

#include <cstdint>

#include "board.h"
#include "types.h"

namespace jmchess
{

class Fen
{
public:
  Fen();

  uint8_t getCastlingRights() const;
  uint8_t getEnpassantColumn() const;
  uint16_t getFullMoveCounter() const;
  uint16_t getHalfMoveClock() const;
  Color getSideToMove() const;
  void setCastlingRights(uint8_t value);
  void setEnpassantColumn(uint8_t value);
  void setFullMoveCounter(uint16_t value);
  void setHalfMoveClock(uint16_t value);
  void setSideToMove(Color color);

  PieceType getPieceType(uint8_t row, uint8_t col) const;
  void setPieceType(uint8_t row, uint8_t col, PieceType pieceType);
  bool setFromBoard(const Board * board);
  bool setFromString(const std::string & fenString);
  std::string toString() const;

private:
  void init();

  uint8_t mCastlingRights;
  uint8_t mEnPassantColumn;
  uint16_t mFullMoveCounter;
  uint16_t mHalfMoveClock;
  PieceType mPieceTypes[8][8];
  Color mSideToMove;
};

}
#endif // #ifndef FEN_H
