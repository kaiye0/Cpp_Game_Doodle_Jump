#ifndef PLATFORM_H
#define PLATFORM_H
#include <QGraphicsObject>

class Platform : public QGraphicsObject
{
Q_OBJECT
public:
    Platform();
    Platform(int a, int b);
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);

    double X;
    double Y;
    int mode;


public slots:
    void fall(double a);

signals:
    void outview();

private:
    QPixmap plat;





};


#endif // PLATFORM_H
