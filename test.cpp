#include "Game.h"
#include <stdio.h>

class GameCli : public Game {
public:
  void event_handler(Event event) override;
  void print_map();
  GameCli();
  GameCli(int width, int height, int arrow_count, int pit_count);
};

GameCli::GameCli() {}
GameCli::GameCli(int width, int height, int arrow_count, int pit_count)
    : Game(width, height, arrow_count, pit_count) {}
void GameCli::print_map() {
  for (int x = 0; x < _height; ++x) {
    for (int y = 0; y < _width; ++y) {
      switch (_game_map[x][y]) {
      case (int)ObjectType::None:
        if ((x != _gold_x || y != _gold_y) || _player->is_grab_gold()) {
          cout << ".";
        } else {
          cout << "#";
        }
        break;
      case (int)ObjectType::PlayerType:
        switch (_player->get_direction(nullptr, nullptr)) {
        case Direction::Up:
          cout << "^";
          break;
        case Direction::Down:
          cout << "V";
          break;
        case Direction::Left:
          cout << "<";
          break;
        case Direction::Right:
          cout << ">";
          break;
        }
        break;
      case (int)ObjectType::WumpusType:
        cout << "W";
        break;
      case (int)ObjectType::PitType:
        cout << "P";
        break;
      }
    }
    cout << endl;
  }
}
void GameCli::event_handler(Event event) {
  int score = 0;
  switch (event) {
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
    if (_player->is_grab_gold())
      score += 1000;
    score -= _player->get_action_count();
    score -= DEFAULT_ARROW_COUNT - _player->get_arrow_count();
    cout << "Game over! Score = " << score << endl;
    break;
  case Event::SmellPit:
    cout << "Look out! There is a pit near!" << endl;
    break;
  }
}

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
