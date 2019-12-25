#include "drawboard.h"

DrawBoard::DrawBoard(QWidget *parent)
    : QWidget(parent), information_map(nullptr) {
  this->setMinimumSize(4 * 3 * UNIT_SIZE, 4 * 3 * UNIT_SIZE);
}
/*
enum class ObjectType : int {
  None = 0,
  PlayerType = 1,
  PitType = 2,
  WumpusType = 3
};
*/

void DrawBoard::paintEvent(QPaintEvent *) {
  if (information_map == nullptr)
    return;
  QPainter qp;
  QImage image_pit;
  image_pit.load(":/resource/image/pit.png");
  QImage image_wumpus;
  image_wumpus.load(":/resource/image/wumpus.png");
  QImage image_background;
  image_background.load(":/resource/image/gold.png");
  QImage image_gold;
  image_gold.load(":/resource/image/gold.png");
  QImage image_breeze;
  image_breeze.load(":/resource/image/breeze.png");
  QImage image_stench;
  image_stench.load(":/resource/image/stench.png");
  qp.begin(this);
  for (int i = 0; i < 12; ++i) {
    for (int j = 0; j < 12; ++j) {
      qp.drawImage(32 * i, 32 * j, image_background);
    }
  }
  /*
  for(int i = 0; i <= 4; ++i) {
    qp.drawLine();
  }
  */
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (information_map[i][j].is_pit == TriState::Yes) {
        qp.drawImage(32 * 3 * i + 32, 32 * 3 * j + 32, image_pit);
      }
      if (information_map[i][j].is_wumpus == TriState::Yes) {
        qp.drawImage(32 * 3 * i + 32, 32 * 3 * j + 32, image_wumpus);
      }
      if (information_map[i][j].is_breeze == TriState::Yes) {
        qp.drawImage(32 * 3 * i, 32 * 3 * j, image_breeze);
      }
      if (information_map[i][j].is_stench == TriState::Yes) {
        qp.drawImage(32 * 3 * i + 32 + 32, 32 * 3 * j, image_stench);
      }
      if (information_map[i][j].is_gold == TriState::Yes) {
        qp.drawImage(32 * 3 * i + 32 + 32, 32 * 3 * j + 32, image_gold);
      }
      if (information_map[i][j].is_player == TriState::Yes) {
        // qp.drawImage(32 * 3 * i + 32 + 32, 32 * 3 * j + 32, image_gold);
      }
    }
  }
  qp.end();
}

void DrawBoard::set_map(Block **information_map) {
  this->information_map = information_map;
}
