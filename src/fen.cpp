#include "fen.h"

#include <cstdint>
#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "board.h"
#include "board_state.h"
#include "color.h"
#include "piece_type.h"
#include "string_utilities.h"

namespace jmchess
{

Fen::Fen()
{
  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;
  mBoardState.castlingRights = CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN | CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN;
  mBoardState.halfMoveClock = 0;
  mBoardState.fullMoveClock = 1;
  mBoardState.sideToMove = Color::White;

  for (uint8_t i = 0; i < 8; i++)
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      mPieceTypes[i][j] = PieceType::None;
    }
  }

  for (uint8_t i = 0; i < 8; i++)
  {
    mPieceTypes[1][i] = PieceType::WhitePawn;
    mPieceTypes[6][i] = PieceType::BlackPawn;
  }

  mPieceTypes[0][0] = mPieceTypes[0][7] = PieceType::WhiteRook;
  mPieceTypes[0][1] = mPieceTypes[0][6] = PieceType::WhiteKnight;
  mPieceTypes[0][2] = mPieceTypes[0][5] = PieceType::WhiteBishop;
  mPieceTypes[0][3] = PieceType::WhiteQueen;
  mPieceTypes[0][4] = PieceType::WhiteKing;

  mPieceTypes[7][0] = mPieceTypes[7][7] = PieceType::BlackRook;
  mPieceTypes[7][1] = mPieceTypes[7][6] = PieceType::BlackKnight;
  mPieceTypes[7][2] = mPieceTypes[7][5] = PieceType::BlackBishop;
  mPieceTypes[7][3] = PieceType::BlackQueen;
  mPieceTypes[7][4] = PieceType::BlackKing;
}

BoardState Fen::getBoardState() const
{
  return mBoardState;
}

PieceType Fen::getPieceType(uint8_t row, uint8_t col) const
{
  return mPieceTypes[row][col];
}

void Fen::init()
{

}

void Fen::setBoardState(const BoardState & value)
{
  mBoardState = value;
}

bool Fen::setFromBoard(const Board * board)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      mPieceTypes[i][j] = board->getPieceType(i, j);
    }
  }

  mBoardState = board->getBoardState(); 
  return true;
}

bool Fen::setFromString(const std::string & fenString)
{
  std::string s(fenString);
  std::vector<std::string> tokens = splitString(trimString(s));
  if (tokens.size() != 6)
  {
    std::cerr << "Invalid FEN string: " << fenString << std::endl;
    return false;
  }

  std::string pieceString = tokens[0];
  std::string sideString = tokens[1];
  std::string castleString = tokens[2];
  std::string epString = tokens[3];
  std::string halfMoveString = tokens[4];
  std::string fullMoveString = tokens[5];

  std::vector<std::string> pieceTokens = splitString(pieceString, "/");
  if (pieceTokens.size() != 8)
  {
    std::cerr << "Invalid number of tokens in piece data in FEN string: " << pieceString << std::endl;
    return false;
  }

  std::size_t pos = pieceString.find_first_not_of("rnbqkpRNBQKP12345678/");
  if (pos != std::string::npos)
  {
    std::cerr << "Invalid piece data in FEN string: " << pieceString << std::endl;
    return false;
  }

  pos = sideString.find_first_not_of("bw");
  if (pos != std::string::npos)
  {
    std::cerr << "Invalid side data in FEN string: " << sideString << std::endl;
    return false;
  }

  pos = castleString.find_first_not_of("KQkq-");
  if (pos != std::string::npos)
  {
    std::cerr << "Invalid castle data in FEN string: " << castleString << std::endl;
    return false;
  }

  pos = epString.find_first_not_of("abcdefgh36-");
  if (pos != std::string::npos)
  {
    std::cerr << "Invalid en-passant data in FEN string: " << epString << std::endl;
    return false;
  }

  for (uint8_t i = 0; i < 8; i++)
  {
    for (uint8_t j = 0; j < 8; j++)
    {
      mPieceTypes[i][j] = PieceType::None;
    }
  }

  for (int8_t i = 0; i < 8; i++)
  {
    int8_t row = 7 - i;
    uint8_t columnIndex = 0;
    std::string rowString = pieceTokens[i];
    if (rowString.size() > 8)
    {
      std::cerr << "Invalid piece string in FEN string: " << rowString << std::endl;
      return false;
    }

    for (std::size_t j = 0; j < rowString.size(); j++)
    {
      if (rowString[j] == 'P')
      {
        mPieceTypes[row][columnIndex] = PieceType::WhitePawn;
        columnIndex++;
      }
      else if (rowString[j] == 'R')
      {
        mPieceTypes[row][columnIndex] = PieceType::WhiteRook;
        columnIndex++;
      }
      else if (rowString[j] == 'N')
      {
        mPieceTypes[row][columnIndex] = PieceType::WhiteKnight;
        columnIndex++;
      }
      else if (rowString[j] == 'B')
      {
        mPieceTypes[row][columnIndex] = PieceType::WhiteBishop;
        columnIndex++;
      }
      else if (rowString[j] == 'Q')
      {
        mPieceTypes[row][columnIndex] = PieceType::WhiteQueen;
        columnIndex++;
      }
      else if (rowString[j] == 'K')
      {
        mPieceTypes[row][columnIndex] = PieceType::WhiteKing;
        columnIndex++;
      }
      else if (rowString[j] == 'p')
      {
        mPieceTypes[row][columnIndex] = PieceType::BlackPawn;
        columnIndex++;
      }
      else if (rowString[j] == 'r')
      {
        mPieceTypes[row][columnIndex] = PieceType::BlackRook;
        columnIndex++;
      }
      else if (rowString[j] == 'n')
      {
        mPieceTypes[row][columnIndex] = PieceType::BlackKnight;
        columnIndex++;
      }
      else if (rowString[j] == 'b')
      {
        mPieceTypes[row][columnIndex] = PieceType::BlackBishop;
        columnIndex++;
      }
      else if (rowString[j] == 'q')
      {
        mPieceTypes[row][columnIndex] = PieceType::BlackQueen;
        columnIndex++;
      }
      else if (rowString[j] == 'k')
      {
        mPieceTypes[row][columnIndex] = PieceType::BlackKing;
        columnIndex++;
      }
      else
      {
        int32_t value = rowString[j] - '0';
        columnIndex += value;
      }
    }
  }

  mBoardState.sideToMove = (sideString == "w") ? Color::White : Color::Black;

  mBoardState.castlingRights = CASTLE_NONE;
  if (castleString != "-")
  {
    if (castleString.find_first_of("K") != std::string::npos)
    {
      mBoardState.castlingRights |= CASTLE_WHITE_KING;
    }

    if (castleString.find_first_of("Q") != std::string::npos)
    {
      mBoardState.castlingRights |= CASTLE_WHITE_QUEEN;
    }

    if (castleString.find_first_of("k") != std::string::npos)
    {
      mBoardState.castlingRights |= CASTLE_BLACK_KING;
    }

    if (castleString.find_first_of("q") != std::string::npos)
    {
      mBoardState.castlingRights |= CASTLE_BLACK_QUEEN;
    }
  }

  mBoardState.enpassantColumn = INVALID_ENPASSANT_COLUMN;
  if (epString != "-")
  {
    std::map<std::string, uint8_t> epMap;
    epMap["a"] = 0;
    epMap["b"] = 1;
    epMap["c"] = 2;
    epMap["d"] = 3;
    epMap["e"] = 4;
    epMap["f"] = 5;
    epMap["g"] = 6;
    epMap["h"] = 7;

    std::string epColumn = epString.substr(0,1);
    mBoardState.enpassantColumn = epMap[epColumn];
  }

  mBoardState.halfMoveClock = std::atoi(halfMoveString.c_str());
  mBoardState.fullMoveClock = std::atoi(fullMoveString.c_str());

  return false;
}

