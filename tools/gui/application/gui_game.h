#pragma once

#include <QObject>

#include "game.h"

class GuiGame : public QObject, public jmchess::Game
{
  Q_OBJECT

public:
  explicit GuiGame(QObject * parent = nullptr);

  void makeMove(const jmchess::Move & move);
  void unmakeMove();

signals:
  void checkmate();
  void moveMade(const jmchess::Move & move);
  void moveUndone(const jmchess::Move & move);
  void stalemate();

private:

};