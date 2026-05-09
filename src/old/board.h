#ifndef BOARD_H
#define BOARD_H

#include <cstdint>

#include "movelist.h"
#include "types.h"

namespace jmchess
{

class Board
{
public:
  Board();

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

  void reset();

  virtual bool generateMoves(MoveList & moveList) const = 0;
  virtual bool generateMoves(uint8_t row, uint8_t col, MoveList & moveList) const = 0;
  virtual PieceType getPieceType(uint8_t row, uint8_t col) const = 0;
  virtual bool isCellAttacked(uint8_t row, uint8_t col) const = 0;
  virtual bool makeMove(const Move * move) = 0;
  virtual bool unmakeMove(const Move * move) = 0;

protected:
  virtual void doReset() = 0;

private:

  void init();

  uint8_t mCastlingRights;
  uint8_t mEnpassantColumn;
  uint16_t mFullMoveCounter;
  uint16_t mHalfMoveClock;
  Color mSideToMove;
};

inline uint8_t Board::getCastlingRights() const
{
  return mCastlingRights;
}

inline uint8_t Board::getEnpassantColumn() const
{
  return mEnpassantColumn;
}

inline uint16_t Board::getFullMoveCounter() const
{
  return mFullMoveCounter;
}

inline uint16_t Board::getHalfMoveClock() const
{
  return mHalfMoveClock;
}

inline Color Board::getSideToMove() const
{
  return mSideToMove;
}

inline void Board::setCastlingRights(uint8_t value)
{
  mCastlingRights = value;
}

inline void Board::setEnpassantColumn(uint8_t value)
{
  mEnpassantColumn = value;
}

inline void Board::setFullMoveCounter(uint16_t value)
{
  mFullMoveCounter = value;
}

inline void Board::setHalfMoveClock(uint16_t value)
{
  mHalfMoveClock = value;
}

inline void Board::setSideToMove(Color color)
{
  mSideToMove = color;
}

}

#endif // #ifndef BOARD_H
