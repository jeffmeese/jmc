#include "console_game.h"

#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>

#include "board.h"
#include "board_state.h"
#include "fen.h"
#include "perft.h"
#include "string_utilities.h"
#include "timer.h"

template <typename T>
T readValue(std::istream & iss)
{
  T value = T();
  iss >> value;
  return value;
}

void ConsoleGame::execute()
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
    else if (commandString == "divide")
    {
      handleDivideCommand(iss);
    }
    else if (commandString == "help")
    {
      handleHelpCommand();
    }
    else if (commandString == "print")
    {
      handlePrintCommand();
    }
    else if (commandString == "disp")
    {
      handlePrintCommand();
    }
    else if (commandString == "perft")
    {
      handlePerftCommand(iss);
    }
    else if (commandString == "move")
    {
      handleMoveCommand(iss);
    }
    else if (commandString == "setboard")
    {
      handleSetBoardCommand(iss);
    }
    else if (commandString == "testmovegen")
    {
      handleTestMoveGen();
    }
    else if (commandString == "eval")
    {
      handleEvalCommand();
    }
    else if (commandString == "engine")
    {
      handleEngineCommand();
    }
    else if (commandString == "new")
    {
      handleNewCommand();
    }
    else if (commandString == "show")
    {
      handleShowCommand();
    }
    else if (commandString == "undo")
    {
      handleUndoCommand();
    }
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
}

void ConsoleGame::handleDivideCommand(std::istream & input)
{
  const jmchess::Board * board = this->getBoard();

  std::int32_t perftLevel = readLevel(input);
  if (perftLevel == 0)
  {
    return;
  }

  jmchess::Perft perft(getBoard());
  perft.divide(perftLevel);
}

void ConsoleGame::handleEngineCommand()
{
  if (isCheckmate() || isStalemate())
  {
    std::cout << "Game is over" << std::endl;
    return;
  }

  performEngineCommand();

  if (isCheckmate())
  {
    jmchess::Color sideToMove = getBoard()->getBoardState().sideToMove;
    if (sideToMove == jmchess::Color::White)
    {
      std::cout << "Checkmate: Black Wins" << std::endl;
    }
    else
    {
      std::cout << "Checkmate: White Wins" << std::endl;
    }
  }

  if (isStalemate())
  {
    std::cout << "Stalemate" << std::endl;
  }
}

void ConsoleGame::handleEvalCommand()
{
  const jmchess::Board * board = this->getBoard();
  jmchess::Evaluation evaluation;
  double score = evaluation.evaluate(board);
  std::cout << "Evaluation: " << score << "\n";
}

void ConsoleGame::handleHelpCommand() const
{
  std::cout << "quit.................Quits the program\n";
  std::cout << "help.................Displays this help menu\n";
  std::cout << "print................Prints the current board position\n";
  std::cout << "disp.................Same as print\n";
  std::cout << "new..................Start a new game\n";
  std::cout << "eval.................Evaluation the current board position\n";
  std::cout << "move <smith>.........Performs a move\n";
  std::cout << "perft <level>........Counts the total number of nodes to depth <level>\n";
  std::cout << "divide <level>.......Displays the number of child moves\n";
  std::cout << "engine...............Instructs the engine to make a move\n";
  // std::cout << "table <level>........Displays a table of all perft results from 1 to <level>\n";
  std::cout << "setboard <fen>.......Sets the board position to <fen>\n";
  std::cout << "testmovegen..........Tests the move generator\n";
  std::cout << "undo.................Undoes the last move\n";
}

