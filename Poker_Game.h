#ifndef POKERGAMEHEADER_H
#define POKERGAMEHEADER_H
#include "Player.h"
#include "DeckHeader.h"
#include "Dealer.h"

// Game<U> drives an entire round of Texas-Hold'em-style poker between a
// single Player and a single Dealer: it owns the deck, both hands, the
// community ("board") cards, the current bet/credit totals, and all of
// the hand-evaluation logic used to decide a winner.
template <typename U>
class Game
{
  public:
    template <typename T>
    friend ostream &operator<< (ostream &,const Game<T> &);// lets "cout << game" print a quick status summary

    Game();             // Constructor: zeroes out state and shuffles a fresh deck
    Game(const Game&);  // Copy Constructor: duplicates another Game's state
    ~Game();		// Destructor(nothing to clean up manually)

    void setBet(const int&);   // sets the current bet, clamping negative values to 0
    int getBet() const;        // returns the current bet
    void placeBet();           // prompts the user for a bet, validates it, then deducts it from credit
    void resetBet();           // sets the bet back to 0

    void computeCredit();          // subtracts the current bet from credit and stores the result
    void setCredit(const int&);    // sets the credit total, clamping negative values to 0
    int getCredit() const;         // returns the current credit total

    void dealCards(int&);// deals 2 hole cards to player & dealer, plus a given number of community cards
    void display3Cards(); // prints the first 3 community cards (the flop)
    void display2Cards();      // prints the 4th and 5th community cards (turn & river)
    void displayHands();       // prints both the player's and dealer's hands
    void dealerHands();        // prints only the dealer's hand
    void playerHands();        // prints only the player's hand

    void addWins();// increments the win counter of whichever side is stored in "winner"
    void check(const vector<DeckOfCards<U>>&, const vector<DeckOfCards<U>>&);// evaluates both combined hands and decides the winner
    void determineWinner();// combines hole+community cards, calls check(), and pays out credit
    int getWinner() const;// returns who won the last evaluated round: 1=player, 2=dealer, 0=tie, -1=none yet

    void setRankValue(const string&);// maps a hand-name string (e.g. "Flush") to its numeric rank and stores both
    int getRankValue() const;         // returns the numeric rank of the last determined hand

    void setHandName(const string&); // stores a hand-name string directly
    string getHandName() const;      // returns the stored hand-name string

    void setHighCard(const vector<DeckOfCards<U>>&); // finds and stores the highest-rank card among the given cards
    string getHighCard() const;                       // returns the stored high card as a "Rank of Suit" string

    void incrementRound();     // advances the round counter by 1
    int getRoundNumber() const; // returns the current round number

    int valueInput(int&); // validates a generic integer input is between 1 and 300, re-prompting until valid
//    bool SaveToFile(const string&) const;

    // builds player's and dealer's hole+community card lists
    void buildCombineHands(std::vector<DeckOfCards<U>>&, std::vector<DeckOfCards<U>>&) const;
    void startNewRound(); // shuffles + deals 2 hole cards
    bool revealNextCommunity(); // burns + reveals flop/turn/river
    bool canShowDown() const;// true once all 5 community cards have been revealed
    void dealFlop();   // burns a card, then deals the first 3 community cards
    void dealTurn();   // burns a card, then deals the 4th community card
    void dealRiver();  // burns a card, then deals the 5th community card
    void revealNext(); // reveals + prints the next community card stage based on current street
    bool isRoundOver() const;   // true once the river has been dealt (5 community cards are out)
    bool isRoundActive() const; // true while a round is currently in progress
    void fold();          // player forfeits the round; dealer wins the bet
    bool raise(int);      // increases the bet by the given amount if the player has enough credit
    void printShowdown() const;   // board + both hands + winner banner
    void printLastResult() const; // last finished round (handles folds)
    static string rankName(int);  // converts a rank index (0..12, or 13=ace-high) into its name string

  private:
    void evaluteHand(const vector<DeckOfCards<U>>&,int&,int&,string&); // works out the best hand category/high-card/name for a set of cards
    void computeCounts(const vector<DeckOfCards<U>>&, std::vector<int>[], int&); // tallies rank/suit counts used by the other detect* helpers
    void detectMultiples(int&, std::vector<int>&, std::vector<int>&); // finds four-of-a-kind, three-of-a-kinds, and pairs from rank counts
    void detectStraight(bool&, int&); // checks the rank counts for a 5-in-a-row straight
    void detectFlushAndStraightFlush(std::vector<int>[], bool&, int&, int&, bool&, int&); // checks for a flush and, within it, a straight flush
    void burnOne();              // draws and discards one card into the burn pile (standard poker "burn" before each street)
    DeckOfCards<U> makeCardFromId(unsigned int); // converts a raw card ID (1..52) into a DeckOfCards<U> "card" object

    vector<pair<int,int>> playerCards; // will combine player hole cards and community cards into a single list
    vector<pair<int,int>> dealerCards; // will combine dealer hole cards and community cards into a single list
    int rankCount[13] = {0}; // scratch: how many of each rank (0..12) appear in the hand being evaluated
    int suitCount[4] = {0};  // scratch: how many of each suit (0..3) appear in the hand being evaluated

    int bet, credit, winner, newCredit; // current bet, credit total, last round's winner code, and a scratch credit value
    int rankValue;      // numeric strength of the most recently determined hand (0=High Card ... 8=Straight/Royal Flush)
    string handName;    // textual name of the most recently determined hand
    int roundNum;        // how many rounds have been played so far
    int revealedCommCard = 0; // how many community cards have been revealed via revealNextCommunity()
    int street = 0; // 0=preflop, 1=flop, 2=turn, 3=river, 4=showdown
    bool roundActive = false; // true while the current round hasn't finished yet
    bool folded = false;      // true if the player folded during the current/last round
    int lastCreditChange = 0; // net credit change from the most recently finished round
    string playerResult, dealerResult;   // hand names from last showdown
    int playerBestRank = -1, dealerBestRank = -1; // high-card rank index for each side from the last showdown

    DeckOfCards<U> highCard; // the overall high card from the winning side's combined hand
    DeckOfCards<U> deck;     // the shared 52-card deck used to deal this game
    Player<U> player;        // the human player
    Dealer<U> dealer;        // the computer dealer
    vector<DeckOfCards<U>> commonCards; // the shared community ("board") cards
    vector<DeckOfCards<U>> burnPile;    // cards discarded face-down before each street, per poker convention};
};
#endif
