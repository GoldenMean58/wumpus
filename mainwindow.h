#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "GameQt.h"
#include "drawboard.h"
#include <QMainWindow>
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

private slots:
  void random_layout_btn_clicked(bool);
  void custom_layout_btn_clicked(bool);
  void ai_btn_clicked(bool);
  Block **extract_map_info(int **map);

private:
  GameQt *game_qt;
  DrawBoard *db;
  QPushButton *random_layout_btn;
  QPushButton *custom_layout_btn;
  QPushButton *ai_btn;
  QLayout *layout;
  Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
