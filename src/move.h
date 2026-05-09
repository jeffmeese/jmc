#pragma once

#include <cstdint>
#include <string>

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
    Piece piece,
    Type type = Type::Quiet,
    Piece capturePiece = Piece::None,
    Piece promotedPiece = Piece::None
  );

  Piece getCapturedPiece() const;
  Square getDestinationSquare() const;
  Piece getPiece() const;
  Piece getPromotedPiece() const;
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
  
  friend bool operator==(const Move & move1, const Move & move2);

private:
  Square mSource       = {0, 0};
  Square mDestination  = {0, 0};
  Piece mPiece         = Piece::None;
  Piece mCapturedPiece = Piece::None;
  Piece mPromotedPiece = Piece::None;
  Type mType           = Type::Quiet;
};

} // namespace jmchess

// #ifndef MOVE_H
// #define MOVE_H

// #include <cstdint>
// #include <string>

// #include "types.h"

// namespace jmchess
// {

// class Move
// {
// public:
//   enum class Type : uint8_t
//   {
//     Quiet = 0,
//     Capture = 1,
//     EpCapture = 2,
//     Castle = 3,
//     EpPush = 4,
//     Promotion = 5,
//     PromotionCapture = 6,
//     Null = 7
//   };

//   Move(uint8_t sourceRow,
//        uint8_t sourceColumn,
//        uint8_t destinationRow,
//        uint8_t destinationColumn,
//        Piece piece);

//   uint8_t getCastlingRights() const;
//   Piece getCapturedPiece() const;
//   uint8_t getDestinationColumn() const;
//   uint8_t getDestinationRow() const;
//   uint8_t getEnPassantColumn() const;
//   uint8_t getFullMoveCounter() const;
//   uint8_t getHalfMoveClock() const;
//   Piece getPiece() const;
//   Piece getPromotedPiece() const;
//   uint8_t getSourceColumn() const;
//   uint8_t getSourceRow() const;
//   Type getType() const;
//   bool isCapture() const;
//   bool isCastle() const;
//   bool isEnPassantCapture() const;
//   bool isEnPassantPush() const;
//   bool isPromotion() const;
//   bool isPromotionCapture() const;
//   void setCapturedPiece(Piece piece);
//   void setCastlingRights(uint8_t value);
//   void setEnpassantColumn(uint8_t value);
//   void setHalfMoveClock(uint8_t value);
//   void setFullMoveCounter(uint8_t value);
//   void setPromotedPiece(Piece piece);
//   void setType(Type type);
//   std::string toSmithNotation() const;

// private:
//   uint8_t mCastlingRights;
//   uint8_t mDestinationColumn;
//   uint8_t mDestinationRow;
//   uint8_t mEnpassantColumn;
//   uint8_t mFullMoveCounter;
//   uint8_t mHalfMoveClock;
//   uint8_t mSourceColumn;
//   uint8_t mSourceRow;
//   Piece mPiece;
//   Piece mCapturedPiece;
//   Piece mPromotedPiece;
//   Type mType;
// };

// inline uint8_t Move::getCastlingRights() const
// {
//   return mCastlingRights;
// }

// inline uint8_t Move::getDestinationColumn() const
// {
//   return mDestinationColumn;
// }

// inline uint8_t Move::getDestinationRow() const
// {
//   return mDestinationRow;
// }

// inline uint8_t Move::getEnPassantColumn() const
// {
//   return mEnpassantColumn;
// }

// inline uint8_t Move::getFullMoveCounter() const
// {
//   return mFullMoveCounter;
// }

// inline uint8_t Move::getHalfMoveClock() const
// {
//   return mHalfMoveClock;
// }

// inline uint8_t Move::getSourceColumn() const
// {
//   return mSourceColumn;
// }

// inline uint8_t Move::getSourceRow() const
// {
//   return mSourceRow;
// }

// inline Piece Move::getCapturedPiece() const
// {
//   return mCapturedPiece;
// }

// inline Piece Move::getPiece() const
// {
//   return mPiece;
// }

// inline Piece Move::getPromotedPiece() const
// {
//   return mPromotedPiece;
// }

// inline bool Move::isCapture() const
// {
//   return (mType == Type::Capture || mType == Type::PromotionCapture || mType
//   == Type::EpCapture);
// }

// inline bool Move::isCastle() const
// {
//   return mType == Type::Castle;
// }

// inline bool Move::isEnPassantCapture() const
// {
//   return mType == Type::EpCapture;
// }

// inline bool Move::isEnPassantPush() const
// {
//   return mType == Type::EpPush;
// }

// inline bool Move::isPromotion() const
// {
//   return (mType == Type::Promotion || mType == Type::PromotionCapture);
// }

// inline bool Move::isPromotionCapture() const
// {
//   return mType == Type::PromotionCapture;
// }

// inline void Move::setCapturedPiece(Piece piece)
// {
//   mCapturedPiece = piece;
// }

// inline void Move::setCastlingRights(uint8_t value)
// {
//   mCastlingRights = value;
// }

// inline void Move::setEnpassantColumn(uint8_t value)
// {
//   mEnpassantColumn = value;
// }

// inline void Move::setHalfMoveClock(uint8_t value)
// {
//   mHalfMoveClock = value;
// }

// inline void Move::setFullMoveCounter(uint8_t value)
// {
//   mFullMoveCounter = value;
// }

// inline void Move::setPromotedPiece(Piece piece)
// {
//   mPromotedPiece = piece;
// }

// inline void Move::setType(Type type)
// {
//   mType = type;
// }

// }

// #endif // #ifndef MOVE_H
