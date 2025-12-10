#ifndef DEALERHEADER_H
#define DEALERHEADER_H
#include "DeckHeader.h"

template<typename D>
class Dealer {
  public:
	Dealer();
	Dealer(const Dealer<D>&);
	~Dealer();

        // Hand access functions
        vector<DeckOfCards<D>> getHand() const;
        void clearHand();
        void addCard(DeckOfCards<D> const&);
        void displayHand() const;

	void setWin();
	int getWin() const;

	void setLoss();
	int getLoss() const;

	void setTie();
	int getTie() const;

	bool printStats(const string&) const;

  private:
	string dealerName = "Dealer";
	vector<DeckOfCards<D>> dealerHand;
	int win, loss, tie;

};

#endif

