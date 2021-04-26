#ifndef MINION_H
#define MINION_H

#include <string>

class Minion {
    public:
        Minion(int type, int hp, int atk);
        Minion(int type, int hp, int atk, bool whoseMin);

        void SetHP(int hp);
        void SetATK(int atk);
        void SetAtkState(bool atkd);
        void SetTaunt(bool taunt);
        int GetHP();
        int GetATK();
        bool GetAtkState();
        bool GetTaunt();
        int GetMinionType();
        bool IsPlayerMinion();
        
        void IncreaseHP(int deltaHP);
        void IncreaseATK(int deltaATK);

        std::string toString();

    private:
        int minionType;
        int health;
        int attack;
        bool isDivine;
        bool isPoison;
        bool isTaunt;
        bool attacked;

        bool isPlayerMinion; // contains information on whose minion it belongs to

    
};

#endif