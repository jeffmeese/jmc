#pragma once

#include <cstdint>
#include <string>

#include "board_state.h"
#include "piece.h"
#include "square.h"

namespace jmchess
{

constexpr std::uint8_t MOVE_QUIET                    = 0x00;
constexpr std::uint8_t MOVE_DOUBLE_PAWN_PUSH         = 0x01;
constexpr std::uint8_t MOVE_KING_CASTLE              = 0x02;
constexpr std::uint8_t MOVE_QUEEN_CASTLE             = 0x03;
constexpr std::uint8_t MOVE_CAPTURE                  = 0x04;
constexpr std::uint8_t MOVE_ENPASSANT_CAPTURE        = 0x05;
constexpr std::uint8_t MOVE_KNIGHT_PROMOTION         = 0x08;
constexpr std::uint8_t MOVE_BISHOP_PROMOTION         = 0x09;
constexpr std::uint8_t MOVE_ROOK_PROMOTION           = 0x0A;
constexpr std::uint8_t MOVE_QUEEN_PROMOTION          = 0x0B;
constexpr std::uint8_t MOVE_KNIGHT_PROMOTION_CAPTURE = 0x0C;
constexpr std::uint8_t MOVE_BISHOP_PROMOTION_CAPTURE = 0x0D;
constexpr std::uint8_t MOVE_ROOK_PROMOTION_CAPTURE   = 0x0E;
constexpr std::uint8_t MOVE_QUEEN_PROMOTION_CAPTURE  = 0x0F;

class Move
{
public:
  Move(
    std::int8_t sourceIndex,
    std::int8_t destIndex,
    BoardState boardState,
    std::uint8_t flags,
    Piece capturePiece  = Piece::None);

  BoardState getBoardState() const;
  Piece getCapturedPiece() const;
  std::int8_t getDestinationColumn() const;
  std::int8_t getDestinationIndex() const;
  std::int8_t getDestinationRow() const;
  Piece getPromotedPiece() const;
  std::int8_t getSourceIndex() const;
  std::int8_t getSourceRow() const;
  std::int8_t getSourceColumn() const;

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
  std::int8_t mSourceIndex      = 0;
  std::int8_t mDestinationIndex = 0;
  std::uint8_t mFlags           = 0;
  Piece mCapturedPiece          = Piece::None;
  //Piece mPromotedPiece          = Piece::None;
  BoardState mBoardState;
};

} // namespace jmchess
