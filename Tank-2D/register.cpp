#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVBoxLayout>
#include <QtUiTools/QUiLoader>
#include <QTimer>
#include "register.h"
#include "ui_register.h"
#include "menu_event.h"
#include <game.h>

QTimer *timer;
Game* game;

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("Tank Battle City");
    this->setFixedSize(this->width(), this->height());
    loadBackground();
    loadIcon();
}

void Register::loadBackground(){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Arts/signUpBack.png";
    qDebug() << path;
    QIcon background(path);
    if(!background.isNull())
    {
        QPixmap pixMap = background.pixmap(ui->backgroundView->width(),ui->backgroundView->height());
        QGraphicsPixmapItem* pixMapItem = new QGraphicsPixmapItem(pixMap);
        QGraphicsScene* scene = new QGraphicsScene();
        scene->addItem(pixMapItem);
        ui->backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        ui->backgroundView->setScene(scene);
    } else
    {
        qDebug() << "Background image not found.";
    }
}

void Register::loadIcon(){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Arts/signUpBack.png";
    QIcon icon(path);
    if(!icon.isNull())
    {
        this->setWindowIcon(icon);
    } else
    {
        qDebug() << "Icon not found.";
    }
}

User Register::loadUserInfo(QString username){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Database/info-of-users.txt";
    QFile file(path);
    if(file.exists())
    {
        if(!file.open(QIODevice::OpenModeFlag::ReadOnly))
            qDebug() << "File not found.";
        QJsonObject json = QJsonObject();
        QByteArray bytes = file.readAll();
        QJsonDocument document = QJsonDocument::fromJson(bytes);
        json = document.object();
        file.close();

        for(auto it = json.begin(); it != json.end(); ++it){
            if(it.key() == username)
            {
                QJsonObject value = it.value().toObject();
                int highestScore = value["highestScore"].toInt();
                int gamesPlayed = value["gamesPlayed"].toInt();
                User user(username, highestScore, gamesPlayed);
                return user;
            }
        }
    } else
    {
        qDebug() << "File not found.";
    }

    return User(username, 0, 0);
}

void Register::saveUserInfo(){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Database/info-of-users.txt";
    QFile file(path);
    if(file.exists())
    {
        if(!file.open(QIODevice::OpenModeFlag::ReadOnly))
            qDebug() << "File not found.";
        QJsonObject json = QJsonObject();
        QByteArray bytes = file.readAll();
        QJsonDocument document = QJsonDocument::fromJson(bytes);
        json = document.object();

        file.close();

        for(auto it = json.begin(); it != json.end(); ++it)
        {
            if(it.key() == user.getUsername())
            {
                //it.value().toObject()     is info of our target user
                QJsonObject targetUser = it.value().toObject();
                targetUser["highestScore"] = user.getHighScore();
                targetUser["gamesPlayed"] = user.getGamesPlayed();
                json.erase(it);
                json.insert(user.getUsername(), targetUser);
                if(!file.open(QIODevice::OpenModeFlag::ReadWrite))
                    qDebug() << "File not found.";
                document = QJsonDocument(json);
                file.write(document.toJson());
                file.close();
                return;
            }
        }

        //if user was not in the file
        QJsonObject targetUser;
        targetUser["highestScore"] = user.getHighScore();
        targetUser["gamesPlayed"] = user.getGamesPlayed();
        json.insert(user.getUsername(), targetUser);
        if(!file.open(QIODevice::OpenModeFlag::ReadWrite))
            qDebug() << "File not found.";
        document = QJsonDocument(json);
        file.write(document.toJson());
        file.close();
        return;

    } else
    {
        qDebug() << "File not found.";
    }
}

