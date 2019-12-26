#ifndef GAMEQT_H
#define GAMEQT_H
#include "AI.h"
class GameQt : public Database {
public:
  GameQt();
  void event_handler(Event event) override;
  int **get_map();
  Player *get_player();
};

#endif
