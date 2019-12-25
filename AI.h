#ifndef AI_H
#define AI_H
#include "Game.h"
#include <algorithm>
#include <deque>
#include <vector>
using std::deque;
using std::vector;

enum class TriState : int {
  Unknown = 0,
  Yes = 1,
  No = 2,
  Maybe = 3,
};
class Block {
public:
  TriState is_unknown;
  TriState is_visited;
  TriState is_breeze;
  TriState is_stench;
  TriState is_gold;
  TriState is_wumpus;
  TriState is_pit;
  TriState is_safe;
  TriState is_player;
  Block();
};

class Database : public Game {
private:
  Block **_origin_map;
  Block **_information_map;
  void _tell(Event event);
  bool _moveto(int x, int y);

public:
  // Update
  void event_handler(Event event) override; // Tell
  // void print_map();
  // Ask
  Action ask(int *data);
  Database();
  Database(int **game_map, int height, int width, int arrow_count,
           int pit_count);
  ~Database();
};

/*
class AI{
private:
  int _x;
  int _y;
public:
  void get_next_action();
};
*/

#endif
