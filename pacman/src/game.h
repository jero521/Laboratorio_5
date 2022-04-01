#ifndef GAME_H
#define GAME_H

#include <QGraphicsView>
#include <QDialog>
#include <QKeyEvent>
#include <QTimer>
#include <unistd.h>
#include <QGraphicsPixmapItem>
#include <QDebug>
#include <QEventLoop>
#include <QMediaPlayer>
#include "game/compass.h"
#include "game/dashboard.h"
#include "game/animaterect.h"
#include "gameover.h"
#include "game/entities/character.h"
#include "game/entities/item.h"
#include "game/entities/ghost.h"
#include "game/entities/blinky.h"





class Game : public QGraphicsView
{
    Q_OBJECT

public:
    Game();

    void keyPressEvent(QKeyEvent *event);
    void putDots();
    void clearDots();
    void pause();
    void resume();

    void wait(qreal msec);

    // panel de control
    void menuPanel(bool ipt);
    void playPanel(bool ipt);
    void pausePanel(bool ipt);

    // modo control
    void gameClear();

    Dashboard *board;

public slots:
    void itemEat(QPoint pos);
    void dotsAte();
    void pelletAte();
    void countDown();

    // control de fin del game
    void afterGameStart();
    void gameStart();
    void lifesManager();

private slots:
    void stopAndShutdown();


private:
    static const int font_size = 18;
    static const int font_size_2 = 15;
    static const int font_size_3 = 13;
    static const int font_size_title = 22;
    const QString font_family = "Joystix";

    void loadUI();
    void loadGameEntities();
    void refreshScore(int score);


    QGraphicsTextItem *title;
    QGraphicsTextItem *text_score;
    QGraphicsTextItem *editable_score;
    QGraphicsTextItem *text_lives;
    QGraphicsTextItem *get_ready;
    QGraphicsPixmapItem *editable_lifes;
    QGraphicsPixmapItem *editable_cherry;
    QMediaPlayer* less_life;

    const int width = 448;
    const int height = 564;

    int mode;
    int remainDots;
    int times;

    QGraphicsScene *scene;
    Item *item[31][28];

    Character *player, *blinky;
    Compass *compass;
    QTimer *pacmanMove, *shine, *ghostMove, *lag;


};

enum Mode{
    Menu, Pause, Play
};

#endif // GAME_H
