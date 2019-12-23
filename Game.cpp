#include "Game.h"

Game::Game()
    : _game_map(nullptr), _width(4), _height(4), _player(new Player()),
      _wumpus(nullptr), _wumpus_count(0), _pit(nullptr), _pit_count(0),
      _gold_x(0), _gold_y(0), DEFAULT_ARROW_COUNT(1), DEFAULT_PITS_COUNT(3) {
  srand((unsigned int)time(nullptr));
}
Game::~Game() { destroy_map(); }
Game::Game(int width, int height, int arrow_count, int pit_count)
    : _game_map(nullptr), _width(width), _height(height),
      _player(new Player(arrow_count)), _wumpus(nullptr), _wumpus_count(0),
      _pit(nullptr), _pit_count(0), _gold_x(0), _gold_y(0),
      DEFAULT_ARROW_COUNT(arrow_count), DEFAULT_PITS_COUNT(pit_count) {
  srand((unsigned int)time(nullptr));
}
void Game::generate_map() {
  if (_player) {
    delete _player;
    _player = new Player(DEFAULT_ARROW_COUNT);
  }
  _player->init();
  _game_map = new int *[_height];
  for (int i = 0; i < _height; ++i) {
    _game_map[i] = new int[_width];
  }
  for (int x = 0; x < _width; ++x) {
    for (int y = 0; y < _width; ++y) {
      _game_map[x][y] = (int)ObjectType::None;
    }
  }
  _pit = new Pit[DEFAULT_PITS_COUNT];
  for (int i = 0; i < DEFAULT_PITS_COUNT; ++i) {
    int index = (rand() + 1) % (_width * _height);
    if (_game_map[index / _width][index % _width] != (int)ObjectType::None) {
      --i;
      continue;
    }
    _game_map[index / _width][index % _width] = (int)ObjectType::PitType;
    _pit[i].set_x(index / _width);
    _pit[i].set_y(index % _width);
    _pit_count++;
  }
  _wumpus = new Wumpus[DEFAULT_ARROW_COUNT];
  for (int i = 0; i < DEFAULT_ARROW_COUNT; ++i) {
    int index = (rand() + 1) % (_width * _height);
    if (_game_map[index / _width][index % _width] != (int)ObjectType::None) {
      --i;
      continue;
    }
    // FIXME: pit block overwrite
    _game_map[index / _width][index % _width] = (int)ObjectType::WumpusType;
    _wumpus[i].set_x(index / _width);
    _wumpus[i].set_y(index % _width);
    _wumpus_count++;
  }
  while (true) {
    int index = (rand() % (_width * _height - 1)) + 1;
    if (_game_map[index / _width][index % _width] != (int)ObjectType::PitType) {
      _gold_x = index / _width;
      _gold_y = index % _width;
      break;
    }
  }
  _game_map[0][0] = (int)ObjectType::PlayerType;
}
void Game::generate_map(int width, int height) {
  destroy_map();
  _width = width;
  _height = height;
  generate_map();
}
void Game::destroy_map() {
  if (_player)
    delete _player;
  _player = nullptr;
  if (_wumpus)
    delete[] _wumpus;
  _wumpus = nullptr;
  _wumpus_count = 0;
  if (_pit)
    delete[] _pit;
  _pit = nullptr;
  _pit_count = 0;
}

void Game::smell(int x, int y) {
  for (int i = 0; i < _wumpus_count; ++i) {
    int wumpus_x = _wumpus[i].get_x();
    int wumpus_y = _wumpus[i].get_y();
    bool wumpus_dead = _wumpus[i].is_dead();
    if (((abs(x - wumpus_x) == 1 && y - wumpus_y == 0) ||
         (abs(y - wumpus_y) == 1 && x - wumpus_x == 0)) &&
        !wumpus_dead) {
      auto res = Event::SmellWumpus;
      event_handler(res);
    }
  }
  for (int i = 0; i < _pit_count; ++i) {
    int pit_x = _pit[i].get_x();
    int pit_y = _pit[i].get_y();
    if ((abs(x - pit_x) == 1 && y - pit_y == 0) ||
        (abs(y - pit_y) == 1 && x - pit_x == 0)) {
      auto res = Event::SmellPit;
      event_handler(res);
    }
  }
  if (_gold_x == x && _gold_y == y) {
    auto res = Event::MeetGold;
    event_handler(res);
  }
}

