#include "construction.h"
#include "ui_construction.h"
#include "blocks-event-filter.h"
#include <QDir>
#include <QMouseEvent>

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
}

void Construction::loadIcon(){
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

Construction::~Construction()
{
    delete ui;
}

void Construction::mousePressEvent(QMouseEvent* event){
    if(event->button() == Qt::MouseButton::LeftButton){
        int x = event->position().x();
        int y = event->position().y();
        if(x < 0 || x > WIDTH )
            return;
        else if(y < 0 || y > HEIGHT)
            return;


        if(currentObject.getType() == PLAYER){
            blocks[x/cellSize][y/cellSize] = "P";
        } else if(currentObject.getType() == COMMON_TANK){
            blocks[x/cellSize][y/cellSize] = "o";
        } else if(currentObject.getType() == ARMORED_TANK){
            blocks[x/cellSize][y/cellSize] = "O";
        } else if(currentObject.getType() == RANDOM_TANK){
            blocks[x/cellSize][y/cellSize] = "c";
        } else if(currentObject.getType() == ARMORED_RANDOM_TANK){
            blocks[x/cellSize][y/cellSize] = "O";
        } else if(currentObject.getType() == FLAG){
            blocks[x/cellSize][y/cellSize] = "F";
        } else if(currentObject.getType() == BRICK){
            blocks[x/cellSize][y/cellSize] = "B";
        } else if(currentObject.getType() == WATER){
            blocks[x/cellSize][y/cellSize] = "W";
        } else if(currentObject.getType() == STONE){
            blocks[x/cellSize][y/cellSize] = "M";
        } else {
            return;
        }

        QPixmap pixmap = currentObject.getSprite();
        pixmap = pixmap.scaled(cellSize, cellSize);
        QGraphicsPixmapItem* item = new QGraphicsPixmapItem(pixmap);
        item->setPos(x/cellSize * cellSize, y/cellSize * cellSize);
        scene->addItem(item);
        scene->update();
        backgroundView->update();
    }
}

void Construction::saveToFile(QString name){
    QFileInfo info = QFileInfo(QDir::currentPath());
    QString path = info.dir().path();
    path += "/Tank-2D/custom-levels/";
    QFile file(path + name + ".txt");
    file.open(QIODevice::OpenModeFlag::WriteOnly);

    for(int i = 0; i < 20; i++){
        for(int j = 0; j < 25; j++)
        {
            const char* ch = blocks[j][i].toStdString().c_str();
            file.write(ch);
        }

        file.write("\n");
    }
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


void Construction::on_menuButton_clicked()
{

}


void Construction::on_tankiCheckBox_stateChanged(int arg1)
{

}


void Construction::on_starCheckBox_stateChanged(int arg1)
{

}


void Construction::on_clockCheckBox_stateChanged(int arg1)
{

}

GameObject Construction::getCurrentObject() const
{
    return currentObject;
}

void Construction::setCurrentObject(GameObject newCurrentObject)
{
    currentObject = newCurrentObject;
}
