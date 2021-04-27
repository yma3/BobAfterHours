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
        void SetTribe(std::string trb);
        void SetDivine(bool divine);
        void SetPoison(bool poison);
        void SetDeathrattle(bool deathrattle);
        void SetReborn(bool reborn);
        int GetHP();
        int GetATK();
        bool GetAtkState();
        bool GetTaunt();
        int GetMinionType();
        std::string GetTribe();
        bool GetDivine();
        bool GetPoison();
        bool GetDeathrattle();
        bool GetReborn();
        bool IsPlayerMinion();
        
        void IncreaseHP(int deltaHP);
        void IncreaseATK(int deltaATK);

        std::string toString();

    private:
        int minionType;
        int health;
        int attack;
        std::string tribe;
        bool isDivine;
        bool isPoison;
        bool isTaunt;
        bool attacked;
        bool isDeathrattle;
        bool isReborn;

        bool isPlayerMinion; // contains information on whose minion it belongs to

    
};

#endif