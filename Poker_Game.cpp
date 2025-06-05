#include "Poker_Game.h"

// Constructor: set up deck, shuffle, player and dealer, initial bets/credits
template <typename U>
Game<U>::Game(const string& playerName, int& bet, int& credit)
: bet(0), credit(0), winner(-1), newCredit(0), rankValue(0),
   handName(""), highCard(), deck(), player(playerName), dealer("Dealer")
{
  deck.Shuffling();
  setBet(bet);
  setCredit(credit);
}

//////////////////////////////////////////////////////
// Copy constructor
template <typename U>
Game<U>::Game(const Game& copy)
 : bet(copy.bet), credit(copy.credit), winner(copy.winner), newCredit(copy.newCredit),
    rankValue(copy.rankValue), handName(copy.handName),
    highCard(copy.highCard), deck(copy.deck),
    player(copy.player), dealer(copy.dealer), commonCards(copy.commonCards)
{
 // empty
}

//////////////////////////////////////////////////////
// Destructor
template <typename U>
Game<U>::~Game()
{
 // empty
}

//////////////////////////////////////////////////////
// Set bet with validation
template <typename U>
void Game<U>::setBet(const int& bet)
{
  this->bet = (bet < 0) ? 0 : bet;
  if(bet < 0)
  {
    cout << "Invalid bet, setting to 0\n" << endl;
    this->bet = 0;
  }
  else
  {
    this->bet = bet;
  }
}

//////////////////////////////////////////////////////
// Get current bet
template <typename U>
int Game<U>::getBet() const
{
  return bet;
}

//////////////////////////////////////////////////////
// Prompt user to place bet
template <typename U>
void Game<U>::placeBet()
{
  int amount = 0;
  do{
    cout << "Place your bet (0 -"<<getCredit() << "): "<< endl;
    amount = valueInput(amount);

    if(amount < 0 || amount > credit)
    {
      cout << "Invalid bet. Try again.\n";
    }
  }while(amount < 0 || amount > credit);
  setBet(amount);
  computeCredit();
}

//////////////////////////////////////////////////////
// Reset bet to zero
template <typename U>
void Game<U>::resetBet()
{
  setBet(0);
}

//////////////////////////////////////////////////////
// Set credit with validation
template <typename U>
void Game<U>::setCredit(const int& credit)
{
  this->credit = (credit < 0) ? 0 : credit;
  if(credit < 0)
  {
    cout << "Invalid credit; setting to 0" << endl;
    this->credit = 0;
  }
  else
  {
    this->credit = credit;
  }
}

//////////////////////////////////////////////////////
// Get current credit
template <typename U>
int Game<U>::getCredit() const
{
  return credit;
}

//////////////////////////////////////////////////////
// Adjust credit after bet
template <typename U>
void Game<U>::computeCredit()
{
//  newCredit = getCredit() - getBet();
  newCredit = credit - bet;
  setCredit(newCredit);
}

//////////////////////////////////////////////////////
// Deal hole cards and community cards
template <typename U>
void Game<U>::dealCards(int& numCards)
{
  player.clearHand();
  dealer.clearHand();
  commonCards.clear();

//  for(int i = 0; i < numCards; i++)
  // Deal two to player & dealer
  for(int i = 0; i < 2; i++)
  {
    auto playerCard = deck.drawOne();
    player.addCard(playerCard);

    auto dealerCard = deck.drawOne();
    dealer.addCard(dealerCard);
/*
    DeckOfCards<U> playerCard = deck.Dealing();
    player.addCard(playerCard);

    DeckOfCards<U> dealerCard = deck.Dealing();
    dealer.addCard(dealerCard);
*/
  }

  // Community cards
  for(int i = 0; i < numCards; i++)
  {
    auto card = deck.drawOne();
    commonCards.push_back(card);
  }

}

//////////////////////////////////////////////////////
// Display first three community cards (flop)
template <typename U>
void Game<U>::display3Cards()
{
  cout << "Flop: " << endl;
  for(int i = 0; i < 3 && i < commonCards.size(); i++)
  {
    commonCards[i].Dealing();
  }

}

//////////////////////////////////////////////////////
// Display turn and river
template <typename U>
void Game<U>::display2Cards()
{
  cout << "Turn & River: " << endl;
  for(int i = 3; i < 5 && i < commonCards.size(); i++)
  {
    commonCards[i].Dealing();
  }
}

//////////////////////////////////////////////////////
// Show both hands
template <typename U>
void Game<U>::displayHands()
{
  player.displayHand();
  dealer.displayHand();
}

//////////////////////////////////////////////////////
// Show dealer only
template <typename U>
void Game<U>::dealerHands()
{
  dealer.displayHand();
}

