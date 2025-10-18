#ifndef PLAYERHEADER_H
#define PLAYERHEADER_H
#include "DeckHeader.h"

template<typename P>
class Player{
  public:
	template <typename T>
	friend istream &operator>> (istream &,Player<T>&);

	// constructors
	Player();
	Player(const Player<P>&);
//	Player(const string&, int&, int&, int&);
	~Player();

	// Hand access functions
	vector<DeckOfCards<P>> getHand() const;
	void clearHand();
	void addCard(DeckOfCards<P> const&);
	void displayHand() const;

	// set & get Name functions
	void setName(const string&);
	string getName() const;

	// Win/Loss/Tie tracking functions
	void setWin();
	int getWin() const;

	void setLoss();
	int getLoss() const;

	void setTie();
	int getTie() const;

	// stats output
	bool printStats(const string&) const;

	// utility function
	string NameChecker(string&);

  private:
	string playerName;
	vector<DeckOfCards<P>> playerHand;
	int win, loss, tie;
};

#endif