void ConsoleGame::handleMoveCommand(
  std::istream & input)
{
  // Get the move string
  std::string moveString;
  input >> moveString;
  if (moveString.empty())
  {
    std::cout << "A move string must be provided\n";
    return;
  }

  // Generate the candidate moves from the board
  jmchess::MoveList moveList;
  getBoard()->generateMoves(moveList);

  std::uint8_t sourceRow         = 0;
  std::uint8_t sourceColumn      = 0;
  std::uint8_t destinationRow    = 0;
  std::uint8_t destinationColumn = 0;
  if (!parseMovePos(moveString, sourceRow, sourceColumn, destinationRow, destinationColumn))
  {
    return;
  }

  // Check that the move is valid
  std::int32_t moveIndex = -1;
  std::int8_t sourceIndex = jmchess::Board::getIndex(sourceRow, sourceColumn);
  std::int8_t destIndex = jmchess::Board::getIndex(destinationRow, destinationColumn);
  for (uint8_t i = 0; i < moveList.totalMoves(); i++)
  {
    const jmchess::Move & move   = moveList.getMove(i);
    if (move.getSourceIndex() == sourceIndex && move.getDestinationIndex() == destIndex)
    {
      moveIndex = i;
      break;
    }
  }

  if (moveIndex == -1)
  {
    std::cout << "Sorry " << moveString << " is not a valid move\n";
    std::cout << "Type divide 1 to sell all available moves\n";
    return;
  }

  const jmchess::Move & selectedMove = moveList.getMove(moveIndex);

  makeMove(selectedMove);

  if (isCheckmate())
  {
    jmchess::Color sideToMove = getBoard()->getBoardState().sideToMove;
    if (sideToMove == jmchess::Color::White)
    {
      std::cout << "Checkmate: Black Wins" << std::endl;
    }
    else
    {
      std::cout << "Checkmate: White Wins" << std::endl;
    }
  }

  if (isStalemate())
  {
    std::cout << "Stalemate" << std::endl;
  }
}

void ConsoleGame::handleNewCommand()
{
  startNew();
}

void ConsoleGame::handlePerftCommand(
  std::istream & input)
{
  int32_t perftLevel = 0;
  input >> perftLevel;
  if (input.fail())
  {
    std::cerr << "Error reading perft level" << std::endl;
    return;
  }

  if (perftLevel <= 0)
  {
    std::cerr << "Invalid perft level: " << perftLevel << std::endl;
    return;
  }

  jmchess::Board * board = this->getBoard();
  jmchess::Perft perft(board);

  jmchess::Timer timer;
  timer.start();
  std::uint64_t totalNodes = perft.execute(perftLevel);
  timer.stop();
  std::cout << "Total Nodes: " << totalNodes << " Time: " << timer.elapsed()/1e3 << " milliseconds\n";
}

