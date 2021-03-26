#include "platform.h"
#include <QPainter>
#include "random.h"
#include <QTimer>

Platform::Platform()
{
    X=145;
    Y=550;
    setData(196,"plat");
    plat=QPixmap(":/image/platform.png");
}

//enable the set of platforms at certain position
Platform::Platform(int a, int b)
{
    X=a;
    Y=b;
    //special key & value
    setData(196,"plat");
    plat=QPixmap(":/image/platform.png");
}

QRectF Platform::boundingRect() const
{
    qreal penWidth = 1;
    return QRectF(X - penWidth / 2, Y - penWidth / 2,
                  70 + penWidth, 10 + penWidth);
}

void Platform::paint(QPainter *painter,
                   const QStyleOptionGraphicsItem *option,
                   QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    painter->drawPixmap(X,Y,70,10,plat);
}

void Platform::fall(double a){
    moveBy(0,a);
    Y+=a;
    if (Y>640){
        emit outview();
    }
}





