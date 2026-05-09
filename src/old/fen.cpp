#include "fen.h"

#include <iostream>
#include <map>
#include <sstream>
#include <vector>

#include "consts.h"

namespace jmchess
{

std::vector<std::string> splitString(const std::string & s, const std::string & delim = " ")
{
  std::vector<std::string> tokens;

  std::size_t startPos = 0;
  while (true)
  {
    std::size_t endPos = s.find_first_of(delim, startPos);
    if (endPos == std::string::npos)
    {
      tokens.push_back(s.substr(startPos, endPos-startPos));
      break;
    }

    tokens.push_back(s.substr(startPos, endPos-startPos));
    startPos = endPos + 1;
  }

  return tokens;
}

Fen::Fen()
{
  init();
}

uint8_t Fen::getCastlingRights() const
{
  return mCastlingRights;
}

uint8_t Fen::getEnpassantColumn() const
{
  return mEnPassantColumn;
}

uint16_t Fen::getFullMoveCounter() const
{
  return mFullMoveCounter;
}

uint16_t Fen::getHalfMoveClock() const
{
  return mHalfMoveClock;
}

PieceType Fen::getPieceType(uint8_t row, uint8_t col) const
{
  return mPieceTypes[row][col];
}

Color Fen::getSideToMove() const
{
  return mSideToMove;
}

void Fen::init()
{
  mEnPassantColumn = INVALID_EP_COLUMN;
  mCastlingRights = CASTLE_BLACK_KING | CASTLE_BLACK_QUEEN | CASTLE_WHITE_KING | CASTLE_WHITE_QUEEN;
  mHalfMoveClock = 0;
  mFullMoveCounter = 1;
  mSideToMove = Color::White;

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

void Fen::setCastlingRights(uint8_t value)
{
  mCastlingRights = value;
}

void Fen::setEnpassantColumn(uint8_t value)
{
  mEnPassantColumn = value;
}

void Fen::setFullMoveCounter(uint16_t value)
{
  mFullMoveCounter = value;
}

void Fen::setHalfMoveClock(uint16_t value)
{
  mHalfMoveClock = value;
}

void Fen::setPieceType(uint8_t row, uint8_t col, PieceType pieceType)
{
  mPieceTypes[row][col] = pieceType;
}

void Fen::setSideToMove(Color color)
{
  mSideToMove = color;
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

  mEnPassantColumn = board->getEnpassantColumn();
  mHalfMoveClock = board->getHalfMoveClock();
  mFullMoveCounter = board->getFullMoveCounter();
  mSideToMove = board->getSideToMove();
  mCastlingRights = board->getCastlingRights();

  return true;
}

bool Fen::setFromString(const std::string & fenString)
{
  std::vector<std::string> tokens = splitString(fenString);
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

  std::vector<std::string> rowStrings = splitString(pieceString, "/");
  if (rowStrings.size() != 8)
  {
    std::cerr << "Invalid piece string in FEN string: " << pieceString << std::endl;
  }

  for (std::size_t i = 0; i < rowStrings.size(); i++)
  {
    std::string s = rowStrings[i];
    std::size_t pos = s.find_first_not_of("rnbqkpRNBQKP12345678");
    if (pos != std::string::npos)
    {
      std::cerr << "Invalid piece string in FEN string: " << pieceString << std::endl;
      return false;
    }

    int columnIndex = 0;
    for (std::size_t j = 0; j < s.size(); j++)
    {
      char c = s[i];
      switch (c)
      {
      case 'R':
        mPieceTypes[i][columnIndex] = PieceType::WhiteRook;
        columnIndex++;
        break;
      case 'N':
        mPieceTypes[i][columnIndex] = PieceType::WhiteKnight;
        columnIndex++;
        break;
      case 'B':
        mPieceTypes[i][columnIndex] = PieceType::WhiteBishop;
        columnIndex++;
        break;
      case 'Q':
        mPieceTypes[i][columnIndex] = PieceType::WhiteQueen;
        columnIndex++;
        break;
      case 'K':
        mPieceTypes[i][columnIndex] = PieceType::WhiteKing;
        columnIndex++;
        break;
      case 'P':
        mPieceTypes[i][columnIndex] = PieceType::WhitePawn;
        columnIndex++;
        break;
      case 'r':
        mPieceTypes[i][columnIndex] = PieceType::BlackRook;
        columnIndex++;
        break;
      case 'n':
        mPieceTypes[i][columnIndex] = PieceType::BlackKnight;
        columnIndex++;
        break;
      case 'b':
        mPieceTypes[i][columnIndex] = PieceType::BlackBishop;
        columnIndex++;
        break;
      case 'q':
        mPieceTypes[i][columnIndex] = PieceType::BlackQueen;
        columnIndex++;
        break;
      case 'k':
        mPieceTypes[i][columnIndex] = PieceType::BlackKing;
        columnIndex++;
        break;
      case 'p':
        mPieceTypes[i][columnIndex] = PieceType::BlackPawn;
        columnIndex++;
        break;
      case '1':
      case '2':
      case '3':
      case '4':
      case '5':
      case '6':
      case '7':
      case '8':
        columnIndex += c - '0';
        break;
      }
    }
  }

  if (sideString != "w" && sideString != "b")
  {
    std::cerr << "Invalid side to move in FEN string: " << sideString << std::endl;
    return false;
  }
  mSideToMove = (sideString == "w") ? Color::White : Color::Black;

  std::size_t pos = castleString.find_first_not_of("KQkq-");
  if (pos != std::string::npos)
  {
    std::cerr << "Invalid castle string in FEN string: " << castleString << std::endl;
    return false;
  }

  mCastlingRights = CASTLE_NONE;
  for (std::size_t i = 0; i < castleString.size(); i++)
  {
    if (castleString[i] == 'K')
    {
      mCastlingRights |= CASTLE_WHITE_KING;
    }

    if (castleString[i] == 'Q')
    {
      mCastlingRights |= CASTLE_WHITE_QUEEN;
    }

    if (castleString[i] == 'k')
    {
      mCastlingRights |= CASTLE_BLACK_KING;
    }

    if (castleString[i] == 'q')
    {
      mCastlingRights |= CASTLE_BLACK_QUEEN;
    }
  }

  pos = epString.find_first_not_of("36abcdefgh-");
  if (pos != std::string::npos)
  {
    std::cerr << "Invalid en-passant string in FEN string: " << epString << std::endl;
    return false;
  }

  if (epString.size() == 1 && epString[0] == '-')
  {
    mEnPassantColumn = INVALID_EP_COLUMN;
  }
  else if (epString.size() != 2)
  {
    std::cerr << "Invalid en-passant string in FEN string: " << epString << std::endl;
    return false;
  }
  else
  {
    std::map<char, uint8_t> columnMap;
    columnMap['a'] = 0;
    columnMap['b'] = 1;
    columnMap['c'] = 2;
    columnMap['d'] = 3;
    columnMap['e'] = 4;
    columnMap['f'] = 5;
    columnMap['g'] = 6;
    columnMap['h'] = 7;

    char columnChar = epString[0];
    mEnPassantColumn = columnMap[columnChar];
  }

  mHalfMoveClock = std::atoi(halfMoveString.c_str());
  mFullMoveCounter = std::atoi(fullMoveString.c_str());

  return false;
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

  if (mSideToMove == Color::White)
  {
    oss << "w ";
  }
  else
  {
    oss << "b ";
  }

  if (mCastlingRights == CASTLE_NONE)
  {
    oss << "- ";
  }
  else
  {
    if (mCastlingRights & CASTLE_WHITE_KING)
    {
      oss << "K";
    }
    if (mCastlingRights & CASTLE_WHITE_QUEEN)
    {
      oss << "Q";
    }
    if (mCastlingRights & CASTLE_BLACK_KING)
    {
      oss << "k";
    }
    if (mCastlingRights & CASTLE_BLACK_QUEEN)
    {
      oss << "q";
    }

    oss << " ";
  }

  if (mEnPassantColumn == INVALID_EP_COLUMN)
  {
    oss << "- ";
  }
  else
  {
    std::string columnName[] = {"a", "b", "c", "d", "e", "f", "g", "h"};
    oss << columnName[mEnPassantColumn];
    if (mSideToMove == Color::White)
    {
      oss << "6";
    }
    else
    {
      oss << "3";
    }
    oss << " ";
  }

  oss << static_cast<int32_t>(mHalfMoveClock) << " ";
  oss << static_cast<int32_t>(mFullMoveCounter);

  return oss.str();
}

}
