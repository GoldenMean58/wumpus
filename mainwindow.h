#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPainter>
#include <QVBoxLayout>
#include <QPushButton>
#include "AI.h"
#include "drawboard.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


private slots:
    void random_layout_btn_clicked(bool);
    void custom_layout_btn_clicked(bool);
    void ai_btn_clicked(bool);
private:
    DrawBoard *db;
    QPushButton *random_layout_btn;
    QPushButton *custom_layout_btn;
    QPushButton *ai_btn;
    QLayout *layout;
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
