#include "DeckHeader.h"

int
main ()
{
  DeckOfCards deckOfCards;	//object class creation

  deckOfCards.Shuffling ();
  deckOfCards.Dealing ();

  bool flag = deckOfCards.TwoPair();
  if(flag==false) //checks if the cards are two pairs if not print out one pair
  {
   deckOfCards.Pair();
  }

  deckOfCards.Three_Of_A_Kind ();
  deckOfCards.Four_Of_A_Kind ();
  deckOfCards.Flush ();
  deckOfCards.Straight ();
  return 0;
}
