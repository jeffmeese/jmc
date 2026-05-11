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

Perft::Perft(Board * board)
  : mBoard(board)
{
}

void Perft::divide(int32_t perftDepth)
{
  BoardState boardState = mBoard->getBoardState();
  Color sideToMove = boardState.sideToMove;
  Color oppositeColor = (sideToMove == Color::White) ? Color::Black : Color::White;

  MoveList moveList;
  mBoard->generateMoves(moveList);
  uint64_t validMoves = 0;
  uint64_t totalNodes = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);

    mBoard->makeMove(move);
    uint8_t kingRow = mBoard->getKingRow(sideToMove);
    uint8_t kingCol = mBoard->getKingColumn(sideToMove);
    bool attacked = mBoard->isCellAttacked(kingRow, kingCol, oppositeColor);
    if (!attacked)
    {
      uint64_t nodes = executePerft(perftDepth - 1);
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
  //std::cout << "Total time elapsed  : " << timer.get_elapsed_ms() << " milliseconds" << std::endl;
}

uint64_t Perft::execute(int32_t perftDepth)
{
  return executePerft(perftDepth);
}

uint64_t Perft::executePerft(int32_t perftDepth)
{
  if (perftDepth == 0)
  {
    return 1;
  }

  BoardState boardState = mBoard->getBoardState();
  Color sideToMove = boardState.sideToMove;
  Color oppositeColor = (sideToMove == Color::White) ? Color::Black : Color::White;

  MoveList moveList;
  mBoard->generateMoves(moveList);

  uint64_t totalNodes = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);

    mBoard->makeMove(move);
    uint8_t kingRow = mBoard->getKingRow(sideToMove);
    uint8_t kingCol = mBoard->getKingColumn(sideToMove);
    bool attacked = mBoard->isCellAttacked(kingRow, kingCol, oppositeColor);
    if (!attacked)
    {
      totalNodes += executePerft(perftDepth-1);
    }
    mBoard->unmakeMove(move);
  }

  return totalNodes;
}

}