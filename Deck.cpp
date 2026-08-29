#include "DeckHeader.h"

// Constructor: builds a "fresh" ordered deck.
// Fills Deck[row][column] with sequential IDs 1..52 (row = suit, column = rank),
// then seeds the random number generator with the current time so that
// Shuffling()/rand() produce different results each run.
template<typename D>
DeckOfCards<D>::DeckOfCards ()
{
  int cnt = 1;
  for (int row = 0; row <= 3; row++)    //loops through the rows of the deck
  {
    for (int column = 0; column <= 12; column++)      //loops through the columns of the deck
    {
      Deck[row][column] = cnt++;        //setd the deck to incremetn
    }
  }

  srand (time (0));
}

/////////////////////////////////////////////////////////////////////////////////////
// Shuffles the deck in place.
// For every position in the 4x13 Deck array, picks a random row/column and
// swaps the current card with whatever card is sitting at that random
// position (a simple randomized swap-based shuffle).
template<typename D>
void DeckOfCards<D>::Shuffling ()
{
  int randRow;			//siut of the card
  int randCol;			//value of the card
  int temp;			//to place a tempoary value of the new row and col
  int row,column;

  for ( row = 0; row < 4; row++)
  {
    for ( column = 0; column < 13; column++)
    {
	randRow = rand() % 4;
	randCol = rand() % 13;

	temp = Deck[row][column];
	Deck[row][column] = Deck[randRow][randCol];
	Deck[randRow][randCol] = temp;
    }
  }
}

/////////////////////////////////////////////////////////////////////////////
// Draws 5 cards from the deck (via drawOne()), converts each card's numeric
// ID into a suit/rank pair, stores them in the Hand[][] scratch array, and
// prints the resulting 5-card hand to the console.
template<typename D>
void DeckOfCards<D>::Dealing()
{
  // reset draw so each dealing starts on top
  size_t row, col;
  unsigned int id;

  cout << "The hand is:\n";
  for (int i = 0; i < 5; ++i)
  {
    id = drawOne();// grab the next card ID (1..52)

    row = (id - 1) / 13; // suit indexes
    col = (id - 1) % 13;  // face indexes

    Hand[i][0] = row;// store suit for this hand sl
    Hand[i][1] = col;// store rank for this hand slot

    cout << face[col] << " of " << suit[row] << "\n";
  }
  cout << "\n";
}

///////////////////////////////////////////////////////////////////////////////////
// Rebuilds Deck[][] back into ordered card IDs 1..52 and resets the
// "next card to draw" counter to 0, effectively undoing any drawing that
// happened previously (used before a fresh shuffle for a new round).
template<typename D>
void DeckOfCards<D>::resetDeck()
{
  // will reset the deck to be empty to allow new shuffle to happen
  nextCard = 0;
  int count = 1, row, col;
  for(row = 0; row < 4; row++)
  {
    for(col = 0; col < 13; col++)
    {
      Deck[row][col] = count++;
    }
  }
}

///////////////////////////////////////////////////////////////////////////////////
// Draws and returns the ID (1..52) of the next card in the deck.
// If every card has already been drawn (nextCard reached 52), the deck is
// automatically reset and reshuffled before drawing continues. Internally,
// nextCard is treated as a linear index into the 4x13 Deck array (13 cards
// per suit "row"), which is converted to a row/column pair to look up the
// card's ID.
template<typename D>
unsigned int DeckOfCards<D>::drawOne()
{
  if(nextCard >= 52){
    resetDeck();
    Shuffling();
  }

  // maps linear index into the Deck array
  size_t row = nextCard / 13;
  size_t col = nextCard % 13;

  unsigned int cardID = Deck[row][col]; // grab the card id (1..52) from Deck[row][col]
  ++nextCard;

 return cardID;
}

///////////////////////////////////////////////////////////////////////////////////
// Sets this DeckOfCards instance's rank (used when this object represents
// a single card rather than a whole deck). Negative values are treated as
// invalid and clamped to 0, with a warning printed to the console.
template<typename D>
void DeckOfCards<D>::setRankIndex(const int& rankIndex)
{
  this->rankIndex = (rankIndex < 0) ? 0 : rankIndex;
  if(rankIndex < 0)
  {
    cout << "Invalid Rank Index; Setting to 0";
    this->rankIndex = 0;
  }
  else
  {
    this->rankIndex = rankIndex;
  }
}
//////////////////////////////////////////////////////////////////////////
// Returns this card's stored rank index (0..12 for Ace..King).
template<typename D>
int DeckOfCards<D>::getRankIndex() const
{
  return rankIndex;
}
//////////////////////////////////////////////////////////////////////////
// Sets this DeckOfCards instance's suit (used when this object represents
// a single card). Negative values are clamped to 0, with a warning printed.
template<typename D>
void DeckOfCards<D>::setSuitIndex(const int& suitIndex)
{
  this->suitIndex = (suitIndex < 0) ? 0 : suitIndex;
  if(suitIndex < 0)
  {
    cout << "Invalid Suit Index; Setting to 0";
    this->suitIndex = 0;
  }
  else
  {
    this->suitIndex = suitIndex;
  }
}
//////////////////////////////////////////////////////////////////////////
// Returns this card's stored suit index (0..3 for Hearts/Diamonds/Clubs/Spades).
template<typename D>
int DeckOfCards<D>::getSuitIndex() const
{
  return suitIndex;
}
//////////////////////////////////////////////////////////////////////////
// Builds and returns a human-readable "<Rank> of <Suit>" string for this
// card (e.g. "Queen of Hearts"). Returns "Unkown" if the rank/suit indices
// haven't been set to valid values.
template<typename D>
std:: string DeckOfCards<D>::toString() const
{
  if(rankIndex < 0 || rankIndex > 12 || suitIndex < 0 || suitIndex > 3){
    return "Unkown";
  }

  return std::string(face[rankIndex]) + " of " + suit[suitIndex];
}
