#include "move.h"

#include <sstream>
#include <string>

#include "piece.h"
#include "square.h"

namespace jmchess
{

Move::Move(
  std::int8_t sourceIndex,
  std::int8_t destinationIndex,
  BoardState boardState,
  std::uint8_t flags,
  Piece capturePiece,
  Piece promotedPiece)
{
  mSourceIndex      = sourceIndex;
  mDestinationIndex = destinationIndex;
  mBoardState       = boardState;
  mCapturedPiece    = capturePiece;
  mPromotedPiece    = promotedPiece;
  mFlags            = flags;
}

BoardState Move::getBoardState() const
{
  return mBoardState;
}

Piece Move::getCapturedPiece() const
{
  return mCapturedPiece;
}

std::int8_t Move::getDestinationIndex() const
{
  return mDestinationIndex;
}

Piece Move::getPromotedPiece() const
{
  return mPromotedPiece;
}

std::int8_t Move::getSourceIndex() const
{
  return mSourceIndex;
}

bool Move::isCapture() const
{
  return (mFlags & 0x04);
}

bool Move::isCastle() const
{
  return (mFlags == MOVE_KING_CASTLE || mFlags == MOVE_QUEEN_CASTLE);
}

bool Move::isEnpassantCapture() const
{
  return (mFlags == MOVE_ENPASSANT_CAPTURE);
}

bool Move::isEnpassantPush() const
{
  return (mFlags == MOVE_DOUBLE_PAWN_PUSH);
}

bool Move::isPromotion() const
{
  return (mFlags & 0x08);
}

bool Move::isPromotionCapture() const
{
  return (mFlags & 0x0C);
}

bool Move::isQuiet() const
{
  return (mFlags == MOVE_QUIET);
}

bool Move::isStandardCapture() const
{
  return (mFlags == MOVE_CAPTURE);
}

std::string Move::toSmithNotation() const
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::ostringstream oss;
  // oss << colLetter[mSource.col] << (mSource.row + 1) << colLetter[mDestination.col] << (mDestination.row + 1);
  return oss.str();
}

bool operator==(
  const Move & move1,
  const Move & move2)
{
  if (move1.mFlags != move2.mFlags)
  {
    return false;
  }

  if (move1.mSourceIndex != move2.mSourceIndex)
  {
    return false;
  }

  if (move1.mDestinationIndex != move2.mDestinationIndex)
  {
    return false;
  }

  if (move1.mCapturedPiece != move2.mCapturedPiece)
  {
    return false;
  }

  if (move1.mPromotedPiece != move2.mPromotedPiece)
  {
    return false;
  }

  return true;
}

} // namespace jmchess
