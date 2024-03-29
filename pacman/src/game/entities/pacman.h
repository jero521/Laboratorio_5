#ifndef PACMAN_H
#define PACMAN_H

#include <QPixmap>
#include <QTimer>
#include <QDebug>
#include "character.h"
#include "../compass.h"

class Pacman: public Character {
    Q_OBJECT
public:
    Pacman(Compass *compass_ipt);

    void setDirec(QPoint dir);
    void start();
    void pause();
    void morir();
    void reiniciar();

public slots:
    void movi();
    void switchAnimate();

private:
    QPoint direc;
    QPoint tmpDir;
    QTimer *switchTimer;
    QPixmap pic[3], disolve[11];
    Compass *compass;
    int index, add;

    bool dead;
};

#endif // PACMAN_H
