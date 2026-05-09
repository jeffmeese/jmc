#include "board.h"

#include "consts.h"

namespace jmchess
{

Board::Board()
{
  init();
}

void Board::init()
{
  mCastlingRights = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN | CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN;
  mEnpassantColumn = INVALID_EP_COLUMN;
  mFullMoveCounter = 1;
  mHalfMoveClock = 0;
  mSideToMove = Color::White;
}

void Board::reset()
{
  init();
  doReset();
}

}
