#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), game_qt(nullptr) {
  ui->setupUi(this);
  db = new DrawBoard();
  random_layout_btn = new QPushButton("随机布局");
  custom_layout_btn = new QPushButton("人工布局");
  ai_btn = new QPushButton("AI");
  debug_check_box = new QCheckBox("Debug");
  connect(random_layout_btn, SIGNAL(clicked(bool)), this,
          SLOT(random_layout_btn_clicked(bool)));
  connect(custom_layout_btn, SIGNAL(clicked(bool)), this,
          SLOT(custom_layout_btn_clicked(bool)));
  connect(ai_btn, SIGNAL(clicked(bool)), this, SLOT(ai_btn_clicked(bool)));
  connect(debug_check_box, SIGNAL(stateChanged(int)), this,
          SLOT(debug_check_box_state_changed(int)));
  layout = new QVBoxLayout();
  layout->addWidget(db);
  layout->addWidget(random_layout_btn);
  layout->addWidget(custom_layout_btn);
  layout->addWidget(ai_btn);
  layout->addWidget(debug_check_box);
  ui->centralWidget->setLayout(layout);
}
void MainWindow::debug_check_box_state_changed(int state) {
  is_debug = state;
  db->set_map(game_qt->extract_map_info(is_debug));
  db->set_player(game_qt->get_player());
  db->update();
}
void MainWindow::random_layout_btn_clicked(bool) {
  if (game_qt) {
    delete game_qt;
    game_qt = nullptr;
  }
  game_qt = new GameQt();
  game_qt->start();
  db->set_map(game_qt->extract_map_info(is_debug));
  db->set_player(game_qt->get_player());
  db->update();
}
void MainWindow::custom_layout_btn_clicked(bool) { exit(1); }
void MainWindow::ai_btn_clicked(bool) {
  if (game_qt->is_over())
    return;
  int data[10];
  Action action = game_qt->ask(data);
  auto event = game_qt->take_action(action, data);
  if (event == Event::GameOver) {
    int score = 0;
    int grab_gold = 0;
    int dead = 0;
    Player *player = game_qt->get_player();
    if (player->is_grab_gold())
      grab_gold = 1000;
    score += grab_gold;
    score -= player->get_action_count();
    score -= DEFAULT_ARROW_COUNT - player->get_arrow_count();
    if (player->is_dead)
      dead -= 1000;
    score += dead;
    QString prompt =
        "Game over! Score = (Grab gold?) " + QString::number(grab_gold) +
        " + (dead?) " + QString::number(dead) + " - (step) " +
        QString::number(player->get_action_count()) + " - (arrow used) " +
        QString::number(DEFAULT_ARROW_COUNT - player->get_arrow_count()) +
        " = " + QString::number(score);
    QMessageBox::information(this, "Game Over", prompt);
  }
  db->set_map(game_qt->extract_map_info(is_debug));
  db->set_player(game_qt->get_player());
  db->update();
}

MainWindow::~MainWindow() {
  delete ui;
  if (game_qt)
    delete game_qt;
}

void MainWindow::keyPressEvent(QKeyEvent *event) {
  int key = event->key();
  Event game_event;
  switch (key) {
  case Qt::Key_W:
    game_event = game_qt->take_action(Action::MoveForward);
    break;
  case Qt::Key_A:
    game_event = game_qt->take_action(Action::TurnLeft);
    break;
  case Qt::Key_D:
    game_event = game_qt->take_action(Action::TurnRight);
    break;
  case Qt::Key_S:
    game_event = game_qt->take_action(Action::Shoot);
    break;
  case Qt::Key_G:
    game_event = game_qt->take_action(Action::Grab);
    break;
  case Qt::Key_L:
    game_event = game_qt->take_action(Action::Leave);
    break;
  default:
    return;
  }
  db->set_map(game_qt->extract_map_info(is_debug));
  db->update();
  if (game_event == Event::GameOver) {
    exit(0);
  }
}
