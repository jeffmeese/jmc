#pragma once

#include <cstdint>
#include <string>

#include "board_state.h"
#include "piece.h"
#include "square.h"

namespace jmchess
{

static constexpr std::uint32_t MOVE_QUIET                    = 0x00;
static constexpr std::uint32_t MOVE_DOUBLE_PAWN_PUSH         = 0x01;
static constexpr std::uint32_t MOVE_KING_CASTLE              = 0x02;
static constexpr std::uint32_t MOVE_QUEEN_CASTLE             = 0x03;
static constexpr std::uint32_t MOVE_CAPTURE                  = 0x04;
static constexpr std::uint32_t MOVE_ENPASSANT_CAPTURE        = 0x05;
static constexpr std::uint32_t MOVE_KNIGHT_PROMOTION         = 0x08;
static constexpr std::uint32_t MOVE_BISHOP_PROMOTION         = 0x09;
static constexpr std::uint32_t MOVE_ROOK_PROMOTION           = 0x0A;
static constexpr std::uint32_t MOVE_QUEEN_PROMOTION          = 0x0B;
static constexpr std::uint32_t MOVE_KNIGHT_PROMOTION_CAPTURE = 0x0C;
static constexpr std::uint32_t MOVE_BISHOP_PROMOTION_CAPTURE = 0x0D;
static constexpr std::uint32_t MOVE_ROOK_PROMOTION_CAPTURE   = 0x0E;
static constexpr std::uint32_t MOVE_QUEEN_PROMOTION_CAPTURE  = 0x0F;

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
    std::int8_t sourceIndex,
    std::int8_t destIndex,
    // const Square & source,
    // const Square & destination,
    std::uint32_t flags,
    BoardState boardState,
    Type type           = Type::Quiet,
    Piece capturePiece  = Piece::None,
    Piece promotedPiece = Piece::None);

  BoardState getBoardState() const;
  Piece getCapturedPiece() const;
  std::int8_t getDestinationIndex() const;
  // Square getDestinationSquare() const;
  Piece getPromotedPiece() const;
  std::int8_t getSourceIndex() const;
  // Square getSourceSquare() const;
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
  // Square mSource       = {0, 0};
  // Square mDestination  = {0, 0};
  std::int8_t mSourceIndex = -1;
  std::int8_t mDestIndex   = -1;
  std::uint32_t mFlags     = 0;
  Piece mCapturedPiece     = Piece::None;
  Piece mPromotedPiece     = Piece::None;
  Type mType               = Type::Quiet;
  BoardState mBoardState;
};

} // namespace jmchess
