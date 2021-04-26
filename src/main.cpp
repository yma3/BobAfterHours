#include <iostream>
#include <vector>
#include <string>
#include <tuple>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Minion.h"
#include "Board.h"

std::vector<std::tuple<std::string, int, int>> MinionList = {
    std::make_tuple("Alley Cat", 1, 1),
    std::make_tuple("Tabby Cat", 1, 1)
};

void singleSim(Board &board, std::vector<int> &scoreBoard) {
    int turnCounter = 1;
    std::vector< std::pair<Minion, Minion> > fightHistory;
    board.setRandomAttacker();
    while(!board.checkWin()) {
        // std::cout << "====== Turn: " << turnCounter << ", Attacker: " << board.getWhoseTurn() << " ======" << std::endl;
        fightHistory.push_back(board.doTurn());
        turnCounter++;
    }

    if (board.playerBoard.empty() && board.enemyBoard.empty()) {
        scoreBoard[0] += 1;
        // std::cout << "It's a tie!" << std::endl;
    } else if (board.playerBoard.empty() && !board.enemyBoard.empty()) {
        // std::cout << "Player Loses!" << std::endl;
        scoreBoard[1] += 1;
    } else { // (!board.playerBoard.empty() && board.enemyBoard.empty()) {
        // std::cout << "Player Wins!" << std::endl;
        scoreBoard[2] += 1;
    }

    // for (auto it = begin(fightHistory); it != end(fightHistory); ++it) {
    //     std::cout << (*it).first.toString() << " attacks " << (*it).second.toString() << std::endl;
    // }
}







int main(int argc, char** argv) {

    int EPS;
    if (argc == 2) {
        EPS = std::stoi(argv[1]);
    }
    else {
        EPS = 10; // Set default episodes for simulation
    }
    
    bool verbosity = true; // SET VERBOSITY

    // Input filestream



    std::vector<int> scoreBoard = {0, 0, 0}; // [0] os tie, [1] is loss, [2] is win

    // Minion testMinion(0, 2, 4);
    // std::cout << "Hello World!" << std::endl;
    // std::cout << testMinion.toString() << std::endl;

    // for(int i = 0; i < MinionList.size(); i++) {
    //     std::cout << std::get<0>(MinionList[i]) << std::endl;
    // }

    // Testing Board Stuff
    std::vector<Minion> test_playerMins {Minion(1000, 2, 2), Minion(1001, 2, 2)};
    std::vector<Minion> test_enemyMins {Minion(2001, 2, 2, 0), Minion(1004, 2, 2, 0)}; 

    Board testBoard(test_playerMins, test_enemyMins);
    testBoard.isVerbose = verbosity; // SETTING THE VERBOSITY

    // // testBoard.setPlayerBoard(test_playerMins);
    // // testBoard.setEnemyBoard(test_enemyMins);
    // testBoard.printBoard();
    // // std::cout << testBoard.getWhoseTurn() << std::endl;
    // testBoard.doTurn();


    std::cout << "=== SIMMING ===" << std::endl;

    for (int i = 0; i < EPS; i++) {
        if(testBoard.isVerbose) {std::cout << ">>>> EP: "<< i << " <<<<" << std::endl;} 
        testBoard.setPlayerBoard(test_playerMins);
        testBoard.setEnemyBoard(test_enemyMins);

        singleSim(testBoard, scoreBoard);
    }



    // Print Scoreboard
    std::cout << "Ties: " << scoreBoard[0] << " Losses: " << scoreBoard[1] << " Wins: " << scoreBoard[2] << std::endl;



    

    


    return 0;
}