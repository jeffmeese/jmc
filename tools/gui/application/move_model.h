#pragma once

#include <QAbstractTableModel>

#include "gui_game.h"
#include "move.h"

class MoveModel : public QAbstractTableModel
{
  Q_OBJECT

public:
  explicit MoveModel(QObject * parent = nullptr);

  int columnCount(const QModelIndex & parent = QModelIndex()) const override;

  QVariant data(
    const QModelIndex & index,
    int role = Qt::DisplayRole) const override;

  int rowCount(const QModelIndex & parent = QModelIndex()) const override;

  void setGame(GuiGame * game);

public slots:
  void slotMoveMade(const jmchess::Move & move);
  void slotMoveUndone();

private:
  std::vector<jmchess::Move> mMoves;
  GuiGame * mGame = nullptr;
};
