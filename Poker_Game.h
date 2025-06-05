#ifndef POKERGAMEHEADER_H
#define POKERGAMEHEADER_H
#include "Player.cpp"
#include "DeckHeader.h"

template <typename U>
class Game
{
  public:
    template <typename T>
    friend ostream &operator<< (ostream &,const Game<T> &);

    Game(const string&, int&, int&);             // Constructor
    Game(const Game&);  // Copy Constructor
    ~Game();		// Destructor

    void setBet(const int&);
    int getBet() const;
    void placeBet();
    void resetBet();

    void computeCredit();
    void setCredit(const int&);
    int getCredit() const;

    void dealCards(int&);
    void display3Cards();
    void display2Cards();
    void displayHands();
    void dealerHands();

    void addWins();
    void check(const vector<DeckOfCards<U>>&, const vector<DeckOfCards<U>>&);
    void determineWinner();

    void setRankValue(const string&);
    int getRankValue() const;

    void setHandName(const string&);
    string getHandName() const;

    void setHighCard(const vector<DeckOfCards<U>>&);
    string getHighCard() const;

    int valueInput(int&);

  private:
    vector<pair<int,int>> playerCards; // will combine player hole cards and community cards into a single list
    vector<pair<int,int>> dealerCards; // will combine dealer hole cards and community cards into a single list

    int bet, credit, winner, newCredit;
    int rankValue;
    string handName;

    DeckOfCards<U> highCard;
    DeckOfCards<U> deck;
    Player<U> player;
    Player<U> dealer;
    vector<DeckOfCards<U>> commonCards;
};

#endif
