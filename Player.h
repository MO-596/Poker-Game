#ifndef PLAYERHEADER_H
#define PLAYERHEADER_H
#include "DeckHeader.h"
// Player<P> represents the human user: it holds a hand of cards, a
// display name, and running win/loss/tie stats.
template<typename P>
class Player{
  public:
	template <typename T>
	friend istream &operator>> (istream &,Player<T>&);// lets "cin >> player" read and validate a name

	// constructors
	Player();// default constructor: empty name, zeroed stats
	Player(const Player<P>&);// copy constructor: copies name + stats from another Player
	~Player();// destructor (nothing to clean up manually)

	// Hand access functions
	vector<DeckOfCards<P>> getHand() const;// returns a copy of the player's current hand
	void clearHand();// empties the player's hand (start of a new round)
	void addCard(DeckOfCards<P> const&);// adds one card to the player's hand
	void displayHand() const;// prints every card currently in the player's hand

	// set & get Name functions

	void setName(const string&); // stores the player's display name
	string getName() const;      // returns the player's display name

	// Win/Loss/Tie tracking functions
	void setWin();       // increments the player's win counter by 1
	int getWin() const;  // returns the player's total wins

	void setLoss();       // increments the player's loss counter by 1
	int getLoss() const;  // returns the player's total losses

	void setTie();        // increments the player's tie counter by 1
	int getTie() const;   // returns the player's total ties

	// stats output
	bool printStats(const string&) const; // writes win/loss/tie stats out to the given filename

	// utility function
	string NameChecker(string&); // validates/re-prompts until the given name is letters/spaces only
  private:
	string playerName;// the player's display name
	vector<DeckOfCards<P>> playerHand;// the player's current cards
	int win, loss, tie; // running totals for the player
};

#endif

