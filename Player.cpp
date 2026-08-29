#include "Player.h"

// Primary constructor: set initial name and zero stats
// Starts the player with an empty name and win/loss/tie counters at 0.
// The name is filled in later, typically via the templated operator>>.
template<typename P>
Player<P>::Player()
 : playerName(""), win(0), loss(0), tie(0)
{
  // Empty, gets name from the operator overloading of >>
}

///////////////////////////////////////////////////
// Auxiliary name-based constructor: same behavior
// Copy constructor: duplicates another Player's name and win/loss/tie stats.
template<typename P>
Player<P>::Player(const Player<P>& copy)
 : playerName(copy.playerName), win(copy.win), loss(copy.loss), tie(copy.tie)
{
// Empty, copies from primary constrcutor
}

///////////////////////////////////////////////////
// Destructor, nothing happens
// No dynamically allocated resources to release, so this is a no-op.
template<typename P>
Player<P>::~Player()
{
 // empty
}

///////////////////////////////////////////////////
// Clear all cards from current hand
// Empties playerHand so a new round can start with no leftover cards.
template<typename P>
void Player<P>::clearHand()
{
  playerHand.clear();
}

///////////////////////////////////////////////////
// Adds a card to the player's hand
// Appends one card (a DeckOfCards<P> acting as a single card) to the
// player's hand.
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
// Prints the player's name (or "Player" if no name has been set yet)
// followed by every card currently held, using each card's toString().
template<typename P>
void Player<P>::displayHand() const
{
  cout << (playerName.empty() ? string("Player") : playerName) << "'s Hand: " << endl;
  for(const auto& card : playerHand)
  {
    cout << card.toString() << "\n"; // Prints the cards in the hand
  }
}

///////////////////////////////////////////////////
// Sets player's name
// Stores the given string as the player's display name.
template<typename P>
void Player<P>::setName(const string& playerName)
{
  this->playerName = playerName;
}

///////////////////////////////////////////////////
// Gets player's name
// Returns the player's currently stored display name.
template<typename P>
string Player<P>::getName() const
{
  return this->playerName;
}

///////////////////////////////////////////////////
// Increment win count
// Adds 1 to the player's win counter (called when the player wins a round).
template<typename P>
void Player<P>::setWin()
{
  win += 1;
}

///////////////////////////////////////////////////
// Gets win count
// Returns the player's total number of wins so far.
template<typename P>
int Player<P>::getWin() const
{
  return win;
}

///////////////////////////////////////////////////
// Increment loss count
// Adds 1 to the player's loss counter (called when the player loses a round).
template<typename P>
void Player<P>::setLoss()
{
  loss += 1;
}

///////////////////////////////////////////////////
// Gets loss count
// Returns the player's total number of losses so far
template<typename P>
int Player<P>::getLoss() const
{
  return loss;
}

///////////////////////////////////////////////////
// Increment tie count
// Adds 1 to the player's tie counter (called when a round ends in a push/tie).
template<typename P>
void Player<P>::setTie()
{
  tie += 1;
}

///////////////////////////////////////////////////
// Gets tie count
// Returns the player's total number of ties so fa
template<typename P>
int Player<P>::getTie() const
{
  return tie;
}

///////////////////////////////////////////////////
// Save stats to a binary file (overwrites)
// Opens (or creates) the given file in binary output mode and writes the
// player's name plus win/loss/tie totals as plain text lines.
// Returns true if the file was opened and written successfully, false if
// the file couldn't be opened.
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
// Loops while the given name is empty OR contains any character that
// isn't a letter or whitespace, re-reading a full line from cin each
// time until a valid name is entered, then returns that valid name.
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
// Overloads ">>" so that "cin >> player" prompts for and reads a name,
// validates it via NameChecker(), and stores the result on the Player
// object via setName().
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