Event Game::take_action(Action action) {
  Event res = Event::InvalidAction;
  int x = _player->get_x();
  int y = _player->get_y();
  int bak_x = x;
  int bak_y = y;
  _player->add_action_count();
  switch (action) {
  case Action::Grab:
    if (x == _gold_x && y == _gold_y) {
      _player->grab_gold();
      res = Event::GoldGrabbed;
      event_handler(res);
    }
    break;
  case Action::Leave:
    if (x == 0 && y == 0) {
      res = Event::Leaved;
      event_handler(res);
      res = Event::GameOver;
      event_handler(res);
    } else {
      event_handler(res);
    }
    break;
  case Action::MoveForward:
    int dx, dy;
    _player->get_direction(&dx, &dy);
    x = _player->get_x() + dx;
    y = _player->get_y() + dy;
    if (!(x < 0 || x >= _height || y < 0 || y > _width)) {
      _game_map[bak_x][bak_y] = (int)ObjectType::None;
      _player->move();
      _game_map[x][y] = (int)ObjectType::PlayerType;
      res = Event::None;
      event_handler(res);
      for (int i = 0; i < _wumpus_count; ++i) {
        int wumpus_x = _wumpus[i].get_x();
        int wumpus_y = _wumpus[i].get_y();
        bool wumpus_dead = _wumpus[i].is_dead();
        if (x == wumpus_x && y == wumpus_y && !wumpus_dead) {
          res = Event::EatenByWumpus;
          event_handler(res);
          res = Event::GameOver;
          event_handler(res);
          break;
        }
      }
      for (int i = 0; i < _pit_count; ++i) {
        int pit_x = _pit[i].get_x();
        int pit_y = _pit[i].get_y();
        if (x == pit_x && y == pit_y) {
          res = Event::DropIntoPit;
          event_handler(res);
          res = Event::GameOver;
          event_handler(res);
          break;
        }
      }
      smell(x, y);
    } else {
      event_handler(res);
    }
    break;
  case Action::Shoot:
    if (_player->get_arrow_count() > 0) {
      _player->shoot();
      int tmp_x = x;
      int tmp_y = y;
      int dx, dy;
      bool kill = false;
      _player->get_direction(&dx, &dy);
      while (!(tmp_x < 0 || tmp_x >= _height || tmp_y < 0 || tmp_y >= _width ||
               kill)) {
        for (int i = 0; i < _wumpus_count; ++i) {
          if (_wumpus[i].get_x() == tmp_x && _wumpus[i].get_y() == tmp_y &&
              !_wumpus[i].is_dead()) {
            kill = true;
            _wumpus[i].kill();
            _game_map[tmp_x][tmp_y] = (int)ObjectType::None;
            break;
          }
        }
        tmp_x += dx;
        tmp_y += dy;
      }
      if (kill) {
        res = Event::KillWumpus;
        event_handler(res);
      } else {
        res = Event::ArrowCrashWall;
        event_handler(res);
      }
    } else {
      event_handler(res);
    }
    break;
  case Action::Smell:
    smell(x, y);
    break;
  case Action::TurnLeft:
    res = Event::None;
    _player->turn_left();
    event_handler(res);
    break;
  case Action::TurnRight:
    res = Event::None;
    _player->turn_right();
    event_handler(res);
    break;
  default:
    event_handler(res);
    break;
  }
  return res;
}
void Game::start() {
  if (!_game_map) {
    generate_map();
  }
  smell(0, 0);
}
// virtual void event_handler(Event event) = 0;
