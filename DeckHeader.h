#ifndef DECKHEADER_H
#define DECKHEADER_H
#include "MainHeader.h"

template<typename D>
class DeckOfCards
{
  public:
	DeckOfCards();		//constuctor
	void Shuffling();
	void Dealing();
	void resetDeck();
	unsigned int drawOne();

	void Pair();			//to check if the placement is a pair
	bool TwoPair();
	void Three_Of_A_Kind();
	void Four_Of_A_Kind();
	void Straight();
	void Flush();
	void FullHouse();
	bool StraightFlush();
	void RoyalFlush();

	int setRankIndex(const int&);
	void getRankIndex() const;

	int setSuitIndex(const int&);
	void getSuitIndex() const;

  private:
	size_t nextCard = 0; // tracks the amount of cards that were drawn

	const char *suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" };
	const char *face[13] =
	{ "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
	  "Eight", "Nine", "Ten", "Jack", "Queen", "King"
	};

	int rankIndex = -1; //holds a single face index (0..12 for Ace...King)
	int suitIndex = -1; //holds a single suit index (0..3 for Hearts, Diamonds, Clubs, Spades)
	int Hand[5][2] = { };
	unsigned int Deck[4][13] = { };
};
  #endif

