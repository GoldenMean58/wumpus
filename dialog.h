#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QMouseEvent>
#include <QComboBox>
#include "drawboard.h"
#include "Game.h"
#include "GameQt.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(GameQt *game, QWidget *parent = nullptr);
    GameQt *game;
    QVBoxLayout *layout;
    DrawBoard *db;
    QComboBox* place_combo_box;
    QPushButton *ok_btn;
    int index;
    QPushButton *cancel_btn;
    bool ok;
    virtual void mousePressEvent(QMouseEvent *event);
    ~Dialog();
private slots:
  void combo_index_changed(int);
  void cancel_btn_clicked(bool);
  void ok_btn_clicked(bool);

private:
    Ui::Dialog *ui;
};

#endif // DIALOG_H
