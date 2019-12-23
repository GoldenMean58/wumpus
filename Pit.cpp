#include "Pit.h"

Pit::Pit() { this->_type = ObjectType::PitType; }
Pit::Pit(int x, int y) {
  this->_x = x;
  this->_y = y;
  this->_type = ObjectType::PitType;
}
