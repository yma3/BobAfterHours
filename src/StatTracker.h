#ifndef STATTRACKER_H
#define STATTRACKER_H

#include <vector>
#include <string>

class StatTracker {
    public:
        StatTracker();

        int wins;
        int ties;
        int losses;
        int wins_givenFirst;
        int losses_givenFirst;
        std::vector<int> damageBreakdown;

        std::string toString();
};


#endif