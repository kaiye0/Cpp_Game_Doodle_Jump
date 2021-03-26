#include <QtWidgets>
#include <QTimer>
#include "doodler.h"
#include "platform.h"
#include "random.h"
#include <QApplication>
#include <QTextCodec>
#include <QTime>
#include <QSplashScreen>
#include "doodleview.h"

int main(int argc, char* argv[]) {

    QApplication  app(argc, argv);
    //qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));
    QPixmap pix(":/image/scre.jpg");
    QSplashScreen *splash = new QSplashScreen;
    //splash->setGeometry(0,0,600,800);
    splash->setPixmap(pix);
    splash->show();
    DoodleView view;
    view.show();
    splash->finish(&view);//当窗口view完成初始化工作后就结束启动画面
    return app.exec();
}
