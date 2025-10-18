//#include "DeckHeader.h"
#include "Deck.cpp"
#include "Player.cpp"
#include "Dealer.cpp"
#include "Poker_Game.cpp"
 
//displays
void WelcomeFunction();
void MenuDisplay();

// user input validation
int validInput(int&);
int InputValidation(int&);

// user processes
bool process(int, DeckOfCards<int> *, Dealer<string> *, Player<string> *, Game<int> *);


int main()
{
  bool Repeat = true;
  int choice = 0;

  DeckOfCards<int> *Deck = new DeckOfCards<int>();	//object class creation
  Player<string> *player = new Player<string>();
  Dealer<string> *dealer = new Dealer<string>();
  Game<int> *game = new Game<int>();

//  cin >> *player;
//  Game<int> *game = new Game<int>(player->getName());

  WelcomeFunction();
  do{
     MenuDisplay();
     cout << "Enter your choice: " << endl;
     cin >> choice;
     choice = validInput(choice);
     Repeat = process(choice, Deck, dealer, player, game);
  }while(Repeat);

}

///////////////////////////////////////////////////////////////
void WelcomeFunction()
{
  cout << "Welcome to a Poker Game!" << endl;
  cout << "Here's the Main Menu, please choose one of the following: " << endl;
  cout << "Owned by Melvin" << endl;
}
///////////////////////////////////////////////////////////////
void MenuDisplay()
{
  cout << "\n=============== Poker Game Time! =============== " << endl;
  cout << "" << endl;
}
///////////////////////////////////////////////////////////////

int validInput(int& choice)
{
  while((choice < 1) || (choice > 8) || cin.fail())
  {
    cin.clear();//clears the last input to avoid it repeating
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');//ignores anything that isnt a number
    cout << "Invalid Input, Try again" << endl;
    cin >> choice;
  }
  return choice;
}
///////////////////////////////////////////////////////////////
int InputValidation(int& value)
{
  while((cin.fail()) || (value < 1) || (value > 300))
  {
    cin.clear();//clears the last input to avoid it repeating
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');//ignores anything that isnt a number
    cout << "Input needs to be a integer, Try again" << endl;
    cin >> value;
  }
 return value;
}
//  cin >> *player;
///////////////////////////////////////////////////////////////

bool process(int choice,  DeckOfCards<int>* Deck, Dealer<string>* dealer, Player<string>* player, Game<int>* game)
{
  bool Repeat = true;
  enum Choices { PLAY = 1, STATS, EXIT};

  switch(choice){
   case PLAY:
   cout << "Chosen to play (Good Luck <O-O>)..." << endl;
   break;

   case

   case STATS:
   cout << "You have chosen to save to a file..." << endl;
   //   med->saveDataToFile("PatientsInfo.txt");

   cout << "\n";
   break;

   case EXIT:
    cout << "Exiting, Thank you for you for using the app!" << endl;
    cout << "\n------------------------\n";
    Repeat = false;
  }


 return Repeat;
}

