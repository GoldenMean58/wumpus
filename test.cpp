#include "AI.h"
#include <stdio.h>

class GameCli : public Game {
public:
  void event_handler(Event event) override;
  GameCli();
  GameCli(int width, int height, int arrow_count, int pit_count);
};

GameCli::GameCli() {}
GameCli::GameCli(int width, int height, int arrow_count, int pit_count)
    : Game(nullptr, width, height, arrow_count, pit_count) {}
void GameCli::event_handler(Event event) {
  int score = 0;
  int grab_gold = 0;
  int dead = 0;
  switch (event) {
  case Event::Arrived:
  case Event::None:
    cout << "Nothing happens." << endl;
    break;
  case Event::InvalidAction:
    cout << "You cannot do that!" << endl;
    break;
  case Event::SmellWumpus:
    cout << "Shush! There is a wumpus near." << endl;
    break;
  case Event::KillWumpus:
    cout << "Good job! I just heard a wumpus cry" << endl;
    break;
  case Event::ArrowCrashWall:
    cout << "Oops, target missed!" << endl;
    break;
  case Event::MeetGold:
    cout << "Ohhh! Gold!" << endl;
    break;
  case Event::Dead:
    cout << "You died!" << endl;
    break;
  case Event::GoldGrabbed:
    cout << "You have put it into your pocket." << endl;
    break;
  case Event::Leaved:
    cout << "You leaved the cave." << endl;
    break;
  case Event::EatenByWumpus:
    cout << "A wumpus had had you for lunch." << endl;
    break;
  case Event::DropIntoPit:
    cout << "You dropped into a pit." << endl;
    break;
  case Event::GameOver:
    score = 0;
    if (_player->is_grab_gold())
      grab_gold = 1000;
    score += grab_gold;
    score -= _player->get_action_count();
    score -= DEFAULT_ARROW_COUNT - _player->get_arrow_count();
    if (_player->is_dead)
      dead -= 1000;
    score += dead;
    cout << "Game over! Score = (Grab gold?) " << grab_gold << " + (dead?) "
         << dead << " - (step) " << _player->get_action_count()
         << " - (arrow used) "
         << DEFAULT_ARROW_COUNT - _player->get_arrow_count() << " = " << score
         << endl;
    break;
  case Event::SmellPit:
    cout << "Look out! There is a pit near!" << endl;
    break;
  }
}
/*
int main(void) {
  // GameCli game(10, 10, 3, 6);
  GameCli game;
  game.start();
  game.print_map();
  while (1) {
    char input = getc(stdin);
    if (input == '\n' || input == '\r') {
      game.print_map();
      continue;
    }
    Action action;
    switch (input) {
    case 'w': // move forward
    case 'W':
      action = Action::MoveForward;
      break;
    case 'a': // turn left
    case 'A':
      action = Action::TurnLeft;
      break;
    case 'd': // turn right
    case 'D':
      action = Action::TurnRight;
      break;
    case 's': // shoot
    case 'S':
      action = Action::Shoot;
      break;
    case 'g': // grab gold
    case 'G':
      action = Action::Grab;
      break;
    case 'l': // leave
    case 'L':
      action = Action::Leave;
      break;
    case 'I': // Smell
    case 'i':
      action = Action::Smell;
      break;
    default:
      action = Action::None;
      cout << "Invalid input" << endl;
      break;
    }
    if (Event::GameOver == game.take_action(action)) {
      break;
    }
  }
  return 0;
}
*/
int main(void) {
  Database game(nullptr, 4, 4, 1, 3);
  game.start();
  int data[10];
  // game.smell(0, 0);
  Event event = Event::None;
  while (event != Event::GameOver && !game.is_over()) {
    Action action = game.ask(data);
    event = game.take_action(action, data);
  }
  return 0;
}
