#include "Poker_Game.h"

// Constructor: set up deck, shuffle, player and dealer, initial bets/credits
// Initializes all scalar members to their starting values (no bet, no
// credit yet, no winner decided, round 0), default-constructs the player,
// dealer, deck and high-card holder, then immediately shuffles the deck
// so it's ready for the first round.
template <typename U>
Game<U>::Game()
: bet(0), credit(0), winner(-1), newCredit(0), rankValue(0),
   handName(""), highCard(), deck(), player(), dealer(), roundNum(0)
{
  deck.Shuffling();
}

//////////////////////////////////////////////////////
// Copy constructor
// Copies every relevant piece of game state (bet, credit, winner, rank
// value, hand name, high card, deck, player, dealer, and community cards)
// from another Game object into this new one.
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
// No dynamically allocated resources to release, so this is a no-op.
template <typename U>
Game<U>::~Game()
{
 // empty
}

//////////////////////////////////////////////////////
// Set bet with validation
// Stores the given amount as the current bet. Negative values are
// treated as invalid, print a warning, and get clamped to 0.
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
// Returns the amount currently wagered for this round.
template <typename U>
int Game<U>::getBet() const
{
  return bet;
}

//////////////////////////////////////////////////////
// Prompt user to place bet
// Repeatedly asks the user for a bet amount (via valueInput for basic
// numeric validation) until it's non-negative and doesn't exceed the
// current credit total. Once valid, stores the bet and immediately
// deducts it from credit via computeCredit().
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
// Convenience wrapper that clears the current bet back to 0.
template <typename U>
void Game<U>::resetBet()
{
  setBet(0);
}

//////////////////////////////////////////////////////
// Set credit with validation
// Stores the given amount as the player's credit total. Negative values
// are treated as invalid, print a warning, and get clamped to 0.
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
// Returns the player's current credit balance.
template <typename U>
int Game<U>::getCredit() const
{
  return credit;
}

//////////////////////////////////////////////////////
// Adjust credit after bet
// Subtracts the current bet from the current credit and stores the
// result back into credit (via setCredit, so it stays clamped at 0).
template <typename U>
void Game<U>::computeCredit()
{
  newCredit = credit - bet;
  setCredit(newCredit);
}

//////////////////////////////////////////////////////
// Deal hole cards and community cards
// Clears out any cards left over from a previous round, then deals two
// hole cards each to the player and dealer (alternating draws), followed
// by "numCards" community cards drawn from the shared deck and stored in
// commonCards.
template <typename U>
void Game<U>::dealCards(int& numCards)
{
  player.clearHand();
  dealer.clearHand();
  commonCards.clear();

  // Deal two whole cards to player & dealer
  for(int i = 0; i < 2; i++)
  {
    player.addCard(makeCardFromId(deck.drawOne()));
    dealer.addCard(makeCardFromId(deck.drawOne()));
  }

  //Deals community cards(flop/turn/river)
  for(int i = 0; i < numCards; i++)
  {
    unsigned int id = deck.drawOne();
    DeckOfCards<U> communityCard = makeCardFromId(id);
    commonCards.push_back(communityCard);
  }

}

//////////////////////////////////////////////////////
// Prints up to the first 3 cards currently stored in commonCards (the
// "flop"), guarding against printing past the end of the vector.
template <typename U>
void Game<U>::display3Cards()
{
  cout << "Flop: " << endl;
  for(int i = 0; i < 3 && i < (int)commonCards.size(); i++)
  {
    cout << commonCards[i].toString() << "\n";
  }
}

//////////////////////////////////////////////////////
// Prints community cards at index 3 and 4 (the "turn" and "river"),
// guarding against printing past the end of the vector.
template <typename U>
void Game<U>::display2Cards()
{
  cout << "Turn & River: " << endl;
  for(int i = 3; i < 5 && i < (int)commonCards.size(); i++)
  {
    cout << commonCards[i].toString() << "\n";
  }
}

//////////////////////////////////////////////////////
// Convenience wrapper that prints the player's hand followed by the
// dealer's hand.
template <typename U>
void Game<U>::displayHands()
{
  player.displayHand();
  dealer.displayHand();
}