void Fen::setPieceType(uint8_t row, uint8_t col, PieceType pieceType)
{
  mPieceTypes[row][col] = pieceType;
}

std::string Fen::toString() const
{
  std::ostringstream oss;

  for (int8_t i = 7; i >= 0; i--)
  {
    uint32_t blankSpaces = 0;
    for (uint8_t j = 0; j < 8; j++)
    {
      PieceType pieceType = mPieceTypes[i][j];

      if (pieceType == PieceType::None)
      {
        blankSpaces++;
      }
      else
      {
        if (blankSpaces > 0)
        {
          oss << blankSpaces;
        }

        switch (pieceType)
        {
        case PieceType::WhitePawn:
          oss << "P";
          break;
        case PieceType::WhiteKnight:
          oss << "N";
          break;
        case PieceType::WhiteBishop:
          oss << "B";
          break;
        case PieceType::WhiteRook:
          oss << "R";
          break;
        case PieceType::WhiteQueen:
          oss << "Q";
          break;
        case PieceType::WhiteKing:
          oss << "K";
          break;
        case PieceType::BlackPawn:
          oss << "p";
          break;
        case PieceType::BlackKnight:
          oss << "n";
          break;
        case PieceType::BlackBishop:
          oss << "b";
          break;
        case PieceType::BlackRook:
          oss << "r";
          break;
        case PieceType::BlackQueen:
          oss << "q";
          break;
        case PieceType::BlackKing:
          oss << "k";
          break;
        default:
          break;
        }

        blankSpaces = 0;
      }
    }

    if (blankSpaces > 0)
    {
      oss << blankSpaces;
    }

    if (i > 0)
    {
      oss << "/";
    }
    else
    {
      oss << " ";
    }
  }

  if (mBoardState.sideToMove == Color::White)
  {
    oss << "w ";
  }
  else
  {
    oss << "b ";
  }

  if (mBoardState.castlingRights == CASTLE_NONE)
  {
    oss << "- ";
  }
  else
  {
    if (mBoardState.castlingRights & CASTLE_WHITE_KING)
    {
      oss << "K";
    }
    if (mBoardState.castlingRights & CASTLE_WHITE_QUEEN)
    {
      oss << "Q";
    }
    if (mBoardState.castlingRights & CASTLE_BLACK_KING)
    {
      oss << "k";
    }
    if (mBoardState.castlingRights & CASTLE_BLACK_QUEEN)
    {
      oss << "q";
    }

    oss << " ";
  }

  if (mBoardState.enpassantColumn == INVALID_ENPASSANT_COLUMN)
  {
    oss << "- ";
  }
  else
  {
    std::string columnName[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    oss << columnName[mBoardState.enpassantColumn];
    if (mBoardState.sideToMove == Color::White)
    {
      oss << "6";
    }
    else
    {
      oss << "3";
    }
    oss << " ";
  }

  oss << static_cast<int32_t>(mBoardState.halfMoveClock) << " ";
  oss << static_cast<int32_t>(mBoardState.fullMoveClock);

  return oss.str();
}

}
