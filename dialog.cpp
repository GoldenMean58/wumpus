#include "dialog.h"
#include "ui_dialog.h"

Dialog::Dialog(GameQt *game, QWidget *parent) :
    QDialog(parent), game(game), ok(false),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    db = new DrawBoard();
    game->empty_map();
    game->place_gold(-1, -1);
    db->set_map(game->extract_map_info(true));
    db->set_player(game->get_player());
    QComboBox* place_combo_box = new QComboBox();
    place_combo_box->addItem("空");
    place_combo_box->addItem("勇者");
    place_combo_box->addItem("陷阱");
    place_combo_box->addItem("Wumpus");
    place_combo_box->addItem("宝箱");
    QPushButton *ok_btn = new QPushButton("完成");
    QPushButton *cancel_btn = new QPushButton("取消");
    connect(place_combo_box, SIGNAL(currentIndexChanged(int)), this, SLOT(combo_index_changed(int)));
    connect(ok_btn, SIGNAL(clicked(bool)), this, SLOT(ok_btn_clicked(bool)));
    connect(cancel_btn, SIGNAL(clicked(bool)), this, SLOT(cancel_btn_clicked(bool)));
    layout = new QVBoxLayout();
    layout->addWidget(db);
    layout->addWidget(place_combo_box);
    layout->addWidget(ok_btn);
    layout->addWidget(cancel_btn);
    this->setLayout(layout);
}

void Dialog::mousePressEvent(QMouseEvent *event) {
  int x = event->x();
  int y = event->y();
  int index_x = y / 32 / 3;
  int index_y = x / 32 / 3;
  if(index_x >= game->get_height() || index_y >= game->get_width())
      return;
  switch(index) {
  case 0:
      game->place_none(index_x, index_y);
      break;
  case 1:
      game->place_brave(index_x, index_y);
      break;
  case 2:
      game->place_pit(index_x, index_y);
      break;
  case 3:
      game->place_wumpus(index_x, index_y);
      break;
  case 4:
      game->place_gold(index_x, index_y);
      break;
  }
  db->set_map(game->extract_map_info(true));
  db->set_player(game->get_player());
  db->update();
}

void Dialog::combo_index_changed(int index) {
    this->index = index;
}
void Dialog::ok_btn_clicked(bool) {
    this->ok = true;
    this->close();
}
void Dialog::cancel_btn_clicked(bool) {
    this->ok = false;
    this->close();
}
Dialog::~Dialog()
{
    delete ui;
}
