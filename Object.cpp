#include "Object.h"

int Object::get_x() { return this->_x; }
int Object::get_y() { return this->_y; }
ObjectType Object::get_type() { return this->_type; }
void Object::set_x(int x) { this->_x = x; }
void Object::set_y(int y) { this->_y = y; }