//////////////////////////////////////////////////////
//Prints just the dealer's current hand.
template <typename U>
void Game<U>::dealerHands()
{
  dealer.displayHand();
}

//////////////////////////////////////////////////////
// Prints just the player's current hand.
template <typename U>
void Game<U>::playerHands()
{
  player.displayHand();
}
//////////////////////////////////////////////////////
// Increment win count for winner
// Looks at the stored "winner" code and adds a win to whichever side
// (player or dealer) actually won; does nothing for a tie (winner == 0).
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
// Given the player's and dealer's fully-combined hands (hole + community
// cards), evaluates each one to find its best 5-card poker hand, then
// compares the two results to decide the winner:
//   winner = 1 -> player wins, 2 -> dealer wins, 0 -> tie
// If both hands are the same category (e.g. both have "Two Pair"), the
// higher of the two "high card" ranks breaks the tie; if that's equal
// too, it's recorded as an exact tie.
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

  // Work out each side's best possible hand category/high-card/name
  // out of their combined 7 cards (2 hole + up to 5 community).
  evaluteHand(playerCom, playerRankValue, playerHighCard, playerHandName);
  evaluteHand(dealerCom, dealerRankValue, dealerHighCard, dealerHandName);

  // remember each side's result so the UI can show both
  playerResult = playerHandName;
  dealerResult = dealerHandName;
  playerBestRank = playerHighCard;
  dealerBestRank = dealerHighCard;

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
   // NOTE: the block above describes the "ideal" full 7-choose-5 approach;
   // the simpler rank/suit-counting approach actually implemented in
   // evaluteHand() below approximates this without enumerating every subset.

  // Compare playerRankValue vs. dealerRankValue:
  if (playerRankValue > dealerRankValue)
  {  // Player has better hand category
    winner = 1;  // player
    rankValue = playerRankValue;
    handName  = playerHandName;
    setHighCard(playerCom); // Choose the best high card from player's combined cards
  }
  else if (playerRankValue < dealerRankValue)
  { // Dealer has better hand category
    winner = 2;  // dealer
    rankValue = dealerRankValue;
    handName  = dealerHandName;
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
       setHighCard(playerCom);
     }
     else if (playerHighCard < dealerHighCard)
     {
        winner = 2;  // dealer
        rankValue = dealerRankValue;
        handName  = dealerHandName;
        setHighCard(dealerCom);
     }
     else
     {  // Excat tie: same cat. and same high card
        winner = 0;
        rankValue = playerRankValue;
        handName  = playerHandName;
        setHighCard(playerCom);// TODO: THIS IS TEMPORARY, NEED TO DO SET THIS A TIE & NOONE WINS
     }
  }
}

