#include "Board.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <chrono>
#include <utility>
#include <algorithm>

Board::Board(std::vector<Minion> pBoard, std::vector<Minion> eBoard) {
    seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    generator.seed(seed);
    isPlayerTurn = getDistFromRange(0,1);
    std::cout << isPlayerTurn << std::endl;
    setPlayerBoard(pBoard);
    setEnemyBoard(eBoard);
    isVerbose = false;
}

Board::Board(std::vector<Minion> pBoard, std::vector<Minion> eBoard, bool verbose) {
    seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    generator.seed(seed);
    isPlayerTurn = getDistFromRange(0,1);
    std::cout << isPlayerTurn << std::endl;
    setPlayerBoard(pBoard);
    setEnemyBoard(eBoard);
    isVerbose = verbose;
}

void Board::setPlayerBoard(std::vector<Minion> minionBoard) {playerBoard = minionBoard;}
void Board::setEnemyBoard(std::vector<Minion> minionBoard) {enemyBoard = minionBoard;}
void Board::setRandomAttacker() {isPlayerTurn = getDistFromRange(0,1);}
int Board::getDistFromRange(int start, int end) {std::uniform_int_distribution<int> distr(start, end); return distr(generator);}
void Board::printBoard() {
    std::cout << "========== PRINT BOARD ==========" << std::endl;
    std::cout << "Allied Board" << std::endl;
    for (auto it = begin (playerBoard); it != end (playerBoard); ++it) {
        std::cout << it->toString() << std::endl;
    }
    std::cout << "Enemy Board" << std::endl;
    for (auto it = begin (enemyBoard); it != end (enemyBoard); ++it) {
        std::cout << it->toString() << std::endl;
    }
    std::cout << "=================================" << std::endl;
}

// ====== Attacking Code ======

void Board::doAttack(Minion &attacker, Minion &defender) {
    defender.SetHP(defender.GetHP()-attacker.GetATK());
    attacker.SetHP(attacker.GetHP()-defender.GetATK());
}

Minion& Board::getAttacker(std::vector<Minion> &minionBoard) {
    for (auto it = begin(minionBoard); it != end (minionBoard); ++it) {
        if (!(it->GetAtkState())) {
            return (*it);
        }
    }
}

Minion& Board::getDefender(std::vector<Minion> &minionBoard) {
    std::vector<int> valid;
    for (auto it = begin(minionBoard); it != end (minionBoard); ++it) {
        if (it->GetTaunt()) {
            valid.push_back(it - begin(minionBoard));
        }
    }
    // std::cout << "Checked Taunts" << std::endl;
    if (valid.empty()) {
        // Choose randomly from the initial board
        return minionBoard.at(getDistFromRange(0, minionBoard.size()-1));
    }
    else {
        // Choose from the valid list
        return minionBoard.at(valid.at(getDistFromRange(0, valid.size()-1)));
    }
}

// ====== Death & Deathrattle Code ======

void Board::checkDeaths() {
    std::vector<std::pair<int, int>> deadMinionIdxVec;
    if (isVerbose) {std::cout << "Checking Player Deaths" << std::endl;}
    for (auto it = begin (playerBoard); it != end (playerBoard);) {
        if (it->GetHP() <= 0) {
            if (isVerbose) {std::cout << it->toString() << " IS DEAD" << std::endl;}
            deadMinionIdxVec.push_back(std::make_pair(0, it-playerBoard.begin()));
            it++;
            // doDeathrattle(*it);
            // it = playerBoard.erase(it);
        } else {
            it++;
        }
    }

    if (isVerbose) {std::cout << "Checking Enemy Deaths" << std::endl;}
    for (auto it = begin (enemyBoard); it != end (enemyBoard);) {
        if (it->GetHP() <= 0) {
            if (isVerbose) {std::cout << it->toString() << " IS DEAD" << std::endl;}
            deadMinionIdxVec.push_back(std::make_pair(1, it-enemyBoard.begin()));
            it++;
            // doDeathrattle(*it);
            // it = enemyBoard.erase(it);
        } else {
            it++;
        }
    }

    if (isVerbose) {std::cout << "Done Checking Deaths" << std::endl;}

    std::shuffle(std::begin(deadMinionIdxVec), std::end(deadMinionIdxVec), std::default_random_engine());

    for (auto it = begin (deadMinionIdxVec); it != end (deadMinionIdxVec); ++it) {
        // if (isVerbose) {std::cout << (*it).first << "  " << (*it).second << std::endl;}

        if ((*it).first == 0) {
            doDeathrattle(*((*it).second + playerBoard.begin()), (*it).second);
            playerBoard.erase((*it).second + playerBoard.begin());
        } else {
            doDeathrattle(*((*it).second + enemyBoard.begin()), (*it).second);
            enemyBoard.erase((*it).second + enemyBoard.begin());
        }



        // if ((*it)->GetHP() <= 0) {
        //     std::cout << (*it)->toString() << " DEATHRATTLE?" << std::endl;
        //     doDeathrattle((*it));
        //     it = deadMinionVec.erase(it);
        // } else {
        //     it++;
        // }
    }
}

