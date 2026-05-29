#include "board8x8.h"

#include <cmath>
#include <cstdint>
#include <sstream>
#include <stdexcept>
#include <string>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "fen.h"
#include "piece.h"
#include "piece_type.h"
#include "square.h"

namespace jmchess
{

// Constants for movement
static constexpr std::int8_t NORTH = 8;
static constexpr std::int8_t SOUTH = -8;
static constexpr std::int8_t EAST  = 1;
static constexpr std::int8_t WEST  = -1;

// Attack vectors
static constexpr std::int8_t PAWN_ROW_INCREMENTS[2]        = {+1, +1};
static constexpr std::int8_t PAWN_COLUMN_INCREMENTS[2]     = {+1, -1};
static constexpr std::int8_t KNIGHT_ROW_INCREMENTS[8]      = {+1, +2, +2, +1, -1, -2, -2, -1};
static constexpr std::int8_t KNIGHT_COLUMN_INCREMENTS[8]   = {+2, +1, -1, -2, -2, -1, +1, +2};
static constexpr std::int8_t KING_ROW_INCREMENTS[8]        = {+1, -1, +0, +0, +1, +1, -1, -1};
static constexpr std::int8_t KING_COLUMN_INCREMENTS[8]     = {+0, +0, +1, -1, +1, -1, -1, +1};
static constexpr std::int8_t STRAIGHT_ROW_INCREMENTS[4]    = {+1, -1, +0, +0};
static constexpr std::int8_t STRAIGHT_COLUMN_INCREMENTS[4] = {+0, +0, +1, -1};
static constexpr std::int8_t DIAGONAL_ROW_INCREMENTS[4]    = {+1, +1, -1, -1};
static constexpr std::int8_t DIAGONAL_COLUMN_INCREMENTS[4] = {+1, -1, +1, -1};

Board8x8::Board8x8()
{
  initAttacks();
  reset();
}

bool Board8x8::checkJumpAttacks(
  std::int8_t index,
  const std::int8_t * attacks,
  std::int8_t attacksSize,
  Piece piece,
  Color attackColor) const
{
  for (std::int8_t i = 0; i < attacksSize; i++)
  {
    std::int8_t destIndex = attacks[i];

    // Check to make sure we are still on the board
    if (destIndex != INVALID_INDEX)
    {
      Piece attackPiece      = mCells[destIndex].piece;
      Color attackPieceColor = mCells[destIndex].color;

      if (attackPiece == piece && attackPieceColor == attackColor)
      {
        return true;
      }
    }
  }

  return false;
}

bool Board8x8::checkSliderAttacks(
  std::int8_t index,
  const std::int8_t * attacks,
  std::int8_t attacksSize,
  Piece piece1,
  Piece piece2,
  Color attackColor) const
{
  for (std::int8_t i = 0; i < attacksSize; i++)
  {
    std::int8_t destIndex = attacks[i];

    // Check for off board
    if (destIndex == INVALID_INDEX)
    {
      break;
    }

    Piece attackPiece      = mCells[destIndex].piece;
    Color attackPieceColor = mCells[destIndex].color;

    // Check if we have encountered a piece
    if (attackPiece != Piece::None)
    {
      if (attackPieceColor == attackColor && (attackPiece == piece1 || attackPiece == piece2))
      {
        return true;
      }

      break;
    }
  }

  return false;
}

void Board8x8::generateCastlingMoves(
  Color sideToMove,
  MoveList & moveList) const
{
  std::int8_t castlingRights = mBoardState.castlingRights;
  if (sideToMove == Color::White)
  {
    Color attackColor = Color::Black;
    if (castlingRights & CASTLE_WHITE_KING)
    {
      bool f1Empty = (mCells[F1].piece == Piece::None);
      bool g1Empty = (mCells[G1].piece == Piece::None);
      if (f1Empty && g1Empty)
      {
        if (!isCellAttacked(E1, attackColor) && !isCellAttacked(F1, attackColor) && !isCellAttacked(G1, attackColor))
        {
          pushMove(E1, G1, MOVE_KING_CASTLE, Piece::None, moveList);
        }
      }
    }

    if (castlingRights & CASTLE_WHITE_QUEEN)
    {
      bool d1Empty = (mCells[D1].piece == Piece::None);
      bool c1Empty = (mCells[C1].piece == Piece::None);
      bool b1Empty = (mCells[B1].piece == Piece::None);
      if (d1Empty && c1Empty && b1Empty)
      {
        if (!isCellAttacked(E1, attackColor) && !isCellAttacked(D1, attackColor) && !isCellAttacked(C1, attackColor))
        {
          pushMove(E1, C1, MOVE_QUEEN_CASTLE, Piece::None, moveList);
        }
      }
    }
  }
  else
  {
    Color attackColor = Color::White;
    if (castlingRights & CASTLE_BLACK_KING)
    {
      bool f8Empty = (mCells[F8].piece == Piece::None);
      bool g8Empty = (mCells[G8].piece == Piece::None);
      if (f8Empty && g8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(F8, attackColor) && !isCellAttacked(G8, attackColor))
        {
          pushMove(E8, G8, MOVE_KING_CASTLE, Piece::None, moveList);
        }
      }
    }

    if (castlingRights & CASTLE_BLACK_QUEEN)
    {
      bool d8Empty = (mCells[D8].piece == Piece::None);
      bool c8Empty = (mCells[C8].piece == Piece::None);
      bool b8Empty = (mCells[B8].piece == Piece::None);
      if (d8Empty && c8Empty && b8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(D8, attackColor) && !isCellAttacked(C8, attackColor))
        {
          pushMove(E8, C8, MOVE_QUEEN_CASTLE, Piece::None, moveList);
        }
      }
    }
  }
}

