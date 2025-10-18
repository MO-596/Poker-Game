#include "Dealer.h"
//Primary constructor: set zero stats
template<typename D>
Dealer<D>::Dealer()
: win(0), loss(0), tie(0)
{
//  this->Player<D>::setName("Dealer");
  // Empty, gets name from the operator overloading of >>
}

///////////////////////////////////////////////////
// Auxiliary name-based constructor: same behavior

template<typename D>
Dealer<D>::Dealer(const Dealer<D>& copy)
: win(copy.win), loss(copy.loss), tie(copy.tie)
{
// Empty, copies from primary constrcutor
}

///////////////////////////////////////////////////
// Destructor, nothing happens
template<typename D>
Dealer<D>::~Dealer(){
 // empty
}

template<typename D>
vector<DeckOfCards<D>> Dealer<D>::getHand() const
{
  return dealerHand;
}

///////////////////////////////////////////////////
// Clear all cards from current hand
template<typename D>
void Dealer<D>::clearHand(){
  dealerHand.clear();
//  this->Player<D>::clearHand();

}

///////////////////////////////////////////////////
// Adds a card to the player's hand
template<typename D>
void Dealer<D>::addCard(DeckOfCards<D> const& cards){
  dealerHand.push_back(cards);
}

///////////////////////////////////////////////////
// Display each stored "hand" (calls each DeckOfCards::Dealing)
template<typename D>
void Dealer<D>::displayHand() const{
  cout << dealerName <<" Hand: " << endl;
  for(const auto& deckHand : dealerHand)
  {
    deckHand.Dealing(); // Prints the cards in the hand
    // std::cout << "Card: " << card.getRank() << " of " << card.getSuit() << std::endl;
  }

}

///////////////////////////////////////////////////
// Increment win count
template<typename D>
void Dealer<D>::setWin(){
  win += 1;
}

///////////////////////////////////////////////////
// Gets win count
template<typename D>
int Dealer<D>::getWin() const{
  return win;
}


///////////////////////////////////////////////////
// Increment loss count
template<typename D>
void Dealer<D>::setLoss(){
  loss += 1;
}

///////////////////////////////////////////////////
// Gets loss count
template<typename D>
int Dealer<D>::getLoss() const{
  return loss;
}

///////////////////////////////////////////////////
// Increment tie count
template<typename D>
void Dealer<D>::setTie(){
  tie += 1;
}

///////////////////////////////////////////////////
// Gets tie count
template<typename D>
int Dealer<D>::getTie() const{
  return tie;
}


///////////////////////////////////////////////////
// Save stats to a binary file (overwrites)
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
