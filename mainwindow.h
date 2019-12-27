#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameQt.h"
#include "drawboard.h"
#include <QCheckBox>
#include <QKeyEvent>
#include <QMainWindow>
#include <QMessageBox>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  virtual void keyPressEvent(QKeyEvent *);

private slots:
  void random_layout_btn_clicked(bool);
  void custom_layout_btn_clicked(bool);
  void ai_btn_clicked(bool);
  void debug_check_box_state_changed(int state);

private:
  GameQt *game_qt;
  DrawBoard *db;
  QPushButton *random_layout_btn;
  QPushButton *custom_layout_btn;
  QCheckBox *debug_check_box;
  QPushButton *ai_btn;
  QLayout *layout;
  Ui::MainWindow *ui;
  bool is_debug = false;
};

#endif // MAINWINDOW_H