//////////////////////////////////////////////////////
// Evaluate a 7-card hand for High Card / Pair / Two Pair
// This is the core hand-evaluation function. It delegates the grunt work
// to several helpers:
//   computeCounts()                -> tallies how many of each rank/suit are present
//   detectMultiples()              -> finds any four-of-a-kind / trips / pairs
//   detectStraight()               -> checks for 5 consecutive ranks
//   detectFlushAndStraightFlush()  -> checks for a flush, and a straight within it
// It then picks the single best category (checked from strongest to
// weakest: Straight/Royal Flush, Four of a Kind, Full House, Flush,
// Straight, Three of a Kind, Two Pair, Pair, High Card) and fills in:
//   outRankValue: 0 (High Card) .. 8 (Straight/Royal Flush)
//   outHighCard:  the rank index that makes this category (e.g. the pair's rank)
//   outHandName:  a human-readable name for the category
template <typename U> // Helper Function
void Game<U>::evaluteHand(const vector<DeckOfCards<U>>& cards, int& outRankValue, int& outHighCard,string& outHandName)
{
  vector<int> rankBySuit[4]; //Store ranks by suit for flush / straight flush
  int highestRank = -1;

  // Does basic count
  computeCounts(cards, rankBySuit, highestRank);

  // multiples (pairs / trips / quads)
  int fourKindRank = -1;
  vector<int> tripRanks;
  vector<int> pairRanks;
  detectMultiples(fourKindRank, tripRanks, pairRanks);

  //straight / flush / straight flush
  bool hasStraight;
  int  highStraightRank;
  detectStraight(hasStraight, highStraightRank);

  // Does StraightFlush & Flush
  bool hasFlush;
  int  flushSuit;
  int  flushHighRank;
  bool hasStraightFlush;
  int  highStraightFlushRank;
  detectFlushAndStraightFlush(rankBySuit, hasFlush, flushSuit,
    flushHighRank, hasStraightFlush, highStraightFlushRank);

  // Deos full house determining uses trips + pairs
  // A full house needs three-of-a-kind plus at least one pair. If there
  // are two separate trip ranks, the second trip can double as the "pair"
  // half of the full house.
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
   // Checked from strongest to weakest so the first match found is the
   // best possible hand for this set of cards.
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
       outHighCard  = tripRanks.back();
       outHandName  = "Three of a Kind";
   }
   else if (pairRanks.size() >= 2)
   {
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
// Combines each side's hole cards with the shared community cards,
// evaluates them via check(), then pays out based on the result:
//   player wins -> credit gains back double the bet (bet returned + winnings)
//   dealer wins -> credit already reflects the lost bet from startNewRound(), nothing added
//   tie         -> the bet is simply returned
// Also records the net credit change and prints the full showdown.
template <typename U>
void Game<U>::determineWinner()
{
  vector<DeckOfCards<U>> playerAll = player.getHand();        // hole cards
  playerAll.insert(playerAll.end(), commonCards.begin(), commonCards.end());

  vector<DeckOfCards<U>> dealerAll = dealer.getHand();
  dealerAll.insert(dealerAll.end(), commonCards.begin(), commonCards.end());
  // 2) Evaluate both hands and set internal members
  check(playerAll, dealerAll);
  roundActive = false;
  folded = false;

  int before = credit;
  if(winner == 1)
  {
    setCredit(credit + bet * 2);
    player.setWin();
    dealer.setLoss();
  }
  else if(winner == 2)
  {
    dealer.setWin();
    player.setLoss();
  }
  else
  {
    setCredit(credit + bet);
    player.setTie();
    dealer.setTie();
  }
  lastCreditChange = credit - before - bet; // net vs. start of round

  printShowdown();
}
//////////////////////////////////////////////////////
// Prints the board, both hands, and a clear winner banner
// Uses a small local lambda (listCards) to join a vector of cards into a
// single comma-separated string, then prints: the round number, the
// community board, both players' hole cards plus their evaluated hand
// name/high card, and finally a banner announcing the winner (or a push)
// along with the resulting credit change and new credit total.
template <typename U>
void Game<U>::printShowdown() const
{
  auto listCards = [](const vector<DeckOfCards<U>>& cards){
    string out;
    for(size_t i = 0; i < cards.size(); ++i)
    {
      if(i) out += ", ";
      out += cards[i].toString();
    }
    return out;
  };

  cout << "\n========== SHOWDOWN (Round " << roundNum << ") ==========" << endl;
  cout << "Board:    " << listCards(commonCards) << endl;
  cout << "----------------------------------------------" << endl;
  cout << "YOU:      " << listCards(player.getHand()) << endl;
  cout << "          -> " << playerResult << " (high: " << rankName(playerBestRank) << ")" << endl;
  cout << "DEALER:   " << listCards(dealer.getHand()) << endl;
  cout << "          -> " << dealerResult << " (high: " << rankName(dealerBestRank) << ")" << endl;
  cout << "----------------------------------------------" << endl;

  if(winner == 1)
  {
    cout << ">>> YOU WIN with " << playerResult << "!  (+" << lastCreditChange << " credits)" << endl;
  }
  else if(winner == 2)
  {
    cout << ">>> DEALER WINS with " << dealerResult << ".  (" << lastCreditChange << " credits)" << endl;
  }
  else
  {
    cout << ">>> PUSH - both have " << playerResult << ". Bet returned." << endl;
  }
  cout << "Credits: " << credit << endl;
  cout << "==============================================" << endl;
}

//////////////////////////////////////////////////////
// Converts a rank index (0..12, or 13 = ace-high) to its name
// Used for display purposes (e.g. "high: Queen"). Rank 13 is treated as
// a special "Ace playing high" case (as in a Ten-to-Ace straight) and
// still returns "Ace"; any value outside 0..13 returns "?".
template <typename U>
string Game<U>::rankName(int r)
{
  static const char* face[13] =
    { "Ace", "Two", "Three", "Four", "Five", "Six", "Seven",
      "Eight", "Nine", "Ten", "Jack", "Queen", "King" };

  if(r == 13){
    return "Ace";
  }

  if(r < 0 || r > 12)
  {
    return "?";
  }
  return face[r];
}

//////////////////////////////////////////////////////
// Result summary of the last finished round (used by the Play menu)
// Prints nothing useful ("No finished round yet.") if no round has ever
// completed, or if a round is still actively in progress and wasn't
// folded. Otherwise, if the player folded, prints a short fold summary
// with both hands revealed; if the round went to a full showdown,
// delegates to printShowdown() for the detailed breakdown. Always ends
// by printing the current credit total.
template <typename U>
void Game<U>::printLastResult() const
{
  if(roundNum == 0 || (roundActive && !folded))
  {
    cout << "No finished round yet." << endl;
    return;
  }

  if(folded)
  {
    cout << "You folded round " << roundNum << " and lost your bet of " << bet << "." << endl;
    cout << "Your hand was: ";
    for(const auto& c : player.getHand()) cout << c.toString() << "  ";
    cout << "\nDealer had:    ";
    for(const auto& c : dealer.getHand()) cout << c.toString() << "  ";
    cout << endl;
  }
  else
  {
    printShowdown();
  }
  cout << "Current credits: " << credit << endl;
}

//////////////////////////////////////////////////////
// Returns who won the last evaluated round: 1 = player, 2 = dealer,
template <typename U>
int Game<U>::getWinner() const
{
  return winner;
}

//////////////////////////////////////////////////////
// Map hand name to a numeric value
// Stores the given hand-name string in handName, then looks it up
// against a fixed table of poker hand names to determine its numeric
// strength (0 = High Card up to 9 = Royal Flush). Unrecognized names
// result in rankValue being set to -1.
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
// Returns the numeric strength of the most recently determined hand.
template <typename U>
int Game<U>::getRankValue() const
{
  return rankValue;
}

//////////////////////////////////////////////////////
// Record hand name
// Directly stores a hand-name string without touching rankValue.
template <typename U>
void Game<U>::setHandName(const string& handName)
{
  this->handName = handName;
}

//////////////////////////////////////////////////////
// Get hand-name index (should be the same as rankValue)
// Returns the stored hand-name string (e.g. "Flush").
template <typename U>
string Game<U>::getHandName() const
{
  return handName;
}

//////////////////////////////////////////////////////
// Set high-card based on a list of cards
// Scans the given list of cards and remembers whichever one has the
// highest rank index, storing a copy of that card in the "highCard"
// member for later display via getHighCard().
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
// Builds and returns a "Rank of Suit" string (e.g. "King of Spades") for
// the card previously stored via setHighCard(). Returns "Unknown" if the
// high card was never set to valid rank/suit indices.
template <typename U>
string Game<U>::getHighCard() const
{
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

    return std::string(face[rankIndex]) + " of " + suit[suitIndex];
}

//////////////////////////////////////////////////////
// Advances the round counter by 1 (called at the start of a new round).
template <typename U>
void Game<U>::incrementRound()
{
  ++roundNum;
}

//////////////////////////////////////////////////////
// Returns how many rounds have been played (including the current one).
template <typename U>
int Game<U>::getRoundNumber() const
{
  return roundNum;
}

//////////////////////////////////////////////////////
// Validate integer input
// Generic numeric-input validator: re-prompts until "input" is a real
// integer between 1 and 300 inclusive, clearing any failed cin state
// and discarding leftover characters along the way.
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
// Tallies how many times each rank (0..12) and each suit (0..3) appear
// among the given cards, resetting rankCount[]/suitCount[] first. Also
// fills rankBySuit[suit] with the list of ranks seen for that suit (used
// later to check for flushes/straight flushes), and reports the single
// highest rank seen across all cards via "highestRank".
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

  for(int i = 0; i < 4; ++i){
    rankBySuit[i].clear(); // clears suit array
  }

  highestRank = -1;

  for(const auto& card:cards)
  { // count ranks
    int r = card.getRankIndex(); // 0..12
    int s = card.getSuitIndex(); // 0..3

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
        suitCount[s]++;
        if(r >=0 && r < 13)
        {
          rankBySuit[s].push_back(r);
        }
    }
  }
}

