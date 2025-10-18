#include "DeckHeader.h"

//constuctor
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
template<typename D>
void DeckOfCards<D>::Dealing()
{
  // reset draw so each dealing starts on top
  nextCard = 0;

  size_t row, col;
  unsigned int id;

  cout << "The hand is:\n";
  for (int i = 0; i < 5; ++i)
  {
    id = drawOne();

    row = (id - 1) / 13; // suit indexes
    col = (id - 1) % 13;  // face indexes

    Hand[i][0] = row;
    Hand[i][1] = col;

    cout << face[col] << " of " << suit[row] << "\n";
  }
  cout << "\n";
}

///////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::resetDeck()
{
  // will reset the deck to be empty to allow new shuffle to happen
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
template<typename D>
unsigned int DeckOfCards<D>::drawOne()
{
  // maps linear index into the Deck array
  size_t row = nextCard / 13;
  size_t col = nextCard % 13;

  unsigned int cardID = Deck[row][col]; // grab the card id (1..52) from Deck[row][col]
  ++nextCard;

 return cardID;
}

///////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::Pair()
{
  int counter[13] = { 0 };	//the number of cards in each hand
  size_t i, j;
  for (i = 0; i < 5; i++)	//records the value of the card
  {
    ++counter[Hand[i][1]];	//adds up the value of card and compares them
  }

  for (j = 0; j < 13; j++)
  {
    if (counter[j] == 2)
    {
	cout << "Hand contains a pair of: " << face[j] << endl;
    }
  }

}

/////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
bool DeckOfCards<D>::TwoPair()
{
  int counter[13] = { 0 };      //the number of cards in each hand
  size_t i, j, m;
  for (i = 0; i < 5; i++)       //records the value of the card
  {
    ++counter[Hand[i][1]];    //adds up the value of card and compares them
  }

  for (j = 0; j < 13; j++)      //first loop checks if there is a pair
  {
    for(m = 0; m < j; m++)
    {
       if (counter[j]==2 && counter[m]== 2)
       {
         cout << "Hand contains two pair of: " << face[j]<<" and " << face[m] <<endl;
         return true;
       }
    }
  }

return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::Three_Of_A_Kind()
{
  int counter[13] = { 0 };	//the number of cards in each hand
  size_t n, m;
  for (n = 0; n < 5; n++)	//records the value of the card
  {
    ++counter[Hand[n][1]];	//adds up the value of card and compares them
  }

  for (m = 0; m < 13; m++)
  {
    if (counter[m] == 3)//compares the number of the suit to see if they all four of them have suit 3
    {
      cout << "Hand contains a three of: " << face[m] << endl;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::Four_Of_A_Kind()
{
  int counter[13] = { 0 };	//the number of cards in each hand
  size_t l, o;
  for (l = 0; l < 5; l++)	//records the value of the card
  {
    ++counter[Hand[l][1]];	//adds up the value of card and compares them
  }

  for (o = 0; o < 13; o++)
  {
    if (counter[o] == 4)//compares the number of the suit to see if they all four of them have suit 4
    {
      cout << "Hand contains a four of: " << face[o] << endl;
    }
  }
}

//////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::Flush()
{
  int counter[13] = { 0 };	//the number of cards in each hand
  size_t r, f;
  for (r = 0; r < 5; r++)	//records the value of the card
  {
      ++counter[Hand[r][0]];	//adds up the value of card and compares them
  }//starts of on zero to check if all five cards have the same face

  for (f = 0; f < 13; f++)
  {
    if (counter[f] == 5)//if the cards has the five same faces
    {
	  cout << "Hand contains a Flush of: " << suit[f]<<endl;
    }
  }
}

////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::Straight()
{
  int S[5] = { 0 };		//array to store copy of hand to compare
  int temp = 0;
  size_t pass, compare;	//loop counters
  bool isStraight = true;

  for (size_t i = 0; i < 5; i++)
  {
    S[i] = Hand[i][1];	//stores the array here
  }

  for (pass = 0; pass < 4; pass++)	//sort the hand using bubble sorting to move the placementss
    //by moving them from greatest to least
  {
    for (compare = 0; compare < 4 - pass; compare++)
    {
      if (S[compare] > S[compare + 1])
      {
	temp = S[compare];//hold the value of S[compare] in temporay value
	S[compare] = S[compare + 1];	//increments the compare
	S[compare + 1] = temp;//swap the value to the next value in the array
      }
    }
  }

  // Check for a consecutive run
  for(size_t i = 1; i < 5; i++)
  {
    if(S[i] != S[i - 1] + 1)
    {
      isStraight = false;
      break;
    }
  }

  // special case play (Ace-High) : A-10-J-Q-K
  if(!isStraight)
  {
    if( S[0] == 0 && S[1] == 9 && S[2] == 10 &&
    S[3] == 11 & S[4] == 12 )
    {
      isStraight = true;
      cout << "Hand is a Straight from Ten to Ace" << endl;
    }
  }

  // printing the standard straight play
  if(isStraight)
  {
    cout << "Hand is a Straight: " << face[S[0]] << " of " << face[S[4]] << endl;
  }
/*
  if (S[4] - 1 == S[3] && S[3] - 1 == S[2] && S[2] - 1 == S[1]
      && S[1] - 1 == S[0])
    //compares the value by the previous value of the card and type of the face to see if its a straight
  {
    cout << "Hand is a Straight: " << face[S[0]] << " of " << face[S[4]]<<endl;
  }
*/
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::FullHouse()
{
  int counter[13] = { 0 };      //the number of cards in each hand
  bool hasThree = false, hasTwo = false;
  int threeFace = 0, twoFace = 0;

  for (size_t i = 0; i < 5; i++)       //records the value of the card
  {
    ++counter[ Hand[i][1] ];      //adds up the value of card and compares them
  }

  for (size_t j = 0; j < 13; j++)
  {
    if (counter[j] == 3)//compares the number of the suit to see if they all four of them have suit 4
    {
      hasThree = true;
      threeFace = j;
    }
    else if(counter[j] == 2)
    {
      hasTwo = true;
      twoFace = j;
    }
  }

  if(hasThree && hasTwo)
  {
      cout << "Hand contains a full house of: "
	<< face[threeFace] << " over " << face[twoFace] << endl;
  }
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
bool DeckOfCards<D>::StraightFlush()
{
  // counts suits and identify suits
  int suitCount[4] = {0};
  int flushSuit = -1;
  vector<int> ranks;

  for (int i = 0; i < 5; i++)
  {
    ++suitCount[ Hand[i][0] ];
  }

  for (int s = 0; s < 4; s++)
  {
   if (suitCount[s] == 5)
   {
     flushSuit = s;
     break;
   }
  }

  if (flushSuit < 0)
  {
    // No suit has all 5 cards
    return false;
  }

  ranks.reserve(5); // Msut be a better way

  for(int i = 0; i < 5; i++)
  {
    if(Hand[i][0] == flushSuit)
    {
	ranks.push_back(Hand[i][1]); // compares suit, push rank
    }
  }

  sort(ranks.begin(),ranks.end()); // sort and checks for consecutive runs

  for(int i = 1; i < 5; i++)
  {
    if(ranks[i] != ranks[i - 1] + 1)
    {
	return false;
    }
  }

  cout << "Hand contains a Straight Flush: " << face[ranks.front()] <<
  " to " << face[ranks.back()] << " of " << suit[flushSuit] << endl;

 return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<typename D>
void DeckOfCards<D>::RoyalFlush()
{
  const int ROYAL_RANKS[5] = {0 , 9, 10, 11, 12}; // Default ranks for RF
  int suitCount[4] = {0};
  int flushSuit = -1;

  for (int i = 0; i < 5; i++) // Counts suits to see if all 5 cards share the same suits
  {
    ++suitCount[ Hand[i][0] ];
  }

  // Identify which suit (if any) has all 5 cards
  for(int i = 0; i < 4; i++)
  {
    if(suitCount[i] == 5)
    {
      flushSuit = i;
      break;
    }
  }

  if(flushSuit < 0){ // if suit doesn't have 5 cards, no royal flush
   return;
  }

  // Checks if each required rank appears in the flush suit
  for(int needed : ROYAL_RANKS)
  {
    bool found = false;
    for(int i = 0; i < 5; i++)
    { // only looks at cards of the flush suit
      if(Hand[i][0] == flushSuit &&  Hand[i][1] == needed)
      {
	found = true;
	break;
      }
    }
    // if any rank is missing, it's not a royal flush
    if(!found)
    {
      return;
    }
  }
  cout << "Hand contains a Royal Flush of: " << suit[flushSuit] << endl;
}
//////////////////////////////////////////////////////////////////////////

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

template<typename D>
int DeckOfCards<D>::getRankIndex() const
{
  return rankIndex;
}
//////////////////////////////////////////////////////////////////////////

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

template<typename D>
int DeckOfCards<D>::getSuitIndex() const
{
  return suitIndex;
}
