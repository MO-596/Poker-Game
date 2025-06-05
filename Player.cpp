#include "Player.h"
// Primary constructor: set initial name and zero stats
template<typename P>
Player<P>::Player()
 : playerName(playerName), win(0), loss(0), tie(0)
{
  // Empty, gets name from the operator overloading of >>
}

///////////////////////////////////////////////////
// Auxiliary name-based constructor: same behavior
template<typename P>
Player<P>::Player(const string& copyName)
 : playerName(copyName), win(0), loss(0), tie(0)
{
// Empty, copies from primary constrcutor
}

///////////////////////////////////////////////////
// Destructor, nothing happens
template<typename P>
Player<P>::~Player()
{
 // empty
}

///////////////////////////////////////////////////
// Clear all cards from current hand
template<typename P>
void Player<P>::clearHand()
{
  playerHand.clear();
}

///////////////////////////////////////////////////
// Adds a card to the player's hand
template<typename P>
void Player<P>::addCard(DeckOfCards<P> const& cards)
{
  playerHand.push_back(cards);
}

///////////////////////////////////////////////////
// Returns a copy of the current hand
template<typename P>
vector<DeckOfCards<P>> Player<P>::getHand() const
{
 return playerHand;
}

///////////////////////////////////////////////////
// Display each stored "hand" (calls each DeckOfCards::Dealing)
template<typename P>
void Player<P>::displayHand() const
{
  cout << getName() << "'s Hand: " << endl;
  for(const auto& deckHand : playerHand)
  {
    deckHand.Dealing(); // Prints the cards in the hand
    // std::cout << "Card: " << card.getRank() << " of " << card.getSuit() << std::endl;
  }
}

///////////////////////////////////////////////////
// Sets player's name
template<typename P>
void Player<P>::setName(const string& playerName)
{
  this->playerName = playerName;
}

///////////////////////////////////////////////////
// Gets player's name
template<typename P>
string Player<P>::getName() const
{
  return this->playerName;
}

///////////////////////////////////////////////////
// Increment win count
template<typename P>
void Player<P>::setWin()
{
  win += 1;
}

///////////////////////////////////////////////////
// Gets win count
template<typename P>
int Player<P>::getWin() const
{
  return win;
}

///////////////////////////////////////////////////
// Increment loss count
template<typename P>
void Player<P>::setLoss()
{
  loss += 1;
}

///////////////////////////////////////////////////
// Gets loss count
template<typename P>
int Player<P>::getLoss() const
{
  return loss;
}

///////////////////////////////////////////////////
// Increment tie count
template<typename P>
void Player<P>::setTie()
{
  tie += 1;
}

///////////////////////////////////////////////////
// Gets tie count
template<typename P>
int Player<P>::getTie() const
{
  return tie;
}

///////////////////////////////////////////////////
// Save stats to a binary file (overwrites)
template<typename P>
bool Player<P>::printStats(const string& filename) const
{
  fstream inputFile(filename, ios::out | ios::binary);
  if(inputFile.is_open())
  {
    inputFile << "Player: " << getName() << std::endl;
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

/////////////////////////////
// Ensure name contains only letters/spaces; prompts until valid
template<typename P>
string Player<P>::NameChecker(string& player_name)
{
  while (player_name.empty() || !std::all_of(player_name.begin(),
    player_name.end(), [](char c){return std::isalpha(c) || std::isspace(c);}))
  {
   cout << "Invalid name. Only letters and spaces are allowed. Try again: ";
   std::getline(cin, player_name);
  }
 return player_name;
}
/////////////////////////////
// Extraction operator for Player: reads and validates name
template<typename T>
istream &operator>> (istream &input, Player<T>& player_input)
{
  string player_name;

  std::cout << "Please enter your name: ";
  input >> player_name;
  player_name = player_input.NameChecker(player_name);

  player_input.setName(player_name);

 return input;
}
