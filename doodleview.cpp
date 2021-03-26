#include "doodleview.h"
#include "doodler.h"
#include "platform.h"
#include "random.h"

#include <QIcon>
#include <QPropertyAnimation>
#include <QGraphicsBlurEffect>
#include <QTimer>
#include <QPushButton>
#include <QGraphicsProxyWidget>
#include <QApplication>
#include <QLabel>
#include <QFileInfo>

//constructor
DoodleView::DoodleView(QWidget *parent) :
    QGraphicsView(parent)
{
    init_view();
}

//initialize the welcome user interface
void DoodleView::init_view() {
    setWindowTitle("Doodle Jump");
    setWindowIcon(QIcon(":/image/doo.ico"));
    resize(360,640); //set window size
    //set scene for the window
    QGraphicsScene *scene = new QGraphicsScene;
    scene->setSceneRect(0, 0, 360, 640);
    scene->setBackgroundBrush(bg);
    setScene(scene);

    //text information: score of the game
    game_score = new QGraphicsTextItem();
    scene->addItem(game_score);
    game_score->setFont(QFont("Times", 20, QFont::Bold));
    game_score->setPos(10, 3);
    game_score->hide();

    //
    QWidget *shade = new QWidget;
    shade->setAutoFillBackground(true);
    shade->setPalette(QPalette(QColor(0, 0, 0, 50)));
    shade->resize(360, 640);

    shade_widget = scene->addWidget(shade);
    shade_widget->setPos(0, 0);
    shade_widget->setZValue(1);
    shade_widget->hide();
    //

    //option widget | set size and background
    QWidget *option = new QWidget;
    option->resize(200, 300);
    QPalette optionbg;
    QPixmap option_bg = QPixmap(":/image/option.jpg");
    optionbg.setBrush(QPalette::Background,QBrush(option_bg));
    option->setPalette(optionbg);
    option->setAutoFillBackground(true);

    //set a close button | set size & position, set click function
    QPushButton *option_close = new QPushButton(tr("Close"),option);
    option_close->setGeometry(80,260,40,20);
    connect(option_close, SIGNAL(clicked()), option, SLOT(hide()));

    //set three button for change mode of the game | set size & position, set click function
    QPushButton *mode1 = new QPushButton(tr("Chrismas"),option);
    mode1->setGeometry(65,220,70,20);
    connect(mode1, SIGNAL(clicked()), this, SLOT(setMode1()));

    QPushButton *mode2 = new QPushButton(tr("Jungle"),option);
    mode2->setGeometry(140,220,50,20);
    connect(mode2, SIGNAL(clicked()), this, SLOT(setMode2()));

    QPushButton *mode3 = new QPushButton(tr("Space"),option);
    mode3->setGeometry(10,220,50,20);
    connect(mode3, SIGNAL(clicked()), this, SLOT(setMode3()));

    //add to scene by "QGraphicsProxyWidget" as an "item"
    QGraphicsWidget *option_widget = scene->addWidget(option);
    option_widget->setPos(80, 150);
    option_widget->setZValue(3); //set Z depth--item with bigger Z shows on surface
    option_widget->hide();


    //help widget | set size and background
    QWidget *help = new QWidget;
    QPalette helpbg;
    QPixmap help_bg = QPixmap(":/image/help.jpg");
    helpbg.setBrush(QPalette::Background,QBrush(help_bg));
    help->setPalette(helpbg);
    help->setAutoFillBackground(true);
    help->resize(200, 300);

    //set a close button | set size & position, set click function
    QPushButton *help_close = new QPushButton(tr("Close"), help);
    help_close->setGeometry(75,260,50,25);
    connect(help_close, SIGNAL(clicked()), help, SLOT(hide()));

    //add to scene by "QGraphicsProxyWidget" as an "item"
    QGraphicsWidget *help_widget = scene->addWidget(help);
    help_widget->setPos(80, 150);
    help_widget->setZValue(3);
    help_widget->hide();

    //welcome text & title | set content & font & position & depth
    welcome_text = new QGraphicsTextItem();
    scene->addItem(welcome_text);
    welcome_text->setHtml("<font color=black>Doodle Jump</font>");
    welcome_text->setFont(QFont("Times", 40, QFont::Bold));
    welcome_text->setPos(60, 100);
    welcome_text->setZValue(2);

    //pause inform | set content & font & position & depth
    pause_text = new QGraphicsTextItem();
    scene->addItem(pause_text);
    pause_text->setHtml("<font color=black>Paused</font>");
    pause_text->setFont(QFont("Times", 40, QFont::Bold));
    pause_text->setPos(120, 100);
    pause_text->setZValue(2);
    pause_text->hide();

    //finish inform | set content & font & position & depth
    game_over_text = new QGraphicsTextItem();
    scene->addItem(game_over_text);
    game_over_text->setHtml("<font color=black>Life Sucks！</font>");
    game_over_text->setFont(QFont("Times", 40, QFont::Bold));
    game_over_text->setPos(80, 100);
    game_over_text->setZValue(2);
    game_over_text->hide();


    //buttons used in the game and menu
    QPushButton *button1 = new QPushButton("Start");
    connect(button1, SIGNAL(clicked()), this, SLOT(start_game()));
    start_button = scene->addWidget(button1);
    start_button->setPos(150, 200);
    start_button->setZValue(2);

    QPushButton *button2 = new QPushButton("Option");
    connect(button2, SIGNAL(clicked()), option, SLOT(show()));
    option_button = scene->addWidget(button2);
    option_button->setPos(145, 300);
    option_button->setZValue(2);


    QPushButton *button3 = new QPushButton("Help");
    connect(button3, SIGNAL(clicked()), help, SLOT(show()));
    help_button = scene->addWidget(button3);
    help_button->setPos(150, 400);
    help_button->setZValue(2);

    QPushButton *button4 = new QPushButton("Quit");
    connect(button4, SIGNAL(clicked()), qApp, SLOT(quit()));
    exit_button = scene->addWidget(button4);
    exit_button->setPos(150, 500);
    exit_button->setZValue(2);

    QPushButton *button5 = new QPushButton("Restart");
    connect(button5, SIGNAL(clicked()), this, SLOT(restart_game()));
    restart_button = scene->addWidget(button5);
    restart_button->setPos(130, 0);
    restart_button->setZValue(2);

    QPushButton *button6 = new QPushButton("Pause");
    connect(button6, SIGNAL(clicked()), this, SLOT(pause_game()));
    pause_button = scene->addWidget(button6);
    pause_button->setPos(280, 0);
    pause_button->setZValue(2);

    QPushButton *button7 = new QPushButton("Return");
    connect(button7, SIGNAL(clicked()), this, SLOT(return_game()));//返回主菜单
    return_button = scene->addWidget(button7);
    return_button->setPos(145, 200);
    return_button->setZValue(2);

    QPushButton *button8 = new QPushButton("Menu");
    connect(button8, SIGNAL(clicked()), this, SLOT(back_to_menu()));
    backmenu_button = scene->addWidget(button8);
    backmenu_button->setPos(210, 0);

    restart_button->hide();
    pause_button->hide();
    backmenu_button->hide();
    return_button->hide();
}

