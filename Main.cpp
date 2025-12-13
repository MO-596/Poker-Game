//#include "DeckHeader.h"
#include "Deck.cpp"
#include "Player.cpp"
#include "Dealer.cpp"
#include "Poker_Game.cpp"
// TODO: HAVE To crete the menu process and logic
//displays
void WelcomeFunction();
void MenuDisplay();
void playDisplay();

// user input validation
int validMainMenuInput(int&);
int InputValidation(int&);
int validPlayMenuInput(int&);
// user processes
bool process(int, DeckOfCards<int> *, Dealer<string> *, Player<string> *, Game<int> *);
bool playProcess(int,Game<int>*);

// sections
void Play_Section(Game<int>*);
//menus
void Play_Menu();

////////////////////////////////////////////////////////////

int main()
{
  bool Repeat = true;
  int choice = 0;
  int startingCredit;

  DeckOfCards<int> *Deck = new DeckOfCards<int>();	//object class creation
  Player<string> *player = new Player<string>();
  Dealer<string> *dealer = new Dealer<string>();
  Game<int> *game = new Game<int>();

//  cin >> *player;
//  Game<int> *game = new Game<int>(player->getName());

  WelcomeFunction();
  cout << "\nEnter your starting credits: ";
  cin >> startingCredit;
  startingCredit = InputValidation(startingCredit);
  game->setCredit(startingCredit);

  do{
     MenuDisplay();
     cout << "Enter your choice: " << endl;
     cin >> choice;
     choice = validMainMenuInput(choice);
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
  cout << "1. Play" << endl;
  cout << "2. Load Stats" << endl;
  cout << "3. Exit" << endl;
}
///////////////////////////////////////////////////////////////

//  cin >> *player;
///////////////////////////////////////////////////////////////

bool process(int choice,  DeckOfCards<int>* Deck, Dealer<string>* dealer,
	Player<string>* player, Game<int>* game)
{
  bool Repeat = true;
  enum Choices { PLAY = 1, STATS, EXIT};

  switch(choice){
   case PLAY:
   cout << "Chosen to play (Good Luck <O-O>)..." << endl;
   Play_Section(game); // goes into play sub-menu
   break;

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
///////////////////////////////////////////////////////////////

void Play_Section(Game<int>* game)
{
  bool Repeat = true;
  int choice;

  do{
    Play_Menu();
    cout << endl << "Enter Choice: ";
    cin >> choice;
    choice = validPlayMenuInput(choice);
    Repeat = playProcess(choice, game);
  }while(Repeat);
}

///////////////////////////////////////////////////////////////
void Play_Menu()
{
  cout << "\n=================Play Menu=================" << endl;
  cout << "Let's Go Gambling" << endl;
  cout << "1. Play new round" << endl;
  cout << "2. Show last hand results" << endl;
  cout << "3. Exit to Main Menu" << endl;

}
///////////////////////////////////////////////////////////////
bool playProcess(int choice, Game<int>* game)
{
  bool Repeat = true;
  enum Choices { NEW_ROUND = 1, LAST_ROUND, EXIT };

  switch(choice)
  {
    case NEW_ROUND:{
      cout << "<------ New Round ------>" << endl;
      //Display round
      cout << "<------ Round" << game->getRoundNumber() <<" ------>" << endl;

      // Show current credits
      int currCredit = game->getCredit();
      cout << "Current credits: " << endl;

      int bet;
      do{ // Get player bet
        cout << "Enter your bet: ";
        cin >> bet;
        bet = InputValidation(bet);

        if(bet <= 0)
        {
	  cout << "Bet has to be greater than 0. Try again!" << endl;
        }
        else if(bet > currCredit)
        {
  	  cout << "Bet can't exceed current credit amount. Try again!" << endl;
        }
      }while(bet <= 0 || bet > currCredit);

      // Set player bet in Game class
      game->setBet(bet);

      // Deal Community cards + hole cards
      int CommCards = 5;
//      game->dealCards(CommCards);
      game->playRound(CommCards);

      break;
    }

    case LAST_ROUND:{
      cout << "<------ Last Hand Results (Round " << game->getRoundNumber() << ") ------>" << endl;
      int winner = game->getWinner();
      string handName = game->getHandName();
      string highCard = game->getHighCard();
      int credit = game->getCredit();

      if(winner == 1)
      {
	cout << "Player won with: " << handName << endl;
      }
      else if(winner == 2)
      {
        cout << "Dealer won with: " << handName << endl;
      }
      else
      {
        cout << "Tie with: " << endl;
      }

      cout << " (High card: " << highCard << ")" << endl;
      cout << "Current credits: " << credit << endl;
      break;
    }

    case EXIT:{
      cout << "Chosen the quit(Good Idea!)" << endl;
      cout << "\n------------------------\n";
      Repeat = false;
    }
  }
 return Repeat;
}

///////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////

int validPlayMenuInput(int& choice)
{
  while((choice < 1) || (choice > 4) || cin.fail())
  {
    cin.clear();//clears the last input to avoid it repeating
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');//ignores anything that isnt a number
    cout << "Invalid Input, Try again" << endl;
    cin >> choice;
  }
  return choice;
}

///////////////////////////////////////////////////////////////
/*
int validPlayMenuInput(int& choice)
{
  while((choice < 1) || (choice > 3) || cin.fail())
  {
    cin.clear();//clears the last input to avoid it repeating
    cin.ignore(numeric_limits <streamsize> ::max(), '\n');//ignores anything that isnt a number
    cout << "Invalid Input, Try again" << endl;
    cin >> choice;
  }
  return choice;
}
*/
///////////////////////////////////////////////////////////////

int validMainMenuInput(int& choice)
{
  while((choice < 1) || (choice > 3) || cin.fail())
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
