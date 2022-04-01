#include "blinky.h"

Blinky::Blinky(Compass *compass_ipt): Ghost (compass_ipt), compass(compass_ipt) {
    loadPicture(":/img/ghost/blinky/");
    setInitDirection(Dir::Left);
    setCritical(QPoint(1, 27));

    tmr = new QTimer();
}

QPoint Blinky::setTarget() {
    QPoint player = compass->getPlayerLoc();

    return player;
}

void Blinky::sendOut() {}