void ConsoleGame::handlePrintCommand()
{
  std::map<jmchess::PieceType, std::string> pieceMap;
  pieceMap[jmchess::PieceType::None]        = std::string("   ");
  pieceMap[jmchess::PieceType::WhitePawn]   = std::string(" P ");
  pieceMap[jmchess::PieceType::WhiteRook]   = std::string(" R ");
  pieceMap[jmchess::PieceType::WhiteKnight] = std::string(" N ");
  pieceMap[jmchess::PieceType::WhiteBishop] = std::string(" B ");
  pieceMap[jmchess::PieceType::WhiteQueen]  = std::string(" Q ");
  pieceMap[jmchess::PieceType::WhiteKing]   = std::string("[K]");
  pieceMap[jmchess::PieceType::BlackPawn]   = std::string(" p ");
  pieceMap[jmchess::PieceType::BlackRook]   = std::string(" r ");
  pieceMap[jmchess::PieceType::BlackKnight] = std::string(" n ");
  pieceMap[jmchess::PieceType::BlackBishop] = std::string(" b ");
  pieceMap[jmchess::PieceType::BlackQueen]  = std::string(" q ");
  pieceMap[jmchess::PieceType::BlackKing]   = std::string("[k]");

  const jmchess::Board * board   = this->getBoard();
  jmchess::BoardState boardState = board->getBoardState();
  jmchess::Color sideToMove      = boardState.sideToMove;
  std::uint8_t castlingRights    = boardState.castlingRights;

  std::ostringstream output;
  output << "\n";
  for (int row = 7; row >= 0; row--)
  {
    output << " +---+---+---+---+---+---+---+---+\n";
    output << row + 1 << "|";

    for (int col = 0; col < 8; col++)
    {
      jmchess::PieceType type = board->getPieceType(row, col);
      output << pieceMap[type];
      output << "|";

      if (col == 7)
      {
        if (row == 7)
        {
          output << " To Move: " << (sideToMove == jmchess::Color::White ? "White" : "Black");
        }

        if (row == 6)
        {
          // uint8_t castling = mBoard->getCastlingRights();
          output << " Castling: ";
          if (castlingRights & jmchess::CASTLE_WHITE_KING)
          {
            output << "K";
          }
          if (castlingRights & jmchess::CASTLE_WHITE_QUEEN)
          {
            output << "Q";
          }
          if (castlingRights & jmchess::CASTLE_BLACK_KING)
          {
            output << "k";
          }
          if (castlingRights & jmchess::CASTLE_BLACK_QUEEN)
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

  jmchess::Fen fenData;
  fenData.setFromBoard(getBoard());
  std::string fenString = fenData.toString();
  output << "FEN: " << fenString << "\n";

  std::cout << output.str();
}

void ConsoleGame::handleSetBoardCommand(
  std::istream & input)
{
  std::string fenString;
  std::getline(input, fenString);
  if (fenString.empty())
  {
    std::cout << "A fen string must be provided\n";
    return;
  }

  getBoard()->setPosition(fenString);
}

void ConsoleGame::handleShowCommand()
{
  jmchess::Board * board    = this->getBoard();
  jmchess::Color sideToMove = board->getBoardState().sideToMove;
  jmchess::Color otherColor = (sideToMove == jmchess::Color::White) ? jmchess::Color::Black : jmchess::Color::White;

  jmchess::MoveList moveList;
  board->generateMoves(moveList);

  std::int32_t totalMoves = 0;
  for (std::uint8_t i = 0; i < moveList.totalMoves(); i++)
  {
    const jmchess::Move & move = moveList.getMove(i);
    bool isLegal = board->makeMove(move);
    if (isLegal)
    {
      totalMoves++;
      board->unmakeMove(move);
      std::cout << move.toAlgebraicNotation(board) << "\n";
    }
  }

  std::cout << std::endl;
  std::cout << "Total Moves: " << totalMoves << std::endl;
}

void ConsoleGame::handleTestMoveGen()
{
  std::string fileName("perftsuite.epd");
  std::ifstream inputStream(fileName.c_str());
  if (inputStream.fail())
  {
    std::cout << "Could not locate perft test file " << fileName << "\n";
    return;
  }

  while (true)
  {
    std::string inputLine;
    std::getline(inputStream, inputLine);
    if (inputStream.fail())
    {
      break;
    }

    std::vector<std::string> tokens = jmchess::splitString(inputLine, ";");
    if (tokens.size() < 1)
    {
      continue;
    }

    std::string fenString = tokens.at(0);
    if (tokens.size() > 1)
    {
      std::cout << "fen: " << fenString << "\n";
      getBoard()->setPosition(fenString);
    }

    for (uint8_t i = 1; i < tokens.size(); i++)
    {
      std::string perftDepthString = tokens.at(i);

      std::vector<std::string> perftTokens = jmchess::splitString(perftDepthString, " ");
      if (perftTokens.size() < 2)
      {
        continue;
      }

      std::string depthString = perftTokens.at(0);
      std::string nodesString = perftTokens.at(1);

      int32_t depthLevel = atoi(depthString.substr(1).c_str());
      uint64_t numNodes  = atoi(nodesString.c_str());

      jmchess::Timer timer;
      jmchess::Perft perft(getBoard());

      timer.start();
      uint64_t totalNodes = perft.execute(depthLevel);
      timer.stop();

      bool success = (totalNodes == numNodes);
      std::cout << "Perft (" << static_cast<int>(depthLevel) << "): " << totalNodes << " nodes, ";
      std::cout << "Time: " << timer.elapsed()/1e6 << " s, [" << static_cast<int>(numNodes) << "], ";
      std::cout << (success ? "OK" : "FAIL") << "\n";

      // if (!success)
      //   return;
    }
  }
}

void ConsoleGame::handleUndoCommand()
{
  undoLastMove();
}

bool ConsoleGame::parseMovePos(
  const std::string & moveString,
  uint8_t & srcRow,
  uint8_t & srcCol,
  uint8_t & dstRow,
  uint8_t & dstCol) const
{
  static std::map<char, uint8_t> columnMap;
  static std::map<char, uint8_t> rowMap;

  if (columnMap.size() == 0)
  {
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

  if (moveString.size() > 4 || moveString.size() < 4)
  {
    std::cout << "Invalid move string " << moveString << "\n";
    return false;
  }

  // Check format of move string
  if (moveString.find_first_not_of("123456789abcdefghpnbrqkEcCNBRQ") != std::string::npos)
  {
    std::cout << "Invalid move string " << moveString << "\n";
    return false;
  }

  srcCol = columnMap[moveString.at(0)];
  srcRow = rowMap[moveString.at(1)];
  dstCol = columnMap[moveString.at(2)];
  dstRow = rowMap[moveString.at(3)];

  return true;
}

int32_t ConsoleGame::readLevel(std::istream & iss) const
{
  int32_t perftLevel = readValue<int32_t>(iss);
  if (iss.fail()) 
  {
    std::cout << "Invalid perft level\n";
    return 0;
  }
  return perftLevel;
}
