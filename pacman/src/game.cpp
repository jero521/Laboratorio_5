#include "game.h"
#include "game/entities/pacman.h"
#include "game/entities/dot.h"

Game::Game()
{
    this->setWindowTitle("Pacman");
    this->setWindowIcon(QPixmap(":/img/pacman/1.png"));
    this->setFixedSize(width+5, 600);
    loadGameEntities();
}

void Game::loadGameEntities(){

    // create the scene
    scene = new QGraphicsScene(this);
    scene->setSceneRect(0, 0, width, height);
    scene->setBackgroundBrush(QPixmap(":/img/map/map_pure.png").scaledToHeight(564));
    setScene(scene);

    // create the compass item
    compass = new Compass(this);
    connect(compass, SIGNAL(eat(QPoint)), this, SLOT(itemEat(QPoint)));

    mode = Mode::Menu;

    // create the shining timer for power pellets shine
    shine = new QTimer(this);

    // create the lag timer for anytime the game should pause a little
    lag = new QTimer(this);

    // create the pacman (the player)
    player = new Pacman(compass);
    scene->addItem(player);
    player->hide();

    // create the ghost
    blinky = new Blinky(compass);
    scene->addItem(blinky);
    blinky->setPos(32, 51);


    // create the move timer
    pacmanMove = new QTimer(this);

    // player move
    connect(pacmanMove, SIGNAL(timeout()), player, SLOT(movi()));

    ghostMove = new QTimer(this);

    // ghost move
    connect(ghostMove, SIGNAL(timeout()), blinky, SLOT(movi()));


    connect(blinky, SIGNAL(fail()), this, SLOT(lifesManager()));





    // create the dashboard
    board = new Dashboard(this);
    scene->addItem(board);
    board->hide();



    loadUI();
    show();
}

void Game::loadUI(){
    //Load UI
    title = new QGraphicsTextItem();
    title->setPlainText("PACMAN");
    title->setDefaultTextColor(Qt::yellow);
    title->setFont(QFont(font_family, font_size_title));
    title->setPos(150,-10);
    scene->addItem(title);

    text_score = new QGraphicsTextItem();
    text_score->setPlainText("Puntos");
    text_score->setDefaultTextColor(Qt::white);
    text_score->setFont(QFont(font_family, font_size));
    text_score->setPos(0,535);
    scene->addItem(text_score);

    text_lives = new QGraphicsTextItem();
    text_lives->setPlainText("Vidas");
    text_lives->setDefaultTextColor(Qt::white);
    text_lives->setFont(QFont(font_family, font_size));
    text_lives->setPos(220,535);
    scene->addItem(text_lives);

    editable_score = new QGraphicsTextItem();
    editable_score->setPlainText(QString::number(board->getScore()));
    editable_score->setDefaultTextColor(Qt::yellow);
    editable_score->setFont(QFont(font_family, font_size_2));
    editable_score->setPos(125,538);

    editable_lifes = new QGraphicsPixmapItem();
    editable_lifes->setScale(0.20);
    editable_lifes->setPos(340, 538);
    editable_lifes->setPixmap(QPixmap(":/img/lives_3.png"));
    scene->addItem(editable_lifes);



    refreshScore(board->getScore());

    get_ready = new QGraphicsTextItem();
    get_ready->setPlainText("Listo!");
    get_ready->setDefaultTextColor(Qt::yellow);
    get_ready->setFont(QFont(font_family, font_size_3));
    get_ready->setPos(180,300);

    less_life = new QMediaPlayer;
    less_life->setMedia(QUrl("qrc:/audio/pacman_less_life.wav"));
    less_life->setVolume(100);

}

void Game::keyPressEvent(QKeyEvent *event) {
    if (mode == Mode::Play) {
        if ((event->key() == Qt::Key_Up) || (event->key() == Qt::Key_W))
            player->setDirec(Dir::Up);
        else if ((event->key() == Qt::Key_Down)|| (event->key() == Qt::Key_S))
            player->setDirec(Dir::Down);
        else if ((event->key() == Qt::Key_Left) || (event->key() == Qt::Key_A))
            player->setDirec(Dir::Left);
        else if ((event->key() == Qt::Key_Right) || (event->key() == Qt::Key_D))
            player->setDirec(Dir::Right);
        else if(event->key() == Qt::Key_Space){
            mode = Mode::Pause;
            pause();
            get_ready->setPlainText("JUEGO PAUSADO");
        }
    }
    else if(mode == Mode::Pause){
        if(event->key() == Qt::Key_Space){
            mode = Mode::Play;
            resume();
            get_ready->setPlainText("");
            QMediaPlayer* pause = new QMediaPlayer;
            pause->setMedia(QUrl("qrc:/audio/pacman_extrapac.wav"));
            pause->setVolume(100);
            pause->play();
        }
    }
}

