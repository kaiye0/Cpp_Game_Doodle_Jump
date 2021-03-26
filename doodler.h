#ifndef DOODLER_H
#define DOODLER_H
#include <QGraphicsObject>

class Doodler : public QGraphicsObject
{
Q_OBJECT
public:
    Doodler();
    Doodler(int a);
    QRectF boundingRect() const;
    void paint(QPainter *painter,
               const QStyleOptionGraphicsItem *option,
               QWidget *widget);
    QPainterPath shape() const;
    double X;
    double Y;
    double speed;
    double H;
    double totalH=0;
    int i=0;
    int score=0;
    void startTimer(int interval);
    void stop_timer();
    int mode=0;


public slots:
    void move();
    void fall(double a);

signals:
    void view_down(double a);
    void ggwp();

private:
    double gravity=3;
    QTimer *timer;
    QPixmap doo;


protected:
    void keyPressEvent(QKeyEvent *event);

};

#endif // DOODLER_H
