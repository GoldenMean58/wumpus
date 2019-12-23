#ifndef MY_GAME_H
#define MY_GAME_H
#include "Object.h"
#include "Pit.h"
#include "Player.h"
#include "Wumpus.h"
#include <complex>
#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <time.h>
using std::cin;
using std::cout;
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
  int DEFAULT_ARROW_COUNT;
  int DEFAULT_WIDTH;      // = 4;
  int DEFAULT_HEIGHT;     // = 4;
  int DEFAULT_PITS_COUNT; // = 3;

public:
  Game();
  ~Game();
  Game(int width, int height, int arrow_count, int pit_count);
  void generate_map();
  void generate_map(int width, int height);
  void destroy_map();
  Event take_action(Action action);
  virtual void event_handler(Event event) = 0;
  void smell(int x, int y);
  void start();
};

#endif
