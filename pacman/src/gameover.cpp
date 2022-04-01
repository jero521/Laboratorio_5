#include "gameover.h"
#include "ui_gameover.h"

GameOver::GameOver(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameOver)
{
    ui->setupUi(this);
    loadUI();
}

GameOver::~GameOver()
{
    delete ui;
}

void GameOver::loadUI(){
    QMovie *movie=new QMovie(":/img/initial_gif.gif");
    ui->initial_gif_label->setMovie(movie);
    movie->start();
    QFontDatabase::addApplicationFont(":/font/pixel.ttf");
    ui->goback_button->setFont(QFont(font_family, font_size));
    ui->editable->setFont(QFont(font_family, font_size_2));
//    ui->editable->setText(board->getScore());
    qDebug() << board->getScore();
}

void GameOver::on_goback_button_clicked()
{
    this->close();
    MainWindow *w = new MainWindow();
    w->show();
}


void GameOver::on_ok_button_clicked()
{
    this->close();

}
