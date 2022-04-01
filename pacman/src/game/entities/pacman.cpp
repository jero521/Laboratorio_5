#include "pacman.h"

Pacman::Pacman(Compass *compass_ipt) {
    // set the origin to fit the coordinate
    setOffset(QPoint(-7, -7));
    direc = QPoint(0, 0);
    tmpDir = QPoint(0, 0);

    // load the picture in
    for (int i = 0; i < 3; i ++) {
        pic[i].load(":/img/pacman/" + QString::number(i) + ".png");
        pic[i] = pic[i].scaledToHeight(30);
    }
    setPixmap(pic[0]);
    setTransformOriginPoint(boundingRect().width() / 2 - 7, boundingRect().height() / 2 - 7);

    for (int i = 0; i < 11; i ++) {
        disolve[i].load(":/img/pacman/dead/" + QString::number(i) + ".png");
        disolve[i] = disolve[i].scaledToHeight(30);
    }

    // set the timer for moving
    switchTimer = new QTimer();
    connect(switchTimer, SIGNAL(timeout()), this, SLOT(switchAnimate()));
    switchTimer->setInterval(50);

    // create the compass object
    compass = compass_ipt;


    index = 0;
    add = 1;
    dead = false;
}

void Pacman::movi() {
    if (dead)
        return;
    // handling bounding
    compass->setPos(pos());
    if (y() == 259. && (x() < 0 || x() >= 448)) {
        if (tmpDir == Dir::Left || tmpDir == Dir::Right)
            direc = tmpDir;
        if (direc == Dir::Left)
            setRotation(180);
        else if (direc == Dir::Right)
            setRotation(0);
        setPos(pos() + direc * 2);

       // if (x() < -30)
         //   setX(478);
       // else if (x() > 478)
         //   setX(-30);
    }
    // handling normal move
    else if (int(y() - 35) % 16 == 0 && int(x()) % 16 == 0) {
        if (tmpDir != direc && compass->P_movi(pos(), tmpDir)) {
            direc = tmpDir;
            if (direc == Dir::Up)
                setRotation(270);
            else if (direc == Dir::Down)
                setRotation(90);
            else if (direc== Dir::Left)
                setRotation(180);
            else if (direc == Dir::Right)
                setRotation(0);
        }
        else if (compass->P_movi(pos(), direc)) {
            compass->check(pos(), direc);
            setPos(pos() + direc * 2);
            switchTimer->start();
        }
        else {
            compass->check(pos(), direc);
            switchTimer->stop();
            setPixmap(pic[1]);
        }
    }
    else
        setPos(pos() + direc * 2);
}

void Pacman::setDirec(QPoint dir) {
    if (x() == 216. && y() == 403. && direc == QPoint(0, 0) && (dir == Dir::Left || dir == Dir::Right)) {
        direc = dir;
        if (direc == Dir::Left)
            setRotation(180);
        else if (direc == Dir::Right)
            setRotation(0);
    }
    tmpDir = dir;
}

void Pacman::switchAnimate() {
    if (!dead) {
        setPixmap(pic[index]);
        index += add;
        if (index >= 2 || index <= 0)
            add = -add;
    }
    else {
        if (index < 11) {
            setPixmap(disolve[index]);
            index += add;
        }
        else if (index >= 11) {
            switchTimer->stop();
            dead = false;
            hide();
        }

    }
}

void Pacman::morir() {
    qDebug() << "die";
    dead = true;
    switchTimer->stop();
    setRotation(0);
    switchTimer->start(50);

}

void Pacman::pause() {
    switchTimer->stop();
}

void Pacman::start() {
    switchTimer->start();
}

void Pacman::reiniciar() {
    direc = QPoint(0, 0);
    tmpDir = QPoint(0, 0);
    setPixmap(pic[0]);
    switchTimer->stop();
    compass->setPos(pos());
    dead = false;
    index = 0;
    add = 1;
}
