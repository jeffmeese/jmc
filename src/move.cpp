#include "move.h"

#include <sstream>
#include <string>

#include "piece.h"
#include "square.h"

namespace jmchess
{

Move::Move(
  const Square & source,
  const Square & desination,
  std::int8_t sourceIndex,
  std::int8_t destinationIndex,
  Piece piece,
  BoardState boardState,
  Type type,
  Piece capturePiece,
  Piece promotedPiece)
{
  mSource           = source;
  mDestination      = desination;
  mSourceIndex      = sourceIndex;
  mDestinationIndex = destinationIndex;
  mPiece            = piece;
  mBoardState       = boardState;
  mCapturedPiece    = capturePiece;
  mPromotedPiece    = promotedPiece;
  mType             = type;
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

Square Move::getDestinationSquare() const
{
  return mDestination;
}

Piece Move::getPiece() const
{
  return mPiece;
}

Piece Move::getPromotedPiece() const
{
  return mPromotedPiece;
}

std::int8_t Move::getSourceIndex() const
{
  return mSourceIndex;
}

Square Move::getSourceSquare() const
{
  return mSource;
}

Move::Type Move::getType() const
{
  return mType;
}

bool Move::isCapture() const
{
  return (mType == Type::Capture || mType == Type::PromotionCapture || mType == Type::EnpassantCapture);
}

bool Move::isCastle() const
{
  return mType == Type::Castle;
}

bool Move::isEnpassantCapture() const
{
  return mType == Type::EnpassantCapture;
}

bool Move::isEnpassantPush() const
{
  return mType == Type::EnpassantPush;
}

bool Move::isPromotion() const
{
  return (mType == Type::Promotion || mType == Type::PromotionCapture);
}

bool Move::isPromotionCapture() const
{
  return mType == Type::PromotionCapture;
}

bool Move::isQuiet() const
{
  return mType == Type::Quiet;
}

bool Move::isStandardCapture() const
{
  return (mType == Type::Capture && mType != Type::PromotionCapture && mType != Type::EnpassantCapture);
}

std::string Move::toSmithNotation() const
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::ostringstream oss;
  oss << colLetter[mSource.col] << (mSource.row + 1) << colLetter[mDestination.col] << (mDestination.row + 1);
  return oss.str();
}

bool operator==(
  const Move & move1,
  const Move & move2)
{
  if (move1.mType != move2.mType)
  {
    return false;
  }

  if (move1.mSource.row != move2.mSource.row)
  {
    return false;
  }

  if (move1.mSource.col != move2.mSource.col)
  {
    return false;
  }

  if (move1.mDestination.row != move2.mDestination.row)
  {
    return false;
  }

  if (move1.mDestination.col != move2.mDestination.col)
  {
    return false;
  }

  if (move1.mPiece != move2.mPiece)
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
