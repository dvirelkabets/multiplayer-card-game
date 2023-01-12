#include "Mtmchkin.h"


void readCard(std::shared_ptr<Card>& card, const std::string read){
   if (m_cardMap.count(read)){
    card = m_cardMap[read]();
   }
}

void readFileToDeck (const std::string &fileName){
    std::ifstream file (fileName);
    if (!file){
        throw DeckFileNotFound();
    }
    std::string read;
    int fileLine = 1;
    while (std::getline(file,read)){
        try{
            std::shared_ptr<Card> card;
            readCard(&card, read);
            m_cards.push_back(card);
            fileLine++;
        }
        catch(...){
            throw DeckFileFormatError(fileLine,read);
        }
    }
    if (fileLine<=4){
        throw DeckFileInvalidSize();
    }
}

bool isValidPlayerName (std::string player){
    try{
        Player tmpPlayer(player);
        return true;
    }
    catch(...){
        printInvalidName();
        return false;
    }  
}

bool assiagnJob (std::shared_ptr<Player>& player, std::string jobName, std::string playerName){
    if (!(isValidPlayerName(playerName))){
        return false;
    }
    if (m_playerMap.count(jobName)){
        player = m_playerMap[jobName](playerName);
    }
   else{
        printInvalidClass();
        return false;
    }
    return true;
}

void readPlayer (std::shared_ptr<Player>& player){
    std::string input;
    std::string playerName;
    std::string jobName;
    bool flag = true;
    int pos;
    while (flag){
        std::cin >> input;
        pos = input.find(" ");
        if (pos<=0){
            printInvalidName();
            continue;
        }
        playerName = input.substr(0,pos);
        jobName = input.substr(pos+1);
        if(assiagnJob(&player, jobName, playerName)){
            flag = false;
        }
    }
}

int readPlayerNumber(){
    int playersNum;
    bool flag = true;
    printStartGameMessage();
    while(flag){
        printEnterTeamSizeMessage();
        std::cin >> playersNum;
        if (std::cin.fail()||playersNum<2 || playersNum>6){
            printInvalidTeamSize();
            continue;
        }
        return playersNum;
    }
}

void Mtmchkin::playNextCard(std::shared_ptr<Player>& player){
    m_index = (m_index+1)%(m_cards.size());
    std::shared_ptr<Card> currentCard = m_cards[m_index];
    *currentCard.applyEncounter(&(*player)); // לעבור עם מאור
    
}

/*
Mtmchkin c'tor - 
read the cards from the file and if there is a problem with one of the cards it throws exception.
after that read the players number
for every player read the name and class and makeing sure everything is legel.
set index to the size of the vector of cards - using it as cyclic group.
*/
Mtmchkin::Mtmchkin (const std::string &fileName){
    // ---------------reading from file -------------------------
    readFileToDeck(fileName);
    //-----------------reading players num-----------------------
    int playersNum = readPlayerNumber();
    //----------------reading players----------------------------
    for (int i=0; i<playersNum;++i){
        printInsertPlayerMessage();
        std::shared_ptr<Player> player;
        readPlayer(&player);
        m_players.push_back(player);
    }
    m_index=m_cards.size();
}

void Mtmchkin::playRound(){
    printRoundStartMessage();
    for (std::shared_ptr<Player> player : m_players){
        if(*player.isKnockedOut() || *player.getLevel() == 10){
            continue;
        };
        printTurnStartMessage(*player.getName());
        playNextCard(&playerPtr)
        if (*player.getLevel() == 10){
            m_winnerStack.push(player);
        }
        if (*player.isKnockedOut()){
            m_loserStack.push(player);
        }
    }
    m_roundCounter++;
    if (isGameOver()){
        printGameEndMessage();
    }
}

void printLeaderBoard() const{
    if (m_winnerStack.empty() && m_loserStack.empty()){
        return;
    }
    std::deque<std::shared_ptr<Player>> leaderBoard;
    printLeaderboardStartMessage();
    for (std::shared_ptr<Player> player : m_winnerStack){
        leaderBoard.push_front(*player);
    }
    for (std::shared_ptr<Player> player : m_loserStack){
        leaderBoard.push_back(*player);
    }
    int i = 1;
    for (std::shared_ptr<Player> player : leaderBoard){
        printPlayerLeaderboard(i, &(*player));
        i++;
    }
}

bool isGameOver() const{
    if (m_players.size() == m_loserStack.size() + m_winnerStack.size()){
        return true;
    }
    return false;
}

 int getNumberOfRounds() const{
    return m_roundCounter;
 }