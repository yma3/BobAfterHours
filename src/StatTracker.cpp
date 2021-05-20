#include "StatTracker.h"


StatTracker::StatTracker() {
    wins = 0;
    losses = 0;
    ties = 0;
    wins_givenFirst = 0;
    losses_givenFirst = 0;
    playerSumOfTiers = 0;
    enemySumOfTiers = 0;
    for(int i = 0; i < 97; i++) {
        damageBreakdown.push_back(0);
    }
}

std::string StatTracker::toString() {
    return "Wins:"+std::to_string(wins)+",losses:"+std::to_string(losses)+",ties:"+
        std::to_string(ties)+",P(W|F):"+std::to_string(wins_givenFirst)+
        ",P(L|F):"+std::to_string(losses_givenFirst);
}

std::string StatTracker::damageToString() {
    std::ostringstream oss;
    std::copy(damageBreakdown.begin(), damageBreakdown.end()-1, std::ostream_iterator<int>(oss, ","));
    oss << damageBreakdown.back();
    return oss.str();
}