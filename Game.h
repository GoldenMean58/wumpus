#ifndef MY_GAME_H
#define MY_GAME_H
#include "Object.h"
#include "Pit.h"
#include "Player.h"
#include "Wumpus.h"
#include <complex>
#include <deque>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using std::cin;
using std::cout;
using std::deque;
using std::endl;

enum class Smell : int {
  None = 0,
  PitSmell = 1,
  WumpusSmell = 2,
  PitWumpusSmell = 3
};

enum class Action : int {
  TurnLeft = 0,
  TurnRight = 1,
  MoveForward = 2,
  Grab = 3,
  Shoot = 4,
  Leave = 5,
  Smell = 6,
  None = 7,
  MoveTo = 8,
};

enum class Event : int {
  None = 0,
  InvalidAction = 1,
  SmellWumpus = 2,
  KillWumpus = 3,
  ArrowCrashWall = 4,
  MeetGold = 5,
  Dead = 6,
  GoldGrabbed = 7,
  Leaved = 8,
  EatenByWumpus = 9,
  DropIntoPit = 10,
  SmellPit = 11,
  GameOver = 12,
  Arrived = 13,
  Safe = 14,
};
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

class Game {
protected:
  int **_game_map;
  int _width;
  int _height;
  Player *_player;
  Wumpus *_wumpus;
  int _wumpus_count;
  Pit *_pit;
  int _pit_count;
  int _gold_x;
  int _gold_y;
  bool _is_over;
  int _arrow_count;
  Block **_information_map;
  Block **_view_map;
  deque<Action> _action_queue;
  int DEFAULT_ARROW_COUNT;
  int DEFAULT_WIDTH;      // = 4;
  int DEFAULT_HEIGHT;     // = 4;
  int DEFAULT_PITS_COUNT; // = 3;
  void _init();

public:
  Game();
  ~Game();
  Game(int **map, int width, int height, int arrow_count, int pit_count);
  void generate_map(bool is_random_build);
  void generate_map(int width, int height);
  void destroy_map();
  void empty_map();
  int get_height();
  int get_width();
  void place_none(int x, int y);
  void place_brave(int x, int y);
  void place_pit(int x, int y);
  void place_wumpus(int x, int y);
  void place_gold(int x, int y);
  void print_map();
  void set_direction(Direction);
  bool is_over();
  void add_action_to_queue(Action action);
  Event take_action(Action action, int *data = nullptr);
  Event move(int from_x, int from_y, int x, int y);
  virtual void event_handler(Event event) = 0;
  Block **extract_map_info(bool debug = false);
  void smell(int x, int y);
  void start();
  int get_gold_x();
  int get_gold_y();
};

#endif
