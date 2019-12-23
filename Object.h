#ifndef MY_OBJECT_H
#define MY_OBJECT_H
enum class ObjectType : int {
  None = 0,
  PlayerType = 1,
  PitType = 2,
  WumpusType = 3
};

class Object {
protected:
  int _x;
  int _y;
  ObjectType _type;

public:
  int get_x();
  int get_y();
  void set_x(int x);
  void set_y(int y);
  ObjectType get_type();
};

#endif
