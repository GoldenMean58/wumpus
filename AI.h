#ifndef AI_H
#define AI_H
#include "Game.h"
#include <algorithm>
#include <deque>
#include <vector>
using std::deque;
using std::vector;

class Database : public Game {
private:
  int _moveto(int x, int y);

protected:
  void _tell(Event event);

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
