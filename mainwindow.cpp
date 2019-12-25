#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), game_qt(nullptr) {
  ui->setupUi(this);
  db = new DrawBoard();
  random_layout_btn = new QPushButton("随机布局");
  custom_layout_btn = new QPushButton("人工布局");
  ai_btn = new QPushButton("AI");
  connect(random_layout_btn, SIGNAL(clicked(bool)), this,
          SLOT(random_layout_btn_clicked(bool)));
  connect(custom_layout_btn, SIGNAL(clicked(bool)), this,
          SLOT(custom_layout_btn_clicked(bool)));
  connect(ai_btn, SIGNAL(clicked(bool)), this, SLOT(ai_btn_clicked(bool)));
  layout = new QVBoxLayout();
  layout->addWidget(db);
  layout->addWidget(random_layout_btn);
  layout->addWidget(custom_layout_btn);
  layout->addWidget(ai_btn);
  ui->centralWidget->setLayout(layout);
}

Block **MainWindow::extract_map_info(int **map) {
  Block **info = new Block *[4];
  for (int i = 0; i < 4; ++i) {
    info[i] = new Block[4];
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      info[i][j].is_stench = TriState::No;
      info[i][j].is_pit = TriState::No;
      info[i][j].is_gold = TriState::No;
      info[i][j].is_breeze = TriState::No;
      info[i][j].is_wumpus = TriState::No;
      info[i][j].is_unknown = TriState::No;
      info[i][j].is_player = TriState::No;
    }
  }
  for (int i = 0; i < 4; ++i) {
    for (int j = 0; j < 4; ++j) {
      switch (map[i][j]) {
      case (int)ObjectType::None:
        info[i][j].is_pit = TriState::No;
        info[i][j].is_gold = TriState::No;
        info[i][j].is_wumpus = TriState::No;
        info[i][j].is_unknown = TriState::No;
        info[i][j].is_player = TriState::No;
        break;
      case (int)ObjectType::PitType:
        info[i][j].is_pit = TriState::Yes;
        if (i - 1 >= 0) {
          info[i - 1][j].is_breeze = TriState::Yes;
        }
        if (i + 1 < 4) {
          info[i + 1][j].is_breeze = TriState::Yes;
        }
        if (j - 1 >= 0) {
          info[i][j - 1].is_breeze = TriState::Yes;
        }
        if (j + 1 < 4) {
          info[i][j + 1].is_breeze = TriState::Yes;
        }
        break;
      case (int)ObjectType::PlayerType:
        info[i][j].is_player = TriState::Yes;
        break;
      case (int)ObjectType::WumpusType:
        info[i][j].is_wumpus = TriState::Yes;
        if (i - 1 >= 0) {
          info[i - 1][j].is_stench = TriState::Yes;
        }
        if (i + 1 < 4) {
          info[i + 1][j].is_stench = TriState::Yes;
        }
        if (j - 1 >= 0) {
          info[i][j - 1].is_stench = TriState::Yes;
        }
        if (j + 1 < 4) {
          info[i][j + 1].is_stench = TriState::Yes;
        }
        break;
      }
    }
  }
  return info;
}

void MainWindow::random_layout_btn_clicked(bool) {
  if (game_qt) {
    delete game_qt;
    game_qt = nullptr;
  }
  game_qt = new GameQt();
  game_qt->generate_map();
  db->set_map(extract_map_info(game_qt->get_map()));
  db->update();
}
void MainWindow::custom_layout_btn_clicked(bool) { exit(1); }
void MainWindow::ai_btn_clicked(bool) { exit(1); }

MainWindow::~MainWindow() {
  delete ui;
  if (game_qt)
    delete game_qt;
}
