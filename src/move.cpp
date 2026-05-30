#include "move.h"

#include <sstream>
#include <string>

#include "board.h"
#include "piece.h"
#include "square.h"

namespace jmchess
{

Move::Move(
  std::int8_t sourceIndex,
  std::int8_t destinationIndex,
  std::uint8_t flags,
  BoardState boardState,
  Piece capturePiece)
{
  mSourceIndex      = sourceIndex;
  mDestinationIndex = destinationIndex;
  mBoardState       = boardState;
  mCapturedPiece    = capturePiece;
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

std::int8_t Move::getDestinationColumn() const
{
  return (mDestinationIndex & 7);
}

std::int8_t Move::getDestinationIndex() const
{
  return mDestinationIndex;
}

std::int8_t Move::getDestinationRow() const
{
  return (mDestinationIndex >> 3);
}

std::int8_t Move::getIndex(
  std::int8_t row,
  std::int8_t col)
{
  return (row << 3) | col;
}

Piece Move::getPromotedPiece() const
{
  if (mFlags == MOVE_QUEEN_PROMOTION || mFlags == MOVE_QUEEN_PROMOTION_CAPTURE)
  {
    return Piece::Queen;
  }

  if (mFlags == MOVE_ROOK_PROMOTION || mFlags == MOVE_ROOK_PROMOTION_CAPTURE)
  {
    return Piece::Rook;
  }

  if (mFlags == MOVE_BISHOP_PROMOTION || mFlags == MOVE_BISHOP_PROMOTION_CAPTURE)
  {
    return Piece::Bishop;
  }

  if (mFlags == MOVE_KNIGHT_PROMOTION || mFlags == MOVE_KNIGHT_PROMOTION_CAPTURE)
  {
    return Piece::Knight;
  }

  return Piece::None;
}

std::int8_t Move::getSourceColumn() const
{
  return (mSourceIndex & 7);
}

std::int8_t Move::getSourceIndex() const
{
  return mSourceIndex;
}

std::int8_t Move::getSourceRow() const
{
  return (mSourceIndex >> 3);
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

bool Move::isQuiet() const
{
  return (mFlags == MOVE_QUIET);
}

std::string Move::toAlgebraicNotation(
  Board * board) const
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::int32_t sourceRow = static_cast<std::int32_t>(Board::getRow(mSourceIndex));
  std::int8_t sourceCol = Board::getCol(mSourceIndex);
  std::int32_t destRow = static_cast<std::int32_t>(Board::getRow(mDestinationIndex));
  std::int8_t destCol = Board::getCol(mDestinationIndex);

  PieceType pieceType = board->getPieceType(sourceRow, sourceCol);

  std::ostringstream oss;

  if (isCastle())
  {
    if (mFlags == MOVE_QUEEN_CASTLE)
    {
      oss << "O-O-O";
    }
    else if (mFlags == MOVE_KING_CASTLE)
    {
      oss << "O-O";
    }
  } 
  else
  {
    bool addDestCol = false;
    bool addSourceCol = true;

    if (pieceType == PieceType::WhiteKnight || pieceType == PieceType::BlackKnight)
    {
      oss << "N";
      addDestCol = true;
      addSourceCol = false;
    }

    if (pieceType == PieceType::WhiteBishop || pieceType == PieceType::BlackBishop)
    {
      oss << "B";
      addDestCol = true;
      addSourceCol = false;
    }

    if (pieceType == PieceType::WhiteRook || pieceType == PieceType::BlackRook)
    {
      oss << "R";
      addDestCol = true;
      addSourceCol = false;
    }

    if (pieceType == PieceType::WhiteQueen || pieceType == PieceType::BlackQueen)
    {
      oss << "Q";
      addDestCol = true;
      addSourceCol = false;
    }

    if (pieceType == PieceType::WhiteKing || pieceType == PieceType::BlackKing)
    {
      oss << "K";
      addDestCol = true;
      addSourceCol = false;
    }

    if (addSourceCol)
    {
      oss << colLetter[sourceCol];
    }

    if (isCapture())
    {
      oss << "x";
      addDestCol = true;
    }
 
    if (addDestCol)
    {
      oss << colLetter[destCol];
    }

    oss << destRow + 1;

    if (isPromotion())
    {
      oss << "=";
      if (mFlags == MOVE_BISHOP_PROMOTION || mFlags == MOVE_BISHOP_PROMOTION_CAPTURE)
      {
        oss << "B";
      }

      if (mFlags == MOVE_KNIGHT_PROMOTION || mFlags == MOVE_KNIGHT_PROMOTION_CAPTURE)
      {
        oss << "N";
      }

      if (mFlags == MOVE_ROOK_PROMOTION || mFlags == MOVE_ROOK_PROMOTION_CAPTURE)
      {
        oss << "R";
      }

      if (mFlags == MOVE_QUEEN_PROMOTION || mFlags == MOVE_QUEEN_PROMOTION_CAPTURE)
      {
        oss << "Q";
      }
    }
  }

  return oss.str();
}

std::string Move::toSmithNotation() const
{
  static const char colLetter[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
  std::ostringstream oss;
  std::int8_t sourceRow = getSourceRow();
  std::int8_t sourceCol = getSourceColumn();
  std::int8_t destRow   = getDestinationRow();
  std::int8_t destCol   = getDestinationColumn();
  oss << colLetter[sourceCol] << (sourceRow + 1) << colLetter[destCol] << (destRow + 1);

  if (isEnpassantCapture())
  {
    oss << "E";
  }

  if (mCapturedPiece != Piece::None && mFlags != MOVE_ENPASSANT_CAPTURE)
  {
    std::string pieceString;
    switch (mCapturedPiece)
    {
    case Piece::Pawn:
      pieceString = "p";
      break;
    case Piece::Rook:
      pieceString = "r";
      break;
    case Piece::Bishop:
      pieceString = "b";
      break;
    case Piece::Knight:
      pieceString = "k";
      break;
    case Piece::Queen:
      pieceString = "q";
      break;
    default:
      break;
    }

    oss << pieceString;
  }

  if (isCastle())
  {
    std::string castleString = (mFlags == MOVE_QUEEN_CASTLE) ? "C" : "c";
    oss << castleString;
  }

  if (mFlags == MOVE_KNIGHT_PROMOTION || mFlags == MOVE_KNIGHT_PROMOTION_CAPTURE)
  {
    oss << "k";
  }

  if (mFlags == MOVE_BISHOP_PROMOTION || mFlags == MOVE_BISHOP_PROMOTION_CAPTURE)
  {
    oss << "b";
  }

  if (mFlags == MOVE_ROOK_PROMOTION || mFlags == MOVE_ROOK_PROMOTION_CAPTURE)
  {
    oss << "r";
  }

  if (mFlags == MOVE_QUEEN_PROMOTION || mFlags == MOVE_QUEEN_PROMOTION_CAPTURE)
  {
    oss << "q";
  }

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

  return true;
}

} // namespace jmchess
