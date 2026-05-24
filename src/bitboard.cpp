#include "bitboard.h"

#include <bitset>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <string>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "fen.h"
#include "move.h"
#include "move_list.h"

namespace jmchess
{

// Constant for board squares
static constexpr std::int8_t A1            = 0;
static constexpr std::int8_t B1            = 1;
static constexpr std::int8_t C1            = 2;
static constexpr std::int8_t D1            = 3;
static constexpr std::int8_t E1            = 4;
static constexpr std::int8_t F1            = 5;
static constexpr std::int8_t G1            = 6;
static constexpr std::int8_t H1            = 7;
static constexpr std::int8_t A2            = 8;
static constexpr std::int8_t B2            = 9;
static constexpr std::int8_t C2            = 10;
static constexpr std::int8_t D2            = 11;
static constexpr std::int8_t E2            = 12;
static constexpr std::int8_t F2            = 13;
static constexpr std::int8_t G2            = 14;
static constexpr std::int8_t H2            = 15;
static constexpr std::int8_t A3            = 16;
static constexpr std::int8_t B3            = 17;
static constexpr std::int8_t C3            = 18;
static constexpr std::int8_t D3            = 19;
static constexpr std::int8_t E3            = 20;
static constexpr std::int8_t F3            = 21;
static constexpr std::int8_t G3            = 22;
static constexpr std::int8_t H3            = 23;
static constexpr std::int8_t A4            = 24;
static constexpr std::int8_t B4            = 25;
static constexpr std::int8_t C4            = 26;
static constexpr std::int8_t D4            = 27;
static constexpr std::int8_t E4            = 28;
static constexpr std::int8_t F4            = 29;
static constexpr std::int8_t G4            = 30;
static constexpr std::int8_t H4            = 31;
static constexpr std::int8_t A5            = 32;
static constexpr std::int8_t B5            = 33;
static constexpr std::int8_t C5            = 34;
static constexpr std::int8_t D5            = 35;
static constexpr std::int8_t E5            = 36;
static constexpr std::int8_t F5            = 37;
static constexpr std::int8_t G5            = 38;
static constexpr std::int8_t H5            = 39;
static constexpr std::int8_t A6            = 40;
static constexpr std::int8_t B6            = 41;
static constexpr std::int8_t C6            = 42;
static constexpr std::int8_t D6            = 43;
static constexpr std::int8_t E6            = 44;
static constexpr std::int8_t F6            = 45;
static constexpr std::int8_t G6            = 46;
static constexpr std::int8_t H6            = 47;
static constexpr std::int8_t A7            = 48;
static constexpr std::int8_t B7            = 49;
static constexpr std::int8_t C7            = 50;
static constexpr std::int8_t D7            = 51;
static constexpr std::int8_t E7            = 52;
static constexpr std::int8_t F7            = 53;
static constexpr std::int8_t G7            = 54;
static constexpr std::int8_t H7            = 55;
static constexpr std::int8_t A8            = 56;
static constexpr std::int8_t B8            = 57;
static constexpr std::int8_t C8            = 58;
static constexpr std::int8_t D8            = 59;
static constexpr std::int8_t E8            = 60;
static constexpr std::int8_t F8            = 61;
static constexpr std::int8_t G8            = 62;
static constexpr std::int8_t H8            = 63;
static constexpr std::int8_t INVALID_INDEX = 64;

// Constants for files and ranks
static constexpr std::uint64_t RANK_1 = 0x00000000000000ffULL;
static constexpr std::uint64_t RANK_2 = 0x000000000000ff00ULL;
static constexpr std::uint64_t RANK_7 = 0x00ff000000000000ULL;
static constexpr std::uint64_t RANK_8 = 0xff00000000000000ULL;

// Attack vectors
static constexpr std::int8_t PAWN_ROW_INCREMENTS[2]        = {+1, +1};
static constexpr std::int8_t PAWN_COLUMN_INCREMENTS[2]     = {+1, -1};
static constexpr std::int8_t KNIGHT_ROW_INCREMENTS[8]      = {+1, +2, +2, +1, -1, -2, -2, -1};
static constexpr std::int8_t KNIGHT_COLUMN_INCREMENTS[8]   = {+2, +1, -1, -2, -2, -1, +1, +2};
static constexpr std::int8_t KING_ROW_INCREMENTS[8]        = {+1, -1, +0, +0, +1, +1, -1, -1};
static constexpr std::int8_t KING_COLUMN_INCREMENTS[8]     = {+0, +0, +1, -1, +1, -1, -1, +1};
static constexpr std::int8_t STRAIGHT_ROW_INCREMENTS[4]    = {+1, +0, -1, +0}; // N-E-S-W
static constexpr std::int8_t STRAIGHT_COLUMN_INCREMENTS[4] = {+0, +1, +0, -1}; // N-E-S-W
static constexpr std::int8_t DIAGONAL_ROW_INCREMENTS[4]    = {+1, +1, -1, -1}; // NE-NW-SE-SW
static constexpr std::int8_t DIAGONAL_COLUMN_INCREMENTS[4] = {+1, -1, +1, -1};

static constexpr std::int32_t PAWN_INDEX   = 0;
static constexpr std::int32_t KNIGHT_INDEX = 1;
static constexpr std::int32_t BISHOP_INDEX = 2;
static constexpr std::int32_t ROOK_INDEX   = 3;
static constexpr std::int32_t QUEEN_INDEX  = 4;
static constexpr std::int32_t KING_INDEX   = 5;

// clang-format off
const int index64[64] = 
{
    0, 47,  1, 56, 48, 27,  2, 60,
   57, 49, 41, 37, 28, 16,  3, 61,
   54, 58, 35, 52, 50, 42, 21, 44,
   38, 32, 29, 23, 17, 11,  4, 62,
   46, 55, 26, 59, 40, 36, 15, 53,
   34, 51, 20, 43, 31, 22, 10, 45,
   25, 39, 14, 33, 19, 30,  9, 24,
   13, 18,  8, 12,  7,  6,  5, 63
};
// clang-format on

/**
 * bitScanForward
 * @author Kim Walisch (2012)
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of least significant one bit
 */
int BitBoard::bitScanForward(
  std::uint64_t bb) const
{
  const std::uint64_t debruijn64 = std::uint64_t(0x03f79d71b4cb0a89);
  assert(bb != 0);
  return index64[((bb ^ (bb - 1)) * debruijn64) >> 58];
}

/**
 * bitScanReverse
 * @authors Kim Walisch, Mark Dickinson
 * @param bb bitboard to scan
 * @precondition bb != 0
 * @return index (0..63) of most significant one bit
 */
int BitBoard::bitScanReverse(
  std::uint64_t bb) const
{
  const std::uint64_t debruijn64 = std::uint64_t(0x03f79d71b4cb0a89);
  assert(bb != 0);
  bb |= bb >> 1;
  bb |= bb >> 2;
  bb |= bb >> 4;
  bb |= bb >> 8;
  bb |= bb >> 16;
  bb |= bb >> 32;
  return index64[(bb * debruijn64) >> 58];
}

constexpr std::int8_t BitBoard::getIndex(
  std::int8_t row,
  std::int8_t col) const
{
  return (row << 3) | col;
}

constexpr std::int8_t BitBoard::getRow(
  std::int8_t index) const
{
  return (index >> 3);
}

constexpr std::int8_t BitBoard::getCol(
  std::int8_t index) const
{
  return (index & 7);
}

BitBoard::BitBoard()
{
  reset();
  initAtacks();
}

void BitBoard::generateCastlingMoves(
  Color sideToMove,
  MoveList & moveList) const
{
  std::int8_t castlingRights = mBoardState.castlingRights;
  if (sideToMove == Color::White)
  {
    Color attackColor = Color::Black;
    if (castlingRights & CASTLE_WHITE_KING)
    {
      bool f1Empty = ((1ULL << F1) & ~mAllPieces);
      bool g1Empty = ((1ULL << G1) & ~mAllPieces);
      if (f1Empty && g1Empty)
      {
        if (!isCellAttacked(E1, attackColor) && !isCellAttacked(F1, attackColor) && !isCellAttacked(G1, attackColor))
        {
          pushMove(E1, G1, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }

    if (castlingRights & CASTLE_WHITE_QUEEN)
    {
      bool d1Empty = ((1ULL << D1) & ~mAllPieces);
      bool c1Empty = ((1ULL << C1) & ~mAllPieces);
      bool b1Empty = ((1ULL << B1) & ~mAllPieces);
      if (d1Empty && c1Empty && b1Empty)
      {
        if (!isCellAttacked(E1, attackColor) && !isCellAttacked(D1, attackColor) && !isCellAttacked(C1, attackColor))
        {
          pushMove(E1, C1, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }
  }
  else
  {
    Color attackColor = Color::White;
    if (castlingRights & CASTLE_BLACK_KING)
    {
      bool f8Empty = ((1ULL << F8) & ~mAllPieces);
      bool g8Empty = ((1ULL << G8) & ~mAllPieces);
      if (f8Empty && g8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(F8, attackColor) && !isCellAttacked(G8, attackColor))
        {
          pushMove(E8, G8, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }

    if (castlingRights & CASTLE_BLACK_QUEEN)
    {
      bool d8Empty = ((1ULL << D8) & ~mAllPieces);
      bool c8Empty = ((1ULL << C8) & ~mAllPieces);
      bool b8Empty = ((1ULL << B8) & ~mAllPieces);
      if (d8Empty && c8Empty && b8Empty)
      {
        if (!isCellAttacked(E8, attackColor) && !isCellAttacked(D8, attackColor) && !isCellAttacked(C8, attackColor))
        {
          pushMove(E8, C8, Piece::King, Piece::None, Piece::None, Move::Type::Castle, moveList);
        }
      }
    }
  }
}

void BitBoard::generateDiagonalMoves(
  std::uint64_t pieces,
  Piece pieceType,
  std::uint64_t enemyPieces,
  MoveList & moveList) const
{
  static constexpr std::int32_t forward[] = {0, 1};
  static constexpr std::int32_t reverse[] = {2, 3};
  while (pieces)
  {
    std::int8_t fromSquare   = bitScanForward(pieces);
    std::uint64_t allAttacks = 0;
    for (int32_t i = 0; i < 2; i++)
    {
      std::uint64_t attacks  = mAttacks[fromSquare].diagonalAttacks[forward[i]];
      std::uint64_t blockers = attacks & mAllPieces;
      if (blockers)
      {
        std::int8_t toSquare = bitScanForward(blockers);
        attacks ^= mAttacks[toSquare].diagonalAttacks[forward[i]];
      }
      allAttacks |= attacks;
    }

    for (int32_t i = 0; i < 2; i++)
    {
      std::uint64_t attacks  = mAttacks[fromSquare].diagonalAttacks[reverse[i]];
      std::uint64_t blockers = attacks & mAllPieces;
      if (blockers)
      {
        std::int8_t toSquare = bitScanReverse(blockers);
        attacks ^= mAttacks[toSquare].diagonalAttacks[reverse[i]];
      }
      allAttacks |= attacks;
    }

    std::uint64_t quietMoves   = allAttacks & ~mAllPieces;
    std::uint64_t captureMoves = allAttacks & enemyPieces;

    while (quietMoves)
    {
      std::int8_t toSquare = bitScanForward(quietMoves);
      pushMove(fromSquare, toSquare, pieceType, Piece::None, Piece::None, Move::Type::Quiet, moveList);
      quietMoves &= quietMoves - 1;
    }

    while (captureMoves)
    {
      std::int8_t toSquare = bitScanForward(captureMoves);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, pieceType, capturePiece, Piece::None, Move::Type::Capture, moveList);
      captureMoves &= captureMoves - 1;
    }

    pieces &= pieces - 1;
  }
}

void BitBoard::generateKingMoves(
  std::uint64_t kings,
  std::uint64_t enemyPieces,
  MoveList & moveList) const
{
  while (kings)
  {
    std::int8_t fromSquare     = bitScanForward(kings);
    std::uint64_t attacks      = mAttacks[fromSquare].kingAttacks;
    std::uint64_t quietMoves   = attacks & ~mAllPieces;
    std::uint64_t captureMoves = attacks & enemyPieces;

    while (quietMoves)
    {
      std::int8_t toSquare = bitScanForward(quietMoves);
      pushMove(fromSquare, toSquare, Piece::King, Piece::None, Piece::None, Move::Type::Quiet, moveList);
      quietMoves &= quietMoves - 1;
    }

    while (captureMoves)
    {
      std::int8_t toSquare = bitScanForward(captureMoves);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, Piece::King, capturePiece, Piece::None, Move::Type::Capture, moveList);
      captureMoves &= captureMoves - 1;
    }

    kings &= kings - 1;
  }
}

void BitBoard::generateKnightMoves(
  std::uint64_t knights,
  std::uint64_t enemyPieces,
  MoveList & moveList) const
{
  while (knights)
  {
    std::int8_t fromSquare     = bitScanForward(knights);
    std::uint64_t attacks      = mAttacks[fromSquare].knightAttacks;
    std::uint64_t quietMoves   = attacks & ~mAllPieces;
    std::uint64_t captureMoves = attacks & enemyPieces;
    while (quietMoves)
    {
      std::int8_t toSquare = bitScanForward(quietMoves);
      pushMove(fromSquare, toSquare, Piece::Knight, Piece::None, Piece::None, Move::Type::Quiet, moveList);
      quietMoves &= quietMoves - 1;
    }

    while (captureMoves)
    {
      std::int8_t toSquare = bitScanForward(captureMoves);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, Piece::Knight, capturePiece, Piece::None, Move::Type::Capture, moveList);
      captureMoves &= captureMoves - 1;
    }

    knights &= knights - 1;
  }
}

void BitBoard::generateMoves(
  MoveList & moveList) const
{
  Color sideToMove = mBoardState.sideToMove;

  std::int32_t indexIncrement  = 0;
  std::uint64_t friendlyPieces = mWhitePieces;
  std::uint64_t enemyPieces    = mBlackPieces;
  if (sideToMove == Color::Black)
  {
    indexIncrement = 6;
    friendlyPieces = mBlackPieces;
    enemyPieces    = mWhitePieces;
  }

  std::uint64_t pawns   = mBitBoards[PAWN_INDEX + indexIncrement];
  std::uint64_t knights = mBitBoards[KNIGHT_INDEX + indexIncrement];
  std::uint64_t bishops = mBitBoards[BISHOP_INDEX + indexIncrement];
  std::uint64_t rooks   = mBitBoards[ROOK_INDEX + indexIncrement];
  std::uint64_t queens  = mBitBoards[QUEEN_INDEX + indexIncrement];
  std::uint64_t kings   = mBitBoards[KING_INDEX + indexIncrement];

  if (sideToMove == Color::White)
  {
    generatePawnPushesWhite(moveList);
    generatePawnCapturesWhite(pawns, enemyPieces, moveList);
  }
  else
  {
    generatePawnPushesBlack(moveList);
    generatePawnCapturesBlack(pawns, enemyPieces, moveList);
  }

  generateCastlingMoves(sideToMove, moveList);
  generateKnightMoves(knights, enemyPieces, moveList);
  generateKingMoves(kings, enemyPieces, moveList);
  generateDiagonalMoves(bishops, Piece::Bishop, enemyPieces, moveList);
  generateDiagonalMoves(queens, Piece::Queen, enemyPieces, moveList);
  generateStraightMoves(queens, Piece::Queen, enemyPieces, moveList);
  generateStraightMoves(rooks, Piece::Rook, enemyPieces, moveList);
}

void BitBoard::generateMoves(
  std::int8_t row,
  std::int8_t col,
  MoveList & moveList) const
{
  generateMoves(getIndex(row, col), moveList);
}

void BitBoard::generateMoves(
  std::int8_t index,
  MoveList & moveList) const
{
  Color sideToMove = mBoardState.sideToMove;

  std::int32_t indexIncrement  = 0;
  std::uint64_t friendlyPieces = mWhitePieces;
  std::uint64_t enemyPieces    = mBlackPieces;
  if (sideToMove == Color::Black)
  {
    enemyPieces    = mWhitePieces;
    friendlyPieces = mBlackPieces;
    indexIncrement = 6;
  }

  std::uint64_t knights = mBitBoards[KNIGHT_INDEX + indexIncrement];
  std::uint64_t kings   = mBitBoards[KING_INDEX + indexIncrement];
  std::uint64_t bishops = mBitBoards[BISHOP_INDEX + indexIncrement];
  std::uint64_t rooks   = mBitBoards[ROOK_INDEX + indexIncrement];
  std::uint64_t queens  = mBitBoards[QUEEN_INDEX + indexIncrement];
  std::uint64_t pawns   = mBitBoards[PAWN_INDEX + indexIncrement];

  std::uint64_t pieceBitboard = (1ULL << index);
  std::uint64_t bis           = bishops & pieceBitboard;

  if (sideToMove == Color::White)
  {
    if (pawns & pieceBitboard)
    {
      generatePawnPushesWhite(moveList);
    }

    generatePawnCapturesWhite(pawns & pieceBitboard, enemyPieces, moveList);
  }
  else
  {
    if (pawns & pieceBitboard)
    {
      generatePawnPushesBlack(moveList);
    }

    generatePawnCapturesBlack(pawns & pieceBitboard, enemyPieces, moveList);
  }

  generateKnightMoves(knights & pieceBitboard, enemyPieces, moveList);
  generateKingMoves(kings & pieceBitboard, enemyPieces, moveList);
  generateDiagonalMoves(bishops & pieceBitboard, Piece::Bishop, enemyPieces, moveList);
  generateDiagonalMoves(queens & pieceBitboard, Piece::Queen, enemyPieces, moveList);
  generateStraightMoves(queens & pieceBitboard, Piece::Queen, enemyPieces, moveList);
  generateStraightMoves(rooks & pieceBitboard, Piece::Rook, enemyPieces, moveList);
}

void BitBoard::generatePawnCapturesBlack(
  std::uint64_t pawns,
  std::uint64_t enemyPieces,
  MoveList & moveList) const
{
  while (pawns)
  {
    std::int8_t fromSquare          = bitScanForward(pawns);
    std::uint64_t attacks           = mAttacks[fromSquare].blackPawnAttacks;
    std::uint64_t captureMoves      = attacks & enemyPieces;
    std::uint64_t promotionCaptures = captureMoves & RANK_1;
    captureMoves &= ~promotionCaptures;

    while (captureMoves)
    {
      std::int8_t toSquare = bitScanForward(captureMoves);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::None, Move::Type::Capture, moveList);
      captureMoves &= captureMoves - 1;
    }

    while (promotionCaptures)
    {
      std::int8_t toSquare = bitScanForward(promotionCaptures);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
      promotionCaptures &= promotionCaptures - 1;
    }

    if (mBoardState.enpassantColumn != INVALID_ENPASSANT_COLUMN)
    {
      std::int8_t row = getRow(fromSquare);
      std::int8_t col = getCol(fromSquare);
      if (row == BLACK_ENPASSANT_ROW)
      {
        if (abs(col - mBoardState.enpassantColumn) == 1)
        {
          std::int8_t toSquare = getIndex(row - 1, mBoardState.enpassantColumn);
          pushMove(fromSquare, toSquare, Piece::Pawn, Piece::Pawn, Piece::None, Move::Type::EnpassantCapture, moveList);
        }
      }
    }
    pawns &= pawns - 1;
  }
}

void BitBoard::generatePawnPushesBlack(
  MoveList & moveList) const
{
  std::uint64_t empty          = ~mAllPieces;
  std::uint64_t pawns          = mBitBoards[PAWN_INDEX + 6];
  std::uint64_t unmovedPawns   = pawns & RANK_7;
  std::uint64_t singlePushOpen = ~(mAllPieces << 8);
  std::uint64_t doublePushOpen = singlePushOpen & ~(mAllPieces << 16);
  std::uint64_t singlePushes   = ((pawns >> 8) & empty);
  std::uint64_t doublePushes   = (((unmovedPawns >> 8) & empty) >> 8) & empty;
  std::uint64_t pawnPromotions = (singlePushes & RANK_1);

  singlePushes &= ~pawnPromotions;

  while (singlePushes)
  {
    std::int8_t toSquare   = bitScanForward(singlePushes);
    std::int8_t fromSquare = toSquare + 8;
    pushMove(fromSquare, toSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::Quiet, moveList);
    singlePushes &= singlePushes - 1;
  }

  while (doublePushes)
  {
    std::int8_t toSquare   = bitScanForward(doublePushes);
    std::int8_t fromSquare = toSquare + 16;
    pushMove(fromSquare, toSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::EnpassantPush, moveList);
    doublePushes &= doublePushes - 1;
  }

  while (pawnPromotions)
  {
    uint8_t toSq   = bitScanForward(pawnPromotions);
    uint8_t fromSq = toSq + 8;
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Queen, Move::Type::Promotion, moveList);
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Rook, Move::Type::Promotion, moveList);
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Bishop, Move::Type::Promotion, moveList);
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Knight, Move::Type::Promotion, moveList);
    pawnPromotions &= pawnPromotions - 1;
  }
}

void BitBoard::generatePawnCapturesWhite(
  std::uint64_t pawns,
  std::uint64_t enemyPieces,
  MoveList & moveList) const
{
  while (pawns)
  {
    std::int8_t fromSquare          = bitScanForward(pawns);
    std::uint64_t attacks           = mAttacks[fromSquare].whitePawnAttacks;
    std::uint64_t captureMoves      = attacks & enemyPieces;
    std::uint64_t promotionCaptures = captureMoves & RANK_8;
    captureMoves &= ~promotionCaptures;

    while (captureMoves)
    {
      std::int8_t toSquare = bitScanForward(captureMoves);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::None, Move::Type::Capture, moveList);
      captureMoves &= captureMoves - 1;
    }

    while (promotionCaptures)
    {
      std::int8_t toSquare = bitScanForward(promotionCaptures);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Queen, Move::Type::PromotionCapture, moveList);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Rook, Move::Type::PromotionCapture, moveList);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Bishop, Move::Type::PromotionCapture, moveList);
      pushMove(fromSquare, toSquare, Piece::Pawn, capturePiece, Piece::Knight, Move::Type::PromotionCapture, moveList);
      promotionCaptures &= promotionCaptures - 1;
    }

    if (mBoardState.enpassantColumn != INVALID_ENPASSANT_COLUMN)
    {
      std::int8_t row = getRow(fromSquare);
      std::int8_t col = getCol(fromSquare);
      if (row == WHITE_ENPASSANT_ROW)
      {
        if (abs(col - mBoardState.enpassantColumn) == 1)
        {
          std::int8_t toSquare = getIndex(row + 1, mBoardState.enpassantColumn);
          pushMove(fromSquare, toSquare, Piece::Pawn, Piece::Pawn, Piece::None, Move::Type::EnpassantCapture, moveList);
        }
      }
    }
    pawns &= pawns - 1;
  }
}