//////////////////////////////////////////////////////
// Scans the rank counts (previously filled by computeCounts) to find:
//   - a four-of-a-kind rank (if any; -1 if none)
//   - every rank that appears exactly three times (trips), sorted ascending
//   - every rank that appears exactly twice (pairs), sorted ascending
// These lists feed into full-house/three-of-a-kind/two-pair/pair checks.
template <typename U>// Helper Function
void Game<U>::detectMultiples(int& fourKindRank, std::vector<int>& tripRanks, std::vector<int>& pairRanks)
{
  fourKindRank = -1; // reset val
  tripRanks.clear();
  pairRanks.clear();

  for(int rank = 0; rank < 13; ++rank)
  {
    int count = rankCount[rank];

    if(count == 4)
    { // Four of a kind
      if(rank > fourKindRank)
      {
        fourKindRank = rank;
      }
    }
    else if(count == 3)
    { // Track highest trip
       tripRanks.push_back(rank);
    }
    else if(count == 2)
    {//Store all pair ranks
      pairRanks.push_back(rank);
    }
  }
 std::sort(tripRanks.begin(),tripRanks.end());
 std::sort(pairRanks.begin(),pairRanks.end());
}
//////////////////////////////////////////////////////
// Checks the rank counts for 5 consecutive ranks (a straight), including
// the special Ace-high case (Ten-Jack-Queen-King-Ace, represented as rank
// 13). Builds a "present[]" table marking which ranks appear at least
// once, then scans downward from rank 13 to 4 looking for 5 ranks in a
// row that are all present. Sets hasStriaght/hasStraightRank accordingly
// (hasStraightRank is the top rank of the straight found, or -1 if none).
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

  for(int rank = 13; rank >= 4; --rank)
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
// Checks for a flush (5+ cards of the same suit) using suitCount[], and
// if one exists, further checks whether the ranks within that flush suit
// form a straight (a straight flush). Outputs:
//   hasFlush / flushSuit / flushHighRank   -> whether there's a flush, which suit, and its highest rank
//   hasStraightFlush / hasStraightFlushRank -> whether that flush is also a straight, and its top rank
// (hasStraightFlushRank == 13 signals a Royal Flush, i.e. Ace-high straight flush.)
template <typename U>// Helper Function
void Game<U>::detectFlushAndStraightFlush(std::vector<int> rankBySuit[4], bool& hasFlush,
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
// Draws one card from the deck and discards it into the burn pile
// without revealing it, matching standard poker convention of "burning"
// a card before dealing each new community stage.
template<typename U>
void Game<U>::burnOne()
{
  unsigned int id = deck.drawOne();
  burnPile.push_back(makeCardFromId(id));
}

//////////////////////////////////////////////////////
// Converts a raw card ID (1..52, as produced by DeckOfCards::drawOne())
// into a fully-formed DeckOfCards<U> "card" object by working out its
// suit (row) and rank (column) and storing them on a new card instance.
template<typename U>
DeckOfCards<U> Game<U>::makeCardFromId(unsigned int id)
{
  DeckOfCards<U> card;
  int row = static_cast<int>((id - 1) / 13);
  int col = static_cast<int>((id - 1) % 13);

  card.setSuitIndex(row);
  card.setRankIndex(col);
 return card;
}
//////////////////////////////////////////////////////
// Deals the flop: only runs if we're still pre-flop (street == 0).
// Burns one card, then draws and adds 3 community cards, and advances
// the street counter to 1.
template<typename U>
void Game<U>::dealFlop()
{
  if(street != 0){
    return;
  }
  burnOne();
  for(int i = 0; i < 3; ++i)
  {
    commonCards.push_back(makeCardFromId(deck.drawOne()));
  }
 street = 1;
}
//////////////////////////////////////////////////////
// Deals the turn: only runs if the flop has already been dealt
// (street == 1). Burns one card, adds a single community card, and
// advances the street counter to 2.
template<typename U>
void Game<U>::dealTurn()
{
  if(street != 1){
    return;
  }

  burnOne();
  commonCards.push_back(makeCardFromId(deck.drawOne()));
  street = 2;
}
//////////////////////////////////////////////////////
// Deals the river: only runs if the turn has already been dealt
// (street == 2). Burns one card, adds the final community card, and
// advances the street counter to 3.
template<typename U>
void Game<U>::dealRiver()
{
  if(street != 2){
    return;
  }
  burnOne();
  commonCards.push_back(makeCardFromId(deck.drawOne()));
  street = 3;
}
//////////////////////////////////////////////////////
// The round is considered "over" once the river has been dealt
// (street reached 3) and all 5 community cards are on the board.
template<typename U>
bool Game<U>::isRoundOver() const
{
  return street >= 3 && commonCards.size() == 5;
}
//////////////////////////////////////////////////////
// Reveal the next community stage and print only the new card(s)
// Looks at the current street and deals/prints whichever community
// stage comes next: the 3-card flop, then the single turn card, then
// the single river card. If every street has already been dealt, just
// informs the user nothing more is left to reveal.
template<typename U>
void Game<U>::revealNext()
{
  if(street == 0)
  {
    dealFlop();
    display3Cards();
  }
  else if(street == 1)
  {
    dealTurn();
    cout << "Turn: " << endl;
    cout << commonCards.back().toString() << "\n";
  }
  else if(street == 2)
  {
    dealRiver();
    cout << "River: " << endl;
    cout << commonCards.back().toString() << "\n";
  }
  else
  {
    cout << "All community cards are already on the board." << endl;
  }
}
//////////////////////////////////////////////////////
/*template<typename U>
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
*/
//////////////////////////////////////////////////////
// Builds the full 7-card hands used for evaluation: combines the
// player's hole cards with the community cards into playerCom, and does
// the same for the dealer's hole cards into dealerCom. Note the actual
// evaluation/winner logic below is currently commented out — this
// function, as written, only assembles the combined hands and leaves
// checking/paying out to be done elsewhere (see determineWinner()).
template<typename U>
void Game<U>::buildCombineHands(std::vector<DeckOfCards<U>>&playerCom, std::vector<DeckOfCards<U>>&dealerCom) const
{
  playerCom.clear();
  dealerCom.clear();

  auto& playerHand = player.getHand();
  auto& dealerHand = dealer.getHand();

  // Player: hole + common
  playerCom.insert(playerCom.end(), playerHand.begin(), playerHand.end());
  playerCom.insert(playerCom.end(), commonCards.begin(), commonCards.end());
  // Dealer: hole + common
  dealerCom.insert(dealerCom.end(), dealerHand.begin(), dealerHand.end());
  dealerCom.insert(dealerCom.end(), commonCards.begin(), commonCards.end());

}

//////////////////////////////////////////////////////
// Starts a brand-new round: resets and reshuffles the deck, clears out
// both hands, the community cards, and the burn pile, resets the street
// back to pre-flop, then deals 2 fresh hole cards each to the player and
// dealer. Also resets the winner/hand-name state for the new round,
// deducts the bet from credit immediately (the bet "leaves the stack"
// as soon as the round starts), marks the round as active, and shows the
// player their hole cards.
template<typename U>
void Game<U>::startNewRound()
{
  // resets deck and shuffles for a new round
  deck.resetDeck();
  deck.Shuffling();

  // clears old round hands
  player.clearHand();
  dealer.clearHand();
  commonCards.clear();
  burnPile.clear();
  street = 0;
  //Helper lambda to convert a card ID (1..52) into a DeckOfCards<U> "card"
  // Deal two whole cards to player & dealer
  for(int i = 0; i < 2; i++)
  {
    player.addCard(makeCardFromId(deck.drawOne()));
    dealer.addCard(makeCardFromId(deck.drawOne()));
  }

  winner = -1; // no result yet this round
  handName = "";
  credit -= bet; // bet leaves your stack when the round starts
  roundActive = true;

  cout << "\nYour hole cards: " << endl;
  player.displayHand();
}
//////////////////////////////////////////////////////
// Alternate community-card revealer (burns 1 card before revealing the
// next community card, tracked via revealedCommCard instead of street).
// Returns false if the round isn't active or all 5 community cards are
// already out. On the first call (revealedCommCard == 0) it reveals the
// 3-card flop; subsequent calls reveal one card at a time (turn, then
// river). Returns true whenever a card (or cards) were successfully
// revealed.template<typename U>// burns 1 card before revealing next community card
template<typename U>// burns 1 card before revealing next community card
bool Game<U>::revealNextCommunity()
{
  if(!roundActive){
    return false;
  }

  if(revealedCommCard >= 5)
  {
    return false;
  }

  deck.drawOne();

  // flop: reveals 3 cards
  if(revealedCommCard == 0)
  {
    for(int i = 0; i < 3; i++)
    {
      commonCards.push_back(makeCardFromId(deck.drawOne()));
    }
    revealedCommCard = 3;
    return true;
  }
  // turn/river: reveal 1 card
  commonCards.push_back(makeCardFromId(deck.drawOne()));
  revealedCommCard += 1;
  return true;
}

//////////////////////////////////////////////////////
// True once all 5 community cards have been revealed via
// revealNextCommunity(), meaning the hand is ready for a showdown.
template<typename U>
bool Game<U>::canShowDown() const
{
  return revealedCommCard == 5;
}

//////////////////////////////////////////////////////
// Player folds: bet is forfeited, dealer takes the round
// Marks the dealer as the winner and the hand name as "Fold", records
// that the player folded (so printLastResult() knows to show the short
// fold summary instead of a full showdown), records the credit loss,
// updates both sides' win/loss stats, ends the round, and prints a
// short message confirming the fold.
template<typename U>
void Game<U>::fold()
{
  winner = 2;
  handName = "Fold";
  folded = true;
  lastCreditChange = -bet;
  dealer.setWin();
  player.setLoss();
  roundActive = false;
  cout << "You folded. Dealer takes the pot of " << bet << "." << endl;
}

//////////////////////////////////////////////////////
// Player raises: adds to the bet if credit allows
// Validates that the raise amount is positive and doesn't exceed the
// player's current credit; if so, moves that amount from credit into the
// bet and reports the new bet total, returning true. Otherwise prints an
// error and returns false without changing anything.
template<typename U>
bool Game<U>::raise(int amount)
{
  if(amount <= 0 || amount > credit)
  {
    cout << "Can't raise by " << amount << " (credit: " << credit << ")" << endl;
    return false;
  }
  credit -= amount;
  bet += amount;
  cout << "Bet is now " << bet << "." << endl;
  return true;
}

//////////////////////////////////////////////////////
// Returns whether a round is currently in progress (true from
// startNewRound() until the round finishes via fold or showdown).
template<typename U>
bool Game<U>::isRoundActive() const
{
  return roundActive;
}

//////////////////////////////////////////////////////
// Output operator
// Overloads "<<" so a Game object can be streamed directly to cout,
// printing a quick status summary: player's name, current bet, current
// credit, the last hand's name, and its numeric rank.
template<typename TYPE>
std::ostream &operator<< (std::ostream &output,const Game<TYPE> &Game_Output)
{
   output << "Player: " << Game_Output.player.getName() << "\nBet: " << Game_Output.getBet()
   << "\nCredit: " << Game_Output.getCredit() << "\nLast Hand: " << Game_Output.getHandName()
   << "\nRank: " << Game_Output.getRankValue() << endl;

 return output;
}
