#include "ghost.h"

Ghost::Ghost(Compass *compass_ipt): compass(compass_ipt) {
    setOffset(QPoint(-8, -8));
    tmpDir = QPoint(-1, -1);
    direction = QPointF(0, 0);

    switchTimer = new QTimer();
    connect(switchTimer, SIGNAL(timeout()), this, SLOT(switchAnimate()));
    switchTimer->start(80);

    chaseTimer = new QTimer();
    connect(chaseTimer, SIGNAL(timeout()), this, SLOT(changeMode()));
    chaseTimer->start(20000);

    nerfTimer = new QTimer();
    connect(nerfTimer, SIGNAL(timeout()), this, SLOT(timeLeft()));

    shine = new QTimer();
    connect(shine, SIGNAL(timeout()), this, SLOT(shining()));

    step_size = 1;

    index_i = 1;
    index_j = 0;

    nerf = false;
    home = false;
    remainNerf = 0;
}

void Ghost::movi() {
    // check for the pos


    if (mode == Mode::Home) {
        setPos(pos() + direction * step_size);
        if (((y() <= 249) && home) || y() >= 269){
            direction = -direction;
        }
        return;
    }
    if ((x() < 0 || x() >= 448)) {
        setPos(pos() + direction * step_size);
        if (x() < -32){
            setX(448);
        } else if (x() > 480) {
            setX(-32);
        }
    }

    else {
        setPos(pos() + direction * step_size);
    }
}

void Ghost::setDirec(QPoint dir) {
    tmpDir = dir;
}

qreal Ghost::distance(QPoint a, QPoint b) {
    QPoint vector = b - a;
    return sqrt(pow(vector.x(), 2) + pow(vector.y(), 2));
}

void Ghost::loadPicture(QString filepath) {
    QString dire = "udlr";
    for (int i = 0; i < 4; i ++) {
        for (int j = 0; j < 2; j ++) {
            pic[i][j].load(filepath + dire[i] + QString::number(j + 1) + ".png");
            pic[i][j] = pic[i][j].scaledToHeight(32);
        }
    }
    setPixmap(pic[0][0]);
}


void Ghost::switchAnimate() {
    if (mode != Mode::Frighten) {
        if (direction == Dir::Up)
            index_i = 0;
        else if (direction == Dir::Down)
            index_i = 1;
        else if (direction == Dir::Left)
            index_i = 2;
        else
            index_i = 3;

    }
    index_j ++;
    if (index_j >= 2)
        index_j = 0;
}



void Ghost::setCritical(QPoint pt) {
    critical = pt;
}





void Ghost::timeLeft() {
    remainNerf --;
    if (remainNerf > 3) {
        index_i = 1;
        shine->stop();
    }
    else if (remainNerf <= 3 && remainNerf > 0)
        shine->start(200);
    else if (remainNerf <= 0) {
        chaseTimer->start();
        mode = prevMode;
        shine->stop();
        nerfTimer->stop();
    }
}



void Ghost::shining() {
    index_i ++;
    index_i %= 2;
    setPixmap(fright[index_i][index_j]);
}

void Ghost::pause() {
    chaseTimer->stop();
    nerfTimer->stop();
    shine->stop();
    switchTimer->stop();
    tmr->stop();
}

void Ghost::start() {
    switchTimer->start();
    if (mode == Mode::Frighten)
        nerfTimer->start();
    else if (mode != Mode::Home)
        chaseTimer->start();
    else
        tmr->start();
}

void Ghost::reiniciar() {
    step_size = 1;
    index_i = 1;
    index_j = 0;
    nerf = false;
    tmpDir = QPoint(-1, -1);
    direction = Dir::Left;
}

void Ghost::setInitDirection(QPoint dir) {
    direction = dir;
}

void Ghost::setMode(int mode_ipt) {
    mode = mode_ipt;
    if (mode == Mode::Home) {
        chaseTimer->stop();
        home = true;
    }
}


