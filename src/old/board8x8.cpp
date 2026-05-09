#include "board8x8.h"

#include "consts.h"

namespace jmchess
{

#define getIndex(row,col) (((row)<<3)+(col))
#define getRow(index) ( (index >> 3) )
#define getCol(index) ( (index % 8) )

constexpr int8_t NORTH =  8;
constexpr int8_t SOUTH = -8;
constexpr int8_t EAST = 1;
constexpr int8_t WEST = -1;

enum Square
{
  A1=0  , B1, C1, D1, E1, F1, G1, H1,
  A2=8  , B2, C2, D2, E2, F2, G2, H2,
  A3=16 , B3, C3, D3, E3, F3, G3, H3,
  A4=24 , B4, C4, D4, E4, F4, G4, H4,
  A5=32 , B5, C5, D5, E5, F5, G5, H5,
  A6=40 , B6, C6, D6, E6, F6, G6, H6,
  A7=48 , B7, C7, D7, E7, F7, G7, H7,
  A8=56 , B8, C8, D8, E8, F8, G8, H8
};

Board8x8::Board8x8()
{
  init();
}

void Board8x8::doReset()
{
  init();
}

bool Board8x8::generateMoves(MoveList & moveList) const
{
  for (uint8_t index = 0; index < 64; index++)
  {
    if (!generateMoves(index, moveList))
    {
      return false;
    }
  }
  return true;
}

bool Board8x8::generateMoves(uint8_t row, uint8_t col, MoveList & moveList) const
{
  uint8_t index = getIndex(row, col);
  Color sideToMove = this->getSideToMove();
  Piece piece = mPieces[index];
  Color color = mColors[index];
  if (piece == Piece::None || (color != sideToMove))
  {
    return true;
  }

  return generateMoves(index, moveList);
}

bool Board8x8::generateMoves(uint8_t index, MoveList & moveList) const
{
  Color sideToMove = this->getSideToMove();
  Piece piece = mPieces[index];
  Color color = mColors[index];

  if (piece == Piece::None || (color != sideToMove))
  {
    return true;
  }

  switch (piece)
  {
  case Piece::Pawn:
    generatePawnMoves(index, sideToMove, moveList);
    break;
  }

  return true;
}

void Board8x8::generatePawnMoves(uint8_t index, Color sideToMove, MoveList & moveList) const
{
  Color otherSide = (sideToMove == Color::White) ? Color::Black : Color::White;
  int8_t dir = (sideToMove == Color::White) ? NORTH : SOUTH;
  uint8_t startRow = (sideToMove == Color::White) ? 1 : 6;

  uint8_t pushSquare = index + dir;
  uint8_t doublePushSquare = index + dir + dir;
  uint8_t sourceRow = getRow(index);

  // Generate non-capture moves
  if (mPieces[pushSquare] == Piece::None)
  {
    uint8_t destRow = getRow(pushSquare);
    if (destRow == WHITE_PROMOTION_ROW || destRow == BLACK_PROMOTION_ROW)
    {
      pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Bishop, Move::Type::Promotion, moveList);
      pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Knight, Move::Type::Promotion, moveList);
      pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Rook, Move::Type::Promotion, moveList);
      pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::Queen, Move::Type::Promotion, moveList);
    }
    else
    {
      pushMove(index, pushSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::Quiet, moveList);
    }

    if (sourceRow == startRow && mPieces[doublePushSquare] == Piece::None)
    {
      pushMove(index, doublePushSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::EpPush, moveList);
    }
  }

  // Generate capture moves except en-passant captures
  uint8_t captureEast = index + dir + EAST;
  uint8_t captureWest = index + dir + WEST;
  uint8_t squares[] = {captureEast, captureWest};
  for (int i = 0; i < 2; i++)
  {
    uint8_t sq = squares[i];
    if (mColors[sq] == otherSide)
    {
      uint8_t destRow = getRow(sq);
      Piece capturePiece = mPieces[sq];
      if (sideToMove == Color::White && destRow == WHITE_PROMOTION_ROW)
      {
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
      }
      else if (sideToMove == Color::Black && destRow == BLACK_PROMOTION_ROW)
      {
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
      }
      else
      {
        pushMove(index, sq, Piece::Pawn, capturePiece, Piece::None, Move::Type::Capture, moveList);
      }
    }
  }

  // Generate en-passant captures moves
  if (getEnpassantColumn() != INVALID_EP_COLUMN)
  {
    int8_t captureEast = index + EAST;
    int8_t captureWest = index + WEST;
    int8_t cols[] = {captureEast, captureWest};

    if (sideToMove == Color::White && sourceRow == 4)
    {

    }
    else if (sideToMove == Color::Black && sourceRow == 3)
    {

    }
  }
}

