#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <tuple>

#include "nlohmann/json.hpp"
using json = nlohmann::json;

#include "Minion.h"
#include "Board.h"
#include "StatTracker.h"
#include "SqlHandler.h"

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
        it->SetDeathrattle( (j[miniontype].at("isDeathrattle") == 1) );
        it->SetReborn( (j[miniontype].at("isReborn") == 1) );
        it->tier =  j[miniontype].at("tier");
        // std::cout << it->toString() << std::endl;
    }
}



std::vector<Minion> extractMinionVectFromJson(json &j, bool isPlayerBoard) {
    std::vector<Minion> board;
    std::string boardKey =  isPlayerBoard ? "Allied" : "Enemy";
    // std::cout << boardKey << std::endl;
    for (auto it = j[boardKey].begin(); it != j[boardKey].end(); ++it) {
        // std::cout << *it << std::endl;
        Minion m = Minion((*it).at("id"), (*it).at("hp"), (*it).at("atk"), isPlayerBoard);
        m.SetTaunt((*it).at("isTaunt") == 1);
        m.SetDivine((*it).at("isDivine") == 1);
        m.SetPoison((*it).at("isPoison") == 1);
        m.SetDeathrattle((*it).at("isDeathrattle") == 1);
        m.SetReborn((*it).at("isReborn") == 1);

        board.push_back(m);
        // std::cout << m.toString() << std::endl;
    }

    return board;
}

void extractJsonToMinionVects(std::vector<Minion> &playerBoard, std::vector<Minion> &enemyBoard, std::string filename_json) {
    std::ifstream i(filename_json);
    json inputBoards;
    i >> inputBoards;

    playerBoard = extractMinionVectFromJson(inputBoards, 1);
    enemyBoard = extractMinionVectFromJson(inputBoards, 0);
}

void simBoards(Board &board, bool verbosity, int eps, std::vector<Minion>&init_ally, std::vector<Minion>&init_enemy, StatTracker &tracker) {
    std::cout << "=== SIMMING ===" << std::endl;

    if (verbosity) {board.printBoard();}

    for (int i = 0; i < eps; i++) {
        if(board.isVerbose) {std::cout << ">>>> EP: "<< i << " <<<<" << std::endl;} 
        board.setPlayerBoard(init_ally);
        board.setEnemyBoard(init_enemy);

        board.singleSim(tracker);
    }

    // Print Scoreboard
    std::cout << "Ties: " << tracker.ties << " Losses: " << tracker.losses << " Wins: " << tracker.wins << std::endl;
    
    int BOUNDS = 40;
    for(int i = (0+BOUNDS); i < (board.damageBreakdown.size()-BOUNDS); i++) {
        std::cout << "Damage: " << (i-48) << " = " << board.damageBreakdown[i] << std::endl;
    }
}

void saveIntoDB(std::string dbfname, std::string jsonfname, std::string wl, std::string dmg, int eps) {
    std::ifstream i(jsonfname);
    json inputBoards;
    i >> inputBoards;
    std::string s = inputBoards.dump();
    SqlHandler sqlhand = SqlHandler(dbfname);
    sqlhand.insertDataTable(s, wl, dmg, eps);

}





int main(int argc, char** argv) {

    int EPS;
    bool verbosity = false; // SET VERBOSITY
    std::string loadMinionsFname = "sim_test.json";
    std::string dbFname = "test.db";

    if (argc == 2) {
        EPS = std::stoi(argv[1]);
    } else if (argc==3) {
        EPS = std::stoi(argv[1]);
        verbosity = (std::stoi(argv[2])==1);
    } else if (argc==4){
        EPS = std::stoi(argv[1]);
        verbosity = (std::stoi(argv[2])==1);
        loadMinionsFname = argv[3];
    } else if (argc==5) {
        EPS = std::stoi(argv[1]);
        verbosity = (std::stoi(argv[2])==1);
        loadMinionsFname = argv[3];
        dbFname = argv[4];
    } else {
        EPS = 1; // Set default episodes for simulation
    }

    
    std::cout << "Eps: " << EPS << " Verbose: " << verbosity << std::endl;

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
    StatTracker tracker;

    // Minion testMinion(0, 2, 4);
    // std::cout << "Hello World!" << std::endl;
    // std::cout << testMinion.toString() << std::endl;

    // for(int i = 0; i < MinionList.size(); i++) {
    //     std::cout << std::get<0>(MinionList[i]) << std::endl;
    // }

    // Testing Board Stuff
    // std::vector<Minion> test_playerMins {Minion(1000, 1, 1), Minion(10000, 1, 1), Minion(1001, 2, 2), Minion(1002, 1, 2)};
    // std::vector<Minion> test_enemyMins {Minion(2000, 2, 2, 0), Minion(1016, 2, 2, 0)}; 

    std::vector<Minion> test_playerMins {Minion(2000, 2, 2), Minion(1012, 3, 5), Minion(1013, 3, 2), Minion(1011, 1, 2), Minion(10011, 1, 1)};
    std::vector<Minion> test_enemyMins {Minion(2000, 2, 2, 0), Minion(1000, 1, 1, 0), Minion(1008, 2, 2, 0), Minion(1016, 2, 2, 0), Minion(1016, 2, 2, 0)}; 

    std::vector<Minion> input_playerBoard;
    std::vector<Minion> input_enemyBoard;

    extractJsonToMinionVects(input_playerBoard, input_enemyBoard, loadMinionsFname);

    updateMinionData(test_playerMins, minionNameList);
    updateMinionData(test_enemyMins, minionNameList);

    Board testBoard(test_playerMins, test_enemyMins);
    Board inputBoard(input_playerBoard, input_enemyBoard);

    testBoard.isVerbose = verbosity; // SETTING THE VERBOSITY
    inputBoard.isVerbose = verbosity;
    // // testBoard.setPlayerBoard(test_playerMins);
    // // testBoard.setEnemyBoard(test_enemyMins);
    // testBoard.printBoard();
    // // std::cout << testBoard.getWhoseTurn() << std::endl;
    // testBoard.doTurn();


    // std::cout << "=== SIMMING ===" << std::endl;

    // if (verbosity) {testBoard.printBoard();}

    // for (int i = 0; i < EPS; i++) {
    //     if(testBoard.isVerbose) {std::cout << ">>>> EP: "<< i << " <<<<" << std::endl;} 
    //     testBoard.setPlayerBoard(test_playerMins);
    //     testBoard.setEnemyBoard(test_enemyMins);

    //     testBoard.singleSim(scoreBoard);
    // }




    // Print Scoreboard
    // std::cout << "Ties: " << scoreBoard[0] << " Losses: " << scoreBoard[1] << " Wins: " << scoreBoard[2] << std::endl;
    
    // int BOUNDS = 40;
    // for(int i = (0+BOUNDS); i < (testBoard.damageBreakdown.size()-BOUNDS); i++) {
    //     std::cout << "Damage: " << (i-48) << " = " << testBoard.damageBreakdown[i] << std::endl;
    // }


    // simBoards(testBoard, verbosity, EPS, test_playerMins, test_enemyMins, scoreBoard);
    simBoards(inputBoard, verbosity, EPS, input_playerBoard, input_enemyBoard, tracker);
    saveIntoDB(dbFname, loadMinionsFname, tracker.toString(), tracker.damageToString(), EPS);
    

    


    return 0;
}