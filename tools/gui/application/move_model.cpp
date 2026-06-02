#include "move_model.h"

MoveModel::MoveModel(
  QObject * parent)
  : QAbstractTableModel{parent}
{
}

int MoveModel::columnCount(
  const QModelIndex & parent) const
{
  return 2;
}

QVariant MoveModel::data(
  const QModelIndex & index,
  int role) const
{
  if (!index.isValid())
  {
    return QVariant();
  }

  if (mGame != nullptr && role == Qt::DisplayRole)
  {
    int vectorIndex = (index.row() * 2) + index.column();
    if (vectorIndex >= mMoves.size())
    {
      return QVariant();
    }

    const jmchess::Move & move = mMoves[vectorIndex];
    jmchess::Board * board = mGame->getBoard();
    return QString::fromStdString(move.toAlgebraicNotation(board));
  }

  return QVariant();
}

int MoveModel::rowCount(
  const QModelIndex & parent) const
{
  return mMoves.size() / 2 + mMoves.size() % 2;
}

void MoveModel::setGame(
  GuiGame * game)
{
  beginResetModel();
  mGame = game;
  mMoves.clear();
  endResetModel();
}

void MoveModel::slotMoveMade(
  const jmchess::Move & move)
{
  if (mMoves.size() % 2 == 0)
  {
    //beginInsertRows(QModelIndex(), mMoves.size() / 2, mMoves.size() / 2);
  }

  mMoves.push_back(move);
  emit layoutChanged();

  if (mMoves.size() % 2 > 0)
  {
    //endInsertRows();
  }
}

void MoveModel::slotMoveUndone()
{
  if (mMoves.size() > 0)
  {
    beginRemoveRows(QModelIndex(), (mMoves.size() - 1) / 2, (mMoves.size() - 1) / 2);
    mMoves.pop_back();
    endRemoveRows();
  }
}
