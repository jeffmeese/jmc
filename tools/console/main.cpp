#include "console_game.h"

int main(
  int argc,
  char ** argv)
{
  std::unique_ptr<ConsoleGame> game(new ConsoleGame);
  game->execute();

  return 0;
}
