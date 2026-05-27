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

bool Search::execute(
  Board * board,
  Color sideToMove,
  std::int32_t depth,
  Move & bestMove) const
{
  if (depth <= 0)
  {
    throw std::invalid_argument("Depth must be greater than 0");
  }

  MoveList moveList;
  board->generateMoves(moveList);

  std::int32_t legalMoves    = 0;
  std::int32_t bestMoveIndex = -1;
  double bestScore = -std::numeric_limits<double>::infinity();

  if (mAlgorithm == SearchAlgorithm::AlphaBeta)
  {
    double alpha     = -std::numeric_limits<double>::infinity();
    double beta      = std::numeric_limits<double>::infinity();
    for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
    {
      const Move & move = moveList.getMove(i);
      bool isLegal      = board->makeMove(move);
      if (isLegal)
      {
        legalMoves++;

        double score = -alphaBeta(board, board->getBoardState().sideToMove, alpha, beta, depth - 1, bestMoveIndex);
        if (score > bestScore)
        {
          bestScore     = score;
          bestMoveIndex = i;
        }

        std::cout << move.toSmithNotation() << ": " << score << "\n";
        board->unmakeMove(move);
      }
    }
  }
  else
  {
    for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
    {
      const Move & move = moveList.getMove(i);
      bool isLegal      = board->makeMove(move);
      if (isLegal)
      {
        legalMoves++;

        double score = -negamax(board, board->getBoardState().sideToMove, depth - 1, bestMoveIndex);
        if (score > bestScore)
        {
          bestScore     = score;
          bestMoveIndex = i;
        }
        std::cout << move.toSmithNotation() << ": " << score << "\n";
        board->unmakeMove(move);
      }
    }
  }

  // If there are no legal moves, there is either
  // a checkmate or stalemate.
  if (legalMoves == 0)
  {
    return false;
  }

  bestMove = moveList.getMove(bestMoveIndex);
  return true;
}

double Search::alphaBeta(
  Board * board,
  Color sideToMove,
  double alpha,
  double beta,
  std::int32_t depth,
  std::int32_t & moveIndex) const
{
  if (depth == 0)
  {
    return mEvaluation->evaluate(board);
  }

  MoveList moveList;
  board->generateMoves(moveList);

  double bestScore           = -std::numeric_limits<double>::infinity();
  std::int32_t bestMoveIndex = -1;
  std::int32_t legalMoves    = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    bool isLegal      = board->makeMove(move);
    if (isLegal)
    {
      legalMoves++;

      double score = -alphaBeta(board, board->getBoardState().sideToMove, -beta, -alpha, depth - 1, bestMoveIndex);
      if (score > bestScore)
      {
        bestScore     = score;
        bestMoveIndex = i;

        if (score > alpha)
        {
          alpha = score;
        }
      }

      if (score >= beta)
      {
        board->unmakeMove(move);
        bestMoveIndex = i;
        return bestScore;
      }
      board->unmakeMove(move);
    }
  }

  if (legalMoves == 0)
  {
  }

  return bestScore;
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

  double bestScore           = -std::numeric_limits<double>::infinity();
  std::int32_t bestMoveIndex = -1;
  std::int32_t legalMoves    = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    bool isLegal      = board->makeMove(move);
    bool attacked     = board->isKingInCheck(sideToMove);
    if (isLegal)
    {
      legalMoves++;

      double score = -negamax(board, board->getBoardState().sideToMove, depth - 1, bestMoveIndex);
      if (score > bestScore)
      {
        bestScore     = score;
        bestMoveIndex = i;
      }
      board->unmakeMove(move);
    }
  }

  if (legalMoves == 0)
  {
  }

  return bestScore;
}

Move Search::runAlphaBeta(
  Board * board,
  Color sideToMove,
  std::int32_t depth) const
{
  MoveList moveList;
  board->generateMoves(moveList);

  double alpha               = -std::numeric_limits<double>::infinity();
  double beta                = std::numeric_limits<double>::infinity();
  double bestScore           = -std::numeric_limits<double>::infinity();
  std::int32_t bestMoveIndex = -1;
  std::int32_t legalMoves    = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    bool isLegal      = board->makeMove(move);
    if (isLegal)
    {
      legalMoves++;

      double score = -alphaBeta(board, board->getBoardState().sideToMove, alpha, beta, depth - 1, bestMoveIndex);
      if (score > bestScore)
      {
        bestScore     = score;
        bestMoveIndex = i;
      }

      std::cout << move.toSmithNotation() << ": " << score << "\n";
      board->unmakeMove(move);
    }
  }

  // If there are no legal moves, there is either
  // a checkmate or stalemate.
  if (legalMoves == 0)
  {
  }

  return moveList.getMove(bestMoveIndex);
}

Move Search::runNegamax(
  Board * board,
  Color sideToMove,
  std::int32_t depth) const
{
  MoveList moveList;
  board->generateMoves(moveList);

  double bestScore           = -std::numeric_limits<double>::infinity();
  std::int32_t bestMoveIndex = -1;
  std::int32_t legalMoves    = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    bool isLegal      = board->makeMove(move);
    if (isLegal)
    {
      legalMoves++;

      double score = -negamax(board, board->getBoardState().sideToMove, depth - 1, bestMoveIndex);
      if (score > bestScore)
      {
        bestScore     = score;
        bestMoveIndex = i;
      }
      std::cout << move.toSmithNotation() << ": " << score << "\n";
      board->unmakeMove(move);
    }
  }

  if (legalMoves == 0)
  {
  }

  return moveList.getMove(bestMoveIndex);
}

} // namespace jmchess
