/*!
 * \file perft.cpp
 *
 * This file contains the implementation for the Perft object
 */

#include "perft.h"

#include <iostream>
#include <string>

#include "board.h"
#include "move.h"
#include "move_list.h"

namespace jmchess
{

Perft::Perft(
  Board * board)
  : mBoard(board)
{
}

void Perft::divide(
  std::uint32_t perftDepth)
{
  BoardState boardState = mBoard->getBoardState();
  Color sideToMove      = boardState.sideToMove;
  Color oppositeColor   = (sideToMove == Color::White) ? Color::Black : Color::White;

  MoveList moveList;
  mBoard->generateMoves(moveList);
  std::uint64_t validMoves = 0;
  std::uint64_t totalNodes = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);

    mBoard->makeMove(move);
    bool attacked = mBoard->isKingInCheck(sideToMove);
    if (!attacked)
    {
      std::uint64_t nodes    = executePerft(perftDepth - 1);
      std::string moveString = move.toSmithNotation();
      std::cout << moveString << ": " << nodes << "\n";
      totalNodes += nodes;
      validMoves++;
    }

    mBoard->unmakeMove(move);
  }

  std::cout << "\n";
  std::cout << "Moves: " << validMoves << "\n";
  std::cout << "Nodes: " << totalNodes << "\n";
  std::cout << std::endl;
}

std::uint64_t Perft::execute(
  std::uint32_t perftDepth)
{
  return executePerft(perftDepth);
}

std::uint64_t Perft::executePerft(
  std::uint32_t perftDepth)
{
  if (perftDepth == 0)
  {
    return 1;
  }

  BoardState boardState = mBoard->getBoardState();
  Color sideToMove      = boardState.sideToMove;
  Color oppositeColor   = (sideToMove == Color::White) ? Color::Black : Color::White;

  MoveList moveList;
  mBoard->generateMoves(moveList);

  std::uint64_t totalNodes = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);

    mBoard->makeMove(move);
    bool attacked = mBoard->isKingInCheck(sideToMove);
    if (!attacked)
    {
      totalNodes += executePerft(perftDepth - 1);
    }
    mBoard->unmakeMove(move);
  }

  return totalNodes;
}

} // namespace jmchess
