#include "Minion.h"
// #include <string>
// class Minion {
//     public:
//         Minion(int type, int hp, int atk);

//         int minionType;
//         int health;
//         int attack;
//         bool isDivine;
//         bool isPoison;

    
// };
Minion::Minion(int type, int hp, int atk) {
    minionType = type;
    SetHP(hp);
    SetATK(atk);

    isPlayerMinion = 1;

    isDivine = false;
    isPoison = false;
    attacked = false;
}

Minion::Minion(int type, int hp, int atk, bool whoseMin) { // Overloaded constructor to reference which character
    minionType = type;
    SetHP(hp);
    SetATK(atk);

    isPlayerMinion = whoseMin;

    isDivine = false;
    isPoison = false;
    attacked = false;
}

void Minion::SetHP(int hp) {health = hp;}
void Minion::SetATK(int atk) {attack = atk;}
void Minion::SetAtkState(bool atkd) {attacked = atkd;}
void Minion::SetTaunt(bool taunt) {isTaunt = taunt;}

int Minion::GetHP() {return health;}
int Minion::GetATK() {return attack;}
bool Minion::GetAtkState() {return attacked;}
bool Minion::GetTaunt() {return isTaunt;}
int Minion::GetMinionType() {return minionType;}
bool Minion::IsPlayerMinion() {return isPlayerMinion;}

void Minion::IncreaseHP(int deltaHP) {SetHP(GetHP()+deltaHP);}
void Minion::IncreaseATK(int deltaATK) {SetATK(GetATK()+deltaATK);}


std::string Minion::toString() {
    return "Type: " + std::to_string(minionType) + " ATK: " + std::to_string(attack) + " HP: " + std::to_string(health);
}