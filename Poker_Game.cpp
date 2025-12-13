#include "Poker_Game.h"

// Constructor: set up deck, shuffle, player and dealer, initial bets/credits
template <typename U>
//Game<U>::Game(const string& playerName, int& bet, int& credit)
Game<U>::Game()
: bet(0), credit(0), winner(-1), newCredit(0), rankValue(0),
   handName(""), highCard(), deck(), player(), dealer(), roundNum(0)
//player(string("")), dealer(string("Dealer")),
{
  deck.Shuffling();
//  setBet(bet);
  //setCredit(credit);
}

//////////////////////////////////////////////////////
// Copy constructor
template <typename U>
Game<U>::Game(const Game& copy)
 : bet(copy.bet), credit(copy.credit), winner(copy.winner), newCredit(copy.newCredit),
    rankValue(copy.rankValue), handName(copy.handName), highCard(copy.highCard),
    deck(copy.deck), player(copy.player), dealer(copy.dealer), commonCards(copy.commonCards)
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
  //Helper lambda to convert a card ID (1..52) into a DeckOfCards<U> "card"
  auto makeCardFromId = [](unsigned int id){
    DeckOfCards<U> card;
    int row = static_cast<int>((id - 1) / 13);
    int col = static_cast<int>((id - 1) % 13);

    card.setSuitIndex(row);
    card.setRankIndex(col);
   return card;
  };

  // Deal two whole cards to player & dealer
  for(int i = 0; i < 2; i++)
  {
    //TODO: Apperatnly its passing an unsigned int, rework this or the
    // class where it comes from.
    //auto playerCard = deck.drawOne();
    unsigned int playerId = deck.drawOne();
    DeckOfCards<U> playerCard = makeCardFromId(playerId);
    player.addCard(playerCard);

    //auto dealerCard = deck.drawOne();
    unsigned int dealerId = deck.drawOne();
    DeckOfCards<U> dealerCard = makeCardFromId(dealerId);
    dealer.addCard(dealerCard);
/*
    DeckOfCards<U> playerCard = deck.Dealing();
    player.addCard(playerCard);

    DeckOfCards<U> dealerCard = deck.Dealing();
    dealer.addCard(dealerCard);
*/
  }

  //Deals community cards(flop/turn/river)
  for(int i = 0; i < numCards; i++)
  {
    //auto card = deck.drawOne();
    unsigned int id = deck.drawOne();
    DeckOfCards<U> communityCard = makeCardFromId(id);
    commonCards.push_back(communityCard);
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
  if(winner == 1)
  {
    player.setWin();
  }
  else if(winner == 2)
  {
    dealer.setWin();
  }
}