PieceType Board8x8::getPieceType(uint8_t row, uint8_t col) const
{
  uint8_t index = getIndex(row, col);

  Piece piece = mPieces[index];
  Color color = mColors[index];

  PieceType pieceType = PieceType::None;
  if (piece != Piece::None)
  {
    if (color == Color::White)
    {
      switch (piece)
      {
      case Piece::Pawn:
        pieceType = PieceType::WhitePawn;
        break;
      case Piece::Rook:
        pieceType = PieceType::WhiteRook;
        break;
      case Piece::Knight:
        pieceType = PieceType::WhiteKnight;
        break;
      case Piece::Bishop:
        pieceType = PieceType::WhiteBishop;
        break;
      case Piece::Queen:
        pieceType = PieceType::WhiteQueen;
        break;
      case Piece::King:
        pieceType = PieceType::WhiteKing;
        break;
      default:
        break;
      }
    }
    else
    {
      switch (piece)
      {
      case Piece::Pawn:
        pieceType = PieceType::BlackPawn;
        break;
      case Piece::Rook:
        pieceType = PieceType::BlackRook;
        break;
      case Piece::Knight:
        pieceType = PieceType::BlackKnight;
        break;
      case Piece::Bishop:
        pieceType = PieceType::BlackBishop;
        break;
      case Piece::Queen:
        pieceType = PieceType::BlackQueen;
        break;
      case Piece::King:
        pieceType = PieceType::BlackKing;
        break;
      default:
        break;
      }
    }
  }

  return pieceType;
}

void Board8x8::init()
{
  static const Piece pieces[] = { Piece::Rook, Piece::Knight, Piece::Bishop, Piece::Queen,
                                  Piece::King, Piece::Bishop, Piece::Knight, Piece::Rook };

  for (uint8_t sq = 0; sq < 64; sq++)
  {
    mPieces[sq] = Piece::None;
    mColors[sq] = Color::None;
  }

  for (uint8_t col = 0; col < 8; col++)
  {
    mPieces[getIndex(1, col)] = Piece::Pawn;
    mColors[getIndex(1, col)] = Color::White;

    mPieces[getIndex(0, col)] = pieces[col];
    mColors[getIndex(0, col)] = Color::White;

    mPieces[getIndex(6, col)] = Piece::Pawn;
    mColors[getIndex(6, col)] = Color::Black;

    mPieces[getIndex(7, col)] = pieces[col];
    mColors[getIndex(7, col)] = Color::Black;
  }

  mKingCol[Color::White] = 4;
  mKingRow[Color::White] = 0;
  mKingCol[Color::Black] = 4;
  mKingRow[Color::Black] = 7;
}

bool Board8x8::isCellAttacked(uint8_t row, uint8_t col) const
{
  return false;
}

bool Board8x8::makeMove(const Move * move)
{
  Color side = this->getSideToMove();
  Color otherSide = (side == Color::White) ? Color::Black : Color::White;

  uint8_t fromSquare = getIndex(move->getSourceRow(), move->getSourceColumn());
  uint8_t toSquare = getIndex(move->getDestinationRow(), move->getDestinationColumn());

  mColors[fromSquare] = Color::None;
  mPieces[fromSquare] = Piece::None;
  mColors[toSquare] = side;
  mPieces[toSquare] = move->getPiece();

  if (move->isEnPassantCapture())
  {
    char dir = (side == Color::White) ? SOUTH : NORTH;
    uint8_t sq = toSquare + dir;
    mPieces[sq] = Piece::None;
    mColors[sq] = Color::None;
  }

  if (move->isPromotion() || move->isPromotionCapture())
  {
    mPieces[toSquare] = move->getPromotedPiece();
  }

  if (move->getPiece() == Piece::King)
  {
    mKingCol[side] = move->getDestinationColumn();
    mKingRow[side] = move->getDestinationRow();
  }

  setEnpassantColumn(INVALID_EP_COLUMN);
  if (move->isEnPassantPush())
  {
    setEnpassantColumn(move->getDestinationColumn());
  }

  uint8_t castlingRights = this->getCastlingRights();
  switch (fromSquare)
  {
  case H1:
    castlingRights &= ~CASTLE_WHITE_KING;
    break;
  case E1:
    castlingRights &= ~(CASTLE_WHITE_KING|CASTLE_WHITE_QUEEN);
    break;
  case A1:
    castlingRights &= ~CASTLE_WHITE_QUEEN;
    break;
  case H8:
    castlingRights &= ~CASTLE_BLACK_KING;
    break;
  case E8:
    castlingRights &= ~(CASTLE_BLACK_KING|CASTLE_BLACK_QUEEN);
    break;
  case A8:
    castlingRights &= ~CASTLE_BLACK_QUEEN;
    break;
  }

  switch (toSquare)
  {
  case H1:
    castlingRights &= ~CASTLE_WHITE_KING;
    break;
  case E1:
    castlingRights &= ~(CASTLE_WHITE_KING|CASTLE_WHITE_QUEEN);
    break;
  case A1:
    castlingRights &= ~CASTLE_WHITE_QUEEN;
    break;
  case H8:
    castlingRights &= ~CASTLE_BLACK_KING;
    break;
  case E8:
    castlingRights &= ~(CASTLE_BLACK_KING|CASTLE_BLACK_QUEEN);
    break;
  case A8:
    castlingRights &= ~CASTLE_BLACK_QUEEN;
    break;
  }

  if (move->isCastle())
  {
    switch (toSquare)
    {
    case C1:
      mPieces[D1] = Piece::Rook;
      mColors[D1] = Color::White;
      mPieces[A1] = Piece::None;
      mColors[A1] = Color::None;
      break;
    case G1:
      mPieces[F1] = Piece::Rook;
      mColors[F1] = Color::White;
      mPieces[H1] = Piece::None;
      mColors[H1] = Color::None;
      break;
    case C8:
      mPieces[D8] = Piece::Rook;
      mColors[D8] = Color::Black;
      mPieces[A8] = Piece::None;
      mColors[A8] = Color::None;
      break;
    case G8:
      mPieces[F8] = Piece::Rook;
      mColors[F8] = Color::Black;
      mPieces[H8] = Piece::None;
      mColors[H8] = Color::None;
      break;
    }
  }

  setCastlingRights(castlingRights);

  setHalfMoveClock(getHalfMoveClock()+1);
  if (move->getPiece() == Piece::Pawn || move->isCapture())
    setHalfMoveClock(0);

  setSideToMove(otherSide);
  return true;
}