void BitBoard::generatePawnPushesWhite(
  MoveList & moveList) const
{
  std::uint64_t empty          = ~mAllPieces;
  std::uint64_t pawns          = mBitBoards[PAWN_INDEX];
  std::uint64_t unmovedPawns   = pawns & RANK_2;
  std::uint64_t singlePushOpen = ~(mAllPieces >> 8);
  std::uint64_t doublePushOpen = singlePushOpen & ~(mAllPieces >> 16);
  std::uint64_t singlePushes   = ((pawns << 8) & empty);
  std::uint64_t doublePushes   = (((unmovedPawns << 8) & empty) << 8) & empty;
  std::uint64_t pawnPromotions = (singlePushes & RANK_8);

  singlePushes &= ~pawnPromotions;

  while (singlePushes)
  {
    std::int8_t toSquare   = bitScanForward(singlePushes);
    std::int8_t fromSquare = toSquare - 8;
    pushMove(fromSquare, toSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::Quiet, moveList);
    singlePushes &= singlePushes - 1;
  }

  while (doublePushes)
  {
    std::int8_t toSquare   = bitScanForward(doublePushes);
    std::int8_t fromSquare = toSquare - 16;
    pushMove(fromSquare, toSquare, Piece::Pawn, Piece::None, Piece::None, Move::Type::EnpassantPush, moveList);
    doublePushes &= doublePushes - 1;
  }

  while (pawnPromotions)
  {
    std::uint8_t toSq   = bitScanForward(pawnPromotions);
    std::uint8_t fromSq = toSq - 8;
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Queen, Move::Type::Promotion, moveList);
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Rook, Move::Type::Promotion, moveList);
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Bishop, Move::Type::Promotion, moveList);
    pushMove(fromSq, toSq, Piece::Pawn, Piece::None, Piece::Knight, Move::Type::Promotion, moveList);
    pawnPromotions &= pawnPromotions - 1;
  }
}