//start game--only used for enter game from main menu "start"
void DoodleView::start_game() {
    welcome_text->hide();
    start_button->hide();
    option_button->hide();
    help_button->hide();
    exit_button->hide();
    init_game();
}

//init game splited out for "restart" function
void DoodleView::init_game() {
    //set new doodler
    doodoo = new Doodler(mode);
    doodoo->mode = mode;
    scene()->setBackgroundBrush(bg);
    scene()->addItem(doodoo);
    connect(doodoo, SIGNAL(view_down(double)), doodoo, SLOT(fall(double)));
    doodoo->startTimer(25);
    doodoo->setFocus(); //to accept key event
    doodoo->show();

    //generate the initial map
    for (int i=0; i<9; i++){
        int a = randomInteger(0,290);
        //bottom one has a fixed "y" positon
        Platform *platform = new Platform(a,initbottom);
        int b = randomInteger(11,15);
        //next with a distance of 55~75, keep this interval within one jump
        initbottom -= b*5;
        //newly generated the platforms should also have ability to fit view change
        connect(doodoo, SIGNAL(view_down(double)), platform, SLOT(fall(double)));
        scene()->addItem(platform);
        platform->show();
    }
    initbottom=625;//recover constant

    //showing of some button
    game_score->setHtml("<font color = red >Score: 0</font>");
    restart_button->show();
    pause_button->show();
    backmenu_button->show();
    game_score->show();

    //view change & game over
    connect(doodoo, SIGNAL(view_down(double)), this, SLOT(addNew()));
    connect(doodoo,SIGNAL(ggwp()),this,SLOT(game_over()));

    //timer for updating of score
    timer2 = new QTimer;
    QObject::connect(timer2,SIGNAL(timeout()),this,SLOT(update_score()));
    timer2->start(5);
}

