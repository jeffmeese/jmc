#include "search.h"

#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <stdexcept>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "evaluation.h"
#include "move.h"
#include "move_list.h"

namespace jmchess
{

Search::Search()
{
  mEvaluation.reset(new Evaluation);
}

Move Search::execute(
  Board * board,
  Color sideToMove,
  std::int32_t depth) const
{
  if (depth <= 0)
  {
    throw std::invalid_argument("Depth must be greater than 0");
  }

  MoveList moveList;
  board->generateMoves(moveList);

  double bestScore = -std::numeric_limits<double>::infinity();
  std::int32_t bestMoveIndex = -1;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    board->makeMove(move);
    double score = -negamax(board, sideToMove, depth - 1, bestMoveIndex);
    board->unmakeMove(move);

    std::cout << move.toSmithNotation() << ": " << score << "\n";

    if (score > bestScore)
    {
      bestScore = score;
      bestMoveIndex = i;
    }
  }

  return moveList.getMove(bestMoveIndex);
}

double Search::negamax(
  Board * board,
  Color sideToMove,
  std::int32_t depth, 
  std::int32_t & moveIndex) const
{
  if (depth == 0)
  {
    return mEvaluation->evaluate(board);
  }

  MoveList moveList;
  board->generateMoves(moveList);

  double bestScore = -std::numeric_limits<double>::infinity();
  std::int32_t bestMoveIndex = -1;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    board->makeMove(move);
    double score = -negamax(board, sideToMove, depth - 1, bestMoveIndex);
    board->unmakeMove(move);

    if (score > bestScore)
    {
      bestScore = score;
      bestMoveIndex = i;
    }
  }

  return bestScore;
}

} // namespace jmchess
