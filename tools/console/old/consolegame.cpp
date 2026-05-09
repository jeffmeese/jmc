#include "consolegame.h"

#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "consts.h"
#include "fen.h"
#include "move.h"
#include "movelist.h"
#include "types.h"

ConsoleGame::ConsoleGame(jmchess::Board * board)
  : mBoard(board)
{
}

void ConsoleGame::doMove(const jmchess::Move * move) const
{
  mBoard->makeMove(move);
}

int ConsoleGame::execute()
{
  while (true)
  {
    std::cout << "\njmc: ";

    std::string inputString;
    std::getline(std::cin, inputString);

    std::istringstream iss(inputString);
    std::string commandString;
    iss >> commandString;
    if (commandString.empty())
      continue;

    if (commandString == "quit")
    {
      break;
    }
    // else if (commandString == "divide")
    //   handleDivide(iss);
    // else if (commandString == "help")
    //   handleHelp();
    else if (commandString == "print")
    {
      handlePrint();
    }
    // else if (commandString == "disp")
    //   handlePrint();
    // else if (commandString == "fen")
    //   handleFen();
    // else if (commandString == "perft")
    //   handlePerft(iss);
    else if (commandString == "move")
    {
      handleMove(iss);
    }
    // else if (commandString == "setboard")
    //   handleSetBoard(iss);
    // else if (commandString == "testmovegen")
    //   handleTestMoveGen();
    // else if (commandString == "eval")
    //   handleEval();
    // else if (commandString == "engine")
    //   handleEngine();
    // else if (commandString == "new")
    //   handleNewGame();
    else if (commandString == "show")
    {
       handleShow();
    }
    // else if (commandString == "undo")
    //   handleUndo();
    // else if (commandString == "twoplayer")
    //   handleTwoPlayer();
    // else if (commandString == "singleplayer")
    //   handleSinglePlayer();
    else
    {
      std::cout << "Unrecognized command " << commandString << "\n";
      std::cout << "Type help to show command list\n";
    }
  }

  return 0;
}

int ConsoleGame::getMoveIndex(uint8_t srcRow, uint8_t srcCol, uint8_t dstRow, uint8_t dstCol, const jmchess::MoveList & moveList) const
{
  int moveIndex = -1;
  for (uint8_t i = 0; i < moveList.size(); i++)
  {
    const jmchess::Move * move = moveList[i];
    uint8_t sourceRow = move->getSourceRow();
    uint8_t sourceCol = move->getSourceColumn();
    uint8_t destRow = move->getDestinationRow();
    uint8_t destCol = move->getDestinationColumn();
    if (srcRow == sourceRow && dstRow == destRow && srcCol == sourceCol && dstCol == destCol)
    {
      moveIndex = static_cast<int>(i);
      return moveIndex;
    }
  }
  return moveIndex;
}

void ConsoleGame::handleMove(std::istringstream & iss) const
{
  //if (isOver()) {
  //  std::cout << "The game is over. Type new to start a new game\n";
  //  return;
  //}

  // Get the move string
  std::string moveString;
  iss >> moveString;
  if (moveString.empty()) {
    std::cout << "A move string must be provided\n";
    return;
  }

  // Parse the move
  uint8_t srcRow, srcCol, dstRow, dstCol;
  if (!parseMovePos(moveString, srcRow, srcCol, dstRow, dstCol))
    return;

  // Generate the candidate moves from the board
  jmchess::MoveList moveList;
  mBoard->generateMoves(moveList);

  // Check that the move is valid
  int moveIndex = getMoveIndex(srcRow, srcCol, dstRow, dstCol, moveList);
  if (moveIndex == -1) {
    std::cout << "Sorry " << moveString << " is not a valid move\n";
    std::cout << "Type divide 1 to sell all available moves\n";
    return;
  }

  doMove(moveList[moveIndex]);
  //if (isOver()) {
  //  showEndGame();
  //  return;
  //}

  //  executeEngineMove();
  //  handlePrint();
  //  if (isOver()) {
  //    showEndGame();
  //  }
}