void Board::doDeathrattle(Minion &minion, int idx) {
    std::vector<Minion> *affectedBoard;
    affectedBoard = (minion.IsPlayerMinion()) ? &playerBoard : &enemyBoard;


    switch(minion.GetMinionType()) {
        case 1004: // Acolyte of C'Thun (2/2 Reborn)
            {
                if (isVerbose) {std::cout << "DEATHRATTLE: Acolyte of C'Thun" << std::endl;}
                Minion rebornMinion = Minion(10004, 1, 2); // 1004 is a token of the minion-type 4
                (*affectedBoard).insert((*affectedBoard).begin()+idx+1, rebornMinion);
                // for (auto it = begin (*affectedBoard); it != end (*affectedBoard); ++it) {
                //     std::cout << (*it).toString() << std::endl;
                // }
            }
            break;
        case 2001: // Spawn of N'Zoth (2/2, +2/+2)
            {
                if (isVerbose) {std::cout << "DEATHRATTLE: Spawn of N'Zoth" << std::endl;}
                for (auto it = begin (*affectedBoard); it != end (*affectedBoard); ++it) {
                    if(isVerbose) {std::cout << "Buffing " + it->toString() << std::endl;}
                    it->IncreaseATK(2);
                    it->IncreaseHP(2);
                }
            }
            break;
        default:
            if(isVerbose){std::cout << "A normal minion is dying!" << std::endl;}
    }
}

// ====== Check Win ======

bool Board::checkWin() {
    return (playerBoard.empty() || enemyBoard.empty()) ? 1 : 0;
}


// ====== Turn Code ======

std::pair<Minion, Minion> Board::doTurn() {
    std::vector<Minion> *attackerBoard;
    std::vector<Minion> *defenderBoard;
    if (isPlayerTurn) {
        attackerBoard = &playerBoard;
        defenderBoard = &enemyBoard;
    }
    else {
        attackerBoard = &enemyBoard;
        defenderBoard = &playerBoard;
    }

    // std::cout << "Attacking Board" << std::endl;
    // for (auto it = begin (*attackerBoard); it != end (*attackerBoard); ++it) {
    //     std::cout << it->toString() << std::endl;
    // }
    // std::cout << "Defending Board" << std::endl;
    // for (auto it = begin (*defenderBoard); it != end (*defenderBoard); ++it) {
    //     std::cout << it->toString() << std::endl;
    // }


    // iterate to get the attacker
    // std::cout << "Attacker: " << std::endl;
    Minion *attacker = &getAttacker(*attackerBoard);
    // std::cout << (*attacker).toString() << std::endl;

    // std::cout << "Defender: " << std::endl;
    Minion *defender = &getDefender(*defenderBoard);
    // std::cout << (*defender).toString() << std::endl;
    if (isVerbose) {std::cout << "Attacker: " << (*attacker).toString() << " --x--> Defender: " << (*defender).toString() << std::endl;}

    // std::cout << "======== ATTACKING ========" << std::endl;
    std::pair<Minion, Minion> atk_def_pair = std::make_pair(*attacker, *defender);

    doAttack(*attacker, *defender);
    // printBoard();

    checkDeaths();

    // printBoard();

    // std::cout << "======== AFTER DEATHRATTLES ========" << std::endl;
    if (isVerbose) {printBoard();}

    isPlayerTurn = !isPlayerTurn;

    return atk_def_pair;
    
}

bool Board::getWhoseTurn() {return isPlayerTurn;}