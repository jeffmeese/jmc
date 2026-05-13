#pragma once

#include "board.h"

namespace jmchess
{

class Evaluation
{
public:
  double evaluate(const Board * board);
};


}