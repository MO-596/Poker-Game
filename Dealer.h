#ifndef DEALERHEADER_H
#define DEALERHEADER_H
#include "DeckHeader.h"

// Dealer<D> represents the computer-controlled opponent: it holds its
// own hand of cards, tracks win/loss/tie stats, and can decide whether
// to raise during a round.
template<typename D>
class Dealer {
  public:
	Dealer(); // default constructor: zeroes out win/loss/tie stats
	Dealer(const Dealer<D>&);// copy constructor: copies win/loss/tie stats from another Dealer
	~Dealer();// destructor (nothing to clean up manually)

        // Hand access functions
        vector<DeckOfCards<D>> getHand() const;// returns a copy of the dealer's current hand
        void clearHand();// empties the dealer's hand (start of a new round)
        void addCard(DeckOfCards<D> const&);// adds one card to the dealer's hand
        void displayHand() const;// prints every card currently in the dealer's hand

	void setWin();// increments the dealer's win counter by 1
	int getWin() const;// returns the dealer's total wins

	void setLoss();// increments the dealer's loss counter by 1
	int getLoss() const;// returns the dealer's total losses

	void setTie();// increments the dealer's tie counter by 1
	int getTie() const;// returns the dealer's total ties

	bool printStats(const string&) const;// writes win/loss/tie stats out to the given filename
	int chooseRaise(); //has the dealer choose to either raise or no
  private:
	string dealerName = "Dealer";
	vector<DeckOfCards<D>> dealerHand;// the dealer's current cards
	int win, loss, tie;// running totals for the dealer
	int raiseAmount;// (currently unused) intended to hold the dealer's raise amount
};

#endif

