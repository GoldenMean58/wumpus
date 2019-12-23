#include "Wumpus.h"

Wumpus::Wumpus() : _dead(false) { this->_type = ObjectType::WumpusType; }
Wumpus::Wumpus(int x, int y) : _dead(false) {
  this->_x = x;
  this->_y = y;
  this->_type = ObjectType::WumpusType;
}
bool Wumpus::is_dead() { return this->_dead; }
void Wumpus::kill() { this->_dead = true; }
