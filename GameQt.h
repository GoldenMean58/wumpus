#ifndef GAMEQT_H
#define GAMEQT_H
#include "AI.h"
class GameQt : public Game {
public:
  GameQt();
  void event_handler(Event event) override;
  int **get_map();
};

#endif
