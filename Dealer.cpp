#include "Dealer.h"

//Primary constructor: set zero stats
// Initializes win/loss/tie counters to 0. The dealer's name is set later
// via the templated operator>> defined for Player (see the commented-out
// line below), not here.
template<typename D>
Dealer<D>::Dealer()
: win(0), loss(0), tie(0)
{
  // Empty, gets name from the operator overloading of >>
}

///////////////////////////////////////////////////
// Auxiliary name-based constructor: same behavior
// Copy constructor: duplicates another Dealer's win/loss/tie stats.
template<typename D>
Dealer<D>::Dealer(const Dealer<D>& copy)
: win(copy.win), loss(copy.loss), tie(copy.tie)
{
// Empty, copies from primary constrcutor
}

///////////////////////////////////////////////////
// Destructor, nothing happens
// No dynamically allocated resources to release, so this is a no-op.
template<typename D>
Dealer<D>::~Dealer(){
 // empty
}

///////////////////////////////////////////////////
// Returns a copy of the dealer's current hand (vector of cards).
template<typename D>
vector<DeckOfCards<D>> Dealer<D>::getHand() const
{
  return dealerHand;
}

///////////////////////////////////////////////////
// Clear all cards from current hand
// Empties dealerHand so a new round can start with no leftover cards.
template<typename D>
void Dealer<D>::clearHand(){
  dealerHand.clear();
}

///////////////////////////////////////////////////
// Adds a card to the player's hand
// Appends one card (a DeckOfCards<D> acting as a single card) to the
// dealer's hand.
template<typename D>
void Dealer<D>::addCard(DeckOfCards<D> const& cards){
  dealerHand.push_back(cards);
}

///////////////////////////////////////////////////
// Display each stored "hand" (calls each DeckOfCards::Dealing)
// Prints the dealer's name followed by every card currently held,
// using each card's toString() representation.
template<typename D>
void Dealer<D>::displayHand() const{
  cout << dealerName <<"'s Hand: " << endl;
  for(const auto& card : dealerHand)
  {
    cout << card.toString() << "\n"; // Prints the cards in the hand
  }
}

///////////////////////////////////////////////////
// Increment win count
// Adds 1 to the dealer's win counter (called when the dealer wins a round).
template<typename D>
void Dealer<D>::setWin(){
  win += 1;
}

///////////////////////////////////////////////////
// Gets win count
// Returns the dealer's total number of wins so far.
template<typename D>
int Dealer<D>::getWin() const{
  return win;
}


///////////////////////////////////////////////////
// Increment loss count
// Adds 1 to the dealer's loss counter (called when the dealer loses a round).
template<typename D>
void Dealer<D>::setLoss(){
  loss += 1;
}

///////////////////////////////////////////////////
// Gets loss count
// Returns the dealer's total number of losses so far.
template<typename D>
int Dealer<D>::getLoss() const{
  return loss;
}

///////////////////////////////////////////////////
// Increment tie count
// Adds 1 to the dealer's tie counter (called when a round ends in a push/tie).
template<typename D>
void Dealer<D>::setTie(){
  tie += 1;
}

///////////////////////////////////////////////////
// Gets tie count
// Returns the dealer's total number of ties so far
template<typename D>
int Dealer<D>::getTie() const{
  return tie;
}

///////////////////////////////////////////////////
// Save stats to a binary file (overwrites)
// Opens (or creates) the given file in binary output mode and writes the
// dealer's label plus its win/loss/tie totals as plain text lines.
// Returns true if the file was opened and written successfully, false if
// the file couldn't be opened.
template<typename D>
bool Dealer<D>::printStats(const string& filename) const{
  fstream inputFile(filename, ios::out | ios::binary);
  if(inputFile.is_open())
  {
    inputFile << "Dealer"<< std::endl;
    inputFile << "Wins: " << getWin() << std::endl;
    inputFile << "Losses: " << getLoss() << std::endl;
    inputFile << "Ties: " << getTie() << std::endl;

    inputFile.close();
    cout << "Statitacs saved to file" << endl;
    return true;
  }
  else
  {
    cerr << "File could not be opened" << endl;
    return false;
  }
}

///////////////////////////////////////////////////
// Decides whether the dealer chooses to raise.
// Re-seeds rand() with the current time, then picks a random value of
// either 1 or 2 (rand() % 2 gives 0 or 1, then +1 shifts it to 1 or 2)
// and prints/returns that choice as the dealer's decision.
template<typename D>
int Dealer<D>::chooseRaise(){
  int choice;
  srand (time (0));

  choice = (rand() % 2) + 1;
  cout << "Testing rand range for the Dealer: " << choice  << endl;
 return choice;
}
