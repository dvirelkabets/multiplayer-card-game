#include "Mtmchkin.h"
/*
here defining the maps that are static members of the class,
the maps only created once since they are static and will be destroyed when the program is finished.
that way the maps are NOT global varibles :)
*/

std::map<std::string, getCard> Mtmchkin::m_cardMap = {{"Well", &Well::getCard},{"Gremlin", &Gremlin::getCard},{"Witch", &Witch::getCard},
                {"Dragon",&Dragon::getCard},{"Merchant", &Merchant::getCard},{"Treasure",&Treasure::getCard},
                {"Barfight",&Barfight::getCard},{"Mana",&Mana::getCard}};
    
std::map<std::string, getPlayer> Mtmchkin::m_playerMap = {{"Ninja", &Ninja::getPlayer}, {"Warrior", &Warrior::getPlayer}, {"Healer",&Healer::getPlayer}};


void Mtmchkin::readFileToDeck (const std::string &fileName){
    std::ifstream file (fileName);
    if (!file){
        throw DeckFileNotFound();
    }
    std::string read;
    int fileLine = 1;
    while (std::getline(file,read)){
        if (m_cardMap.count(read)){
            m_cards.push_back(m_cardMap.at(read)());
        }
        else{
            throw DeckFileFormatError(fileLine);
        }
        fileLine++;
    }
    if (fileLine<=5){
        throw DeckFileInvalidSize();
    }
}

bool isDigit(std::string& word){
    for (char letter : word){
        if (!(std::isdigit(letter))){
            return false;
        }
    }
    return true;
}


int Mtmchkin::readPlayerNumber() const{
    std::string numberInput;
    int playersNum;
    while(true){
        printEnterTeamSizeMessage();
        std::cin >> numberInput;
        if (isDigit(numberInput)){
            playersNum = std::stoi(numberInput);
            if (playersNum>=2 && playersNum<=6){
                break;
            }
        }
        printInvalidTeamSize();
    }
    return playersNum;
}

bool Mtmchkin::isValidPlayerName (std::string player){
    try{
        Ninja tmpPlayer(player);
        return true;
    }
    catch(...){
        printInvalidName();
        return false;
    }
}

bool Mtmchkin::assignJob (std::shared_ptr<Player>& player, std::string jobName, std::string playerName){
    if (!(isValidPlayerName(playerName))){
        return false;
    }
    if (m_playerMap.count(jobName)){
        player = m_playerMap.at(jobName)(playerName);
    }
    else{
        printInvalidClass();
        return false;
    }
    return true;
}


void Mtmchkin::readPlayer(std::shared_ptr<Player>& player){
    std::string input;
    std::string playerName;
    std::string jobName;
    std::string trash;
    bool flag = true;
    while (flag){
        std::getline(std::cin, input);
        std::istringstream iStream(input);
        iStream>>playerName;
        if (iStream.eof()){
            continue;
        }
        iStream>>jobName;
        iStream>>trash;
        if (!(iStream.fail())){
            printInvalidName();
            continue;
        }
        if(assignJob(player, jobName, playerName)){
            flag = false;
        }
    }
}

void Mtmchkin::playNextCard(std::shared_ptr<Player>& player){
    m_index = (m_index+1)%(m_cards.size());
    std::shared_ptr<Card> currentCard = m_cards[m_index];
    currentCard->applyEncounter(*player);
}

/*
Mtmchkin c'tor - 
read the cards from the file and if there is a problem with one of the cards it throws exception.
after that read the players number
for every player read the name and class and making sure everything is legal.
set index to the size of the vector of cards - using it as cyclic group.
*/

Mtmchkin::Mtmchkin (const std::string &fileName){
    printStartGameMessage();
    // ---------------reading from file -------------------------
    readFileToDeck(fileName);
    //-----------------reading players num-----------------------
    int playersNum = readPlayerNumber();
    std::cin.ignore(20, '\n');
    //----------------reading players----------------------------
    for (int i=0; i<playersNum;++i){
        printInsertPlayerMessage();
        std::shared_ptr<Player> player;
        readPlayer(player);
        m_players.push_back(player);
    }
    m_index=m_cards.size()-1;
}

void Mtmchkin::playRound(){
    typedef std::vector<std::shared_ptr<Player>>::iterator iterator;
    printRoundStartMessage(m_roundCounter);
    std::vector<iterator> needtoRemove;
    iterator index=m_players.begin();
    for (std::shared_ptr<Player> player : m_players) {
        printTurnStartMessage(player->getName());
        playNextCard(player);
        if (player->getLevel() == 10){
            m_winnerVector.push_back(player);
            needtoRemove.push_back(index);
        }
        if (player->isKnockedOut()){
            m_loserVector.push_back(player);
            needtoRemove.push_back(index);
        }
        index++;
    }
    for (std::vector<iterator>::reverse_iterator it = needtoRemove.rbegin(); it!=needtoRemove.rend();++it){
        m_players.erase(*it);
    }
    m_roundCounter++;
    if (isGameOver()){
        printGameEndMessage();
    }
}

void Mtmchkin::printLeaderBoard() const{
    printLeaderBoardStartMessage();
    int i = 1;
    for (std::shared_ptr<Player> player : m_winnerVector){
        printPlayerLeaderBoard(i, *player);
        i++;
    }

    for (std::shared_ptr<Player> player : m_players){
        printPlayerLeaderBoard(i, *player);
        i++;
    }
    
    for (std::vector<std::shared_ptr<Player>>::const_reverse_iterator it = m_loserVector.crbegin(); it != m_loserVector.crend(); ++it){
        printPlayerLeaderBoard(i, *(*it));
        i++;
    }
}

bool Mtmchkin::isGameOver() const{
    if (m_players.empty()){
        return true;
    }
    return false;
}

 int Mtmchkin::getNumberOfRounds() const{
    return m_roundCounter-1;
 }