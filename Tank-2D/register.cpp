#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVBoxLayout>
#include <QtUiTools/QUiLoader>
#include "register.h"
#include "ui_register.h"
#include "menu_event.h"

Register::Register(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    this->setWindowTitle("Sign Up");
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
        this->setWindowTitle("Menu");
        QVBoxLayout* layout = new QVBoxLayout();
        layout->setContentsMargins(0 ,0 ,0 ,0);     //now it fill the whole background
        layout->addWidget(widget);
        this->setLayout(layout);

        //loading graphic views
        QGraphicsView* logoView = widget->findChild<QGraphicsView*>("logoView",Qt::FindChildrenRecursively);
        QGraphicsView* player1View = widget->findChild<QGraphicsView*>("player1View",Qt::FindChildrenRecursively);
        QGraphicsView* player2View = widget->findChild<QGraphicsView*>("player2View",Qt::FindChildrenRecursively);
        QGraphicsView* constructionView = widget->findChild<QGraphicsView*>("constructionView",Qt::FindChildrenRecursively);
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

