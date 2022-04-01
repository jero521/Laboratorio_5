#ifndef COMPASS_H
#define COMPASS_H
#include <QObject>
#include <QGraphicsItem>
#include <QDebug>

class Compass: public QObject {
    Q_OBJECT
public:
    Compass(QObject *parent = 0);

    void initMap();
    void check(QPointF pos, QPoint dir);

    bool P_movi(QPointF pos, QPointF direction);
    QList <QPoint> dots();
    QList <QPoint> remainDots();

    void setLoc(QPoint pos, char character);
    void setPos(QPointF pos);
    void setPowerUp(bool ipt);
    QPoint getPlayerLoc();
    QPointF getPlayerPos();
    QPoint getPlayerDir();
    QPoint getBlinkyPos();

signals:
    void eat(QPoint pos);


private:
    QList <QList <int> > map;
    QPoint player, blinky;
    QPoint dir_player;
    QPointF player_pos;

};


#endif // COMPASS_H