//////////////////////////////////////////////////////
// Evaluate hands and set winner, rankValue, handName, highCard
template <typename U>
void Game<U>::check(const vector<DeckOfCards<U>>& playerCom, const vector<DeckOfCards<U>>& dealerCom)
{
  // Example: winner = 1 for player, 2 for dealer, 0 for tie
  // Collect all rank indices:
  vector<int> playerRanks;
  vector<int> dealerRanks;
  // Local storage for players best hand values
  int playerRankValue = 0;// 0 = High Card, 1 = Pair, ... up to N = Royal Flush
  int playerHighCard = 0;// 0 to 12 for Ace ... King
  std::string playerHandName;

  // Local storage for dealers best‐hand values:
  int dealerRankValue = 0;
  int dealerHighCard = 0;
  std::string dealerHandName;

  //playerRanks.reserve(playerCom.size());
  evaluteHand(playerCom, playerRankValue, playerHighCard, playerHandName);
  evaluteHand(dealerCom, dealerRankValue, dealerHighCard, dealerHandName);

   // Evaluate players best 5-card hand (out of playerCombined)
   // You must fill in this block with your logic to set:
   // playerRankValue, playerHighCard, playerHandName
   //
   // Example approach (pseudocode):
   // Examine all 5 card subsets of playerCombined (size = 7).
   // For each subset, determine what hand it forms:
   // (High Card, Pair, Two Pair, 3 of a kind, Straight, Flush, Full House,
   // 4 of a kind, Straight Flush, Royal Flush, ... etc.)
   // Compare by (rank category) first, then by (highest card within category).
   // Keep the highest ranking subset. At the end:
   // playerRankValue = category of that subset (e.g. Pair=1, TwoPair=2...);
   // playerHighCard   = numeric index of the subsets top card (0...12);
   // playerHandName   = textual name (e.g. Two Pair or Flush).

/*
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
*/
  // Compare playerRankValue vs. dealerRankValue:
  if (playerRankValue > dealerRankValue)
  {  // Player has better hand category
    winner = 1;  // player
    rankValue = playerRankValue;
    handName  = playerHandName;
    //highCard = DeckOfCards<U>::setRankIndex(playerHighCard);
    //highCard.setRankIndex(playerHighCard);
    setHighCard(playerCom); // Choose the best high card from player's combined cards
  }
  else if (playerRankValue < dealerRankValue)
  { // Dealer has better hand category
    winner = 2;  // dealer
    rankValue = dealerRankValue;
    handName  = dealerHandName;
//    highCard = DeckOfCards<U>::setRankIndex(dealerHighCard);
    //highCard.setRankIndex(dealerHighCard);
    setHighCard(dealerCom);// Choose the best high card from dealer's combined cards
  }
  else
  {
    // Same hand event: tiebreak by high card
     if (playerHighCard > dealerHighCard)
     {
       winner = 1;  // player
       rankValue = playerRankValue;
       handName  = playerHandName;
//       highCard = DeckOfCards<U>::setRankIndex(playerHighCard);
       //highCard.setRankIndex(playerHighCard);
       setHighCard(playerCom);
     }
     else if (playerHighCard < dealerHighCard)
     {
        winner = 2;  // dealer
        rankValue = dealerRankValue;
        handName  = dealerHandName;
//        highCard = DeckOfCards<U>::setRankIndex(dealerHighCard);
	//highCard.setRankIndex(dealerHighCard);
        setHighCard(dealerCom);
     }
     else
     {  // Excat tie: same cat. and same high card
        winner = 0;
        rankValue = playerRankValue;
        handName  = playerHandName;
//        highCard = DeckOfCards<U>::setRankIndex(playerHighCard);
	//highCard.setRankIndex(playerHighCard);
        setHighCard(playerCom);// TODO: THIS IS TEMPORARY, NEED TO DO SET THIS A TIE & NOONE WINS
     }
  }
}

