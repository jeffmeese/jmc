#pragma once

#include "color.h"
#include "piece.h"

namespace jmchess
{

struct Cell
{
  Piece piece;
  Color color;
};

} // namespace jmchess