void Game::putDots() {
    QList <QPoint> list = compass->dots();
    foreach(QPoint pos, list) {
        item[pos.x()][pos.y()] = new Dot(this);
        item[pos.x()][pos.y()]->setPos(16 * pos.y(), 35 + 16 * pos.x());
        scene->addItem(item[pos.x()][pos.y()]);
        connect(item[pos.x()][pos.y()], SIGNAL(dotEaten()), this, SLOT(dotsAte()));
    }
}

void Game::itemEat(QPoint pos) {
    if (mode == Mode::Play) {
        item[pos.x()][pos.y()]->eaten();
    }
}

void Game::pause() {
    QMediaPlayer* pause = new QMediaPlayer;
    pause->setMedia(QUrl("qrc:/audio/pacman_extrapac.wav"));
    pause->setVolume(100);
    pause->play();


    pacmanMove->stop();
    ghostMove->stop();
    shine->stop();

    // turn off the timer on the character
    player->pause();
    blinky->pause();

}

void Game::resume() {
    pacmanMove->start();
    ghostMove->start();
    shine->start();

    // turn on all timers on the objects
    player->start();
    blinky->start();

}

void Game::dotsAte() {
    QMediaPlayer* eatDotMusic = new QMediaPlayer;
    eatDotMusic->setMedia(QUrl("qrc:/audio/pacman_simple_homp.wav"));
    eatDotMusic->setVolume(100);
    eatDotMusic->play();

    board->addScore(10);
    refreshScore(board->getScore());
    remainDots --;

}

void Game::pelletAte() {
    QMediaPlayer* eatDotMusic = new QMediaPlayer;
    eatDotMusic->setMedia(QUrl("qrc:/audio/pacman_eatfruit.wav"));
    eatDotMusic->setVolume(100);
    eatDotMusic->play();

    board->addScore(50);
    refreshScore(board->getScore());
    times = 0;

}

void Game::afterGameStart(){
    QMediaPlayer* openning = new QMediaPlayer;
    openning->setMedia(QUrl("qrc:/audio/pacman_beginning.wav"));
    openning->setVolume(100);
    openning->play();

    scene->addItem(get_ready);

    QTimer *waitingToStart = new QTimer(this);
    waitingToStart->setSingleShot(true);
    connect(waitingToStart, SIGNAL(timeout()), this, SLOT(gameStart()));
    waitingToStart->start(4000);
    wait(0.5);
}

void Game::gameStart() {
    get_ready->setPlainText("");


    // change mode
    mode = Mode::Play;

    // put the dots and pellets on the map
    compass->initMap();
    putDots();

    // set timer start
    pacmanMove->start(20);
    ghostMove->start(11);
    shine->start(300);

    // set the characters' pos
    player->setPos(width / 2 - player->boundingRect().width() / 2 + 7, 403);
    blinky->setPos(width / 2 - blinky->boundingRect().width() / 2 + 8, 211);


    remainDots = 240;

    board->reset();

    player->reiniciar();
    blinky->reiniciar();


    connect(lag, SIGNAL(timeout()), this, SLOT(countDown()));

    wait(0.5);
}

void Game::lifesManager(){
    board->lifes--;

    switch (board->getLifes()) {
    case 3:
        less_life->play();
        editable_lifes->setPixmap(QPixmap(":/img/lives_3.png"));
        wait(0.5);
        break;
    case 2:
        less_life->play();
        editable_lifes->setPixmap(QPixmap(":/img/lives_2.png"));
        wait(0.5);
        break;
    case 1:
        less_life->play();
        editable_lifes->setPixmap(QPixmap(":/img/lives_1.png"));
        wait(0.5);
        break;
    case 0:
        editable_lifes->setPixmap(QPixmap(":/img/lives_0.png"));
        wait(0.5);
        break;
    default:
        less_life->play();
        editable_lifes->setPixmap(QPixmap(":/img/lives_3.png"));
        wait(0.5);
        break;
    }
}



void Game::stopAndShutdown(){
    this->close();

}

void Game::clearDots() {
    QList <QPoint> list = compass->remainDots();
    foreach(QPoint pos, list) {
        delete item[pos.x()][pos.y()];
    }
}

void Game::wait(qreal msec) {
    if (mode != Mode::Play)
        return;
    mode = Mode::Pause;
    lag->start(int(1000 * msec));
//    pause();
}

void Game::countDown() {
    mode = Mode::Play;
    player->show();
    lag->stop();
    resume();
}




void Game::refreshScore(int score){
    editable_score->setPlainText(QString::number(board->getScore()));
    scene->addItem(editable_score);
}