void Register::setMenuUI(){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D";
    QUiLoader loader;
    QFile file(path + "/menu.ui");
    if(file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        QWidget* widget = loader.load(&file);
        ui->setupUi(this);      //it heavily prevents our code from bugs
        this->setWindowTitle("Tank Battle City");
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setContentsMargins(0 ,0 ,0 ,0);     //now it fill the whole background
        layout->addWidget(widget);
        delete this->layout();      //clearing previous layout
        this->setLayout(layout);

        //loading graphic views
        QGraphicsView* logoView = widget->findChild<QGraphicsView*>("logoView",Qt::FindChildrenRecursively);
        QGraphicsView* player1View = widget->findChild<QGraphicsView*>("player1View",Qt::FindChildrenRecursively);
        QGraphicsView* player2View = widget->findChild<QGraphicsView*>("player2View",Qt::FindChildrenRecursively);
        QGraphicsView* constructionView = widget->findChild<QGraphicsView*>("constructionView",Qt::FindChildrenRecursively);
        QPushButton* scoreBoardButton = widget->findChild<QPushButton*>("scoreBoardButton", Qt::FindChildrenRecursively);
        if(!(logoView->isWidgetType()) || !(player1View->isWidgetType()) || !(player2View->isWidgetType()) || !(constructionView->isWidgetType()))
        {
            qDebug() << "Failed to find children widget.";
            return;
        }
        //installing events to make our menu responsive
        MenuEvent* menuEvent = new MenuEvent();
        logoView->installEventFilter(menuEvent);
        player1View->installEventFilter(menuEvent);
        player2View->installEventFilter(menuEvent);
        constructionView->installEventFilter(menuEvent);

        //connecting to slot
        connect(scoreBoardButton, &QPushButton::clicked, this, &Register::on_scoreBoardButton_clicked);

        //disabling scrolls
        logoView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        logoView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        player1View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        player1View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        player2View->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        player2View->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        constructionView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        constructionView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

        //loading pictures
        QIcon logo(path + "/Arts/beforeStart.png");
        QPixmap pixmapLogo = logo.pixmap(logoView->width(), logoView->height());
        QGraphicsPixmapItem* pixItemLogo = new QGraphicsPixmapItem(pixmapLogo);
        QGraphicsScene* sceneLogo = new QGraphicsScene();
        sceneLogo->addItem(pixItemLogo);
        logoView->setScene(sceneLogo);

        QIcon player1(path + "/Arts/1-player.png");     //player1 is intended for 1 player (we can not declare 1player identifier)
        QPixmap pixmapPlayer1 = player1.pixmap(player1View->width(), player1View->height());
        QGraphicsPixmapItem* pixItemPlayer1 = new QGraphicsPixmapItem(pixmapPlayer1);
        QGraphicsScene* scenePlayer1 = new QGraphicsScene();
        scenePlayer1->addItem(pixItemPlayer1);
        player1View->setScene(scenePlayer1);

        QIcon player2(path + "/Arts/2-players.png");    //player2 is intended for 2 players (we can not declare 2players identifier)
        QPixmap pixmapPlayer2 = player2.pixmap(player2View->width(), player2View->height());
        QGraphicsPixmapItem* pixItemPlayer2 = new QGraphicsPixmapItem(pixmapPlayer2);
        QGraphicsScene* scenePlayer2 = new QGraphicsScene();
        scenePlayer2->addItem(pixItemPlayer2);
        player2View->setScene(scenePlayer2);

        QIcon construction(path + "/Arts/construction.png");
        QPixmap pixmapConstruction = construction.pixmap(constructionView->width(), constructionView->height());
        QGraphicsPixmapItem* pixItemConstruction = new QGraphicsPixmapItem(pixmapConstruction);
        QGraphicsScene* sceneConstruction = new QGraphicsScene();
        sceneConstruction->addItem(pixItemConstruction);
        constructionView->setScene(sceneConstruction);
        //end of loading pictures
    } else
    {
        qDebug() << "Failed to open file.";
    }

}

