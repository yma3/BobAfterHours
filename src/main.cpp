#include <iostream>
#include <fstream>
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

// void singleSim(Board &board, std::vector<int> &scoreBoard) {
//     int turnCounter = 1;
//     std::vector< std::pair<Minion, Minion> > fightHistory;
//     board.setRandomAttacker();
//     while(!board.checkWin()) {
//         // std::cout << "====== Turn: " << turnCounter << ", Attacker: " << board.getWhoseTurn() << " ======" << std::endl;
//         fightHistory.push_back(board.doTurn());
//         turnCounter++;
//     }

//     if (board.playerBoard.empty() && board.enemyBoard.empty()) {
//         scoreBoard[0] += 1;
//         // std::cout << "It's a tie!" << std::endl;
//     } else if (board.playerBoard.empty() && !board.enemyBoard.empty()) {
//         // std::cout << "Player Loses!" << std::endl;
//         scoreBoard[1] += 1;
//     } else { // (!board.playerBoard.empty() && board.enemyBoard.empty()) {
//         // std::cout << "Player Wins!" << std::endl;
//         scoreBoard[2] += 1;
//     }

//     // for (auto it = begin(fightHistory); it != end(fightHistory); ++it) {
//     //     std::cout << (*it).first.toString() << " attacks " << (*it).second.toString() << std::endl;
//     // }
// }

void updateMinionData(std::vector<Minion> &minionList, json &j) {
    for (auto it = begin(minionList); it != end(minionList); ++it) {
        std::string miniontype = std::to_string(it->GetMinionType());
        // std::cout <<  miniontype << " | " << j[miniontype].at("isTaunt") << std::endl;
        it->SetTribe( j[ miniontype ].at("tribe") );
        it->SetTaunt( (j[miniontype].at("isTaunt") == 1) );
        it->SetDivine( (j[miniontype].at("isDivine") == 1) );
        it->SetPoison( (j[miniontype].at("isPoison") == 1) );

        // std::cout << it->toString() << std::endl;
    }
}






int main(int argc, char** argv) {

    int EPS;
    if (argc == 2) {
        EPS = std::stoi(argv[1]);
    }
    else {
        EPS = 1; // Set default episodes for simulation
    }
    
    bool verbosity = false; // SET VERBOSITY

    // Input filestream
    std::ifstream i("minions.json");
    json minionNameList;
    i >> minionNameList;

    // Testing Input Files JSON
    // for (auto &element : minionNameList) {
    //     std::cout << element.at("name") << std::endl;
    // }

    // std::cout << minionNameList["1000"].at("name") << std::endl;



    std::vector<int> scoreBoard = {0, 0, 0}; // [0] os tie, [1] is loss, [2] is win

    // Minion testMinion(0, 2, 4);
    // std::cout << "Hello World!" << std::endl;
    // std::cout << testMinion.toString() << std::endl;

    // for(int i = 0; i < MinionList.size(); i++) {
    //     std::cout << std::get<0>(MinionList[i]) << std::endl;
    // }

    // Testing Board Stuff
    std::vector<Minion> test_playerMins {Minion(1000, 1, 1), Minion(10000, 1, 1), Minion(1001, 2, 2), Minion(1002, 1, 2)};
    std::vector<Minion> test_enemyMins {Minion(2000, 2, 2, 0), Minion(1016, 2, 2, 0)}; 

    updateMinionData(test_playerMins, minionNameList);
    updateMinionData(test_enemyMins, minionNameList);

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

        testBoard.singleSim(scoreBoard);
    }



    // Print Scoreboard
    std::cout << "Ties: " << scoreBoard[0] << " Losses: " << scoreBoard[1] << " Wins: " << scoreBoard[2] << std::endl;



    

    


    return 0;
}