#ifndef DRAWBOARD_H
#define DRAWBOARD_H

#include "AI.h"
#include "Object.h"
#include <QPainter>
#include <QWidget>

constexpr int UNIT_SIZE = 32;

class DrawBoard : public QWidget {
  Q_OBJECT
public:
  explicit DrawBoard(QWidget *parent = nullptr);
  virtual void paintEvent(QPaintEvent *);
  void set_map(Block **map);
  void set_player(Player *player);
  Block **information_map;
  Player *player;
signals:

public slots:
};

#endif // DRAWBOARD_H
