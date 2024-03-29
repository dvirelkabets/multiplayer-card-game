#include "Treasure.h" 
#include "../utilities.h"

Treasure::Treasure(): Card("Treasure"){}

void Treasure::applyEncounter(Player& player) const{
    player.addCoins(COINS_PLAYER_GETS);
    printTreasureMessage();
}

std::shared_ptr<Card> Treasure::getCard(){
    return TREASURE_CARD;
}