void ConsoleGame::handlePrint() const
{
  std::map<jmchess::PieceType, std::string> pieceMap;
  pieceMap[jmchess::PieceType::None] = std::string("   ");
  pieceMap[jmchess::PieceType::WhitePawn] = std::string(" P ");
  pieceMap[jmchess::PieceType::WhiteRook] = std::string(" R ");
  pieceMap[jmchess::PieceType::WhiteKnight] = std::string(" N ");
  pieceMap[jmchess::PieceType::WhiteBishop] = std::string(" B ");
  pieceMap[jmchess::PieceType::WhiteQueen] = std::string(" Q ");
  pieceMap[jmchess::PieceType::WhiteKing] = std::string("[K]");
  pieceMap[jmchess::PieceType::BlackPawn] = std::string(" p ");
  pieceMap[jmchess::PieceType::BlackRook] = std::string(" r ");
  pieceMap[jmchess::PieceType::BlackKnight] = std::string(" n ");
  pieceMap[jmchess::PieceType::BlackBishop] = std::string(" b ");
  pieceMap[jmchess::PieceType::BlackQueen] = std::string(" q ");
  pieceMap[jmchess::PieceType::BlackKing] = std::string("[k]");

  std::ostringstream output;
  output << "\n";
  for (int row = 7; row >= 0; row--)
  {
    output << " +---+---+---+---+---+---+---+---+\n";
    output << row + 1 << "|";

    for (int col = 0; col < 8; col++)
    {
      jmchess::PieceType type = mBoard->getPieceType(row, col);
      output << pieceMap[type];
      output << "|";

      if (col == 7)
      {
        if (row == 7)
        {
          output << " To Move: " << ( mBoard->getSideToMove() == jmchess::Color::White ? "White" : "Black");
        }

        if (row == 6)
        {
          uint8_t castling = mBoard->getCastlingRights();
          output << " Castling: ";
          if (castling & jmchess::CASTLE_WHITE_KING)
          {
            output << "K";
          }
          if (castling & jmchess::CASTLE_WHITE_QUEEN)
          {
            output << "Q";
          }
          if (castling & jmchess::CASTLE_BLACK_KING)
          {
            output << "k";
          }
          if (castling & jmchess::CASTLE_BLACK_QUEEN)
          {
            output << "q";
          }
        }
      }
    }
    output << "\n";
  }

  output << " +---+---+---+---+---+---+---+---+\n";
  output << "   A   B   C   D   E   F   G   H  \n";
  output << "\n";

  jmchess::Fen fen;
  fen.setFromBoard(mBoard);
  std::string fenString = fen.toString();

  output << "FEN: " << fenString << "\n";

  std::cout << output.str();
}

void ConsoleGame::handleShow() const
{
  jmchess::MoveList moveList;
  mBoard->generateMoves(moveList);
  for (uint8_t i = 0; i < moveList.size(); i++)
    std::cout << moveList[i]->toSmithNotation() << "\n";
}

bool ConsoleGame::parseMovePos(const std::string & moveString, uint8_t & srcRow, uint8_t & srcCol, uint8_t & dstRow, uint8_t & dstCol) const
{
  static std::map<char, uint8_t> columnMap;
  static std::map<char, uint8_t> rowMap;

  if (columnMap.size() == 0) {
    columnMap['a'] = 0;
    columnMap['b'] = 1;
    columnMap['c'] = 2;
    columnMap['d'] = 3;
    columnMap['e'] = 4;
    columnMap['f'] = 5;
    columnMap['g'] = 6;
    columnMap['h'] = 7;

    rowMap['1'] = 0;
    rowMap['2'] = 1;
    rowMap['3'] = 2;
    rowMap['4'] = 3;
    rowMap['5'] = 4;
    rowMap['6'] = 5;
    rowMap['7'] = 6;
    rowMap['8'] = 7;

  }

  // Check format of move string
  if (moveString.find_first_not_of("123456789abcdefghpnbrqkEcCNBRQ") != std::string::npos || moveString.length() < 4) {
    std::cout << "Invalid move string " << moveString << "\n";
    return false;
  }

  srcCol = columnMap[moveString.at(0)];
  srcRow = rowMap[moveString.at(1)];
  dstCol = columnMap[moveString.at(2)];
  dstRow = rowMap[moveString.at(3)];

  return true;
}