void BitBoard::generateStraightMoves(
  std::uint64_t pieces,
  Piece pieceType,
  std::uint64_t enemyPieces,
  MoveList & moveList) const
{
  static constexpr std::int32_t forward[] = {0, 1};
  static constexpr std::int32_t reverse[] = {2, 3};
  while (pieces)
  {
    std::int8_t fromSquare   = bitScanForward(pieces);
    std::uint64_t allAttacks = 0;
    for (std::int32_t i = 0; i < 2; i++)
    {
      std::uint64_t attacks  = mAttacks[fromSquare].straightAttacks[forward[i]];
      std::uint64_t blockers = attacks & mAllPieces;
      if (blockers)
      {
        std::int8_t toSquare = bitScanForward(blockers);
        attacks ^= mAttacks[toSquare].straightAttacks[forward[i]];
      }
      allAttacks |= attacks;
    }

    for (std::int32_t i = 0; i < 2; i++)
    {
      std::uint64_t attacks  = mAttacks[fromSquare].straightAttacks[reverse[i]];
      std::uint64_t blockers = attacks & mAllPieces;
      if (blockers)
      {
        std::int8_t toSquare = bitScanReverse(blockers);
        attacks ^= mAttacks[toSquare].straightAttacks[reverse[i]];
      }
      allAttacks |= attacks;
    }

    std::uint64_t quietMoves   = allAttacks & ~mAllPieces;
    std::uint64_t captureMoves = allAttacks & enemyPieces;

    while (quietMoves)
    {
      std::int8_t toSquare = bitScanForward(quietMoves);
      pushMove(fromSquare, toSquare, pieceType, Piece::None, Piece::None, Move::Type::Quiet, moveList);
      quietMoves &= quietMoves - 1;
    }

    while (captureMoves)
    {
      std::int8_t toSquare = bitScanForward(captureMoves);
      Piece capturePiece   = getCapturedPiece(toSquare);
      pushMove(fromSquare, toSquare, pieceType, capturePiece, Piece::None, Move::Type::Capture, moveList);
      captureMoves &= captureMoves - 1;
    }

    pieces &= pieces - 1;
  }
}

