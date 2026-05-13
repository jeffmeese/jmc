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

class Search
{
public:
  Search();

  Move execute(
    Board * board,
    Color sideToMove,
    std::int32_t depth) const;

private:

  double negamax(
    Board * board,
    Color sideToMove,
    std::int32_t depth,
    std::int32_t & moveIndex) const;

  std::unique_ptr<Evaluation> mEvaluation;
};

} // namespace jmchess