void Board8x8::pushMove(uint8_t fromSquare, uint8_t toSquare, Piece piece, Piece capturePiece, Piece promotionPiece, Move::Type type, MoveList & moveList) const
{
  uint8_t sourceRow = getRow(fromSquare);
  uint8_t sourceCol = getCol(fromSquare);
  uint8_t destRow = getRow(toSquare);
  uint8_t destCol = getCol(toSquare);

  std::unique_ptr<Move> move(new Move(sourceRow, sourceCol, destRow, destCol, piece));
  move->setCapturedPiece(capturePiece);
  move->setPromotedPiece(promotionPiece);
  move->setType(type);
  move->setCastlingRights(this->getCastlingRights());
  move->setHalfMoveClock(this->getHalfMoveClock());
  move->setFullMoveCounter(this->getFullMoveCounter());
  move->setEnpassantColumn(this->getEnpassantColumn());

  moveList.addMove(std::move(move));
}

bool Board8x8::unmakeMove(const Move * move)
{
  Color side = this->getSideToMove();
  Color otherSide = (side == Color::White) ? Color::Black : Color::White;

  uint8_t fromSquare = getIndex(move->getSourceRow(), move->getSourceColumn());
  uint8_t toSquare = getIndex(move->getDestinationRow(), move->getDestinationColumn());
  Piece piece = move->getPiece();

  // Reset the board state from the move
  setFullMoveCounter(move->getFullMoveCounter());
  setHalfMoveClock(move->getHalfMoveClock());
  setCastlingRights(move->getCastlingRights());
  setEnpassantColumn(move->getEnPassantColumn());

  // Update the piece and color arrays
  mPieces[fromSquare] = piece;
  mColors[fromSquare] = otherSide;
  mPieces[toSquare] = Piece::None;
  mColors[toSquare] = Color::None;

  // Handle captures
  if (move->isCapture())
  {
    uint8_t sq = toSquare;
    if (move->isEnPassantCapture())
    {
      char dir = (side == Color::White) ? NORTH : SOUTH;
      sq = toSquare + dir;
    }

    mPieces[sq] = move->getCapturedPiece();
    mColors[sq] = side;
  }

  // Handle promotions
  if (move->isPromotion() || move->isPromotionCapture())
  {
    mPieces[fromSquare] = Piece::Pawn;
  }

  // Handle king moves
  if (piece == Piece::King) {
    mKingRow[otherSide] = move->getSourceRow();
    mKingCol[otherSide] = move->getSourceColumn();
  }

  // Handle castling move
  if (move->isCastle())
  {
    switch (toSquare) {
    case C1:
      mPieces[A1] = Piece::Rook;
      mColors[A1] = Color::White;
      mPieces[D1] = Piece::None;
      mColors[D1] = Color::None;
      break;
    case G1:
      mPieces[H1] = Piece::Rook;
      mColors[H1] = Color::White;
      mPieces[F1] = Piece::None;
      mColors[F1] = Color::None;
      break;
    case C8:
      mPieces[A8] = Piece::Rook;
      mColors[A8] = Color::Black;
      mPieces[D8] = Piece::None;
      mColors[D8] = Color::None;
      break;
    case G8:
      mPieces[H8] = Piece::Rook;
      mColors[H8] = Color::Black;
      mPieces[F8] = Piece::None;
      mColors[F8] = Color::None;
      break;
    }
  }

  setSideToMove(otherSide);
  return true;
}

}