BoardState BitBoard::getBoardState() const
{
  return mBoardState;
}

Piece BitBoard::getCapturedPiece(
  std::int8_t square) const
{
  static constexpr Piece pieceTypes[5] = {Piece::Pawn, Piece::Knight, Piece::Bishop, Piece::Rook, Piece::Queen};

  std::uint64_t captureBitBoard = (1ULL << square);

  if (mBoardState.sideToMove == Color::White)
  {
    if (captureBitBoard & mBitBoards[PAWN_INDEX + 6])
    {
      return Piece::Pawn;
    }
    else if (captureBitBoard & mBitBoards[KNIGHT_INDEX + 6])
    {
      return Piece::Knight;
    }
    else if (captureBitBoard & mBitBoards[BISHOP_INDEX + 6])
    {
      return Piece::Bishop;
    }
    else if (captureBitBoard & mBitBoards[QUEEN_INDEX + 6])
    {
      return Piece::Queen;
    }
    else if (captureBitBoard & mBitBoards[ROOK_INDEX + 6])
    {
      return Piece::Rook;
    }
  }
  else
  {
    if (captureBitBoard & mBitBoards[PAWN_INDEX])
    {
      return Piece::Pawn;
    }
    else if (captureBitBoard & mBitBoards[KNIGHT_INDEX])
    {
      return Piece::Knight;
    }
    else if (captureBitBoard & mBitBoards[BISHOP_INDEX])
    {
      return Piece::Bishop;
    }
    else if (captureBitBoard & mBitBoards[QUEEN_INDEX])
    {
      return Piece::Queen;
    }
    else if (captureBitBoard & mBitBoards[ROOK_INDEX])
    {
      return Piece::Rook;
    }
  }

  return Piece::None;
}

