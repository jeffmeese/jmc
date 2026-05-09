#include "move.h"

#include <sstream>

#include "consts.h"

namespace jmchess
{

Move::Move(uint8_t sourceRow,
           uint8_t sourceColumn,
           uint8_t destinationRow,
           uint8_t destinationColumn,
           Piece piece)
{
  mSourceColumn = sourceColumn;
  mSourceRow = sourceRow;
  mDestinationColumn = destinationColumn;
  mDestinationRow = destinationRow;
  mPiece = piece;
  mCapturedPiece = Piece::None;
  mPromotedPiece = Piece::None;
  mHalfMoveClock = 0;
  mFullMoveCounter = 0;
  mEnpassantColumn = INVALID_EP_COLUMN;
  mCastlingRights = CASTLE_NONE;
}

std::string Move::toSmithNotation() const
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::stringstream oss;
  oss << colLetter[mSourceColumn] << (mSourceRow+1) << colLetter[mDestinationColumn] << (mDestinationRow+1);
  return oss.str();
}

}
