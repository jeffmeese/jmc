#pragma once

#include <memory>

#include "board.h"
#include "search.h"

namespace jmchess
{

class Engine
{
public:
  Engine();

  Move getMove(
    Board * board) const;

private:
  std::unique_ptr<Search> mSearch;
  std::int32_t mSearchDepth = 3;
};

}