void Board8x8::generateJumpMoves(
  std::int8_t index,
  const std::int8_t * attacks,
  Piece piece,
  Color pieceColor,
  MoveList & moveList) const
{
  for (std::size_t i = 0; i < 8; i++)
  {
    std::int8_t destIndex = attacks[i];

    // Check for on board
    if (destIndex != INVALID_INDEX)
    {
      Piece otherPiece = mCells[destIndex].piece;
      Color otherColor = mCells[destIndex].color;

      if (otherColor == Color::None)
      {
        pushMove(index, destIndex, MOVE_QUIET, Piece::None, moveList);
      }
      else if (isOppositeColor(pieceColor, otherColor))
      {
        pushMove(index, destIndex, MOVE_CAPTURE, otherPiece, moveList);
      }
    }
  }
}

void Board8x8::generateMoves(
  MoveList & moveList) const
{
  Color sideToMove = mBoardState.sideToMove;
  generateCastlingMoves(sideToMove, moveList);

  for (std::int8_t index = 0; index < 64; index++)
  {
    generateMoves(index, moveList);
  }
}

void Board8x8::generateMoves(
  std::int8_t index,
  MoveList & moveList) const
{
  if (index < 0 || index > 63)
  {
    std::ostringstream oss;
    oss << "Invalid index value in generateMoves: " << index;
    throw std::invalid_argument(oss.str());
  }

  Color sideToMove = mBoardState.sideToMove;
  Piece piece      = mCells[index].piece;
  Color color      = mCells[index].color;

  if (piece == Piece::None || (color != sideToMove))
  {
    return;
  }

  if (piece == Piece::Pawn)
  {
    generatePawnMoves(index, sideToMove, moveList);
  }

  if (piece == Piece::Knight)
  {
    generateJumpMoves(index, mAttacks[index].knightAttacks, piece, sideToMove, moveList);
  }

  if (piece == Piece::King)
  {
    generateJumpMoves(index, mAttacks[index].kingAttacks, piece, sideToMove, moveList);
  }

  if (piece == Piece::Bishop || piece == Piece::Queen)
  {
    for (std::int8_t i = 0; i < 4; i++)
    {
      generateSlidingMoves(index, mAttacks[index].diagonalAttacks[i], piece, sideToMove, moveList);
    }
  }

  if (piece == Piece::Rook || piece == Piece::Queen)
  {
    for (std::int8_t i = 0; i < 4; i++)
    {
      generateSlidingMoves(index, mAttacks[index].straightAttacks[i], piece, sideToMove, moveList);
    }
  }
}

