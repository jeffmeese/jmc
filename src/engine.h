#pragma once

#include <memory>

#include "board.h"
#include "search.h"

namespace jmchess
{

class Engine
{
public:
  Move getMove(
    Board * board,
    Search * search) const;

private:
  std::int32_t mSearchDepth = 3;
};

} // namespace jmchess
