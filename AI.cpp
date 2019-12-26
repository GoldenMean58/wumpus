#include "AI.h"
bool Database::_moveto(int goal_x, int goal_y) {
  /*
   * Find a safe path to (x, y) and move there by dfs
   * if there is no safe path, then return false, else true.
   */
  int origin_x = _player->get_x();
  int origin_y = _player->get_y();
  if (origin_x == goal_x && origin_y == goal_y)
    return true;
  int path[_height * _width];
  bool visited[_height * _width];
  for (int i = 0; i < _height; ++i) {
    for (int j = 0; j < _width; ++j) {
      visited[i * _width + j] = false;
    }
  }
  deque<int> path_stack;
  path_stack.push_back(origin_x * _width + origin_y);
  visited[origin_x * _width + origin_y] = true;
  path[origin_x * _width + origin_y] = -1;
  while (!path_stack.empty()) {
    int index = path_stack.back();
    path_stack.pop_back();
    int x = index / _width;
    int y = index % _width;
    if (x == goal_x && y == goal_y) {
      vector<int> res;
      int tmp_x = goal_x;
      int tmp_y = goal_y;
      res.push_back(goal_x * _width + goal_y);
      do {
        int next_x = path[tmp_x * _width + tmp_y] / _width;
        int next_y = path[tmp_x * _width + tmp_y] % _width;
        res.push_back(next_x * _width + next_y);
        tmp_x = next_x;
        tmp_y = next_y;
      } while (path[tmp_x * _width + tmp_y] != -1);
      std::reverse(res.begin(), res.end());
      for (int i = 0; i < (int)res.size() - 1; ++i) {
        int from_index = res[i];
        int to_index = res[i + 1];
        int from_x = from_index / _width;
        int from_y = from_index % _width;
        int to_x = to_index / _width;
        int to_y = to_index % _width;
        if (from_x < to_x) {
          set_direction(Direction::Down);
        } else if (from_x > to_x) {
          set_direction(Direction::Up);
        } else {
          if (from_y < to_y) {
            set_direction(Direction::Right);
          } else if (from_y > to_y) {
            set_direction(Direction::Left);
          }
        }
        take_action(Action::MoveForward);
      }
      return true;
    }
    int up_x = x - 1;
    int up_y = y;
    int down_x = x + 1;
    int down_y = y;
    int left_x = x;
    int left_y = y - 1;
    int right_x = x;
    int right_y = y + 1;
    if (up_x >= 0 && visited[up_x * _width + up_y] == false &&
        (_information_map[up_x][up_y].is_safe == TriState::Yes ||
         (up_x == goal_x && up_y == goal_y))) {
      path_stack.push_back(up_x * _width + up_y);
      visited[up_x * _width + up_y] = true;
      path[up_x * _width + up_y] = x * _width + y;
    }
    if (down_x < _height && visited[down_x * _width + down_y] == false &&
        (_information_map[down_x][down_y].is_safe == TriState::Yes ||
         (down_x == goal_x && down_y == goal_y))) {
      path_stack.push_back(down_x * _width + down_y);
      visited[down_x * _width + down_y] = true;
      path[down_x * _width + down_y] = x * _width + y;
    }
    if (left_y >= 0 && visited[left_x * _width + left_y] == false &&
        (_information_map[left_x][left_y].is_safe == TriState::Yes ||
         (left_x == goal_x && left_y == goal_y))) {
      path_stack.push_back(left_x * _width + left_y);
      visited[left_x * _width + left_y] = true;
      path[left_x * _width + left_y] = x * _width + y;
    }
    if (right_y < _width && visited[right_x * _width + right_y] == false &&
        (_information_map[right_x][right_y].is_safe == TriState::Yes ||
         (right_x == goal_x && right_y == goal_y))) {
      path_stack.push_back(right_x * _width + right_y);
      visited[right_x * _width + right_y] = true;
      path[right_x * _width + right_y] = x * _width + y;
    }
  }
  return false;
}
void Database::event_handler(Event event) {
  _tell(event);
  switch (event) {
  case Event::Arrived:
    break;
  case Event::None:
    break;
  case Event::ArrowCrashWall:
    break;
  case Event::Dead:
    break;
  case Event::DropIntoPit:
    break;
  case Event::EatenByWumpus:
    break;
  case Event::GameOver:
    break;
  case Event::GoldGrabbed:
    break;
  case Event::InvalidAction:
    break;
  case Event::KillWumpus:
    break;
  case Event::Leaved:
    break;
  case Event::MeetGold:
    break;
  case Event::SmellPit:
    break;
  case Event::SmellWumpus:
    break;
  }
}
void Database::_tell(Event event) {
  int x = _player->get_x();
  int y = _player->get_y();
  int up_x = x - 1;
  int up_y = y;
  int down_x = x + 1;
  int down_y = y;
  int left_x = x;
  int left_y = y - 1;
  int right_x = x;
  int right_y = y + 1;

  int dx, dy;
  _player->get_direction(&dx, &dy);
  switch (event) {
  case Event::Arrived:
    this->_information_map[x][y].is_unknown = TriState::No;
    this->_information_map[x][y].is_safe = TriState::Yes;
    this->_information_map[x][y].is_visited = TriState::Yes;
    this->_information_map[x][y].is_pit = TriState::No;
    this->_information_map[x][y].is_wumpus = TriState::No;
    break;
  case Event::None:
    this->_information_map[x][y].is_unknown = TriState::No;
    this->_information_map[x][y].is_safe = TriState::Yes;
    this->_information_map[x][y].is_visited = TriState::Yes;
    this->_information_map[x][y].is_breeze = TriState::No;
    this->_information_map[x][y].is_stench = TriState::No;
    this->_information_map[x][y].is_pit = TriState::No;
    this->_information_map[x][y].is_wumpus = TriState::No;
    this->_information_map[x][y].is_gold = TriState::No;
    break;
  case Event::ArrowCrashWall:
    while (x >= 0 && x < _height && y >= 0 && y < _width) {
      this->_information_map[x][y].is_wumpus = TriState::No;
      x += dx;
      y += dy;
    }
    break;
  case Event::Dead:
    // TODO
    break;
  case Event::DropIntoPit:
    this->_information_map[x][y].is_pit = TriState::Yes;
    break;
  case Event::EatenByWumpus:
    this->_information_map[x][y].is_wumpus = TriState::Yes;
    break;
  case Event::GameOver:
    break;
  case Event::GoldGrabbed:
    this->_information_map[x][y].is_gold = TriState::No;
    break;
  case Event::InvalidAction:
    break;
  case Event::KillWumpus:
    // TODO: consider wumpus count case > 1 and case = 1
    if (_arrow_count == 1) {
      int tmp_x = _player->get_x();
      int tmp_y = _player->get_y();
      _player->get_direction(&dx, &dy);
      while (!(tmp_x < 0 || tmp_x >= _height || tmp_y < 0 || tmp_y >= _width)) {
        _information_map[tmp_x][tmp_y].is_wumpus = TriState::No;
        tmp_x += dx;
        tmp_y += dy;
      }
    }
    break;
  case Event::Leaved:
    break;
  case Event::MeetGold:
    this->_information_map[x][y].is_gold = TriState::Yes;
    break;
  case Event::SmellPit:
    this->_information_map[x][y].is_breeze = TriState::Yes;
    if (up_x >= 0 && _information_map[up_x][up_y].is_pit == TriState::Unknown &&
        _information_map[up_x][up_y].is_safe == TriState::Unknown) {
      _information_map[up_x][up_y].is_pit = TriState::Maybe;
    }
    if (down_x < _height &&
        _information_map[down_x][down_y].is_pit == TriState::Unknown &&
        _information_map[down_x][down_y].is_safe == TriState::Unknown) {
      _information_map[down_x][down_y].is_pit = TriState::Maybe;
    }
    if (left_y >= 0 &&
        _information_map[left_x][left_y].is_pit == TriState::Unknown &&
        _information_map[left_x][left_y].is_safe == TriState::Unknown) {
      _information_map[left_x][left_y].is_pit = TriState::Maybe;
    }
    if (left_y < _width &&
        _information_map[right_x][right_y].is_pit == TriState::Unknown &&
        _information_map[right_x][right_y].is_safe == TriState::Unknown) {
      _information_map[right_x][right_y].is_pit = TriState::Maybe;
    }
    break;
  case Event::SmellWumpus:
    this->_information_map[x][y].is_stench = TriState::Yes;
    if (up_x >= 0 &&
        _information_map[up_x][up_y].is_wumpus == TriState::Unknown &&
        _information_map[up_x][up_y].is_safe == TriState::Unknown) {
      _information_map[up_x][up_y].is_wumpus = TriState::Maybe;
    }
    if (down_x < _height &&
        _information_map[down_x][down_y].is_wumpus == TriState::Unknown &&
        _information_map[down_x][down_y].is_safe == TriState::Unknown) {
      _information_map[down_x][down_y].is_wumpus = TriState::Maybe;
    }
    if (left_y >= 0 &&
        _information_map[left_x][left_y].is_wumpus == TriState::Unknown &&
        _information_map[left_x][left_y].is_safe == TriState::Unknown) {
      _information_map[left_x][left_y].is_wumpus = TriState::Maybe;
    }
    if (left_y < _width &&
        _information_map[right_x][right_y].is_wumpus == TriState::Unknown &&
        _information_map[right_x][right_y].is_safe == TriState::Unknown) {
      _information_map[right_x][right_y].is_wumpus = TriState::Maybe;
    }
    break;
  }
  for (int x = 0; x < _height; ++x) {
    for (int y = 0; y < _width; ++y) {
      /*
      if (this->_information_map[x][y].is_visited != TriState::Yes &&
          this->_information_map[x][y].is_safe != TriState::Yes &&
          _information_map[x][y].is_wumpus != TriState::Yes &&
          _information_map[x][y].is_pit != TriState::Yes) {
          */
      int up_x = x - 1;
      int up_y = y;
      int down_x = x + 1;
      int down_y = y;
      int left_x = x;
      int left_y = y - 1;
      int right_x = x;
      int right_y = y + 1;
      if ((up_x < 0 ||
           _information_map[up_x][up_y].is_stench == TriState::Yes) &&
          (down_x >= _height ||
           _information_map[down_x][down_y].is_stench == TriState::Yes) &&
          (left_y < 0 ||
           _information_map[left_x][left_y].is_stench == TriState::Yes) &&
          (right_y >= _width ||
           _information_map[right_x][right_y].is_stench == TriState::Yes) &&
          _information_map[x][y].is_safe != TriState::Yes) {
        this->_information_map[x][y].is_wumpus = TriState::Yes;
        this->_information_map[x][y].is_safe = TriState::No;
        this->_information_map[x][y].is_unknown = TriState::No;
      }
      if ((up_x < 0 ||
           _information_map[up_x][up_y].is_breeze == TriState::Yes) &&
          (down_x >= _height ||
           _information_map[down_x][down_y].is_breeze == TriState::Yes) &&
          (left_y < 0 ||
           _information_map[left_x][left_y].is_breeze == TriState::Yes) &&
          (right_y >= _width ||
           _information_map[right_x][right_y].is_breeze == TriState::Yes) &&
          _information_map[x][y].is_safe != TriState::Yes) {
        this->_information_map[x][y].is_pit = TriState::Yes;
        this->_information_map[x][y].is_safe = TriState::No;
        this->_information_map[x][y].is_unknown = TriState::No;
      }
      if (_information_map[x][y].is_breeze == TriState::No) {
        if (up_x >= 0) {
          _information_map[up_x][up_y].is_pit = TriState::No;
          _information_map[up_x][up_y].is_unknown = TriState::No;
        }
        if (down_x < _height) {
          _information_map[down_x][down_y].is_pit = TriState::No;
          _information_map[down_x][down_y].is_unknown = TriState::No;
        }
        if (left_y >= 0) {
          _information_map[left_x][left_y].is_pit = TriState::No;
          _information_map[left_x][left_y].is_unknown = TriState::No;
        }
        if (right_y < _width) {
          _information_map[right_x][right_y].is_pit = TriState::No;
          _information_map[right_x][right_y].is_unknown = TriState::No;
        }
      }
      if (_information_map[x][y].is_stench == TriState::No) {
        if (up_x >= 0) {
          _information_map[up_x][up_y].is_wumpus = TriState::No;
          _information_map[up_x][up_y].is_unknown = TriState::No;
        }
        if (down_x < _height) {
          _information_map[down_x][down_y].is_wumpus = TriState::No;
          _information_map[down_x][down_y].is_unknown = TriState::No;
        }
        if (left_y >= 0) {
          _information_map[left_x][left_y].is_wumpus = TriState::No;
          _information_map[left_x][left_y].is_unknown = TriState::No;
        }
        if (right_y < _width) {
          _information_map[right_x][right_y].is_wumpus = TriState::No;
          _information_map[right_x][right_y].is_unknown = TriState::No;
        }
      }
      if (_information_map[x][y].is_breeze == TriState::Yes) {
        // Up is pit, and left not exists or not a pit as right and down is.
        if ((down_x >= _height ||
             _information_map[down_x][down_y].is_pit == TriState::No) &&
            (left_y < 0 ||
             _information_map[left_x][left_y].is_pit == TriState::No) &&
            (right_y >= _width ||
             _information_map[right_x][right_y].is_pit == TriState::No)) {
          if (up_x >= 0 &&
              _information_map[up_x][up_y].is_safe != TriState::Yes) {
            _information_map[up_x][up_y].is_pit = TriState::Yes;
            _information_map[up_x][up_y].is_safe = TriState::No;
            _information_map[up_x][up_y].is_unknown = TriState::No;
          }
        }
        // Down is pit
        if ((up_x < 0 || _information_map[up_x][up_y].is_pit == TriState::No) &&
            (left_y < 0 ||
             _information_map[left_x][left_y].is_pit == TriState::No) &&
            (right_y >= _width ||
             _information_map[right_x][right_y].is_pit == TriState::No)) {
          if (down_x < _height &&
              _information_map[down_x][down_y].is_safe != TriState::Yes) {
            _information_map[down_x][down_y].is_pit = TriState::Yes;
            _information_map[down_x][down_y].is_safe = TriState::No;
            _information_map[down_x][down_y].is_unknown = TriState::No;
          }
        }
        // Left is pit
        if ((up_x < 0 || _information_map[up_x][up_y].is_pit == TriState::No) &&
            (down_x >= _height ||
             _information_map[down_x][down_y].is_pit == TriState::No) &&
            (right_y >= _width ||
             _information_map[right_x][right_y].is_pit == TriState::No)) {
          if (left_y >= 0 &&
              _information_map[left_x][left_y].is_safe != TriState::Yes) {
            _information_map[left_x][left_y].is_pit = TriState::Yes;
            _information_map[left_x][left_y].is_safe = TriState::No;
            _information_map[left_x][left_y].is_unknown = TriState::No;
          }
        }
        // Right is pit
        if ((up_x < 0 || _information_map[up_x][up_y].is_pit == TriState::No) &&
            (down_x >= _height ||
             _information_map[down_x][down_y].is_pit == TriState::No) &&
            (left_y < 0 ||
             _information_map[left_x][left_y].is_pit == TriState::No)) {
          if (right_y < _width &&
              _information_map[right_x][right_y].is_safe != TriState::Yes) {
            _information_map[right_x][right_y].is_pit = TriState::Yes;
            _information_map[right_x][right_y].is_safe = TriState::No;
            _information_map[right_x][right_y].is_unknown = TriState::No;
          }
        }
      }
      if (_information_map[x][y].is_stench == TriState::Yes) {
        if ((down_x >= _height ||
             _information_map[down_x][down_y].is_wumpus == TriState::No) &&
            (left_y < 0 ||
             _information_map[left_x][left_y].is_wumpus == TriState::No) &&
            (right_y >= _width ||
             _information_map[right_x][right_y].is_wumpus == TriState::No)) {
          if (up_x >= 0 &&
              _information_map[up_x][up_y].is_safe != TriState::Yes) {
            _information_map[up_x][up_y].is_wumpus = TriState::Yes;
            _information_map[up_x][up_y].is_safe = TriState::No;
            _information_map[up_x][up_y].is_unknown = TriState::No;
          }
        }
        if ((up_x < 0 ||
             _information_map[up_x][up_y].is_wumpus == TriState::No) &&
            (left_y < 0 ||
             _information_map[left_x][left_y].is_wumpus == TriState::No) &&
            (right_y >= _width ||
             _information_map[right_x][right_y].is_wumpus == TriState::No)) {
          if (down_x < _height &&
              _information_map[down_x][down_y].is_safe != TriState::Yes) {
            _information_map[down_x][down_y].is_wumpus = TriState::Yes;
            _information_map[down_x][down_y].is_safe = TriState::No;
            _information_map[down_x][down_y].is_unknown = TriState::No;
          }
        }
        if ((up_x < 0 ||
             _information_map[up_x][up_y].is_wumpus == TriState::No) &&
            (down_x >= _height ||
             _information_map[down_x][down_y].is_wumpus == TriState::No) &&
            (right_y >= _width ||
             _information_map[right_x][right_y].is_wumpus == TriState::No)) {
          if (left_y >= 0 &&
              _information_map[left_x][left_y].is_safe != TriState::Yes) {
            _information_map[left_x][left_y].is_wumpus = TriState::Yes;
            _information_map[left_x][left_y].is_safe = TriState::No;
            _information_map[left_x][left_y].is_unknown = TriState::No;
          }
        }
        if ((up_x < 0 ||
             _information_map[up_x][up_y].is_wumpus == TriState::No) &&
            (down_x >= _height ||
             _information_map[down_x][down_y].is_wumpus == TriState::No) &&
            (left_y < 0 ||
             _information_map[left_x][left_y].is_wumpus == TriState::No)) {
          if (right_y < _width &&
              _information_map[right_x][right_y].is_safe != TriState::Yes) {
            _information_map[right_x][right_y].is_wumpus = TriState::Yes;
            _information_map[right_x][right_y].is_safe = TriState::No;
            _information_map[right_x][right_y].is_unknown = TriState::No;
          }
        }
      }
      // }
    }
  }
  for (int x = 0; x < _height; ++x) {
    for (int y = 0; y < _width; ++y) {
      if (_information_map[x][y].is_wumpus == TriState::No &&
          _information_map[x][y].is_pit == TriState::No) {
        this->_information_map[x][y].is_safe = TriState::Yes;
        this->_information_map[x][y].is_unknown = TriState::No;
      }
    }
  }
  // Debug
  bool debug = false;
  if (debug)
    for (int i = 0; i < _height; ++i) {
      for (int j = 0; j < _width; ++j) {
        if (_information_map[i][j].is_unknown == TriState::Yes)
          continue;
        cout << "Block(" << i << ", " << j << "):" << endl;
        if (_information_map[i][j].is_unknown == TriState::Yes) {
          cout << "\tis_unknown = Yes" << endl;
        } else if (_information_map[i][j].is_unknown == TriState::No) {
          cout << "\tis_unknown = No" << endl;
        }
        if (_information_map[i][j].is_visited == TriState::Yes) {
          cout << "\tis_visited = Yes" << endl;
        } else if (_information_map[i][j].is_visited == TriState::No) {
          cout << "\tis_visited = No" << endl;
        }
        if (_information_map[i][j].is_breeze == TriState::Yes) {
          cout << "\tis_breeze = Yes" << endl;
        } else if (_information_map[i][j].is_breeze == TriState::No) {
          cout << "\tis_breeze = No" << endl;
        }
        if (_information_map[i][j].is_stench == TriState::Yes) {
          cout << "\tis_stench = Yes" << endl;
        } else if (_information_map[i][j].is_stench == TriState::No) {
          cout << "\tis_stench = No" << endl;
        }
        if (_information_map[i][j].is_gold == TriState::Yes) {
          cout << "\tis_gold = Yes" << endl;
        } else if (_information_map[i][j].is_gold == TriState::No) {
          cout << "\tis_gold = No" << endl;
        }
        if (_information_map[i][j].is_wumpus == TriState::Yes) {
          cout << "\tis_wumpus = Yes" << endl;
        } else if (_information_map[i][j].is_wumpus == TriState::No) {
          cout << "\tis_wumpus = No" << endl;
        }
        if (_information_map[i][j].is_pit == TriState::Yes) {
          cout << "\tis_pit = Yes" << endl;
        } else if (_information_map[i][j].is_pit == TriState::No) {
          cout << "\tis_pit = No" << endl;
        }
        if (_information_map[i][j].is_safe == TriState::Yes) {
          cout << "\tis_safe = Yes" << endl;
        } else if (_information_map[i][j].is_safe == TriState::No) {
          cout << "\tis_safe = No" << endl;
        }
      }
    }
}
/*
void Database::print_map() {
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
*/

