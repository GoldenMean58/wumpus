#ifndef DRAWBOARD_H
#define DRAWBOARD_H

#include <QWidget>
#include <QPainter>

constexpr int UNIT_SIZE = 32;

class DrawBoard : public QWidget
{
    Q_OBJECT
public:
    explicit DrawBoard(QWidget *parent = nullptr);
    virtual void paintEvent(QPaintEvent*);
signals:

public slots:
};

#endif // DRAWBOARD_H
