#ifndef EX4_Treasure_H
#define EX4_Treasure_H
#include "Card.h" 
class Treasure: public Card{
    static const int COINS_PLAYER_GETS = 10;

    public: 
        Treasure(); 
        void applyEncounter(Player& player) const override;
        static std::shared_ptr<Card> getCard();

        /*
        * Here we are explicitly telling the compiler to use the default methods
        */
        Treasure(const Treasure&) = default;
        ~Treasure() = default;
        Treasure& operator=(const Treasure& other) = default;
};

static std::shared_ptr<Treasure> TREASURE_CARD = std::make_shared<Treasure>();

#endif // EX4_Treasure_H