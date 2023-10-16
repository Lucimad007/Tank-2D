#include "construction.h"
#include "ui_construction.h"
#include "blocks-event-filter.h"
#include "register.h"
#include <QDir>
#include <QMouseEvent>
#include <QJsonObject>
#include <QJsonDocument>

extern Register* registerMenu;
extern QApplication* app;

Construction::Construction(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::construction)
{
    spriteLoader = new SpriteLoader();
    currentObject = GameObject(BRICK, spriteLoader->getBrick());
    ui->setupUi(this);
    for(int i = 0; i < 25; i++)
        for(int j = 0; j < 20; j++)
            blocks[i][j] = 'X';
    loadIcon();
    this->setWindowTitle("Tank Battle City");
    this->setFixedSize(this->width(), this->height());
    scene = new QGraphicsScene();
    backgroundView = new QGraphicsView(this);
    backgroundView->setFixedSize(QSize(WIDTH, HEIGHT));
    backgroundView->setStyleSheet("background-color: black;");
    backgroundView->setScene(scene);
    //we should ensure that scene fills the whole QGraphicsView
    scene->setSceneRect(backgroundView->rect());

    //disable scrollbars if you don't want them to appear
    backgroundView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    backgroundView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //installing event filters
    BlocksEventFilter* eventFilter = new BlocksEventFilter();
    ui->playerView->installEventFilter(eventFilter);
    ui->flagView->installEventFilter(eventFilter);
    ui->commonTankView->installEventFilter(eventFilter);
    ui->armoredTankView->installEventFilter(eventFilter);
    ui->randomCommonTankView->installEventFilter(eventFilter);
    ui->randomArmoredTankView->installEventFilter(eventFilter);
    ui->brickView->installEventFilter(eventFilter);
    ui->waterView->installEventFilter(eventFilter);
    ui->stoneView->installEventFilter(eventFilter);
    ui->crossView->installEventFilter(eventFilter);

    //setting sidebar images
    QPixmap pixmap = spriteLoader->getYellow_tank_up();
    pixmap = pixmap.scaled(ui->playerView->width(), ui->playerView->height());
    QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
    QGraphicsScene* tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->playerView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->playerView->setScene(tempScene);

    pixmap = spriteLoader->getFlag();
    pixmap = pixmap.scaled(ui->flagView->width(), ui->flagView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->flagView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->flagView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->flagView->setScene(tempScene);

    pixmap = spriteLoader->getCommon_tank_up();
    pixmap = pixmap.scaled(ui->commonTankView->width(), ui->commonTankView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->commonTankView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->commonTankView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->commonTankView->setScene(tempScene);

    pixmap = spriteLoader->getRandom_tank_up();
    pixmap = pixmap.scaled(ui->randomCommonTankView->width(), ui->randomCommonTankView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->randomCommonTankView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->randomCommonTankView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->randomCommonTankView->setScene(tempScene);

    pixmap = spriteLoader->getArmored_random_tank_up();
    pixmap = pixmap.scaled(ui->randomArmoredTankView->width(), ui->randomArmoredTankView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->randomArmoredTankView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->randomArmoredTankView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->randomArmoredTankView->setScene(tempScene);

    pixmap = spriteLoader->getArmored_tank_up();
    pixmap = pixmap.scaled(ui->armoredTankView->width(), ui->armoredTankView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->armoredTankView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->armoredTankView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->armoredTankView->setScene(tempScene);

    pixmap = spriteLoader->getBrick();
    pixmap = pixmap.scaled(ui->brickView->width(), ui->brickView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->brickView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->brickView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->brickView->setScene(tempScene);

    pixmap = spriteLoader->getWater();
    pixmap = pixmap.scaled(ui->waterView->width(), ui->waterView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->waterView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->waterView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->waterView->setScene(tempScene);

    pixmap = spriteLoader->getStone();
    pixmap = pixmap.scaled(ui->stoneView->width(), ui->stoneView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->stoneView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->stoneView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->stoneView->setScene(tempScene);

    pixmap = spriteLoader->getCross();
    pixmap = pixmap.scaled(ui->crossView->width(), ui->crossView->height());
    item = new QGraphicsPixmapItem(pixmap);
    tempScene = new QGraphicsScene();
    tempScene->addItem(item);
    ui->crossView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->crossView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->crossView->setScene(tempScene);
}

void Construction::loadIcon(){
    QIcon icon(":/Arts/signUpBack.png");
    if(!icon.isNull())
    {
        this->setWindowIcon(icon);
    } else
    {
        qDebug() << "Icon not found.";
    }
}

void Construction::loadMap(QString name){
    currentObject = GameObject(NONE_GAME_OBJECT, spriteLoader->getYellow_tank_up());
    char positions[25][20];
    //loading file
    QDir directory;
    if(!directory.exists("custom-levels"))
        directory.mkdir("custom-levels");
    QFile file("custom-levels/" + name + ".txt");
    QFile jsonFile("custom-levels/" + name + ".json");
    //loading json file
    if(jsonFile.exists()){
        jsonFile.open(QIODevice::OpenModeFlag::ReadOnly);
        QByteArray data = jsonFile.readAll();
        QJsonDocument doc = QJsonDocument::fromJson(data);
        QJsonObject json = doc.object();
        setHasTanki(json["hasTanki"].toBool());
        setHasStar(json["hasStar"].toBool());
        setHasClock(json["hasClock"].toBool());
    } else {
        qDebug() << "Json File Does Not Exist.";
    }
    //loading file
    if(file.exists())
    {
        QString data;
        QByteArray temp;
        file.open(QIODevice::OpenModeFlag::ReadOnly);
        while(!file.atEnd())
        {
           char c;
           file.getChar(&c);
           if((c == '\n') || (c == '\b') || (c == '\r'))
               continue;
           data += c;
        }

        for(int i = 0; i < 20; i++)
            for(int j = 0; j < 25; j++)
            {
                QChar ch = data.at(25 * i + j);
                positions[j][i] = ch.unicode();
            }
    } else
    {
        qDebug() << "File not found.";
        return;
    }
    //end of loading file

    for(int i = 0; i < 20; i++)
    {
        for(int j = 0; j < 25; j++)
        {
           currentObject = GameObject(NONE_GAME_OBJECT, spriteLoader->getYellow_tank_up());
           blocks[j][i] = positions[j][i];

           if(positions[j][i] == 'B')
           {
                currentObject = GameObject(BRICK, spriteLoader->getBrick());;
           } else if(positions[j][i] == 'W')
           {
               currentObject = GameObject(WATER, spriteLoader->getWater());;
           } else if(positions[j][i] == 'M')
           {
               currentObject = GameObject(STONE, spriteLoader->getStone());;
           } else if(positions[j][i] == 'F')
           {
               currentObject = GameObject(FLAG, spriteLoader->getFlag());;
           } else if(positions[j][i] == 'C')
           {
               currentObject = GameObject(ARMORED_RANDOM_TANK, spriteLoader->getArmored_random_tank_down());
           } else if(positions[j][i] == 'c')
           {
               currentObject = GameObject(RANDOM_TANK, spriteLoader->getRandom_tank_down());;
           } else if(positions[j][i] == 'A')
           {
               currentObject = GameObject(ARMORED_TANK, spriteLoader->getArmored_tank_down());
           } else if(positions[j][i] == 'O')
           {
               currentObject = GameObject(COMMON_TANK, spriteLoader->getCommon_tank_down());
           } else if(positions[j][i] == 'P')
           {
               currentObject = GameObject(PLAYER, spriteLoader->getYellow_tank_up());
           }

           if(currentObject.getType() != NONE_GAME_OBJECT){
               QPixmap pixmap = currentObject.getSprite();
               pixmap = pixmap.scaled(cellSize, cellSize);
               QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
               item->setPos(j*cellSize, i * cellSize);
               scene->addItem(item);
               scene->update();
               backgroundView->update();
           }
        }
    }
}

Construction::~Construction()
{
    delete ui;
}

void Construction::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::MouseButton::RightButton){
        currentObject = GameObject(NONE_GAME_OBJECT, spriteLoader->getBlack());
        setDefaultCursor();
        return;
    }
    else if(event->button() == Qt::MouseButton::LeftButton){
        int x = event->position().x();
        int y = event->position().y();
        if(x < 0 || x > WIDTH )
            return;
        else if(y < 0 || y > HEIGHT)
            return;

        if(currentObject.getType() == NONE_GAME_OBJECT)
            return;

        if(currentObject.getType() == PLAYER){
            blocks[x/cellSize][y/cellSize] = 'P';
        } else if(currentObject.getType() == COMMON_TANK){
            blocks[x/cellSize][y/cellSize] = 'o';
        } else if(currentObject.getType() == ARMORED_TANK){
            blocks[x/cellSize][y/cellSize] = 'O';
        } else if(currentObject.getType() == RANDOM_TANK){
            blocks[x/cellSize][y/cellSize] = 'c';
        } else if(currentObject.getType() == ARMORED_RANDOM_TANK){
            blocks[x/cellSize][y/cellSize] = 'C';
        } else if(currentObject.getType() == FLAG){
            blocks[x/cellSize][y/cellSize] = 'F';
        } else if(currentObject.getType() == BRICK){
            blocks[x/cellSize][y/cellSize] = 'B';
        } else if(currentObject.getType() == WATER){
            blocks[x/cellSize][y/cellSize] = 'W';
        } else if(currentObject.getType() == STONE){
            blocks[x/cellSize][y/cellSize] = 'M';
        } else if(currentObject.getType() == CROSS){
            blocks[x/cellSize][y/cellSize] = 'X';
        } else {
            return;
        }

        if(currentObject.getType() == CROSS){
            QPixmap pixmap = spriteLoader->getBlack();
            pixmap = pixmap.scaled(cellSize, cellSize);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
            item->setPos(x/cellSize * cellSize, y/cellSize * cellSize);
            scene->addItem(item);
            scene->update();
            backgroundView->update();
        } else {
            QPixmap pixmap = currentObject.getSprite();
            pixmap = pixmap.scaled(cellSize, cellSize);
            QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
            item->setPos(x/cellSize * cellSize, y/cellSize * cellSize);
            scene->addItem(item);
            scene->update();
            backgroundView->update();
        }
    }
}

void Construction::saveToFile(QString name){
    QDir directory;
    if(!directory.exists("custom-levels"))
        directory.mkdir("custom-levels");
    QFile file(directory.absolutePath() + "/custom-levels/" + name + ".txt");
    QJsonObject json;
    QFile jsonFile(directory.absolutePath() + "/custom-levels/" + name + ".json");
    json["hasTanki"] = hasTanki;
    json["hasStar"] = hasStar;
    json["hasClock"] = hasClock;
    file.open(QIODevice::OpenModeFlag::WriteOnly);
    jsonFile.open(QIODevice::OpenModeFlag::WriteOnly);
    QJsonDocument doc = QJsonDocument(json);
    QByteArray data(doc.toJson());
    jsonFile.write(data);

    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 25; j++)
        {
            char* ch = &blocks[j][i];
            file.write(ch, 1);
        }

        file.write("\n");
    }
    file.close();
    jsonFile.close();
}

void Construction::on_saveButton_clicked()
{
    if(ui->nameLineEdit->text().isEmpty()){
        ui->statusLabel->setText("Invalid Name");
        return;
    }

    ui->statusLabel->setText("");
    saveToFile(ui->nameLineEdit->text());
    ui->nameLineEdit->setText("");
}

void inline Construction::setDefaultCursor(){
    QCursor cursor;
    app->setOverrideCursor(cursor);
}

void Construction::on_menuButton_clicked()
{
    this->close();
    setDefaultCursor();
    registerMenu->setMenuUI();
    registerMenu->show();
}


void Construction::on_tankiCheckBox_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked)
        hasTanki = true;
    else
        hasTanki = false;
}


void Construction::on_starCheckBox_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked)
        hasStar = true;
    else
        hasStar = false;
}


void Construction::on_clockCheckBox_stateChanged(int state)
{
    if(state == Qt::CheckState::Checked)
        hasClock = true;
    else
        hasClock = false;
}

bool Construction::getHasClock() const
{
    return hasClock;
}

void Construction::setHasClock(bool newHasClock)
{
    hasClock = newHasClock;
    if(hasClock)
    {
        ui->clockCheckBox->setChecked(true);
    }
}

bool Construction::getHasStar() const
{
    return hasStar;
}

void Construction::setHasStar(bool newHasStar)
{
    hasStar = newHasStar;
    if(hasStar)
    {
        ui->starCheckBox->setChecked(true);
    }
}

bool Construction::getHasTanki() const
{
    return hasTanki;
}

void Construction::setHasTanki(bool newHasTanki)
{
    hasTanki = newHasTanki;
    if(hasTanki)
    {
        ui->tankiCheckBox->setChecked(true);
    }

}

GameObject Construction::getCurrentObject() const
{
    return currentObject;
}

void Construction::setCurrentObject(GameObject newCurrentObject)
{
    currentObject = newCurrentObject;
}
