#pragma once

#include <cstdint>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "evaluation.h"
#include "move.h"
#include "move_list.h"

namespace jmchess
{

static constexpr double CHECKMATE = 60000;
static constexpr double STALEMATE = 0;

enum class SearchAlgorithm
{
  Negamax   = 0,
  AlphaBeta = 1
};

class Search
{
public:
  Search();

  bool execute(
    Board * board,
    Color sideToMove,
    std::int32_t depth,
    Move & bestMove) const;

//private:
  double alphaBeta(
    Board * board,
    Color sideToMove,
    double alpha,
    double beta,
    std::int32_t depth,
    std::int32_t & moveIndex) const;

  double negamax(
    Board * board,
    Color sideToMove,
    std::int32_t depth,
    std::int32_t & moveIndex) const;

  Move runAlphaBeta(
    Board * board,
    Color sideToMove,
    std::int32_t depth) const;

  Move runNegamax(
    Board * board,
    Color sideToMove,
    std::int32_t depth) const;

  SearchAlgorithm mAlgorithm = SearchAlgorithm::AlphaBeta;
  std::unique_ptr<Evaluation> mEvaluation;
};

} // namespace jmchess