void Board8x8::generateMoves(
  std::int8_t row,
  std::int8_t column,
  MoveList & moveList) const
{
  if (row < 0 || row > 7)
  {
    std::ostringstream oss;
    oss << "Invalid row value in generateMoves: " << row;
    throw std::invalid_argument(oss.str());
  }

  if (column < 0 || column > 7)
  {
    std::ostringstream oss;
    oss << "Invalid column value in generateMoves: " << column;
    throw std::invalid_argument(oss.str());
  }

  std::int8_t index = getIndex(row, column);
  Color sideToMove  = mBoardState.sideToMove;
  Piece piece       = mCells[index].piece;
  Color color       = mCells[index].color;
  if (piece == Piece::None || (color != sideToMove))
  {
    return;
  }

  if (piece == Piece::King)
  {
    generateCastlingMoves(sideToMove, moveList);
  }

  return generateMoves(index, moveList);
}

void Board8x8::generatePawnMoves(
  std::int8_t index,
  Color pieceColor,
  MoveList & moveList) const
{
  std::int8_t sourceRow       = getRow(index);
  std::int8_t sourceColumn    = getCol(index);
  std::int8_t enPassantColumn = mBoardState.enpassantColumn;

  // Compute some values based on the piece color
  std::int8_t startRow       = 1;
  std::int8_t rowIncrement   = 1;
  std::int8_t indexIncrement = NORTH;
  std::int8_t enPassantRow   = 4;
  Color otherColor           = Color::Black;
  if (pieceColor == Color::Black)
  {
    startRow       = 6;
    rowIncrement   = -1;
    indexIncrement = SOUTH;
    enPassantRow   = 3;
    otherColor     = Color::White;
  }

  std::int8_t pushSquare       = index + indexIncrement;
  std::int8_t doublePushSquare = index + indexIncrement + indexIncrement;

  if (pushSquare >= 0 && pushSquare <= 63)
  {
    // Generate non-capture moves
    if (mCells[pushSquare].piece == Piece::None)
    {
      std::int8_t destRow = getRow(pushSquare);
      if (destRow == WHITE_PROMOTION_ROW || destRow == BLACK_PROMOTION_ROW)
      {
        pushMove(index, pushSquare, MOVE_BISHOP_PROMOTION, Piece::None, moveList);
        pushMove(index, pushSquare, MOVE_KNIGHT_PROMOTION, Piece::None, moveList);
        pushMove(index, pushSquare, MOVE_ROOK_PROMOTION, Piece::None, moveList);
        pushMove(index, pushSquare, MOVE_QUEEN_PROMOTION, Piece::None, moveList);
      }
      else
      {
        pushMove(index, pushSquare, MOVE_QUIET, Piece::None, moveList);
      }

      if (doublePushSquare >= 0 && doublePushSquare <= 63)
      {
        if (sourceRow == startRow && mCells[doublePushSquare].piece == Piece::None)
        {
          pushMove(index, doublePushSquare, MOVE_DOUBLE_PAWN_PUSH, Piece::None, moveList);
        }
      }
    }
  }

  // Generate capture moves except en-passant captures
  constexpr std::int8_t columnIncrements[2] = {-1, +1};
  for (int i = 0; i < 2; i++)
  {
    std::int8_t destColumn = sourceColumn + columnIncrements[i];
    if (destColumn >= 0 && destColumn < 8)
    {
      std::int8_t destRow = sourceRow + rowIncrement;
      if (destRow >= 0 && destRow < 8)
      {
        std::int8_t destIndex = getIndex(destRow, destColumn);
        Piece capturePiece    = mCells[destIndex].piece;
        Color otherColor      = mCells[destIndex].color;
        if (capturePiece != Piece::None && isOppositeColor(pieceColor, otherColor))
        {
          if (pieceColor == Color::White && destRow == WHITE_PROMOTION_ROW)
          {
            pushMove(index, destIndex, MOVE_BISHOP_PROMOTION_CAPTURE, capturePiece, moveList);
            pushMove(index, destIndex, MOVE_KNIGHT_PROMOTION_CAPTURE, capturePiece, moveList);
            pushMove(index, destIndex, MOVE_ROOK_PROMOTION_CAPTURE, capturePiece, moveList);
            pushMove(index, destIndex, MOVE_QUEEN_PROMOTION_CAPTURE, capturePiece, moveList);
          }
          else if (pieceColor == Color::Black && destRow == BLACK_PROMOTION_ROW)
          {
            pushMove(index, destIndex, MOVE_BISHOP_PROMOTION_CAPTURE, capturePiece, moveList);
            pushMove(index, destIndex, MOVE_KNIGHT_PROMOTION_CAPTURE, capturePiece, moveList);
            pushMove(index, destIndex, MOVE_ROOK_PROMOTION_CAPTURE, capturePiece, moveList);
            pushMove(index, destIndex, MOVE_QUEEN_PROMOTION_CAPTURE, capturePiece, moveList);
          }
          else
          {
            pushMove(index, destIndex, MOVE_CAPTURE, capturePiece, moveList);
          }
        }
      }
    }
  }

  // Generate en-passant captures moves
  if (enPassantColumn != INVALID_ENPASSANT_COLUMN)
  {
    if (abs(sourceColumn - enPassantColumn) == 1 && sourceRow == enPassantRow)
    {
      std::int8_t destRow    = sourceRow + rowIncrement;
      std::int8_t destColumn = enPassantColumn;
      if (destRow >= 0 && destRow <= 7 && destColumn >= 0 && destColumn <= 7)
      {
        std::int8_t destIndex    = getIndex(destRow, destColumn);
        std::int8_t captureIndex = destIndex - indexIncrement;
        if (mCells[captureIndex].piece == Piece::Pawn && mCells[captureIndex].color == otherColor)
        {
          pushMove(index, destIndex, MOVE_ENPASSANT_CAPTURE, Piece::Pawn, moveList);
        }
      }
    }
  }
}

