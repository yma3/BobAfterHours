#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include <random>
#include "Minion.h"

class Board{
    public:
        Board(std::vector<Minion> pBoard, std::vector<Minion> eBoard); // Constructor
        Board(std::vector<Minion> pBoard, std::vector<Minion> eBoard, bool verbose); // Verbose Constructor

        void setPlayerBoard(std::vector<Minion> minionBoard);
        void setEnemyBoard(std::vector<Minion> minionBoard);
        void setRandomAttacker();
        int getDistFromRange(int start, int end);
        void printBoard();

        void doAttack(Minion &attacker, Minion &defender);
        Minion& getAttacker(std::vector<Minion> &minionBoard);
        Minion& getDefender(std::vector<Minion> &minionBoard);
        void doTurn();

        void checkDeaths();
        void doDeathrattle(Minion &minion, int idx);

        bool checkWin();

        void singleSim(std::vector<int> &scoreBoard);

        bool getWhoseTurn();

        std::vector<Minion> playerBoard;
        std::vector<Minion> enemyBoard;

        std::vector< std::pair<Minion, Minion> > fightHistory;

        bool isPlayerTurn;
        int turnCounter;
        
        bool isVerbose;
    
    private:
        std::mt19937 generator;
        unsigned seed;

};

#endif