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
    setPlayerBoard(pBoard);
    setEnemyBoard(eBoard);
    setAttacker();
    std::cout << isPlayerTurn << std::endl;

    isVerbose = false;

    for(int i = 0; i < 97; i++) {
        damageBreakdown.push_back(0);
    }
}

Board::Board(std::vector<Minion> pBoard, std::vector<Minion> eBoard, bool verbose) {
    seed = static_cast<int> (std::chrono::system_clock::now().time_since_epoch().count());
    generator.seed(seed);
    setPlayerBoard(pBoard);
    setEnemyBoard(eBoard);
    setAttacker();
    std::cout << isPlayerTurn << std::endl;
    isVerbose = verbose;
    for(int i = 0; i < 97; i++) {
        damageBreakdown.push_back(0);
    }
}

void Board::setPlayerBoard(std::vector<Minion> minionBoard) {playerBoard = minionBoard;}
void Board::setEnemyBoard(std::vector<Minion> minionBoard) {enemyBoard = minionBoard;}
void Board::setAttacker() {

    if (playerBoard.size() > enemyBoard.size()) {
        isPlayerTurn = 1;
    } else if (playerBoard.size() < enemyBoard.size()) {
        isPlayerTurn = 0;
    } else {
        isPlayerTurn = getDistFromRange(0, 1);
    }
}
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
        if (it->GetTaunt() && (it->GetHP()>0)) {
            // std::cout << it->toString() << std::endl;
            valid.push_back(it - begin(minionBoard));
        }
    }
    // std::cout << "Checked Taunts" << std::endl;
    if (valid.empty()) {
        // std::cout << "No Taunts Detected" << std::endl;
        for (auto it = begin(minionBoard); it != end (minionBoard); ++it) {
            if (it->GetHP()>0) {
                valid.push_back(it - begin(minionBoard));
            }
        }
        // Choose randomly from the initial board
        return minionBoard.at(valid.at(getDistFromRange(0, valid.size()-1)));
    }
    else {
        // std::cout << "Taunts Detected" << std::endl;
        // Choose from the valid list
        return minionBoard.at(valid.at(getDistFromRange(0, valid.size()-1)));
    }
}

// ====== Death & Deathrattle Code ======