void Board8x8::generateSlidingMoves(
  std::int8_t index,
  const std::int8_t * attacks,
  Piece piece,
  Color pieceColor,
  MoveList & moveList) const
{
  for (std::int8_t i = 0; i < 8; i++)
  {
    std::int8_t destIndex = attacks[i];

    // Check for off board
    if (destIndex == INVALID_INDEX)
    {
      break;
    }

    Piece otherPiece = mCells[destIndex].piece;
    Color otherColor = mCells[destIndex].color;

    // Check for friendly pieces
    if (isSameColor(pieceColor, otherColor))
    {
      break;
    }

    // Check for quiet move or capture
    if (otherColor == Color::None)
    {
      pushMove(index, destIndex, MOVE_QUIET, Piece::None, moveList);
    }
    else
    {
      if (isOppositeColor(pieceColor, otherColor))
      {
        pushMove(index, destIndex, MOVE_CAPTURE, otherPiece, moveList);
      }

      break;
    }
  }
}

BoardState Board8x8::getBoardState() const
{
  return mBoardState;
}

std::int8_t Board8x8::getKingColumn(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
}

std::int8_t Board8x8::getKingRow(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
}

PieceType Board8x8::getPieceType(
  std::int8_t row,
  std::int8_t col) const
{
  std::int8_t index = getIndex(row, col);

  Piece piece = mCells[index].piece;
  Color color = mCells[index].color;

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

void Board8x8::initAttacks()
{
  for (std::int8_t index = 0; index < 64; index++)
  {
    for (std::int8_t i = 0; i < 8; i++)
    {
      mAttacks[index].knightAttacks[i] = INVALID_INDEX;
      mAttacks[index].kingAttacks[i]   = INVALID_INDEX;

      for (std::int8_t j = 0; j < 4; j++)
      {
        mAttacks[index].diagonalAttacks[j][i] = INVALID_INDEX;
        mAttacks[index].straightAttacks[j][i] = INVALID_INDEX;
      }
    }

    for (std::int8_t i = 0; i < 2; i++)
    {
      mAttacks[index].whitePawnAttacks[i] = INVALID_INDEX;
      mAttacks[index].blackPawnAttacks[i] = INVALID_INDEX;
    }

    std::int8_t row = getRow(index);
    std::int8_t col = getCol(index);

    // Initialize pawn attacks
    for (std::int8_t i = 0; i < 2; i++)
    {
      std::int8_t whiteDestRow = row - PAWN_ROW_INCREMENTS[i];
      std::int8_t whiteDestCol = col + PAWN_COLUMN_INCREMENTS[i];
      if (whiteDestRow >= 0 && whiteDestRow <= 7 && whiteDestCol >= 0 && whiteDestCol <= 7)
      {
        mAttacks[index].whitePawnAttacks[i] = getIndex(whiteDestRow, whiteDestCol);
      }

      std::int8_t blackDestRow = row + PAWN_ROW_INCREMENTS[i];
      std::int8_t blackDestCol = col + PAWN_COLUMN_INCREMENTS[i];
      if (blackDestRow >= 0 && blackDestRow <= 7 && blackDestCol >= 0 && blackDestCol <= 7)
      {
        mAttacks[index].blackPawnAttacks[i] = getIndex(blackDestRow, blackDestCol);
      }
    }

    // Initialize knight attacks
    for (std::int8_t i = 0; i < 8; i++)
    {
      std::int8_t destRow = row + KNIGHT_ROW_INCREMENTS[i];
      std::int8_t destCol = col + KNIGHT_COLUMN_INCREMENTS[i];
      if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7)
      {
        mAttacks[index].knightAttacks[i] = getIndex(destRow, destCol);
      }
    }

    // Initialize king attacks
    for (std::int8_t i = 0; i < 8; i++)
    {
      std::int8_t destRow = row + KING_ROW_INCREMENTS[i];
      std::int8_t destCol = col + KING_COLUMN_INCREMENTS[i];
      if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7)
      {
        mAttacks[index].kingAttacks[i] = getIndex(destRow, destCol);
      }
    }

    // Initialize diagonal attacks
    for (std::int8_t i = 0; i < 4; i++)
    {
      std::int8_t destRow     = row + DIAGONAL_ROW_INCREMENTS[i];
      std::int8_t destCol     = col + DIAGONAL_COLUMN_INCREMENTS[i];
      std::int8_t attackIndex = 0;
      while (true)
      {
        if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7)
        {
          mAttacks[index].diagonalAttacks[i][attackIndex] = getIndex(destRow, destCol);

          destRow += DIAGONAL_ROW_INCREMENTS[i];
          destCol += DIAGONAL_COLUMN_INCREMENTS[i];
          attackIndex++;
        }
        else
        {
          break;
        }
      }
    }

    // Initialize straight attacks
    for (std::int8_t i = 0; i < 4; i++)
    {
      std::int8_t destRow     = row + STRAIGHT_ROW_INCREMENTS[i];
      std::int8_t destCol     = col + STRAIGHT_COLUMN_INCREMENTS[i];
      std::int8_t attackIndex = 0;
      while (true)
      {
        if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7)
        {
          mAttacks[index].straightAttacks[i][attackIndex] = getIndex(destRow, destCol);

          destRow += STRAIGHT_ROW_INCREMENTS[i];
          destCol += STRAIGHT_COLUMN_INCREMENTS[i];
          attackIndex++;
        }
        else
        {
          break;
        }
      }
    }
  }
}

