#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    db = new DrawBoard();
    random_layout_btn = new QPushButton("随机布局");
    custom_layout_btn = new QPushButton("人工布局");
    ai_btn = new QPushButton("AI");
    connect(random_layout_btn, SIGNAL(clicked(bool)), this, SLOT(random_layout_btn_clicked(bool)));
    connect(custom_layout_btn, SIGNAL(clicked(bool)), this, SLOT(custom_layout_btn_clicked(bool)));
    connect(ai_btn, SIGNAL(clicked(bool)), this, SLOT(ai_btn_clicked(bool)));
    layout = new QVBoxLayout();
    layout->addWidget(db);
    layout->addWidget(random_layout_btn);
    layout->addWidget(custom_layout_btn);
    layout->addWidget(ai_btn);
    ui->centralWidget->setLayout(layout);
}

void MainWindow::random_layout_btn_clicked(bool) {
    exit(1);
}
void MainWindow::custom_layout_btn_clicked(bool) {
    exit(1);
}
void MainWindow::ai_btn_clicked(bool) {
    exit(1);
}

MainWindow::~MainWindow()
{
    delete ui;
}
