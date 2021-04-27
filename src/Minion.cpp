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
    tribe = "Neutral";
}

Minion::Minion(int type, int hp, int atk, bool whoseMin) { // Overloaded constructor to reference which character
    minionType = type;
    SetHP(hp);
    SetATK(atk);

    isPlayerMinion = whoseMin;

    isDivine = false;
    isPoison = false;
    attacked = false;
    tribe = "Neutral";
}

void Minion::SetHP(int hp) {health = hp;}
void Minion::SetATK(int atk) {attack = atk;}
void Minion::SetAtkState(bool atkd) {attacked = atkd;}
void Minion::SetTaunt(bool taunt) {isTaunt = taunt;}
void Minion::SetTribe(std::string trb) {tribe = trb;}
void Minion::SetDivine(bool divine) {isDivine = divine;}
void Minion::SetPoison(bool poison) {isPoison = poison;}


int Minion::GetHP() {return health;}
int Minion::GetATK() {return attack;}
bool Minion::GetAtkState() {return attacked;}
bool Minion::GetTaunt() {return isTaunt;}
int Minion::GetMinionType() {return minionType;}
std::string Minion::GetTribe() {return tribe;}
bool Minion::GetDivine() {return isDivine;}
bool Minion::GetPoison() {return isPoison;}
bool Minion::IsPlayerMinion() {return isPlayerMinion;}

void Minion::IncreaseHP(int deltaHP) {SetHP(GetHP()+deltaHP);}
void Minion::IncreaseATK(int deltaATK) {SetATK(GetATK()+deltaATK);}


std::string Minion::toString() {
    return "Type: " + std::to_string(minionType) + " ATK: " + std::to_string(attack) + " HP: " + std::to_string(health) + " Tribe: " + tribe;
}