void Board::checkDeaths() {
    std::vector<std::pair<int, int>> deadMinionIdxVec;
    int numDeathrattles = 0;
    if (isVerbose) {std::cout << "Checking Player Deaths" << std::endl;}
    for (auto it = begin (playerBoard); it != end (playerBoard);) {
        if ((it->GetHP() <= 0) && it->GetDeathrattle()) {
            if (isVerbose) {std::cout << it->toString() << " IS DEAD" << std::endl;}
            deadMinionIdxVec.push_back(std::make_pair(0, it-playerBoard.begin()));
            it++;
            // doDeathrattle(*it);
            // it = playerBoard.erase(it);
        } else {
            it++;
        }       
        if (it->GetDeathrattle()) {numDeathrattles++;}
    }

    if (isVerbose) {std::cout << "Checking Enemy Deaths" << std::endl;}
    for (auto it = begin (enemyBoard); it != end (enemyBoard);) {
        if ((it->GetHP() <= 0) && it->GetDeathrattle()) {
            if (isVerbose) {std::cout << it->toString() << " IS DEAD" << std::endl;}
            deadMinionIdxVec.push_back(std::make_pair(1, it-enemyBoard.begin()));
            it++;
            // doDeathrattle(*it);
            // it = enemyBoard.erase(it);
        } else {
            it++;
        }
        if (it->GetDeathrattle()) {numDeathrattles++;}
    }

    for (auto it = begin (playerBoard); it != end (playerBoard);) {
        if ((it->GetHP() <= 0) && !it->GetDeathrattle()) {
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
        if ((it->GetHP() <= 0) && !it->GetDeathrattle()) {
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

    std::shuffle(std::begin(deadMinionIdxVec), std::begin(deadMinionIdxVec)+numDeathrattles, std::default_random_engine());

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


// ++++++++++++++++++++++++++ DEATHRATTLE CODE ++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

void Board::doDeathrattle(Minion &minion, int idx) {
    std::vector<Minion> *affectedBoard;
    affectedBoard = (minion.IsPlayerMinion()) ? &playerBoard : &enemyBoard;

    // Generic Tribe Deaths
    if (minion.GetTribe().compare("Beast") == 0) {
        for (auto it = begin(*affectedBoard); it != end(*affectedBoard); ++it) {
            if (it->GetMinionType() == 1001) { // if it's a scavenging hyena
                // std::cout << "BUFFING HYENA" << std::endl;
                it->IncreaseATK(2);
                it->IncreaseHP(1);
            }
        }
    }


    // Specific Deathrattles
    switch(minion.GetMinionType()) {
        case 1002: // Fiendish Servant
            {
                int buffIdx = getDistFromRange(0, affectedBoard->size());
                if(isVerbose) {std::cout << "DEATHRATTLE: Fiendish Servant" << std::endl;}
                while (buffIdx == idx) {
                    buffIdx = getDistFromRange(0, affectedBoard->size());
                }
                if(isVerbose) {std::cout << "BUFFING IDX: " << buffIdx << std::endl;}
                (affectedBoard->begin() + buffIdx)->IncreaseATK(minion.GetATK());
            }
            break;
        case 1010: // Micro Mummy
            {
                if (isVerbose) {std::cout << "DEATHRATTLE: Micro Mummy" << std::endl;}
                Minion rebornMinion = Minion(10010, 1, 1); // 10010 is a token of the minion-type 1010
                (*affectedBoard).insert((*affectedBoard).begin()+idx+1, rebornMinion);
            }
            break;
        case 1015: // Scallywag <- NEEDS FIX
            {
                Minion skyPirate_Token = Minion(10015, 1, 1);
                affectedBoard->insert(affectedBoard->begin()+idx+1, skyPirate_Token);
                std::vector<Minion> *oppBoard = (!minion.IsPlayerMinion()) ? &playerBoard : &enemyBoard;
                bool victFlag = true;
                for(auto it=begin(*oppBoard); it!=end(*oppBoard); ++it) {
                    if(it->GetHP()>0) {
                        victFlag = false;
                    }
                }
                std::cout << "Vict Flag: " << victFlag << std::endl;
                if (!victFlag) {
                    Minion tokdef = getDefender(*oppBoard);
                    doAttack(*(affectedBoard->begin()+idx+1), tokdef);
                    fightHistory.push_back(std::make_pair(*(affectedBoard->begin()+idx+1), tokdef));
                    // checkDeaths();
                }
            }
            break;
        case 1016: // Acolyte of C'Thun (2/2 Reborn)
            {
                if (isVerbose) {std::cout << "DEATHRATTLE: Acolyte of C'Thun" << std::endl;}
                Minion rebornMinion = Minion(10016, 1, 2); // 10016 is a token of the minion-type 1016
                rebornMinion.SetTaunt(true);
                (*affectedBoard).insert((*affectedBoard).begin()+idx+1, rebornMinion);
                // for (auto it = begin (*affectedBoard); it != end (*affectedBoard); ++it) {
                //     std::cout << (*it).toString() << std::endl;
                // }
            }
            break;
        case 2000: // Spawn of N'Zoth (2/2, +2/+2)
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

// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ====== Check Win ======

bool Board::checkWin() {
    return (playerBoard.empty() || enemyBoard.empty()) ? 1 : 0;
}


// ====== Turn Code ======

void Board::doTurn() {
    std::vector<Minion> *attackerBoard;
    std::vector<Minion> *defenderBoard;
    std::vector<std::pair<int, int>> deadMinionIdxVec; // Every turn there is an index of dead minions. We populate this then resolve

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
    fightHistory.push_back(atk_def_pair);
    // printBoard();

    checkDeaths();

    // printBoard();

    // std::cout << "======== AFTER DEATHRATTLES ========" << std::endl;
    if (isVerbose) {printBoard();}

    isPlayerTurn = !isPlayerTurn;
}

void Board::singleSim(StatTracker &tracker) {
    int turnCounter = 1;
    setAttacker();
    int whoWentFirst = getWhoseTurn();

    for (auto it = begin(playerBoard); it != end(playerBoard); ++it) {
        tracker.playerSumOfTiers += it->tier;
    }
    for (auto it = begin(enemyBoard); it != end(enemyBoard); ++it) {
        tracker.enemySumOfTiers += it->tier;
    }

    while(!checkWin()) {
        // std::cout << "====== Turn: " << turnCounter << ", Attacker: " << (getWhoseTurn() ? "Player" : "Enemy") << " ======" << std::endl;
        doTurn();
        turnCounter++;
    }

    if (playerBoard.empty() && enemyBoard.empty()) {
        tracker.ties += 1;
        if (whoWentFirst) {tracker.ties_givenFirst += 1;}
        tracker.damageBreakdown[0+48] += 1;
        damageBreakdown[0+48] += 1;
        // std::cout << "It's a tie!" << std::endl;
    } else if (playerBoard.empty() && !enemyBoard.empty()) {
        // std::cout << "Player Loses!" << std::endl;
        tracker.losses += 1;
        if (whoWentFirst) {tracker.losses_givenFirst += 1;}
        int totaldamage = 0;
        for (auto it = begin(enemyBoard); it != end(enemyBoard); ++it) {
            totaldamage += it->tier;
        }
        tracker.damageBreakdown[48-totaldamage] += 1;
        damageBreakdown[48-totaldamage] += 1;
    } else { // (!board.playerBoard.empty() && board.enemyBoard.empty()) {
        // std::cout << "Player Wins!" << std::endl;
        tracker.wins += 1;
        if (whoWentFirst) {tracker.wins_givenFirst += 1;}
        int totaldamage = 0;
        for (auto it = begin(playerBoard); it != end(playerBoard); ++it) {
            totaldamage += it->tier;
        }
        tracker.damageBreakdown[48+totaldamage] += 1;
        damageBreakdown[48+totaldamage] += 1;
    }


    // for (auto it = begin(fightHistory); it != end(fightHistory); ++it) {
    //     std::cout << (*it).first.toString() << " attacks " << (*it).second.toString() << std::endl;
    // }
}

bool Board::getWhoseTurn() {return isPlayerTurn;}