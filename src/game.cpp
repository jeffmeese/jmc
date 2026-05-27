#include "game.h"

#include <iostream>
#include <memory>

#include "board.h"
#include "engine.h"
#include "search.h"

#include "bitboard.h"
#include "board8x8.h"

namespace jmchess
{

Game::Game()
{
  // mBoard.reset(new Board8x8);
  mBoard.reset(new BitBoard);
  mEngine.reset(new Engine);
  mSearch.reset(new Search);
}

Board * Game::getBoard()
{
  return mBoard.get();
}

const Board * Game::getBoard() const
{
  return mBoard.get();
}

Engine * Game::getEngine()
{
  return mEngine.get();
}

const Engine * Game::getEngine() const
{
  return mEngine.get();
}

Search * Game::getSearch()
{
  return mSearch.get();
}

const Search * Game::getSearch() const
{
  return mSearch.get();
}

bool Game::isCheckmate() const
{
  return mCheckmate;
}

bool Game::isStalemate() const
{
  return mStalemate;
}

void Game::makeMove(
  const Move & move)
{
  if (mCheckmate || mStalemate)
  {
    return;
  }

  mBoard->makeMove(move);
  mMoves.push(move);

  Color sideToMove = mBoard->getBoardState().sideToMove;
  MoveList moveList;
  mBoard->generateMoves(moveList);

  std::int32_t legalMoves = 0;
  for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
  {
    const Move & move = moveList.getMove(i);
    bool isLegal      = mBoard->makeMove(move);
    if (isLegal)
    {
      legalMoves++;
      mBoard->unmakeMove(move);
    }
  }

  bool kingInCheck = mBoard->isKingInCheck(mBoard->getBoardState().sideToMove);
  mCheckmate = (legalMoves == 0 && kingInCheck);
  mStalemate = (legalMoves == 0 && !kingInCheck);
}

void Game::performEngineCommand()
{
  std::int32_t depth        = 3;
  SearchAlgorithm algorithm = SearchAlgorithm::AlphaBeta;
  Board * board             = mBoard.get();
  Color sideToMove          = board->getBoardState().sideToMove;

  MoveList moveList;
  mBoard->generateMoves(moveList);

  std::int32_t legalMoves    = 0;
  std::int32_t bestMoveIndex = -1;
  double bestScore           = -std::numeric_limits<double>::infinity();
  if (algorithm == SearchAlgorithm::AlphaBeta)
  {
    double alpha = -std::numeric_limits<double>::infinity();
    double beta  = std::numeric_limits<double>::infinity();
    for (std::int32_t i = 0; i < moveList.totalMoves(); i++)
    {
      const Move & move = moveList.getMove(i);
      bool isLegal      = board->makeMove(move);
      if (isLegal)
      {
        legalMoves++;

        double score =
          -mSearch->alphaBeta(board, sideToMove, alpha, beta, depth - 1, bestMoveIndex);
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

        double score = -mSearch->negamax(board, sideToMove, depth - 1, bestMoveIndex);
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
  mCheckmate = false;
  mStalemate = false;
  if (legalMoves == 0)
  {
    bool kingInCheck = mBoard->isKingInCheck(mBoard->getBoardState().sideToMove);
    mCheckmate = kingInCheck;
    mStalemate = !kingInCheck;
    return;
  }

  makeMove(moveList.getMove(bestMoveIndex));
}

void Game::setBoard(
  std::unique_ptr<Board> board)
{
  mBoard = std::move(board);
}

void Game::setSearch(
  std::unique_ptr<Search> search)
{
  mSearch = std::move(search);
}

void Game::startNew()
{
  mBoard->reset();
  while (!mMoves.empty())
  {
    mMoves.pop();
  }
  mCheckmate = false;
  mStalemate = false;
}

void Game::undoLastMove()
{
  if (mMoves.size() == 0)
  {
    return;
  }

  Move move = mMoves.top();
  mBoard->unmakeMove(move);
  mMoves.pop();

  mCheckmate = false;
  mStalemate = false;
}
} // namespace jmchess
