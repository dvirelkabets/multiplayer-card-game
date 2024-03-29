#include "Well.h"
#include "../utilities.h"
#include <string>

Well::Well(): Card("Well"){}

void Well::applyEncounter(Player& player) const{
    bool isNinja = true;
    if(player.getJob() != "Ninja"){
        player.damage(DAMAGE); 
        isNinja = false; 
    }
    printWellMessage(isNinja);
}

std::shared_ptr<Card> Well::getCard(){
    return WELL_CARD;
}
