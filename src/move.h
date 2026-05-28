#pragma once

#include <cstdint>
#include <string>

#include "board_state.h"
#include "piece.h"
#include "square.h"

namespace jmchess
{

class Move
{
public:
  enum class Type
  {
    Quiet            = 0,
    Capture          = 1,
    EnpassantCapture = 2,
    Castle           = 3,
    EnpassantPush    = 4,
    Promotion        = 5,
    PromotionCapture = 6,
    Null             = 7
  };

  Move(
    const Square & source,
    const Square & destination,
    std::int8_t sourceIndex,
    std::int8_t destIndex,
    Piece piece,
    BoardState boardState,
    Type type           = Type::Quiet,
    Piece capturePiece  = Piece::None,
    Piece promotedPiece = Piece::None);

  BoardState getBoardState() const;
  Piece getCapturedPiece() const;
  std::int8_t getDestinationIndex() const;
  Square getDestinationSquare() const;
  Piece getPiece() const;
  Piece getPromotedPiece() const;
  std::int8_t getSourceIndex() const;
  Square getSourceSquare() const;
  Type getType() const;

  bool isCapture() const;
  bool isCastle() const;
  bool isEnpassantCapture() const;
  bool isEnpassantPush() const;
  bool isPromotion() const;
  bool isPromotionCapture() const;
  bool isQuiet() const;
  bool isStandardCapture() const;
  std::string toSmithNotation() const;

  friend bool operator==(
    const Move & move1,
    const Move & move2);

private:
  Square mSource                = {0, 0};
  Square mDestination           = {0, 0};
  std::int8_t mSourceIndex      = 0;
  std::int8_t mDestinationIndex = 0;
  Piece mPiece                  = Piece::None;
  Piece mCapturedPiece          = Piece::None;
  Piece mPromotedPiece          = Piece::None;
  Type mType                    = Type::Quiet;
  BoardState mBoardState;
};

} // namespace jmchess
