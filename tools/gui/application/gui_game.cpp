#include "gui_game.h"

GuiGame::GuiGame(QObject * parent)
  : QObject(parent)
{
}

void GuiGame::makeMove(const jmchess::Move & move)
{
  Game::makeMove(move);
  emit moveMade(move);

  if (isCheckmate())
  {
    emit checkmate();
  }
  else if (isStalemate())
  {
    emit stalemate();
  }
}

void GuiGame::unmakeMove()
{
  Game::undoLastMove();
  //emit moveUndone(move);
}