//////////////////////////////////////////////////////
// Evaluate a 7-card hand for High Card / Pair / Two Pair
// outRankValue: 0 = High Card, 1 = Pair, 2 = Two Pair
// outHighCard:  rank index of the main combination (0..12)
template <typename U> // Helper Function
void Game<U>::evaluteHand(const vector<DeckOfCards<U>>& cards, int& outRankValue, int& outHighCard,string& outHandName)
{
//  int rankCount[13] = {0};
//  int suitCount[4] = {0};
  vector<int> rankBySuit[4]; //Store ranks by suit for flush / straight flush
  int highestRank = -1;

  // Does basic count
  computeCounts(cards, rankBySuit, highestRank);

  //int numPairs = 0;
  //int pairRanks[13];
  // multiples (pairs / trips / quads)
  int fourKindRank = -1;
  vector<int> tripRanks;
  vector<int> pairRanks;
  detectMultiples(fourKindRank, tripRanks, pairRanks);

 // int threeKindRank = -1;
  //straight / flush / straight flush
  bool hasStraight;
  int  highStraightRank;
  detectStraight(hasStraight, highStraightRank);

//  int highestPairRank = -1;
//  int secondPairRank = -1;

  // Does StraightFlush & Flush
  bool hasFlush;
  int  flushSuit;
  int  flushHighRank;
  bool hasStraightFlush;
  int  highStraightFlushRank;
  detectFlushAndStraightFlush(rankBySuit, hasFlush, flushSuit,
    flushHighRank, hasStraightFlush, highStraightFlushRank);

  // Deos full house determining uses trips + pairs
  bool hasFullHouse = false;
  int fullHouseTripRank = -1;
  if (!tripRanks.empty())
  {
    if (tripRanks.size() >= 2)
    {
      hasFullHouse       = true;
      fullHouseTripRank  = tripRanks.back();   // highest trip
    }
    else if (!pairRanks.empty())
    {
       hasFullHouse       = true;
       fullHouseTripRank  = tripRanks.back();
     }
  }
   // Deciding hand category
   if(hasStraightFlush)
   {
       outRankValue = 8;
       outHighCard  = highStraightFlushRank;
       outHandName  = (highStraightFlushRank == 13) ? "Royal Flush" : "Straight Flush";
   }
   else if (fourKindRank != -1)
   {
       // Four of a Kind
       outRankValue = 7;
       outHighCard  = fourKindRank;
       outHandName  = "Four of a Kind";
   }
   else if (hasFullHouse)
   {
       // Full House: three of one rank + at least one pair
       outRankValue = 6;
       outHighCard  = fullHouseTripRank;
       outHandName  = "Full House";
   }
   else if (hasFlush)
   {
       outRankValue = 5;
       outHighCard  = flushHighRank;
       outHandName  = "Flush";
   }
   else if (hasStraight)
   {
       outRankValue = 4;
       outHighCard  = highStraightRank;   // compare by the rank of the trips
       outHandName  = "Straight";
   }
   else if (!tripRanks.empty())
   {
        // Three of a Kind only
       outRankValue = 3;
       outHighCard  = tripRanks.back;
       outHandName  = "Three of a Kind";
   }
   else if (pairRanks.size() >= 2)
   {/*
      // Two Pair: use highest pair rank as the "high card" of the combo
       int highestPair = pairRanks[0];
       for (int i = 1; i < numPairs; ++i)
       {
           if (pairRanks[i] > highestPair)
           {
               highestPair = pairRanks[i];
           }
       }*/
      outRankValue = 2;             // Two Pair
      outHighCard  = pairRanks.back(); // highest pair
      outHandName  = "Two Pair";
   }
   else if (pairRanks.size() == 1)
   {
       outRankValue = 1;             // One Pair
       outHighCard  = pairRanks[0];
       outHandName  = "Pair";
   }
   else
   {
       outRankValue = 0;             // High Card
       outHighCard  = highestRank;
       outHandName  = "High Card";
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
  if(winner == 1)
  {
    cout << "You win with a "<<handName << " !" << endl;
    setCredit(credit + bet * 2);
    player.setWin();
  }
  else if(winner == 2)
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
template <typename U>
int Game<U>::getWinner() const
{
  return winner;
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
  //return rankValue;
  return handName;
}

//////////////////////////////////////////////////////
// Set high-card based on a list of cards
template <typename U>
void Game<U>::setHighCard(const vector<DeckOfCards<U>>& cards)
{
  if(cards.empty())
  {
   return;
  }

  int maxRank = cards[0].getRankIndex();
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

    static const char *face[13] =
        { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
          "Eight", "Nine", "Ten", "Jack", "Queen", "King"
        }; // will be used for rank

    static const char *suit[4] = { "Hearts", "Diamonds", "Clubs", "Spades" }; // will be used in suit

    int rankIndex = highCard.getRankIndex(); // 0..12
    int suitIndex = highCard.getSuitIndex(); // 0..3

    // Safety check: highCard might not have been set
    if(rankIndex < 0 || rankIndex > 12 || suitIndex < 0 || suitIndex > 3){
	return "Unknown";
    }

//    return handName;
    return std::string(face[rankIndex]) + " of " + suit[suitIndex];
}

//////////////////////////////////////////////////////
template <typename U>
void Game<U>::incrementRound()
{
  ++roundNum;
}

//////////////////////////////////////////////////////
template <typename U>
int Game<U>::getRoundNumber() const
{
  return roundNum;
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
template <typename U>
void Game<U>::computeCounts(const vector<DeckOfCards<U>>& cards, std::vector<int> rankBySuit[4],
	int& highestRank) // Helper Function
{
  for(int i = 0; i < 13; i++){
    rankCount[i] = 0;
  }

  for(int i = 0; i < 4; i++){
    suitCount[i] = 0;
  }

  highestRank = -1;

  for(const auto& card:cards)
  { // count ranks
    int r = card.getRankIndex();
    int s = card.getSuitIndex();

    if(r >= 0 && r < 13)
    {
        rankCount[r]++;
        if(r > highestRank)
        {
          highestRank = r;
        }
    }

    if(s >= 0 && s < 4)
    {
        suitCount[r]++;
        if(r >=0 && r < 13)
        {
          rankBySuit[s].push_back(r);
        }
    }
  }
}

//////////////////////////////////////////////////////
template <typename U>// Helper Function
void Game<U>::detectMultiples(int& fourKindRank, std::vector<int>& tripRanks, std::vector<int>& pairRanks)
{
  fourKindRank = -1; // reset val
  tripRanks.clear();
  pairRanks.clear();

  for(int rank = 0; rank < 13; ++rank)
  {
    int count = rankCount[rank];
/*    if(count > 0 && r > highestRank)
    {
        highestRank = rank; // track overall high card
    }
*/
    if(count == 4)
    { // Four of a kind
      if(rank > fourKindRank)
      {
        fourKindRank = rank;
      }
    }
    else if(count == 3)
    { // Track highest trip
  /*    if(rank > threeKindRank)
      {
        threeKindRank = rank;
      }*/
       tripRanks.push_back(rank);
    }
    else if(count == 2)
    {//Store all pair ranks
     //pairRanks[numPairs++] = rank;
      pairRanks.push_back(rank);
    }
  }
 std::sort(tripRanks.begin(),tripRanks.end());
 std::sort(pairRanks.begin(),pairRanks.end());
}
//////////////////////////////////////////////////////
template <typename U>// Helper Function
void Game<U>::detectStraight(bool& hasStriaght, int& hasStraightRank)
{
  bool present[14] = {false}; // 0..12 normal, 13 = ace-high
  hasStriaght = false;
  hasStraightRank = -1;

  for(int rank = 0; rank < 13; rank++)
  {
    if(rankCount[rank] > 0)
    {
      present[rank] = true;
      if(rank == 0)
      {
	present[13] = true;
      }
    }
  }

  for(int rank = 13; rank >= 4; rank++)
  {
    if(present[rank]
	&& present[rank - 1]
	&& present[rank - 2]
	&& present[rank - 3]
	&& present[rank - 4])
    {
      hasStriaght = true;
      hasStraightRank = rank;
      break;
    }
  }
}
//////////////////////////////////////////////////////
template <typename U>// Helper Function
void Game<U>::detectFlushAndStraightFlush(std::vector<int>& rankBySuit, bool& hasFlush,
	int& flushSuit, int& flushHighRank, bool& hasStraightFlush, int& hasStraightFlushRank)
{
  hasFlush = false;
  flushSuit = -1;
  flushHighRank = -1;
  hasStraightFlush = false;
  hasStraightFlushRank = -1;

  std::vector<int> flushRanks;

  // Find any flush
  for(int s = 0; s < 4; ++s)
  {
    if(suitCount[s] >= 5)
    {
      hasFlush  = true;
      flushSuit = s;

      flushRanks = rankBySuit[s];
      std::sort(flushRanks.begin(), flushRanks.end(), std::greater<int>());
      if(!flushRanks.empty())
      {
        flushHighRank = flushRanks.front();
        break;
      }
    }
  }

  if(!hasFlush || flushSuit == -1)
  {
    return;
  }

  // Straight Flush detection within flushSuit
  bool presentSF[14] = {false};

  for(int r : rankBySuit[flushSuit])
  {
    if(r >= 0 && r < 13)
    {
      presentSF[r] = true;
      if (r == 0)
      {
         presentSF[13] = true; // Ace-high in that suit
      }
    }
  }

  for (int rank = 13; rank >= 4; --rank)
  {
    if (presentSF[rank]
        && presentSF[rank - 1]
	&& presentSF[rank - 2]
	&& presentSF[rank - 3]
        && presentSF[rank - 4])
        {
          hasStraightFlush     = true;
          hasStraightFlushRank = rank;
          break;
        }
  }
}

//////////////////////////////////////////////////////
template<typename U>
bool Game<U>::SaveToFile(const string& filename) const
{
  fstream inputFile(filename, ios::out | ios::app);
  if(inputFile.is_open())
  {
    cout << "Records of games wins/loss" << endl;
    inputFile << determineWinner() << endl;
    inputFile.close();
    return true;
  }
  else
  {
    cerr << "File couldn't be opened" << endl;
    return false;
  }
}

//////////////////////////////////////////////////////
template<typename U>
void Game<U>::playRound(int& CommCards)
{
  dealCards(CommCards); // Deal Cards (fills player, dealer, and commonCards members)

  // Build combined 7 cardvectors
  std::vector<DeckOfCards<U>> playerCom;
  std::vector<DeckOfCards<U>> dealerCom;

  const auto& playerHand = player.getHand();
  const auto& dealerHand = dealer.getHand();

  // Player: hole + common
  playerCom.insert(playerCom.end(), playerHand.begin(), playerHand.end());
  playerCom.insert(playerCom.end(), commonCards.begin(), commonCards.end());
  // Dealer: hole + common
  dealerCom.insert(dealerCom.end(), dealerHand.begin(), dealerHand.end());
  dealerCom.insert(dealerCom.end(), commonCards.begin(), commonCards.end());

  // Evaluate and determine winner
  check(playerCom, dealerCom);
  determineWinner();// uses winner, rankValue, handName, highCard

  if(winner == 1)
  {
    credit+=bet; // TODO: Will add some payout rule
  }
  else if(winner == 2){
    credit-=bet;
  }
// winner == 0 -> tie, no credit change
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
