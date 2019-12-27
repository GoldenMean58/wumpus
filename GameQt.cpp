#include "GameQt.h"
GameQt::GameQt() {}
void GameQt::event_handler(Event event) {
  Database::_tell(event);
  switch (event) {
  case Event::Dead:
    break;
  }
}

int **GameQt::get_map() { return _game_map; }
Player *GameQt::get_player() { return _player; }
