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
  image_background.load(":/resource/image/background.png");
  QImage image_gold;
  image_gold.load(":/resource/image/gold.png");
  QImage image_breeze;
  image_breeze.load(":/resource/image/breeze.png");
  QImage image_stench;
  image_stench.load(":/resource/image/stench.png");
  QImage image_brave;
  image_brave.load(":/resource/image/Brave.png");
  qp.begin(this);
  for (int i = 0; i < 12; ++i) {
    for (int j = 0; j < 12; ++j) {
      qp.drawImage(32 * i, 32 * j, image_background);
    }
  }
  /*
  for(int i = 0; i <= 4; ++i) {
    qp.drawLine(i * 32 * 3, 0, )
  }
  */
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      if (information_map[i][j].is_unknown == TriState::Yes) {
        continue;
      }
      if (information_map[i][j].is_pit == TriState::Yes) {
        qp.drawImage(32 * 3 * j + 32, 32 * 3 * i + 32, image_pit);
      }
      if (information_map[i][j].is_wumpus == TriState::Yes) {
        qp.drawImage(32 * 3 * j + 32, 32 * 3 * i + 32, image_wumpus);
      }
      if (information_map[i][j].is_breeze == TriState::Yes) {
        qp.drawImage(32 * 3 * j, 32 * 3 * i, image_breeze);
      }
      if (information_map[i][j].is_stench == TriState::Yes) {
        qp.drawImage(32 * 3 * j + 32 + 32, 32 * 3 * i, image_stench);
      }
      if (information_map[i][j].is_gold == TriState::Yes) {
        qp.drawImage(32 * 3 * j + 32 + 32, 32 * 3 * i + 32, image_gold);
      }
      if (information_map[i][j].is_player == TriState::Yes && player) {
        QRectF target(32 * 3 * j + 32, 32 * 3 * i + 32, 32, 32);
        QRectF source;
        auto direction = player->get_direction(nullptr, nullptr);
        // Down
        // Left
        // Right
        // Up
        switch (direction) {
        case Direction::Down:
          source.setRect(0, 0, 32, 32);
          break;
        case Direction::Left:
          source.setRect(0, 32, 32, 32);
          break;
        case Direction::Right:
          source.setRect(0, 64, 32, 32);
          break;
        case Direction::Up:
          source.setRect(0, 96, 32, 32);
          break;
        }
        qp.drawImage(target, image_brave, source);
      }
    }
  }
  qp.end();
}

void DrawBoard::set_map(Block **information_map) {
  this->information_map = information_map;
}

void DrawBoard::set_player(Player *player) { this->player = player; }