Action Database::ask(int *data) {
  int now_x = _player->get_x();
  int now_y = _player->get_y();
  if (_information_map[now_x][now_y].is_gold == TriState::Yes) {
    return Action::Grab;
  }
  if (_player->is_grab_gold()) {
    if (now_x == 0 && now_y == 0) {
      return Action::Leave;
    } else {
      /*
      _moveto(0, 0);
      return Action::None; // FIXME: need a new NOP Action?
      */
      if (_moveto(0, 0)) {
        *data = 0;
        data++;
        *data = 0;
        return Action::MoveTo;
      }
      /*
      *data = 0;
      data++;
      *data = 0;
      return Action::MoveTo;
      */
    }
  }
  for (int x = 0; x < _height; ++x) {
    for (int y = 0; y < _width; ++y) {
      if (_information_map[x][y].is_safe == TriState::Yes &&
          _information_map[x][y].is_visited == TriState::No) {
        /*
        _moveto(x, y);
        return Action::None;
        */
        if (_moveto(x, y)) {
          *data = x;
          data++;
          *data = y;
          return Action::MoveTo;
        }
      }
    }
  }
  int player_x = _player->get_x();
  int player_y = _player->get_y();
  if (_player->get_arrow_count() > 0) {
    for (int x = 0; x < _height; ++x) {
      for (int y = 0; y < _width; ++y) {
        if (_information_map[x][y].is_wumpus == TriState::Yes) {
          // Shoot
          for (int now_x = player_x; now_x < _height; ++now_x) {
            int xx = now_x % _height;
            if (_information_map[xx][y].is_safe == TriState::Yes) {
              move(player_x, player_y, xx, y);
              auto direction = x < xx ? Direction::Up : Direction::Down;
              set_direction(direction);
              // cout << "Shoot (" << x << ", " << y << ")" << endl;
              if (_player->get_arrow_count() == 0)
                exit(0);
              return Action::Shoot;
            }
          }
          for (int now_y = player_y; now_y < _width; ++now_y) {
            int yy = now_y % _width;
            if (_information_map[x][yy].is_safe == TriState::Yes) {
              int player_x = _player->get_x();
              int player_y = _player->get_y();
              move(player_x, player_y, x, yy);
              auto direction = y < yy ? Direction::Left : Direction::Right;
              set_direction(direction);
              // cout << "Shoot (" << x << ", " << y << ")" << endl;
              if (_player->get_arrow_count() == 0)
                exit(0);
              return Action::Shoot;
            }
          }
        }
      }
    }
  }
  if (_player->get_arrow_count() > 0) {
    for (int x = 0; x < _height; ++x) {
      for (int y = 0; y < _width; ++y) {
        if (_information_map[x][y].is_wumpus == TriState::Maybe &&
            _information_map[x][y].is_safe != TriState::Yes) {
          // Shoot
          for (int now_x = player_x; now_x < _height + player_x; ++now_x) {
            int xx = now_x % _height;
            if (_information_map[xx][y].is_safe == TriState::Yes) {
              move(player_x, player_y, xx, y);
              auto direction = x < xx ? Direction::Up : Direction::Down;
              set_direction(direction);
              // cout << "Shoot (" << x << ", " << y << ") maybe" << endl;
              if (_player->get_arrow_count() == 0)
                exit(0);
              return Action::Shoot;
            }
          }
          for (int now_y = player_y; now_y < _width + player_y; ++now_y) {
            int yy = now_y % _width;
            if (_information_map[x][yy].is_safe == TriState::Yes) {
              move(player_x, player_y, x, yy);
              auto direction = y < yy ? Direction::Left : Direction::Right;
              set_direction(direction);
              // cout << "Shoot (" << x << ", " << y << ") maybe" << endl;
              if (_player->get_arrow_count() == 0)
                exit(0);
              return Action::Shoot;
            }
          }
        }
      }
    }
  }
  for (int x = 0; x < _height; ++x) {
    for (int y = 0; y < _width; ++y) {
      if (_information_map[x][y].is_safe == TriState::Unknown &&
          _information_map[x][y].is_visited == TriState::No) {
        /*
        _moveto(x, y);
        return Action::None;
        */
        if (_moveto(x, y)) {
          *data = x;
          data++;
          *data = y;
          return Action::MoveTo;
        }
        /*
        *data = x;
        data++;
        *data = y;
        return Action::MoveTo;
        */
      }
    }
  }
  cout << "Leave" << endl;
  return Action::Leave;
}

Database::Database() : Game() {}
Database::Database(int **game_map, int height, int width, int arrow_count,
                   int pit_count)
    : Game(game_map, width, height, arrow_count, pit_count) {
}

Database::~Database() {

}
