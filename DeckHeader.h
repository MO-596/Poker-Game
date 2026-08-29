#ifndef DECKHEADER_H
#define DECKHEADER_H
#include "MainHeader.h"

// DeckOfCards<D> represents a standard 52-card deck (4 suits x 13 ranks)
// AND, confusingly, is also reused as the representation of a single
// "card" elsewhere in the program: setRankIndex()/setSuitIndex() are used
// to turn one instance into a single card so it can be stored in a
// Player's/Dealer's hand and printed with toString().
template<typename D>
class DeckOfCards
{
  public:
	DeckOfCards(); //constuctor: fills Deck[][] with card IDs 1..52 and seeds rand()
	void Shuffling(); // randomly shuffles the 52 card IDs inside Deck[][]
	void Dealing(); // draws 5 cards from the deck and prints them as a hand
	void resetDeck();// resets nextCard to 0 and re-fills Deck[][] with 1..52 in order
	unsigned int drawOne();// draws and returns the next card's ID (1..52), reshuffling if the deck is empty

	void setRankIndex(const int&);// sets this card's rank (0..12 for Ace..King), clamping negatives to 0
	int getRankIndex() const;// gets this card's rank index

	void setSuitIndex(const int&);// sets this card's suit (0..3 for Hearts/Diamonds/Clubs/Spades), clamping negatives to 0
	int getSuitIndex() const;// gets this card's suit index

	string toString() const;// returns a human-readable "Rank of Suit" string for this card

  private:
	size_t nextCard = 0; // tracks the amount of cards that were drawn

	const char *suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };
	const char *face[13] =
	{ "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
	  "Eight", "Nine", "Ten", "Jack", "Queen", "King"
	};

	int rankIndex = -1; //holds a single face index (0..12 for Ace...King)
	int suitIndex = -1; //holds a single suit index (0..3 for Hearts, Diamonds, Clubs, Spades)
	int Hand[5][2] = { };// scratch space used by Dealing()/poker-hand-check functions: [i][0]=suit, [i][1]=rank
	unsigned int Deck[4][13] = { };// the 52 card IDs arranged by [suit][rank]
};
  #endif