std::int8_t BitBoard::getKingColumn(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
}

std::int8_t BitBoard::getKingRow(
  Color color) const
{
  return (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
}

PieceType BitBoard::getPieceType(
  std::int8_t row,
  std::int8_t col) const
{
  static constexpr PieceType pieceTypes[12] = {
    PieceType::WhitePawn,   PieceType::WhiteKnight, PieceType::WhiteBishop, PieceType::WhiteRook,
    PieceType::WhiteQueen,  PieceType::WhiteKing,   PieceType::BlackPawn,   PieceType::BlackKnight,
    PieceType::BlackBishop, PieceType::BlackRook,   PieceType::BlackQueen,  PieceType::BlackKing,
  };

  for (std::int8_t pieceIndex = 0; pieceIndex < 12; pieceIndex++)
  {
    std::uint64_t pieceBitBoard = mBitBoards[pieceIndex];
    PieceType pieceType         = pieceTypes[pieceIndex];

    std::int8_t index = getIndex(row, col);
    if ((pieceBitBoard >> index) & 0x1)
    {
      return pieceType;
    }
  }

  return PieceType::None;
}

void BitBoard::initAtacks()
{
  for (std::int8_t index = 0; index < 64; index++)
  {
    std::int8_t row    = getRow(index);
    std::int8_t column = getCol(index);

    mAttacks[index].knightAttacks    = 0;
    mAttacks[index].kingAttacks      = 0;
    mAttacks[index].whitePawnAttacks = 0;
    mAttacks[index].blackPawnAttacks = 0;

    for (std::int8_t i = 0; i < 4; i++)
    {
      mAttacks[index].diagonalAttacks[i] = 0;
      mAttacks[index].straightAttacks[i] = 0;
    }

    // Initialize pawn attacks
    for (std::int8_t i = 0; i < 2; i++)
    {
      std::int8_t whiteDestRow    = row + PAWN_ROW_INCREMENTS[i];
      std::int8_t whiteDestColumn = column + PAWN_COLUMN_INCREMENTS[i];
      if (whiteDestRow >= 0 && whiteDestRow <= 7 && whiteDestColumn >= 0 && whiteDestColumn <= 7)
      {
        mAttacks[index].whitePawnAttacks |= (1ULL << getIndex(whiteDestRow, whiteDestColumn));
      }

      std::int8_t blackDestRow    = row - PAWN_ROW_INCREMENTS[i];
      std::int8_t blackDestColumn = column + PAWN_COLUMN_INCREMENTS[i];
      if (blackDestRow >= 0 && blackDestRow <= 7 && blackDestColumn >= 0 && blackDestColumn <= 7)
      {
        mAttacks[index].blackPawnAttacks |= (1ULL << getIndex(blackDestRow, blackDestColumn));
      }
    }

    // Initialize knight attacks
    for (std::int8_t i = 0; i < 8; i++)
    {
      std::int8_t destRow    = row + KNIGHT_ROW_INCREMENTS[i];
      std::int8_t destColumn = column + KNIGHT_COLUMN_INCREMENTS[i];
      if (destRow >= 0 && destRow <= 7 && destColumn >= 0 && destColumn <= 7)
      {
        mAttacks[index].knightAttacks |= (1ULL << getIndex(destRow, destColumn));
      }
    }

    // Initialize king attacks
    for (std::int8_t i = 0; i < 8; i++)
    {
      std::int8_t destRow    = row + KING_ROW_INCREMENTS[i];
      std::int8_t destColumn = column + KING_COLUMN_INCREMENTS[i];
      if (destRow >= 0 && destRow <= 7 && destColumn >= 0 && destColumn <= 7)
      {
        mAttacks[index].kingAttacks |= (1ULL << getIndex(destRow, destColumn));
      }
    }

    // Intitialize sliding piece attacks
    for (std::int32_t i = 0; i < 4; i++)
    {
      std::int8_t rowIncrement = STRAIGHT_ROW_INCREMENTS[i];
      std::int8_t colIncrement = STRAIGHT_COLUMN_INCREMENTS[i];
      std::int8_t destRow      = row + rowIncrement;
      std::int8_t destColumn   = column + colIncrement;
      for (std::int32_t j = 0; j < 7; j++)
      {
        if (destRow < 0 || destRow > 7 || destColumn < 0 || destColumn > 7)
        {
          break;
        }

        std::int8_t destIndex = getIndex(destRow, destColumn);
        mAttacks[index].straightAttacks[i] |= (1ULL << destIndex);
        destRow += rowIncrement;
        destColumn += colIncrement;
      }
    }

    for (std::int32_t i = 0; i < 4; i++)
    {
      std::int8_t rowIncrement = DIAGONAL_ROW_INCREMENTS[i];
      std::int8_t colIncrement = DIAGONAL_COLUMN_INCREMENTS[i];
      std::int8_t destRow      = row + rowIncrement;
      std::int8_t destColumn   = column + colIncrement;
      for (std::int32_t j = 0; j < 7; j++)
      {
        if (destRow < 0 || destRow > 7 || destColumn < 0 || destColumn > 7)
        {
          break;
        }

        std::int8_t destIndex = getIndex(destRow, destColumn);
        mAttacks[index].diagonalAttacks[i] |= (1ULL << destIndex);
        destRow += rowIncrement;
        destColumn += colIncrement;
      }
    }
  }
}

void BitBoard::initSliderAttacks(
  std::int8_t row,
  std::int8_t col,
  std::uint64_t occupancy,
  std::int8_t rowIncrement,
  std::int8_t colIncrement,
  std::uint64_t & attacks) const
{
  std::int8_t destRow = row + rowIncrement;
  std::int8_t destCol = col + colIncrement;
  while (true)
  {
    if (destRow >= 0 && destRow <= 7 && destCol >= 0 && destCol <= 7)
    {
      attacks |= (1ULL << getIndex(destRow, destCol));

      std::uint64_t bb = (1ULL << getIndex(destRow, destCol));
      if (occupancy & bb)
      {
        break;
      }

      destRow += rowIncrement;
      destCol += colIncrement;
    }
    else
    {
      break;
    }
  }
}

bool BitBoard::isCellAttacked(
  std::int8_t row,
  std::int8_t col,
  Color attackingColor) const
{
  return isCellAttacked(getIndex(row, col), attackingColor);
}

bool BitBoard::isCellAttacked(
  std::int8_t index,
  Color attackingColor) const
{
  std::uint64_t knightAttacks           = mAttacks[index].knightAttacks;
  std::uint64_t kingAttacks             = mAttacks[index].kingAttacks;
  std::uint64_t pawnAttacks             = mAttacks[index].blackPawnAttacks;
  const std::uint64_t * diagonalAttacks = mAttacks[index].diagonalAttacks;
  const std::uint64_t * straightAttacks = mAttacks[index].straightAttacks;

  std::uint64_t attackingKnights = mBitBoards[KNIGHT_INDEX];
  std::uint64_t attackingKings   = mBitBoards[KING_INDEX];
  std::uint64_t attackingPawns   = mBitBoards[PAWN_INDEX];
  std::uint64_t attackingBishops = mBitBoards[BISHOP_INDEX];
  std::uint64_t attackingRooks   = mBitBoards[ROOK_INDEX];
  std::uint64_t attackingQueens  = mBitBoards[QUEEN_INDEX];
  if (attackingColor == Color::Black)
  {
    pawnAttacks      = mAttacks[index].whitePawnAttacks;
    attackingKnights = mBitBoards[KNIGHT_INDEX + 6];
    attackingKings   = mBitBoards[KING_INDEX + 6];
    attackingPawns   = mBitBoards[PAWN_INDEX + 6];
    attackingBishops = mBitBoards[BISHOP_INDEX + 6];
    attackingRooks   = mBitBoards[ROOK_INDEX + 6];
    attackingQueens  = mBitBoards[QUEEN_INDEX + 6];
  }

  if (pawnAttacks & attackingPawns)
  {
    return true;
  }

  if (knightAttacks & attackingKnights)
  {
    return true;
  }

  if (kingAttacks & attackingKings)
  {
    return true;
  }

  for (std::int32_t i = 0; i < 2; i++)
  {
    std::uint64_t straightAttacks  = mAttacks[index].straightAttacks[i];
    std::uint64_t straightBlockers = straightAttacks & mAllPieces;
    if (straightBlockers)
    {
      std::int8_t toSquare = bitScanForward(straightBlockers);
      straightAttacks ^= mAttacks[toSquare].straightAttacks[i];
    }

    if ((straightAttacks & attackingRooks) || (straightAttacks & attackingQueens))
    {
      return true;
    }

    std::uint64_t diagAttacks  = mAttacks[index].diagonalAttacks[i];
    std::uint64_t diagBlockers = diagAttacks & mAllPieces;
    if (diagBlockers)
    {
      std::int8_t toSquare = bitScanForward(diagBlockers);
      diagAttacks ^= mAttacks[toSquare].diagonalAttacks[i];
    }

    if ((diagAttacks & attackingBishops) || (diagAttacks & attackingQueens))
    {
      return true;
    }
  }

  for (std::int32_t i = 0; i < 2; i++)
  {
    std::uint64_t straightAttacks  = mAttacks[index].straightAttacks[i + 2];
    std::uint64_t straightBlockers = straightAttacks & mAllPieces;

    if (straightBlockers)
    {
      std::int8_t toSquare = bitScanReverse(straightBlockers);
      straightAttacks ^= mAttacks[toSquare].straightAttacks[i + 2];
    }

    if ((straightAttacks & attackingRooks) || (straightAttacks & attackingQueens))
    {
      return true;
    }

    std::uint64_t diagAttacks  = mAttacks[index].diagonalAttacks[i + 2];
    std::uint64_t diagBlockers = diagAttacks & mAllPieces;
    if (diagBlockers)
    {
      std::int8_t toSquare = bitScanReverse(diagBlockers);
      diagAttacks ^= mAttacks[toSquare].diagonalAttacks[i + 2];
    }

    if ((diagAttacks & attackingBishops) || (diagAttacks & attackingQueens))
    {
      return true;
    }
  }

  return false;
}

bool BitBoard::isKingInCheck(
  Color color) const
{
  std::int8_t row    = (color == Color::White) ? mWhiteKingSquare.row : mBlackKingSquare.row;
  std::int8_t column = (color == Color::White) ? mWhiteKingSquare.col : mBlackKingSquare.col;
  Color attackColor  = (color == Color::White) ? Color::Black : Color::White;
  std::int8_t index  = getIndex(row, column);

  return isCellAttacked(index, attackColor);
}

void BitBoard::makeMove(
  const Move & move)
{
  // Perform some general calculations
  BoardState boardState        = move.getBoardState();
  Color sideToMove             = boardState.sideToMove;
  Square sourceSquare          = move.getSourceSquare();
  Square destSquare            = move.getDestinationSquare();
  Piece movedPiece             = move.getPiece();
  std::int8_t sourceIndex      = getIndex(sourceSquare.row, sourceSquare.col);
  std::int8_t destIndex        = getIndex(destSquare.row, destSquare.col);
  std::uint64_t sourceBitBoard = (1ULL << sourceIndex);
  std::uint64_t destBitBoard   = (1ULL << destIndex);
  mBoardState.enpassantColumn  = INVALID_ENPASSANT_COLUMN;

  // Get the bitboard index for the piece being moved
  std::int32_t movedPieceIndex = static_cast<std::int32_t>(movedPiece);
  if (sideToMove == Color::Black)
  {
    movedPieceIndex += 6;
  }

  // Handle quiet moves
  if (move.isQuiet())
  {
    mBitBoards[movedPieceIndex] ^= sourceBitBoard;
    mBitBoards[movedPieceIndex] |= destBitBoard;
  }

  // Handle double pushes
  if (move.isEnpassantPush())
  {
    mBitBoards[movedPieceIndex] ^= sourceBitBoard;
    mBitBoards[movedPieceIndex] |= destBitBoard;
    mBoardState.enpassantColumn = sourceSquare.col;
  }

  // Handle promotion moves
  if (move.isPromotion() && !move.isPromotionCapture())
  {
    Piece promotedPiece             = move.getPromotedPiece();
    std::int32_t promotedPieceIndex = static_cast<std::int32_t>(promotedPiece);
    if (sideToMove == Color::Black)
    {
      promotedPieceIndex += 6;
    }

    mBitBoards[movedPieceIndex] ^= sourceBitBoard;
    mBitBoards[promotedPieceIndex] |= destBitBoard;
  }

  // Handle capture moves
  if (move.isCapture())
  {
    Piece capturePiece             = move.getCapturedPiece();
    std::int32_t capturePieceIndex = static_cast<std::int32_t>(capturePiece);
    if (sideToMove == Color::White)
    {
      capturePieceIndex += 6;
    }

    if (move.isPromotionCapture())
    {
      Piece promotedPiece             = move.getPromotedPiece();
      std::int32_t promotedPieceIndex = static_cast<std::int32_t>(promotedPiece);
      if (sideToMove == Color::Black)
      {
        promotedPieceIndex += 6;
      }

      mBitBoards[movedPieceIndex] ^= sourceBitBoard;
      mBitBoards[promotedPieceIndex] |= destBitBoard;
      mBitBoards[capturePieceIndex] ^= destBitBoard;
    }
    else if (move.isEnpassantCapture())
    {
      std::int8_t dir            = (sideToMove == Color::White ? -1 : +1);
      std::int8_t enPassantIndex = getIndex(destSquare.row + dir, boardState.enpassantColumn);
      std::uint64_t epBitBoard   = (1ULL << enPassantIndex);

      mBitBoards[movedPieceIndex] ^= sourceBitBoard;
      mBitBoards[movedPieceIndex] |= destBitBoard;
      mBitBoards[capturePieceIndex] ^= epBitBoard;
    }
    else // Standard captures
    {
      mBitBoards[movedPieceIndex] ^= sourceBitBoard;
      mBitBoards[movedPieceIndex] |= destBitBoard;
      mBitBoards[capturePieceIndex] ^= destBitBoard;
    }
  }

  // Handle castle moves
  if (move.isCastle())
  {
    mBitBoards[movedPieceIndex] ^= sourceBitBoard;
    mBitBoards[movedPieceIndex] |= destBitBoard;

    if (destIndex == C1)
    {
      mBitBoards[ROOK_INDEX] ^= (1ULL << A1);
      mBitBoards[ROOK_INDEX] |= (1ULL << D1);
    }

    if (destIndex == G1)
    {
      mBitBoards[ROOK_INDEX] ^= (1ULL << H1);
      mBitBoards[ROOK_INDEX] |= (1ULL << F1);
    }

    if (destIndex == C8)
    {
      mBitBoards[ROOK_INDEX + 6] ^= (1ULL << A8);
      mBitBoards[ROOK_INDEX + 6] |= (1ULL << D8);
    }

    if (destIndex == G8)
    {
      mBitBoards[ROOK_INDEX + 6] ^= (1ULL << H8);
      mBitBoards[ROOK_INDEX + 6] |= (1ULL << F8);
    }
  }

  // Handle king moves
  if (movedPiece == Piece::King)
  {
    if (sideToMove == Color::White)
    {
      mWhiteKingSquare = destSquare;
    }
    else
    {
      mBlackKingSquare = destSquare;
    }
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

  // Update the aggregated bitboards
  updateAggregateBitboards();
}

void BitBoard::pushMove(
  std::int8_t fromSquare,
  std::int8_t toSquare,
  Piece piece,
  Piece capturePiece,
  Piece promotionPiece,
  Move::Type type,
  MoveList & moveList) const
{
  std::int8_t sourceRow = getRow(fromSquare);
  std::int8_t sourceCol = getCol(fromSquare);
  std::int8_t destRow   = getRow(toSquare);
  std::int8_t destCol   = getCol(toSquare);

  Square sourceSquare = {sourceRow, sourceCol};
  Square destSquare   = {destRow, destCol};
  Move move(sourceSquare, destSquare, piece, mBoardState, type, capturePiece, promotionPiece);
  moveList.addMove(move);
}

void BitBoard::reset()
{
  for (std::int32_t i = 0; i < 12; i++)
  {
    mBitBoards[i] = 0;
  }

  mBitBoards[PAWN_INDEX]   = (0xFFLL << 8);
  mBitBoards[KNIGHT_INDEX] = (1ULL << B1) | (1ULL << G1);
  mBitBoards[BISHOP_INDEX] = (1ULL << C1) | (1ULL << F1);
  mBitBoards[ROOK_INDEX]   = (1ULL << A1) | (1ULL << H1);
  mBitBoards[QUEEN_INDEX]  = (1ULL << D1);
  mBitBoards[KING_INDEX]   = (1ULL << E1);

  mBitBoards[PAWN_INDEX + 6]   = (0xFFLL << 48);
  mBitBoards[KNIGHT_INDEX + 6] = (1ULL << B8) | (1ULL << G8);
  mBitBoards[BISHOP_INDEX + 6] = (1ULL << C8) | (1ULL << F8);
  mBitBoards[ROOK_INDEX + 6]   = (1ULL << A8) | (1ULL << H8);
  mBitBoards[QUEEN_INDEX + 6]  = (1ULL << D8);
  mBitBoards[KING_INDEX + 6]   = (1ULL << E8);

  mWhiteKingSquare = {0, 4};
  mBlackKingSquare = {7, 4};

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;
  mBoardState.sideToMove      = Color::White;
  mBoardState.castlingRights  = CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN | CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN;
  mBoardState.fullMoveClock   = 1;
  mBoardState.halfMoveClock   = 0;

  updateAggregateBitboards();
}

void BitBoard::setPosition(
  const std::string & fenString)
{
  reset();

  Fen fenData;
  fenData.setFromString(fenString);

  for (std::int32_t i = PAWN_INDEX; i <= KING_INDEX; i++)
  {
    mBitBoards[i]     = 0;
    mBitBoards[i + 6] = 0;
  }

  mBoardState = fenData.getBoardState();

  for (std::int8_t i = 0; i < 8; i++)
  {
    for (std::int8_t j = 0; j < 8; j++)
    {
      std::int8_t index      = getIndex(i, j);
      std::int8_t bbIndex    = getIndex(i, j);
      std::uint64_t bitBoard = (1ULL << bbIndex);
      PieceType pieceType    = fenData.getPieceType(i, j);
      switch (pieceType)
      {
      case PieceType::WhitePawn:
        mBitBoards[PAWN_INDEX] |= bitBoard;
        break;
      case PieceType::WhiteRook:
        mBitBoards[ROOK_INDEX] |= bitBoard;
        break;
      case PieceType::WhiteKnight:
        mBitBoards[KNIGHT_INDEX] |= bitBoard;
        break;
      case PieceType::WhiteBishop:
        mBitBoards[BISHOP_INDEX] |= bitBoard;
        break;
      case PieceType::WhiteQueen:
        mBitBoards[QUEEN_INDEX] |= bitBoard;
        break;
      case PieceType::WhiteKing:
        mBitBoards[KING_INDEX] |= bitBoard;
        mWhiteKingSquare = Square{i, j};
        break;
      case PieceType::BlackPawn:
        mBitBoards[PAWN_INDEX + 6] |= bitBoard;
        break;
      case PieceType::BlackRook:
        mBitBoards[ROOK_INDEX + 6] |= bitBoard;
        break;
      case PieceType::BlackKnight:
        mBitBoards[KNIGHT_INDEX + 6] |= bitBoard;
        break;
      case PieceType::BlackBishop:
        mBitBoards[BISHOP_INDEX + 6] |= bitBoard;
        break;
      case PieceType::BlackQueen:
        mBitBoards[QUEEN_INDEX + 6] |= bitBoard;
        break;
      case PieceType::BlackKing:
        mBitBoards[KING_INDEX + 6] |= bitBoard;
        mBlackKingSquare = Square{i, j};
        break;
      default:
        break;
      }
    }
  }

  updateAggregateBitboards();
}

void BitBoard::unmakeMove(
  const Move & move)
{
  // Do some general calculations
  BoardState boardState        = move.getBoardState();
  Color sideThatMoved          = boardState.sideToMove;
  Square sourceSquare          = move.getSourceSquare();
  Square destSquare            = move.getDestinationSquare();
  Piece movedPiece             = move.getPiece();
  std::int8_t sourceIndex      = getIndex(sourceSquare.row, sourceSquare.col);
  std::int8_t destIndex        = getIndex(destSquare.row, destSquare.col);
  std::uint64_t sourceBitBoard = (1ULL << sourceIndex);
  std::uint64_t destBitBoard   = (1ULL << destIndex);

  // Get the bitboard index for the piece that moved
  std::int32_t movedPieceIndex = static_cast<std::int32_t>(movedPiece);
  if (sideThatMoved == Color::Black)
  {
    movedPieceIndex += 6;
  }

  // Reset the board state
  mBoardState = boardState;

  // Handle quiet moves
  if (move.isQuiet())
  {
    mBitBoards[movedPieceIndex] ^= destBitBoard;
    mBitBoards[movedPieceIndex] |= sourceBitBoard;
  }

  // Handle double push moves
  if (move.isEnpassantPush())
  {
    mBitBoards[movedPieceIndex] ^= destBitBoard;
    mBitBoards[movedPieceIndex] |= sourceBitBoard;
  }

  // Handle standard promotion moves
  if (move.isPromotion() && !move.isPromotionCapture())
  {
    Piece promotedPiece             = move.getPromotedPiece();
    std::int32_t promotedPieceIndex = static_cast<std::int32_t>(promotedPiece);
    if (sideThatMoved == Color::Black)
    {
      promotedPieceIndex += 6;
    }

    mBitBoards[promotedPieceIndex] ^= destBitBoard;
    mBitBoards[movedPieceIndex] |= sourceBitBoard;
  }

  // Handle captures
  if (move.isCapture())
  {
    Piece capturePiece             = move.getCapturedPiece();
    std::int32_t capturePieceIndex = static_cast<std::int32_t>(capturePiece);
    if (sideThatMoved == Color::White)
    {
      capturePieceIndex += 6;
    }

    if (move.isPromotionCapture())
    {
      Piece promotedPiece             = move.getPromotedPiece();
      std::int32_t promotedPieceIndex = static_cast<std::int32_t>(promotedPiece);
      if (sideThatMoved == Color::Black)
      {
        promotedPieceIndex += 6;
      }

      mBitBoards[movedPieceIndex] |= sourceBitBoard;
      mBitBoards[capturePieceIndex] |= destBitBoard;
      mBitBoards[promotedPieceIndex] ^= destBitBoard;
    }
    else if (move.isEnpassantCapture())
    {
      std::int8_t dir            = (sideThatMoved == Color::White ? -1 : +1);
      std::int8_t enPassantIndex = getIndex(destSquare.row + dir, boardState.enpassantColumn);
      std::uint64_t epBitBoard   = (1ULL << enPassantIndex);

      mBitBoards[movedPieceIndex] ^= destBitBoard;
      mBitBoards[movedPieceIndex] |= sourceBitBoard;
      mBitBoards[capturePieceIndex] |= epBitBoard;
    }
    else // Standard captures
    {
      mBitBoards[movedPieceIndex] ^= destBitBoard;
      mBitBoards[movedPieceIndex] |= sourceBitBoard;
      mBitBoards[capturePieceIndex] |= destBitBoard;
    }
  }

  // Handle castle moves
  if (move.isCastle())
  {
    mBitBoards[movedPieceIndex] ^= destBitBoard;
    mBitBoards[movedPieceIndex] |= sourceBitBoard;

    if (destIndex == C1)
    {
      mBitBoards[ROOK_INDEX] ^= (1ULL << D1);
      mBitBoards[ROOK_INDEX] |= (1ULL << A1);
    }

    if (destIndex == G1)
    {
      mBitBoards[ROOK_INDEX] ^= (1ULL << F1);
      mBitBoards[ROOK_INDEX] |= (1ULL << H1);
    }

    if (destIndex == C8)
    {
      mBitBoards[ROOK_INDEX + 6] ^= (1ULL << D8);
      mBitBoards[ROOK_INDEX + 6] |= (1ULL << A8);
    }

    if (destIndex == G8)
    {
      mBitBoards[ROOK_INDEX + 6] ^= (1ULL << F8);
      mBitBoards[ROOK_INDEX + 6] |= (1ULL << H8);
    }
  }

  // Handle king moves
  if (movedPiece == Piece::King)
  {
    if (sideThatMoved == Color::White)
    {
      mWhiteKingSquare = sourceSquare;
    }
    else
    {
      mBlackKingSquare = sourceSquare;
    }
  }

  // Update the aggregated bitboards
  updateAggregateBitboards();
}

void BitBoard::updateAggregateBitboards()
{
  mWhitePieces = 0;
  mBlackPieces = 0;
  for (std::int32_t i = PAWN_INDEX; i <= KING_INDEX; i++)
  {
    mWhitePieces |= mBitBoards[i];
    mBlackPieces |= mBitBoards[i + 6];
  }
  mAllPieces = mWhitePieces | mBlackPieces;
}

void BitBoard::writeBitBoard(
  std::uint64_t bb,
  std::ostream & output) const
{
  static const char files[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};

  for (char row = 7; row >= 0; row--)
  {
    output << (row + 1) << " ";
    for (char col = 0; col < 8; col++)
    {
      int shift = getIndex(row, col);
      if ((bb >> shift) & 0x1)
        output << "X ";
      else
        output << ". ";
    }
    output << "\n";
  }

  output << "  ";
  for (uint8_t col = 0; col < 8; col++)
  {
    output << files[col] << " ";
  }
  output << "\n\n";
}

} // namespace jmchess
