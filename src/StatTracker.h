#ifndef STATTRACKER_H
#define STATTRACKER_H

#include <vector>
#include <string>
#include <sstream>
#include <iterator>

class StatTracker {
    public:
        StatTracker();

        int wins;
        int ties;
        int losses;
        int wins_givenFirst;
        int losses_givenFirst;
        int ties_givenFirst;

        int playerSumOfTiers;
        int enemySumOfTiers;

        std::vector<int> damageBreakdown;

        std::string toString();
        std::string damageToString();
};


#endif