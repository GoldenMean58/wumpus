#ifndef MY_WUMPUS_H
#define MY_WUMPUS_H
#include "Object.h"
class Wumpus : public Object {
private:
  bool _dead;

public:
  Wumpus();
  Wumpus(int x, int y);
  bool is_dead();
  void kill();
};
#endif