//////////////////////////////////////////////////////
// Increment win count for winner
template <typename U>
void Game<U>::addWins()
{
  if(winner = 1)
  {
    player.setWin();
  }
  else if(winner = 2)
  {
    dealer.setWin();
  }
}

//////////////////////////////////////////////////////
// Evaluate hands and set winner, rankValue, handName, highCard
template <typename U>
void Game<U>::check(const vector<DeckOfCards<U>>& playerCom, const vector<DeckOfCards<U>>& dealerCom)
{
  // TODO: implement hand-ranking logic, set winner, rankValue, handName, highCard

  // Example: winner = 1 for player, 2 for dealer, 0 for tie
  // Collect all rank indices:
  vector<int> playerRanks;
  vector<int> dealerRanks;
  // Local storage for players best hand values
  int playerRankValue = 0;// 0 = High Card, 1 = Pair, ... up to N = Royal Flush
  int playerHighCard = 0;// 0 to 12 for Ace ... King
  string playerHandName;

// Local storage for dealers best‐hand values:
  int dealerRankValue = 0;
  int dealerHighCard = 0;
  string dealerHandName;

  playerRanks.reserve(playerCom.size());

   //Evaluate players best 5-card hand (out of playerCombined)
    //    You must fill in this block with your logic to set:
    //      playerRankValue, playerHighCard, playerHandName
    //
    //    Example approach (pseudocode):
    //      Examine all 5 card subsets of playerCombined (size = 7).
    //       For each subset, determine what hand it forms:
    //          (High Card, Pair, Two Pair, 3 of a kind, Straight, Flush, Full House, 
    //           4 of a kind, Straight Flush, Royal Flush, ... etc.)
    //      Compare by (rank category) first, then by (highest card within category).
    //      Keep the highest ranking subset. At the end:
    //          playerRankValue = category of that subset (e.g. Pair=1, TwoPair=2...);
    //          playerHighCard   = numeric index of the subsets top card (0...12);
    //          playerHandName   = textual name (e.g. Two Pair or Flush).

 // Combine dealer hole cards and community cards into a single list
  for (auto& card : playerCom)
  {
    int Rank = card.getRankIndex();
    playerRanks.push_back(Rank);
  }
    // Sort descending:
    sort(playerRanks.begin(), playerRanks.end(), greater<int>());
    playerHighCard = playerRanks.front();
    playerRankValue = 0;                // 0 = High Card
    playerHandName = "High Card"; //


  dealerRanks.reserve(dealerCom.size());
  for (auto& card : dealerCom)
  {
    int Rank = card.getRankIndex();
    dealerRanks.push_back(Rank);
  }
    // Sort descending:
    sort(dealerRanks.begin(), dealerRanks.end(), greater<int>());
    dealerHighCard = dealerRanks.front();
    dealerRankValue = 0;                // 0 = High Card
    dealerHandName = "High Card";

  // Compare playerRankValue vs. dealerRankValue:
  if (playerRankValue > dealerRankValue)
  {
    winner = 1;  // player
    rankValue = playerRankValue;
    handName  = playerHandName;
    // highCard (member) should store the actual DeckOfCards<U> instance
    // that corresponds to playerHighCard. You might need a method to
    // convert a rank index back into a single card DeckOfCards<U>.
    highCard = DeckOfCards<U>::setRankIndex(playerHighCard);
  }
  else if (playerRankValue < dealerRankValue)
  {
    winner = 2;  // dealer
    rankValue = dealerRankValue;
    handName  = dealerHandName;
    // highCard (member) should store the actual DeckOfCards<U> instance
    // that corresponds to playerHighCard. You might need a method to
    // convert a rank index back into a single card DeckOfCards<U>.
    highCard = DeckOfCards<U>::setRankIndex(dealerHighCard);
  }
  else
  {
    // tiebreak by high card
     if (playerHighCard > dealerHighCard)
     {
       winner = 1;  // player
       rankValue = playerRankValue;
       handName  = playerHandName;
       // highCard (member) should store the actual DeckOfCards<U> instance
       // that corresponds to playerHighCard. You might need a method to
       // convert a rank index back into a single card DeckOfCards<U>.
       highCard = DeckOfCards<U>::setRankIndex(playerHighCard);
     }
     else if (playerHighCard > dealerHighCard)
     {
        winner = 2;  // dealer
        rankValue = dealerRankValue;
        handName  = dealerHandName;
        // highCard (member) should store the actual deckOfCards<U> instance
        // convert a rank index back into a single card DeckOfCards<U>.
        highCard = DeckOfCards<U>::setRankIndex(dealerHighCard);
     }
     else
     {
        winner = 0;
        rankValue = playerRankValue;
        handName  = playerHandName;
        // highCard (member) should store the actual DeckOfCards<U> instance
        // that corresponds to playerHighCard. You might need a method to
        // convert a rank index back into a single card DeckOfCards<U>.
        highCard = DeckOfCards<U>::setRankIndex(playerHighCard);
     }
  }
}