//update score
void DoodleView::update_score() {
    game_score->setHtml(tr("<font color = red>%1</font>").arg(doodoo->score));
    game_score->show();
}

//game over when doodler falls out of screen
//stop timer is the key--similar to "pause"
void DoodleView::game_over() {
    doodoo->stop_timer();
    pause_button->hide();
    backmenu_button->hide();
    game_over_text->show();
    restart_button->setPos(120, 200);
    exit_button->show();
    shade_widget->show();
}

//pause and recover--start and stop of the timer of doodler
void DoodleView::pause_game()
{
    timer2->stop();
    doodoo->stop_timer();
    restart_button->hide();
    pause_button->hide();
    backmenu_button->hide();
    shade_widget->show();
    pause_text->show();
    return_button->show();
}

void DoodleView::return_game()
{
    timer2->start(5);
    doodoo->startTimer(25);
    doodoo->setFocus();
    return_button->hide();
    pause_text->hide();
    shade_widget->hide();
    restart_button->show();
    pause_button->show();
    backmenu_button->show();
}

//restart if gameover or just want to restart | or faces some unexpexted error |
void DoodleView::restart_game()
{
    shade_widget->hide();
    game_over_text->hide();
    exit_button->hide();
    restart_button->setPos(130, 0);
    doodoo->stop_timer();
    //clear all platforms and doodler
    QList<QGraphicsItem *> itemList = scene()->items(-200,25,600,800,Qt::ContainsItemShape,Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList) {
        scene()->removeItem(item);
    }
    init_game();
}

//back to main menu and also restart game--heading for ones who want to change mode
void DoodleView::back_to_menu()
{
    game_over_text->hide();
    doodoo->stop_timer();
    //stop timer or score will still show
    timer2->stop();
    game_score->hide();
    //clear all platforms and doodler
    QList<QGraphicsItem *> itemList = scene()->items(-200,25,600,800,Qt::ContainsItemShape,Qt::AscendingOrder);
    foreach (QGraphicsItem *item, itemList) {
        scene()->removeItem(item);
    }
    init_view();
}



//set three modes by clicking on certain button
//transfer a value--mode and change background
void DoodleView::setMode1(){
    mode=1;
    bg=QPixmap(":/image/chrisbg.jpg");
}
void DoodleView::setMode2(){
    mode=2;
    bg=QPixmap(":/image/junglebg.jpg");
}
void DoodleView::setMode3(){
    mode=3;
    bg=QPixmap(":/image/spacebg.jpg");
}

//add new platforms after view change
void DoodleView::addNew() {
    for (int i =0;i<5;i++){
        int a = randomInteger(0,290);
        int b = randomInteger(14,17);
        //set fisrt top one's y as a constant=inittop=50;
        Platform *platform = new Platform(a,inittop);
        //the next add 70~85 distance ensure the interval is smaller that one jump (around 90)
        inittop += 5*b;
        //newly generated the platforms should also have ability to fit view change
        connect(doodoo, SIGNAL(view_down(double)), platform, SLOT(fall(double)));
        scene()->addItem(platform);
        platform->show();
    }
    //recover this constant's value for next time's generation
    inittop=50;
}




