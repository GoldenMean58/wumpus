#include "Game.h"

void Game::_init() {
  srand((unsigned int)time(nullptr));
  _information_map = new Block *[_height];
  _view_map = new Block *[_height];
  for (int i = 0; i < _height; ++i) {
    _information_map[i] = new Block[_width];
    _view_map[i] = new Block[_height];
  }
  Block **map[2] = {_information_map, _view_map};
  for (int z = 0; z < 2; ++z)
    for (int i = 0; i < _height; ++i) {
      for (int j = 0; j < _width; ++j) {
        map[z][i][j].is_unknown = TriState::Yes;
        map[z][i][j].is_safe = TriState::Unknown;
        map[z][i][j].is_visited = TriState::No;
        map[z][i][j].is_breeze = TriState::Unknown;
        map[z][i][j].is_stench = TriState::Unknown;
        map[z][i][j].is_pit = TriState::Unknown;
        map[z][i][j].is_wumpus = TriState::Unknown;
        map[z][i][j].is_gold = TriState::Unknown;
      }
    }
}
Game::Game()
    : _game_map(nullptr), _width(4), _height(4), _player(new Player()),
      _wumpus(nullptr), _wumpus_count(0), _pit(nullptr), _pit_count(0),
      _gold_x(0), _gold_y(0), _is_over(false), _arrow_count(1),
      DEFAULT_ARROW_COUNT(1), DEFAULT_PITS_COUNT(3) {
  _init();
}
Game::~Game() { destroy_map(); }
Game::Game(int **game_map, int width, int height, int arrow_count,
           int pit_count)
    : _game_map(game_map), _width(width), _height(height),
      _player(new Player(arrow_count)), _wumpus(nullptr), _wumpus_count(0),
      _pit(nullptr), _pit_count(0), _gold_x(0), _gold_y(0), _is_over(false),
      _arrow_count(arrow_count), DEFAULT_ARROW_COUNT(arrow_count),
      DEFAULT_PITS_COUNT(pit_count) {
  _init();
}
int Game::get_gold_x() { return _gold_x; }
int Game::get_gold_y() { return _gold_y; }
void Game::generate_map(bool is_random_build = true) {
  _is_over = false;
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
  if (!is_random_build)
    return;
  _pit = new Pit[DEFAULT_PITS_COUNT];
  while (_pit_count < DEFAULT_PITS_COUNT) {
    int index = rand() % (_width * _height - 1) + 1;
    if (_game_map[index / _width][index % _width] != (int)ObjectType::None) {
      continue;
    }
    if (index == 0)
      cout << "No" << endl;
    _game_map[index / _width][index % _width] = (int)ObjectType::PitType;
    _pit[_pit_count].set_x(index / _width);
    _pit[_pit_count].set_y(index % _width);
    _pit_count++;
  }
  _wumpus = new Wumpus[DEFAULT_ARROW_COUNT];
  while (_wumpus_count < DEFAULT_ARROW_COUNT) {
    int index = rand() % (_width * _height - 1) + 1;
    if (_game_map[index / _width][index % _width] != (int)ObjectType::None) {
      continue;
    }
    // FIXME: pit block overwrite
    _game_map[index / _width][index % _width] = (int)ObjectType::WumpusType;
    _wumpus[_wumpus_count].set_x(index / _width);
    _wumpus[_wumpus_count].set_y(index % _width);
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
  _action_queue.clear();
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
  if (_information_map) {
    for (int i = 0; i < _height; ++i) {
      delete[] _information_map[i];
    }
    delete[] this->_information_map;
    this->_information_map = nullptr;
  }
  if (_view_map) {
    for (int i = 0; i < _height; ++i) {
      delete[] _view_map[i];
    }
    delete[] this->_view_map;
    this->_view_map = nullptr;
  }
}

void Game::smell(int x, int y) {
  auto res = Event::None;
  for (int i = 0; i < _wumpus_count; ++i) {
    int wumpus_x = _wumpus[i].get_x();
    int wumpus_y = _wumpus[i].get_y();
    bool wumpus_dead = _wumpus[i].is_dead();
    if (((abs(x - wumpus_x) == 1 && y - wumpus_y == 0) ||
         (abs(y - wumpus_y) == 1 && x - wumpus_x == 0)) &&
        !wumpus_dead) {
      res = Event::SmellWumpus;
      event_handler(res);
    }
  }
  for (int i = 0; i < _pit_count; ++i) {
    int pit_x = _pit[i].get_x();
    int pit_y = _pit[i].get_y();
    if ((abs(x - pit_x) == 1 && y - pit_y == 0) ||
        (abs(y - pit_y) == 1 && x - pit_x == 0)) {
      res = Event::SmellPit;
      event_handler(res);
    }
  }
  if (_gold_x == x && _gold_y == y) {
    res = Event::MeetGold;
    event_handler(res);
  }
  if (res == Event::None)
    event_handler(res);
}
bool Game::is_over() { return _is_over; }
Event Game::move(int from_x, int from_y, int x, int y) {
  Event res = Event::InvalidAction;
  if (!(x < 0 || x >= _height || y < 0 || y >= _width)) {
    _game_map[from_x][from_y] = (int)ObjectType::None;
    _player->move_to(x, y);
    _information_map[x][y].is_visited = TriState::Yes;
    _information_map[x][y].is_unknown = TriState::No;
    _view_map[x][y].is_visited = TriState::Yes;
    _view_map[x][y].is_unknown = TriState::No;
    res = Event::Arrived;
    event_handler(res);
    res = Event::Safe;
    event_handler(res);
    _game_map[x][y] = (int)ObjectType::PlayerType;
    for (int i = 0; i < _wumpus_count; ++i) {
      int wumpus_x = _wumpus[i].get_x();
      int wumpus_y = _wumpus[i].get_y();
      bool wumpus_dead = _wumpus[i].is_dead();
      if (x == wumpus_x && y == wumpus_y && !wumpus_dead) {
        res = Event::EatenByWumpus;
        event_handler(res);
        res = Event::Dead;
        _player->is_dead = true;
        event_handler(res);
        res = Event::GameOver;
        _is_over = true;
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
        res = Event::Dead;
        _player->is_dead = true;
        event_handler(res);
        res = Event::GameOver;
        _is_over = true;
        event_handler(res);
        break;
      }
    }
    smell(x, y);
  } else {
    event_handler(res);
  }
  return res;
}
// enum class Direction : int { Up = 0, Down = 2, Left = 1, Right = 3 };
void Game::set_direction(Direction new_direction) {
  int dx, dy;
  Direction old_direction = _player->get_direction(&dx, &dy);
  int times = abs((int)old_direction - (int)new_direction);
  for (int i = 0; i < times; ++i) {
    _player->turn_left();
  }
}

void Game::print_map() {
  cout << endl;
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
void Game::add_action_to_queue(Action action) {
  _action_queue.push_back(action);
}
Event Game::take_action(Action action, int *data) {
  if (action != Action::None)
    _action_queue.push_back(action);
  do {
    if (_action_queue.empty()) {
      return Event::InvalidAction;
    }
    action = _action_queue.front();
    _action_queue.pop_front();
  } while (action == Action::None);
  print_map();
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
      _is_over = true;
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
    res = move(bak_x, bak_y, x, y);
    break;
  case Action::Shoot:
    if (_player->get_arrow_count() > 0) {
      cout << "Shoot action" << endl;
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
    res = Event::Arrived;
    _player->turn_left();
    event_handler(res);
    break;
  case Action::TurnRight:
    res = Event::Arrived;
    _player->turn_right();
    event_handler(res);
    break;
  case Action::MoveTo:
    if (data) {
      int to_x = *data;
      data++;
      int to_y = *data;
      move(x, y, to_x, to_y);
    }
    res = Event::Arrived;
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
  move(0, 0, 0, 0);
}
// virtual void event_handler(Event event) = 0;
Block::Block()
    : is_unknown(TriState::Yes), is_visited(TriState::No),
      is_breeze(TriState::Unknown), is_stench(TriState::Unknown),
      is_gold(TriState::Unknown), is_wumpus(TriState::Unknown),
      is_pit(TriState::Unknown), is_safe(TriState::Unknown) {}

Block **Game::extract_map_info(bool debug) {
  Block **info = this->_view_map;
  if (!info)
    return nullptr;
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      info[i][j].is_stench = TriState::No;
      info[i][j].is_pit = TriState::No;
      info[i][j].is_gold = TriState::No;
      info[i][j].is_breeze = TriState::No;
      info[i][j].is_wumpus = TriState::No;
      info[i][j].is_player = TriState::No;
      if (info[i][j].is_visited == TriState::Yes) {
        info[i][j].is_unknown = TriState::No;
      } else {
        info[i][j].is_unknown = TriState::Yes;
      }
      if (debug) {
        info[i][j].is_unknown = TriState::No;
      }
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      switch (_game_map[i][j]) {
      case (int)ObjectType::None:
        info[i][j].is_pit = TriState::No;
        info[i][j].is_gold = TriState::No;
        info[i][j].is_wumpus = TriState::No;
        info[i][j].is_player = TriState::No;
        break;
      case (int)ObjectType::PitType:
        info[i][j].is_pit = TriState::Yes;
        if (i - 1 >= 0) {
          info[i - 1][j].is_breeze = TriState::Yes;
        }
        if (i + 1 < 4) {
          info[i + 1][j].is_breeze = TriState::Yes;
        }
        if (j - 1 >= 0) {
          info[i][j - 1].is_breeze = TriState::Yes;
        }
        if (j + 1 < 4) {
          info[i][j + 1].is_breeze = TriState::Yes;
        }
        break;
      case (int)ObjectType::PlayerType:
        info[i][j].is_player = TriState::Yes;
        break;
      case (int)ObjectType::WumpusType:
        info[i][j].is_wumpus = TriState::Yes;
        if (i - 1 >= 0) {
          info[i - 1][j].is_stench = TriState::Yes;
        }
        if (i + 1 < 4) {
          info[i + 1][j].is_stench = TriState::Yes;
        }
        if (j - 1 >= 0) {
          info[i][j - 1].is_stench = TriState::Yes;
        }
        if (j + 1 < 4) {
          info[i][j + 1].is_stench = TriState::Yes;
        }
        break;
      }
    }
  }
  if (_gold_x < 0 || _gold_y < 0) {
    return info;
  }
  if (_player->is_grab_gold()) {
    info[_gold_x][_gold_y].is_gold = TriState::No;
  } else {
    info[_gold_x][_gold_y].is_gold = TriState::Yes;
  }
  return info;
}
void Game::empty_map() {
  for (int i = 0; i < _height; ++i) {
    for (int j = 0; j < _width; ++j) {
      place_none(i, j);
    }
  }
}
void Game::place_none(int x, int y) {
  this->_game_map[x][y] = (int)ObjectType::None;
}
void Game::place_brave(int x, int y) {
  int origin_x = this->_player->get_x();
  int origin_y = this->_player->get_y();
  _game_map[origin_x][origin_y] = (int)ObjectType::None;
  _game_map[x][y] = (int)ObjectType::PlayerType;
  this->_player->move_to(x, y);
}
void Game::place_pit(int x, int y) {
  if (!_pit) {
    _pit = new Pit[_height * _width];
  }
  _pit[_pit_count].set_x(x);
  _pit[_pit_count].set_y(y);
  _pit_count++;
  this->_game_map[x][y] = (int)ObjectType::PitType;
}
void Game::place_wumpus(int x, int y) {
  if (!_wumpus) {
    _wumpus = new Wumpus[_height * _width];
  }
  _wumpus[_pit_count].set_x(x);
  _wumpus[_pit_count].set_y(y);
  _wumpus_count++;
  _player->set_arrow_count(_wumpus_count);
  _arrow_count = _wumpus_count;
  this->_game_map[x][y] = (int)ObjectType::WumpusType;
}
void Game::place_gold(int x, int y) {
  this->_gold_x = x;
  this->_gold_y = y;
}

int Game::get_height() { return _height; }
int Game::get_width() { return _width; }
