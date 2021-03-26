#include "doodler.h"
#include <QPainter>
#include <QKeyEvent>
#include <QFrame>
#include <QPainterPath>
#include <QList>
#include <QObject>
#include <QTimer>

Doodler::Doodler()
{
    setFlags(QGraphicsItem::ItemIsFocusable);;
    X=155;
    Y=275;
    H=Y;
    speed=gravity;
    doo=QPixmap(":/image/doodle.png");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

Doodler::Doodler(int a)
{
    //enables different mode
    setFlags(QGraphicsItem::ItemIsFocusable);;
    X=155;
    Y=275;
    H=Y;
    speed=gravity;
    if (a==0) doo=QPixmap(":/image/doodle.png");
    if (a==1) doo=QPixmap(":/image/chris.png");
    if (a==2) doo=QPixmap(":/image/jungle.png");
    if (a==3) doo=QPixmap(":/image/space.png");
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(move()));
}

QRectF Doodler::boundingRect() const
{
    //prepareGeometryChange();
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2, 50+ + penWidth,  50+penWidth);
}

void Doodler::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    //avoid unuse warning
    Q_UNUSED(option);
    Q_UNUSED(widget);
    painter->drawPixmap(X,Y,50,50,doo);

}

//change face direction
void Doodler::keyPressEvent(QKeyEvent *event){
    switch (event->key()) {
    case Qt::Key_Left:
        if(event->isAutoRepeat()) return;
        if (mode==0) doo=QPixmap(":/image/doodle2.png");
        if (mode==1) doo=QPixmap(":/image/chris2.png");
        if (mode==2) doo=QPixmap(":/image/jungle2.png");
        if (mode==3) doo=QPixmap(":/image/space2.png");

        moveBy(-20,0);
        X-=20;

        break;
    case Qt::Key_Right:
        if(event->isAutoRepeat()) return;
        if (mode==0) doo=QPixmap(":/image/doodle.png");
        if (mode==1) doo=QPixmap(":/image/chris.png");
        if (mode==2) doo=QPixmap(":/image/jungle.png");
        if (mode==3) doo=QPixmap(":/image/space.png");

        moveBy(20,0);
        X+=20;

        break;
    }
}

//collision judgement--rewrite of the shape
QPainterPath Doodler::shape() const
{
    QPainterPath path;
    path.addRect(X+15 , Y+49, 20,  1);  //real size
    return path;
}

void Doodler::move()
{
    //self-designed score function
    score=137*totalH+640/Y*1.34247;
    moveBy(0,speed);
    Y+=speed;
    //initial position based Y
    if (Y>440){
        emit ggwp();
    }
    if (speed>0){
        QList<QGraphicsItem *> collisions = collidingItems();
        // Check collisions with other objects on screen
        foreach (QGraphicsItem *collidingItem, collisions) {
            //special key is set as "196"
            if (collidingItem->data(196) == "plat") {
                speed=-gravity;
            }
        }
    }
    if (speed<0){
        i++;
        speed +=0.08;
    }
    if (i>35){
        totalH+=(H-Y);
        H=Y;
        //H represent the current highest position of doodler (in the screen)
        if (H<200) {
            double a=350-H;
            H=350;
            totalH+=a;
            emit view_down(a);
        }
        speed=gravity;
        i=0;
    }
}
void Doodler::fall(double a){
    moveBy(0,a);
    Y+=a;
    speed=0;
}

void Doodler::startTimer(int interval) {
    timer->start(interval);
}

void Doodler::stop_timer() {
    timer->stop();
}


