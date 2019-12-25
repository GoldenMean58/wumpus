#include "drawboard.h"

DrawBoard::DrawBoard(QWidget *parent) : QWidget(parent)
{
    this->setMinimumSize(4 * 3 * UNIT_SIZE, 4 * 3 * UNIT_SIZE);
}

void DrawBoard::paintEvent(QPaintEvent *) {
    QPainter qp;
    QImage image;
    image.load(":/resource/image/pit.png");
    qp.begin(this);
    for(int i = 0; i < 12 ; ++i){
        for(int j = 0 ; j < 12; ++j) {
            qp.drawImage(32 * i, 32 * j, image);
        }
    }
    qp.end();
}