bool Board8x8::isCellAttacked(
  std::int8_t row,
  std::int8_t col,
  Color attackingColor) const
{
  return isCellAttacked(getIndex(row, col), attackingColor);
}

bool Board8x8::isCellAttacked(
  std::int8_t index,
  Color attackingColor) const
{
  // Check for pawn attacks
  const std::int8_t * whitePawnAttacks = mAttacks[index].whitePawnAttacks;
  const std::int8_t * blackPawnAttacks = mAttacks[index].blackPawnAttacks;
  const std::int8_t * pawnAttacks      = (attackingColor == Color::White) ? whitePawnAttacks : blackPawnAttacks;
  if (checkJumpAttacks(index, pawnAttacks, 2, Piece::Pawn, attackingColor))
  {
    return true;
  }

  // Check for knight attacks
  const std::int8_t * knightAttacks = mAttacks[index].knightAttacks;
  if (checkJumpAttacks(index, knightAttacks, 8, Piece::Knight, attackingColor))
  {
    return true;
  }

  // Check for king attacks
  const std::int8_t * kingAttacks = mAttacks[index].kingAttacks;
  if (checkJumpAttacks(index, kingAttacks, 8, Piece::King, attackingColor))
  {
    return true;
  }

  // Check for bishop/queen diagonal attacks
  for (std::int8_t i = 0; i < 4; i++)
  {
    const std::int8_t * attacks = mAttacks[index].diagonalAttacks[i];
    if (checkSliderAttacks(index, attacks, 8, Piece::Bishop, Piece::Queen, attackingColor))
    {
      return true;
    }
  }

  // Check for rook/queen straight attacks
  for (std::int8_t i = 0; i < 4; i++)
  {
    const std::int8_t * attacks = mAttacks[index].straightAttacks[i];
    if (checkSliderAttacks(index, attacks, 8, Piece::Rook, Piece::Queen, attackingColor))
    {
      return true;
    }
  }

  return false;
}

