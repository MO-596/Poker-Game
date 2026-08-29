//#include "DeckHeader.h"
#include "Deck.cpp"
#include "Player.cpp"
#include "Dealer.cpp"
#include "Poker_Game.cpp"
//displays
void WelcomeFunction();     // prints the welcome banner
void MenuDisplay();         // prints the main menu options
void playDisplay();         // (unused/declared but not defined) placeholder for a play display
void subPlayDisplay();      // (unused/declared but not defined) placeholder for a sub-play display

// user input validation
int validMainMenuInput(int&);      // makes sure main menu choice is between 1-3
int InputValidation(int&);         // makes sure a generic numeric input is between 1-300
int validPlayMenuInput(int&);      // makes sure Play menu choice is between 1-4
int validSubPlayMenuInput(int&);   // makes sure Sub-play (in-round) menu choice is between 1-4

// user processes
bool process(int, DeckOfCards<int> *, Dealer<string> *, Player<string> *, Game<int> *);
bool playProcess(int,Game<int>*, DeckOfCards<int>*, Dealer<string>* ,Player<string>*);
bool subPlayProcess(int,Game<int>*, DeckOfCards<int>*, Dealer<string>* ,Player<string>*);

// sections
void Play_Section(Game<int>*, DeckOfCards<int>*, Dealer<string>* ,Player<string>*);
void SubPlay_Section(Game<int>*);

//menus
void Play_Menu();
void SubPlay_Menu();

////////////////////////////////////////////////////////////
// Entry point of the program.
// Sets up the deck, player, dealer and game objects, asks the user for
// their starting credit, then loops showing the main menu until the
// user chooses to exit.
int main()
{

  bool Repeat = true;      // controls the main menu loop; false ends the program
  int choice = 0;          // holds the user's main menu selection
  int startingCredit;      // holds how many credits the player starts with

  // Create the core game objects on the heap (freed only when program exits)
  DeckOfCards<int> *Deck = new DeckOfCards<int>();	//object class creation
  Player<string> *player = new Player<string>();
  Dealer<string> *dealer = new Dealer<string>();
  Game<int> *game = new Game<int>();

  WelcomeFunction();// print the welcome banner

  cout << "\nEnter your starting credits: ";
  cin >> startingCredit;
  startingCredit = InputValidation(startingCredit);
  game->setCredit(startingCredit);

  // Main program loop: show the menu, read a choice, validate it, then
  // act on it via process(). Loop continues until process() returns false
  // (i.e. the user picked "Exit").
  do{
     MenuDisplay();
     cout << "Enter your choice: " << endl;
     cin >> choice;
     choice = validMainMenuInput(choice);
     Repeat = process(choice, Deck, dealer, player, game);
  }while(Repeat);

}

