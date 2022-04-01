#ifndef GHOST_H
#define GHOST_H

#include <QObject>
#include <QTimer>
#include <QDebug>
#include <QEventLoop>
#include "character.h"
#include "../compass.h"

class Ghost: public Character {
    Q_OBJECT
public:
    Ghost(Compass *compass);

    void movi();
    void start();
    void pause();
    void setDirec(QPoint dir);
    void setInitDirection(QPoint dir);
    void setMode(int mode_ipt);
    void reiniciar();
    void loadPicture(QString filepath);
    void setCritical(QPoint pt);

    static qreal distance(QPoint a, QPoint b);

public slots:
    void switchAnimate();
    void changeMode();
    void timeLeft();
    void shining();



private:
    QPointF direction;
    QPoint tmpDir;
    QPoint target;
    QPoint critical;
    QPixmap pic[4][2];
    QPixmap fright[2][2];
    QPixmap dead[4];
    QTimer *switchTimer, *chaseTimer, *nerfTimer, *shine;
    Compass *compass;
    bool nerf;
    int index_i, index_j;
    int mode, prevMode;
    int remainNerf;
    qreal step_size;
    char kind;

protected :
    QTimer *tmr;
    bool home;
    enum Mode {
         Frighten, Home, End
    };
};


#endif // GHOST_H
