#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QJsonObject>
#include <QJsonDocument>
#include "register.h"
#include "ui_register.h"

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
                    //show next menu
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