///////////////////////////////////////////////////////////////
// Prints the one-time welcome banner shown when the program starts.
void WelcomeFunction()
{
  cout << "Welcome to a Poker Game!" << endl;
  cout << "Here's the Main Menu, please choose one of the following: " << endl;
  cout << "Owned by Melvin" << endl;
}
///////////////////////////////////////////////////////////////
// Prints the top-level main menu (Play / Load Stats / Exit).
void MenuDisplay()
{
  cout << "\n=============== Poker Game Time! =============== " << endl;
  cout << "1. Play" << endl;
  cout << "2. Load Stats" << endl;
  cout << "3. Exit" << endl;
}
///////////////////////////////////////////////////////////////
// Handles whatever the user picked from the main menu.
// PLAY  -> enters the Play sub-menu loop (Play_Section)
// STATS -> placeholder for saving stats to a file (not yet implemented)
// EXIT  -> prints a goodbye message and tells main() to stop looping
// Returns true to keep the main loop going, false to end the program.
bool process(int choice,  DeckOfCards<int>* Deck, Dealer<string>* dealer,
	Player<string>* player, Game<int>* game)
{
  bool Repeat = true;
  enum Choices { PLAY = 1, STATS, EXIT};

  switch(choice){
   case PLAY:
   cout << "Chosen to play (Good Luck <O-O>)..." << endl;
   Play_Section(game, Deck, dealer, player); // goes into play sub-menu
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
// Runs the "Play" sub-menu loop: keeps showing the Play menu and
// dispatching choices via playProcess() until the user exits back
// to the main menu (playProcess returns false).
void Play_Section(Game<int>* game, DeckOfCards<int>* Deck, Dealer<string>* dealer,
        Player<string>* player)
{
  bool Repeat = true;
  int choice;

  do{
    Play_Menu();
    cout << endl << "Enter Choice: ";
    cin >> choice;
    choice = validPlayMenuInput(choice);
    Repeat = playProcess(choice, game, Deck, dealer, player);
  }while(Repeat);
}

///////////////////////////////////////////////////////////////
// Prints the Play menu (start a round, see last results, check credit, exit).
void Play_Menu()
{
  cout << "\n=================Play Menu=================" << endl;
  cout << "Let's Go Gambling" << endl;
  cout << "1. Play new round" << endl;
  cout << "2. Show last hand results" << endl;
  cout << "3. Display current credit" << endl;
  cout << "4. Exit to Main Menu" << endl;

}
///////////////////////////////////////////////////////////////
// Handles a single choice made in the Play menu.
// NEW_ROUND  -> increments the round counter, asks for/validates a bet,
//               starts a new round (shuffles + deals hole cards), then
//               drops into the SubPlay ("in-round") menu.
// LAST_ROUND -> prints the results of the most recently finished round.
// CREDITS    -> just prints the player's current credit total.
// EXIT       -> returns to the main menu.
// Returns true to keep looping in the Play menu, false to leave it.
bool playProcess(int choice, Game<int>* game, DeckOfCards<int>* Deck, Dealer<string>* dealer,
        Player<string>* player)
{
  bool Repeat = true;
  enum Choices { NEW_ROUND = 1, LAST_ROUND,CREDITS, EXIT };

  switch(choice)
  {
    case NEW_ROUND:{
      cout << "<------ New Round ------>" << endl;
      game->incrementRound();
      //Display round
      cout << "<------ Round " << game->getRoundNumber() <<" ------>" << endl;

      // Show current credits
      int currCredit = game->getCredit();
      cout << "Current credits: " << currCredit << endl;

      // Keep asking for a bet until it's a valid positive number that
      // doesn't exceed the player's current credit balance.
      int bet;
      do{
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
      game->startNewRound();

      SubPlay_Section(game); // goes into play sub-menu(check/raise/fold loop for the round)
      break;
    }

    case LAST_ROUND:{
      // Prints a summary (via game->printLastResult) of whichever round
      // most recently finished, including a fold or a full showdown.
      cout << "<------ Last Hand Results (Round " << game->getRoundNumber() << ") ------>" << endl;
      game->printLastResult();
      break;
    }

    case CREDITS:{
      // Simply reports how many credits the player currently has.
      cout << "Checking Credits...." << endl;
      cout << "Current Credits: " << game->getCredit() << endl;
      break;
    }

    case EXIT:{
      // Leaves the Play menu loop and returns control to the main menu.
      cout << "Chosen the quit(Good Idea!)" << endl;
      cout << "\n------------------------\n";
      Repeat = false;
    }
  }
 return Repeat;
}

///////////////////////////////////////////////////////////////
// Prints the in-round ("Sub-play") menu shown once cards have been dealt:
// Check, Raise, Fold, or Quit the current game.
void SubPlay_Menu()
{
  cout << "\n=================Game Menu=================" << endl;
  cout << "1. Check" << endl;
  cout << "2. Raise" << endl;
  cout << "3. Fold" << endl;
  cout << "4. Quit Game" << endl;
}

///////////////////////////////////////////////////////////////
// Handles a single choice made during an active round.
// CHECK -> reveals the next community card stage (flop/turn/river) with
//          no extra bet; if that was the river, the round ends and the
//          winner is determined.
// RAISE -> asks for and validates a raise amount, applies it to the bet,
//          then reveals the next community stage just like Check.
// FOLD  -> player forfeits the bet immediately; round ends.
// QUIT  -> same as folding, but frames it as quitting the whole game.
// Returns true to keep looping the sub-play menu (round still going),
// false once the round has ended (win/lose/tie or fold).
bool subPlayProcess(int choice, Game<int>* game)
{
  bool Repeat = true;
  enum Choices { CHECK = 1, RAISE, FOLD, QUIT};

  switch(choice)
  {
    case CHECK:
      cout << "Decided to Check..." << endl;
      game->revealNext();                 // flop, then turn, then river
      if(game->isRoundOver())
      {
        game->determineWinner(); // all 5 community cards are out; settle the hand
        Repeat = false;
      }
      break;

    case RAISE:{
      cout << "Decided to Raise..." << endl;
      int amount;

      // Ask for and validate the raise amount before applying it
      cout << "Raise by how much? ";
      cin >> amount;

      amount = InputValidation(amount);

      game->raise(amount);   // adds to the bet if the player has enough credit
      game->revealNext();    // still advances the board just like a check

      if(game->isRoundOver())
      {
        game->determineWinner();
        Repeat = false;
      }
      break;
    }

    case FOLD:
      // Player gives up the hand; bet is lost, dealer automatically wins.
      game->fold();
      Repeat = false;
      break;

    case QUIT:
      // Treated the same as a fold (bet is forfeited) but with a
      // different message, then leaves the sub-play loop.
      cout << "Chosen to Quit Game (bet is forfeited)" << endl;
      game->fold();
      cout << "\n------------------------\n";
      Repeat = false;
  }
  return Repeat;
}

///////////////////////////////////////////////////////////////
// Runs the in-round menu loop: keeps showing Check/Raise/Fold/Quit and
// dispatching to subPlayProcess() until the round ends.
void SubPlay_Section(Game<int>* game)
{
  bool Repeat = true;
  int choice;

  do{
    SubPlay_Menu();
    cout << endl << "Enter Choice: ";
    cin >> choice;
    choice = validSubPlayMenuInput(choice);
    Repeat = subPlayProcess(choice, game);

  }while(Repeat);
}

///////////////////////////////////////////////////////////////
// Re-prompts the user until they enter a number from 1 to 4 for the
// Play menu (handles both out-of-range numbers and non-numeric input).
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
// Re-prompts the user until they enter a number from 1 to 4 for the
// Sub-play (in-round) menu.
int validSubPlayMenuInput(int& choice)
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
// Re-prompts the user until they enter a number from 1 to 3 for the
// main menu.
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
// General-purpose numeric input validator used for credits, bets and
// raise amounts. Re-prompts until the value is a real integer between
// 1 and 300 inclusive.
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