//////////////////////////////////////////////////////
// Decide winner, announce, and adjust credit
template <typename U>
void Game<U>::determineWinner()
{
  vector<DeckOfCards<U>> playerAll = player.getHand();        // hole cards
  playerAll.insert(playerAll.end(), commonCards.begin(), commonCards.end());

  vector<DeckOfCards<U>> dealerAll = dealer.getHand();
  dealerAll.insert(dealerAll.end(), commonCards.begin(), commonCards.end());

    // 2) Evaluate both hands and set internal members
  check(playerAll, dealerAll);
  if(winner = 1)
  {
    cout << "You win with a "<<handName << " !" << endl;
    setCredit(credit + bet * 2);
    player.setWin();
  }
  else if(winner = 2)
  {
    cout << "You lose!, Dealer wins with a"<<handName<< "!" << endl;
    dealer.setWin();
  }
  else
  {
    cout << "Push-Tie! You both have: " << handName << "!"<< endl;
    setCredit(credit + bet);
    player.setTie();
    dealer.setTie();
  }
// addWins();
}

//////////////////////////////////////////////////////
// Map hand name to a numeric value
template <typename U>
void Game<U>::setRankValue(const string& name)
{
  handName = name;
  // Example mapping (rework it)
  if (name == "High Card")
  {
    rankValue = 0;
  }
  else if (name == "Pair")
  {
    rankValue = 1;
  }
  else if (name == "Two Pair")
  {
    rankValue = 2;
  }
  else if (name == "Three of a Kind")
  {
    rankValue = 3;
  }
  else if (name == "Straight")
  {
    rankValue = 4;
  }
  else if (name == "Flush")
  {
    rankValue = 5;
  }
  else if (name == "Full House")
  {
    rankValue = 6;
  }
  else if (name == "Four of a Kind")
  {
    rankValue = 7;
  }
  else if (name == "Straight Flush")
  {
    rankValue = 8;
  }
  else if (name == "Royal Flush")
  {
    rankValue = 9;
  }
  else
  {
    rankValue = -1;
  }
}

//////////////////////////////////////////////////////
// Get numeric hand value
template <typename U>
int Game<U>::getRankValue() const
{
  return rankValue;
}

//////////////////////////////////////////////////////
// Record hand name
template <typename U>
void Game<U>::setHandName(const string& handName)
{
this->handName = handName;
}

//////////////////////////////////////////////////////
// Get hand-name index (should be the same as rankValue)
template <typename U>
string Game<U>::getHandName() const
{
  return rankValue;
}

//////////////////////////////////////////////////////
// Set high-card based on a list of cards
template <typename U>
void Game<U>::setHighCard(const vector<DeckOfCards<U>>& cards)
{
 // TODO: find highest rank in cards and store in highCard
  int maxRank;
  if(cards.empty())
  {
   return;
  }

  //continue this
  maxRank = cards[0].getRankIndex();
  DeckOfCards<U> bestCards = cards[0];
  for(const auto& card : cards)
  {
    int rank = card.getRankIndex();
    if(rank > maxRank)
    {
      maxRank = rank;
      bestCards = card;
    }
  }
  highCard = bestCards;

}

//////////////////////////////////////////////////////
// Return high-card rankIndex
template <typename U>
string Game<U>::getHighCard() const
{
    // TODO: return a descriptive string, e.g. face + suit
    // this should get the face & suit from the functions getRankIndex() 
    // and getSuitIndex() from the class DeckOfCards
    int rank = highCard.getRankIndex(); // same as face
    int suit = highCard.getSuitIndex();
    const char *suitNames[4] = deck.suit;
    const char *faceNames[13] = deck.face;

//    return handName;
    return string(faceNames[rank]) + " of " + suitNames[suit];
}

//////////////////////////////////////////////////////
// Validate integer input
template <typename U>
int Game<U>::valueInput(int& input)
{
  while((input < 1) || (input > 300) || cin.fail())
  {
    cin.clear();//clears the last input to avoid it repeating
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');//ignores anything that isnt a number
    cout << "Input needs to be an integer, Try again" << endl;
    cin >> input;
  }
 return input;
}

//////////////////////////////////////////////////////
// Output operator
template<typename TYPE>
std::ostream &operator<< (std::ostream &output,const Game<TYPE> &Game_Output)
{
   output << "Player: " << Game_Output.player.getName() << "\nBet: " << Game_Output.getBet()
   << "\nCredit: " << Game_Output.getCredit() << "\nLast Hand: " << Game_Output.getHandName()
   << "\nRank: " << Game_Output.getRankValue() << endl;

 return output;
}