bool Board8x8::isKingInCheck(
  Color color) const
{
  std::int8_t row    = (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
  std::int8_t column = (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
  Color attackColor  = (color == Color::White) ? Color::Black : Color::White;
  std::int8_t index  = getIndex(row, column);

  return isCellAttacked(index, attackColor);
}

bool Board8x8::makeMove(
  const Move & move)
{
  BoardState boardState   = move.getBoardState();
  Color sideToMove        = boardState.sideToMove;
  std::int8_t sourceIndex = move.getSourceIndex();
  std::int8_t destIndex   = move.getDestinationIndex();
  Piece movedPiece        = mCells[sourceIndex].piece;
  Square & kingSquare     = (sideToMove == Color::White) ? mWhiteKingSquare : mBlackKingSquare;

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;

  // Do basic update
  mCells[sourceIndex].piece = Piece::None;
  mCells[sourceIndex].color = Color::None;
  mCells[destIndex].piece   = movedPiece;
  mCells[destIndex].color   = sideToMove;

  // Handle double pawn pushes
  if (move.isEnpassantPush())
  {
    std::int8_t col             = getCol(sourceIndex);
    mBoardState.enpassantColumn = col;
  }

  // Handle promotion moves
  if (move.isPromotion())
  {
    Piece promotedPiece     = move.getPromotedPiece();
    mCells[destIndex].piece = promotedPiece;
  }

  // Handle en-passant captures
  if (move.isEnpassantCapture())
  {
    std::int8_t row              = getRow(destIndex);
    std::int8_t dir              = (sideToMove == Color::White ? -1 : +1);
    std::int8_t enPassantIndex   = getIndex(row + dir, boardState.enpassantColumn);
    mCells[enPassantIndex].piece = Piece::None;
    mCells[enPassantIndex].color = Color::None;
  }

  // Handle castle moves
  if (move.isCastle())
  {
    switch (destIndex)
    {
    case C1:
      mCells[A1].piece = Piece::None;
      mCells[A1].color = Color::None;
      mCells[D1].piece = Piece::Rook;
      mCells[D1].color = sideToMove;
      break;
    case G1:
      mCells[F1].piece = Piece::Rook;
      mCells[F1].color = sideToMove;
      mCells[H1].piece = Piece::None;
      mCells[H1].color = Color::None;
      break;
    case C8:
      mCells[A8].piece = Piece::None;
      mCells[A8].color = Color::None;
      mCells[D8].piece = Piece::Rook;
      mCells[D8].color = sideToMove;
      break;
    case G8:
      mCells[F8].piece = Piece::Rook;
      mCells[F8].color = sideToMove;
      mCells[H8].piece = Piece::None;
      mCells[H8].color = Color::None;
      break;
    }
  }

  // Handle king moves
  if (movedPiece == Piece::King)
  {
    Square destSquare = {getRow(destIndex), getCol(destIndex)};
    kingSquare        = destSquare;
  }

  // Update castling rights
  // If the square associated with a king or rook is
  // modified in any way remove the castling ability
  // for that piece
  switch (sourceIndex)
  {
  case H1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_KING;
    break;
  case E1:
    mBoardState.castlingRights &= ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
    break;
  case A1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_QUEEN;
    break;
  case H8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_KING;
    break;
  case E8:
    mBoardState.castlingRights &= ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
    break;
  case A8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_QUEEN;
    break;
  }

  switch (destIndex)
  {
  case H1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_KING;
    break;
  case E1:
    mBoardState.castlingRights &= ~(CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN);
    break;
  case A1:
    mBoardState.castlingRights &= ~CASTLE_WHITE_QUEEN;
    break;
  case H8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_KING;
    break;
  case E8:
    mBoardState.castlingRights &= ~(CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN);
    break;
  case A8:
    mBoardState.castlingRights &= ~CASTLE_BLACK_QUEEN;
    break;
  }

  // Update half move clock
  mBoardState.halfMoveClock++;
  if (movedPiece == Piece::Pawn || move.isCapture())
  {
    mBoardState.halfMoveClock = 0;
  }

  // Update full move clock
  if (sideToMove == Color::Black)
  {
    mBoardState.fullMoveClock++;
  }

  // Update side to move
  mBoardState.sideToMove = (sideToMove == Color::White) ? Color::Black : Color::White;

  // Check if the king is in check. If it is
  // this is not a legal move. Unmake it right away
  // and return false
  if (isKingInCheck(sideToMove))
  {
    unmakeMove(move);
    return false;
  }

  return true;
}

void Board8x8::pushMove(
  std::int8_t fromSquare,
  std::int8_t toSquare,
  std::uint8_t flags,
  Piece capturePiece,
  MoveList & moveList) const
{
  Move move(fromSquare, toSquare, mBoardState, flags, capturePiece);
  moveList.addMove(move);
}

void Board8x8::reset()
{
  static const Piece pieces[] = {Piece::Rook, Piece::Knight, Piece::Bishop, Piece::Queen,
                                 Piece::King, Piece::Bishop, Piece::Knight, Piece::Rook};

  for (std::int8_t i = 0; i < 64; i++)
  {
    mCells[i].piece = Piece::None;
    mCells[i].color = Color::None;
  }

  for (std::int8_t col = 0; col < 8; col++)
  {
    mCells[getIndex(1, col)].piece = Piece::Pawn;
    mCells[getIndex(1, col)].color = Color::White;
    mCells[getIndex(0, col)].piece = pieces[col];
    mCells[getIndex(0, col)].color = Color::White;
    mCells[getIndex(6, col)].piece = Piece::Pawn;
    mCells[getIndex(6, col)].color = Color::Black;
    mCells[getIndex(7, col)].piece = pieces[col];
    mCells[getIndex(7, col)].color = Color::Black;
  }

  mWhiteKingSquare = {0, 4};
  mBlackKingSquare = {7, 4};

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;
  mBoardState.sideToMove      = Color::White;
  mBoardState.castlingRights  = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN | CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN;
  mBoardState.fullMoveClock   = 1;
  mBoardState.halfMoveClock   = 0;
}

void Board8x8::setPosition(
  const std::string & fenString)
{
  reset();

  Fen fenData;
  fenData.setFromString(fenString);

  mBoardState = fenData.getBoardState();

  for (std::int8_t i = 0; i < 8; i++)
  {
    for (std::int8_t j = 0; j < 8; j++)
    {
      std::int8_t index   = getIndex(i, j);
      mCells[index].piece = Piece::None;
      mCells[index].color = Color::None;

      PieceType pieceType = fenData.getPieceType(i, j);
      switch (pieceType)
      {
      case PieceType::WhitePawn:
        mCells[index].piece = Piece::Pawn;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteRook:
        mCells[index].piece = Piece::Rook;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteKnight:
        mCells[index].piece = Piece::Knight;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteBishop:
        mCells[index].piece = Piece::Bishop;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteQueen:
        mCells[index].piece = Piece::Queen;
        mCells[index].color = Color::White;
        break;
      case PieceType::WhiteKing:
        mCells[index].piece = Piece::King;
        mCells[index].color = Color::White;
        mWhiteKingSquare    = Square{i, j};
        break;
      case PieceType::BlackPawn:
        mCells[index].piece = Piece::Pawn;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackRook:
        mCells[index].piece = Piece::Rook;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackKnight:
        mCells[index].piece = Piece::Knight;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackBishop:
        mCells[index].piece = Piece::Bishop;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackQueen:
        mCells[index].piece = Piece::Queen;
        mCells[index].color = Color::Black;
        break;
      case PieceType::BlackKing:
        mCells[index].piece = Piece::King;
        mCells[index].color = Color::Black;
        mBlackKingSquare    = Square{i, j};
        break;
      default:
        mCells[index].piece = Piece::None;
        mCells[index].color = Color::None;
        break;
      }
    }
  }
}

void Board8x8::unmakeMove(
  const Move & move)
{
  // Set the previous board state
  BoardState boardState   = move.getBoardState();
  std::int8_t sourceIndex = move.getSourceIndex();
  std::int8_t destIndex   = move.getDestinationIndex();
  Color sideThatMoved     = boardState.sideToMove;
  Color otherSide         = (sideThatMoved == Color::White) ? Color::Black : Color::White;
  Piece movedPiece        = mCells[destIndex].piece;
  Square & kingSquare     = (sideThatMoved == Color::White) ? mWhiteKingSquare : mBlackKingSquare;

  // Do basic update
  mBoardState               = boardState;
  mCells[sourceIndex].piece = movedPiece;
  mCells[sourceIndex].color = sideThatMoved;
  mCells[destIndex].piece   = Piece::None;
  mCells[destIndex].color   = Color::None;

  if (move.isPromotion())
  {
    mCells[sourceIndex].piece = Piece::Pawn;
  }

  // Handle capture moves (except enpassant captures)
  if (move.isCapture())
  {
    Piece capturePiece      = move.getCapturedPiece();
    mCells[destIndex].piece = capturePiece;
    mCells[destIndex].color = otherSide;

    // Handle enpassant capture
    if (move.isEnpassantCapture())
    {
      std::int8_t row            = getRow(destIndex);
      std::int8_t dir            = (sideThatMoved == Color::White ? -1 : +1);
      std::int8_t captureIndex   = getIndex(row + dir, boardState.enpassantColumn);
      Piece capturePiece         = move.getCapturedPiece();
      mCells[destIndex].piece    = Piece::None;
      mCells[destIndex].color    = Color::None;
      mCells[captureIndex].piece = capturePiece;
      mCells[captureIndex].color = otherSide;
    }
  }

  // Update king moves
  if (movedPiece == Piece::King)
  {
    Square sourceSquare = {getRow(sourceIndex), getCol(sourceIndex)};
    kingSquare          = sourceSquare;
  }

  // Handle castling moves
  if (move.isCastle())
  {
    switch (destIndex)
    {
    case C1:
      mCells[A1].piece = Piece::Rook;
      mCells[A1].color = sideThatMoved;
      mCells[D1].piece = Piece::None;
      mCells[D1].color = Color::None;
      break;
    case G1:
      mCells[F1].piece = Piece::None;
      mCells[F1].color = Color::None;
      mCells[H1].piece = Piece::Rook;
      mCells[H1].color = sideThatMoved;
      break;
    case C8:
      mCells[A8].piece = Piece::Rook;
      mCells[A8].color = sideThatMoved;
      mCells[D8].piece = Piece::None;
      mCells[D8].color = Color::None;
      break;
    case G8:
      mCells[F8].piece = Piece::None;
      mCells[F8].color = Color::None;
      mCells[H8].piece = Piece::Rook;
      mCells[H8].color = sideThatMoved;
      break;
    }
  }
}

} // namespace jmchess