void Register::setSelectLevelUI(){
    //loading ui file
    QWidget* widget;
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D";
    QUiLoader loader;
    QFile file(path + "/select-level.ui");
    if(file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        widget = loader.load(&file);
        ui->setupUi(this);      //it heavily prevents our code from bugs
        this->setWindowTitle("Tank Battle City");
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setContentsMargins(0 ,0 ,0 ,0);     //now it fills the whole background
        layout->addWidget(widget);
        delete this->layout();      //clearing previous layout
        this->setLayout(layout);

    } else
    {
        qDebug() << "Failed to open file.";
    }

    //loading background
    path = info.dir().path();
    path += "/Tank-2D/Arts/selectLevelBack.png";
    QIcon background(path);
    if(!background.isNull())
    {
        QGraphicsView* backgroundView = widget->findChild<QGraphicsView*>("backgroundView",Qt::FindChildrenRecursively);
        QPixmap pixMap = background.pixmap(backgroundView->width()*3/2, backgroundView->height()*3/2);
        QGraphicsPixmapItem* pixMapItem = new QGraphicsPixmapItem(pixMap);
        QGraphicsScene* scene = new QGraphicsScene();
        scene->addItem(pixMapItem);
        backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        backgroundView->setScene(scene);
    } else
    {
        qDebug() << "Background image not found.";
    }

    //connecting slots for each button
    QPushButton* level1Button = widget->findChild<QPushButton*>("level1Button", Qt::FindChildrenRecursively);
    QPushButton* level2Button = widget->findChild<QPushButton*>("level2Button", Qt::FindChildrenRecursively);
    QPushButton* level3Button = widget->findChild<QPushButton*>("level3Button", Qt::FindChildrenRecursively);
    QPushButton* level4Button = widget->findChild<QPushButton*>("level4Button", Qt::FindChildrenRecursively);
    QPushButton* level5Button = widget->findChild<QPushButton*>("level5Button", Qt::FindChildrenRecursively);
    QPushButton* level6Button = widget->findChild<QPushButton*>("level6Button", Qt::FindChildrenRecursively);
    QPushButton* level7Button = widget->findChild<QPushButton*>("level7Button", Qt::FindChildrenRecursively);
    QPushButton* level8Button = widget->findChild<QPushButton*>("level8Button", Qt::FindChildrenRecursively);
    QPushButton* level9Button = widget->findChild<QPushButton*>("level9Button", Qt::FindChildrenRecursively);
    QPushButton* level10Button = widget->findChild<QPushButton*>("level10Button", Qt::FindChildrenRecursively);
    connect(level1Button, &QPushButton::clicked, this, &Register::on_level1Button_clicked);
    connect(level2Button, &QPushButton::clicked, this, &Register::on_level2Button_clicked);
    connect(level3Button, &QPushButton::clicked, this, &Register::on_level3Button_clicked);
    connect(level4Button, &QPushButton::clicked, this, &Register::on_level4Button_clicked);
    connect(level5Button, &QPushButton::clicked, this, &Register::on_level5Button_clicked);
    connect(level6Button, &QPushButton::clicked, this, &Register::on_level6Button_clicked);
    connect(level7Button, &QPushButton::clicked, this, &Register::on_level7Button_clicked);
    connect(level8Button, &QPushButton::clicked, this, &Register::on_level8Button_clicked);
    connect(level9Button, &QPushButton::clicked, this, &Register::on_level9Button_clicked);
    connect(level10Button, &QPushButton::clicked, this, &Register::on_level10Button_clicked);
}

void Register::setScoreBoardUI(){
    //loading ui file
    QWidget* widget;
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D";
    QUiLoader loader;
    QFile file(path + "/score-board.ui");
    if(file.open(QIODevice::OpenModeFlag::ReadOnly))
    {
        widget = loader.load(&file);
        ui->setupUi(this);      //it heavily prevents our code from bugs
        this->setWindowTitle("Tank Battle City");
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setContentsMargins(0 ,0 ,0 ,0);     //now it fills the whole background
        layout->addWidget(widget);
        delete this->layout();      //clearing previous layout
        this->setLayout(layout);
    } else
    {
        qDebug() << "Failed to open file.";
    }
}

