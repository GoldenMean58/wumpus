#include "Player.h"

Direction Player::get_direction(int *dx, int *dy) {
  if (dx) {
    *dx = _dx;
  }
  if (dy) {
    *dy = _dy;
  }
  if (this->_dx == -1)
    return Direction::Up;
  if (this->_dx == 1)
    return Direction::Down;
  if (this->_dy == -1)
    return Direction::Left;
  return Direction::Right;
}
void Player::set_direction(Direction new_direction) {
  switch (new_direction) {
  case Direction::Up:
    this->_dx = -1;
    this->_dy = 0;
    break;
  case Direction::Down:
    this->_dx = 1;
    this->_dy = 0;
    break;
  case Direction::Left:
    this->_dy = -1;
    this->_dx = 0;
    break;
  case Direction::Right:
    this->_dy = 1;
    this->_dx = 0;
    break;
  }
}
int Player::get_arrow_count() { return this->_arrow_count; }
int Player::get_action_count() { return this->_action_count; }
bool Player::is_grab_gold() { return this->_is_grab_gold; }
int Player::get_state() { return this->_state; }
void Player::turn_left() {
  int dx, dy;
  Direction old = get_direction(&dx, &dy);
  switch (old) {
  case Direction::Up:
    set_direction(Direction::Left);
    break;
  case Direction::Down:
    set_direction(Direction::Right);
    break;
  case Direction::Left:
    set_direction(Direction::Down);
    break;
  case Direction::Right:
    set_direction(Direction::Up);
    break;
  }
}
void Player::turn_right() {
  int dx, dy;
  Direction old = get_direction(&dx, &dy);
  switch (old) {
  case Direction::Up:
    set_direction(Direction::Right);
    break;
  case Direction::Down:
    set_direction(Direction::Left);
    break;
  case Direction::Left:
    set_direction(Direction::Up);
    break;
  case Direction::Right:
    set_direction(Direction::Down);
    break;
  }
}
int Player::move() {
  this->_x += this->_dx;
  this->_y += this->_dy;
  return 0;
}
int Player::move_to(int x, int y) {
  this->_x = x;
  this->_y = y;
  return 0;
}
int Player::shoot() {
  this->_arrow_count--;
  return 0;
}
int Player::grab() {
  this->_is_grab_gold = true;
  return 0;
}
void Player::init() {
  _x = 0;
  _y = 0;
  _dx = 1;
  _dy = 0;
  _is_grab_gold = false;
  _action_count = 0;
  is_dead = false;
}
Player::Player() : _arrow_count(1) { init(); }
Player::Player(int arrow_count) : _arrow_count(arrow_count) { init(); }
void Player::grab_gold() { _is_grab_gold = true; }
void Player::add_action_count() { _action_count++; }
// Smell Player::smell() {}