Register::~Register()
{
    delete ui;
}

void Register::on_loginButton_clicked()
{
    ui->warningLabel->setText("");
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Database/registered-users.txt";
    QFile file(path);
    if(file.exists())
    {
        if(!file.open(QIODevice::OpenModeFlag::ReadOnly))
            qDebug() << "File not found.";
        QJsonObject json = QJsonObject();
        QByteArray bytes = file.readAll();
        QJsonDocument document = QJsonDocument::fromJson(bytes);
        json = document.object();
        file.close();
        QString username = ui->usernameSigninLineEdit->text();
        QString password = ui->passwordSigninLineEdit->text();
        ui->usernameSigninLineEdit->setText("");
        ui->passwordSigninLineEdit->setText("");
        if(username == "")
        {
            ui->warningLabel->setText("invalid username");
            return;
        } else if(password == "")
        {
            ui->warningLabel->setText("invalid password");
            return;
        }

        for(auto it = json.begin(); it != json.end(); ++it){
            if(it.key() == username)
                if(it.value().toString() == password)
                {
                    user = loadUserInfo(username);
                    setMenuUI();
                    return;
                }
        }
        //failed to sign in
        ui->warningLabel->setText("wrong information");
    } else
    {
        qDebug() << "File not found.";
    }
}


void Register::on_registerButton_clicked()
{
    ui->warningLabel->setText("");
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/Database/registered-users.txt";
    QFile file(path);
    if(file.exists())
    {
        if(!file.open(QIODevice::OpenModeFlag::ReadOnly))
            qDebug() << "File not found.";
        QJsonObject json = QJsonObject();
        QByteArray bytes = file.readAll();
        QJsonDocument document = QJsonDocument::fromJson(bytes);
        json = document.object();

        file.close();
        QString username = ui->usernameSignupLineEdit->text();
        QString password = ui->passwordSignupLineEdit->text();
        ui->usernameSignupLineEdit->setText("");
        ui->passwordSignupLineEdit->setText("");
        if(username == "")
        {
            ui->warningLabel->setText("invalid username");
            return;
        } else if(password == "")
        {
            ui->warningLabel->setText("invalid password");
            return;
        }

        for(auto it = json.begin(); it != json.end(); ++it)
        {
            if(it.key() == username)
            {
                ui->warningLabel->setText("already exits");
                return;
            }
        }

        if(!file.open(QIODevice::OpenModeFlag::ReadWrite))
            qDebug() << "File not found.";
        json.insert(username, password);
        document = QJsonDocument(json);
        file.write(document.toJson());
        file.close();

    } else
    {
        qDebug() << "File not found.";
    }
}

void Register::on_scoreBoardButton_clicked(){
    setScoreBoardUI();
}

void Register::on_level1Button_clicked(){
    game = new Game(1);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level2Button_clicked(){
    game = new Game(2);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level3Button_clicked(){
    game = new Game(3);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level4Button_clicked(){
    game = new Game(4);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level5Button_clicked(){
    game = new Game(5);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level6Button_clicked(){
    game = new Game(6);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level7Button_clicked(){
    game = new Game(7);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level8Button_clicked(){
    game = new Game(8);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level9Button_clicked(){
    game = new Game(9);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

void Register::on_level10Button_clicked(){
    game = new Game(10);
    game->show();

    timer = new QTimer();
    QTimer::connect(timer, &QTimer::timeout, [&](){
        game->clear();
        game->updateLogic();
        game->render();
    });
    timer->start(((float)1/game->getFPS() * 1000));

    this->hide();
}

User Register::getUser()
{
    return user;
}

void Register::setUser(User newUser)
{
    user = newUser;
}
