#ifndef REGISTER_H
#define REGISTER_H

#include "user.h"
#include <QWidget>
#include <map>
#include <QVBoxLayout>
#include <QSplitter>
#include <QScrollArea>

namespace Ui {
class Register;
}

class Register : public QWidget
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    void loadBackground();
    void loadIcon();
    User loadUserInfo(QString username);
    void saveUserInfo();
    void setMenuUI();
    void setSelectLevelUI();
    void setScoreBoardUI();
    void setConstructUI();
    std::map<int, User> loadAllUsers();
    ~Register();
    User getUser();
    void setUser(User newUser);
    void addCustomLevelPrototype(QString name);
    void loadCustomLevelPrototypes();
private slots:
    void on_loginButton_clicked();

    void on_registerButton_clicked();

    void on_scoreBoardButton_clicked();

    void on_level1Button_clicked();
    void on_level2Button_clicked();
    void on_level3Button_clicked();
    void on_level4Button_clicked();
    void on_level5Button_clicked();
    void on_level6Button_clicked();
    void on_level7Button_clicked();
    void on_level8Button_clicked();
    void on_level9Button_clicked();
    void on_level10Button_clicked();

    void on_backButton_clicked();   //back to main menu
    void on_newButton_clicked();
    void on_deleteAllButton_clicked();
    void on_randomButton_clicked();

    void on_deleteButtonCustomLevel_clicked();
    void on_editButtonCustomLevel_clicked();
    void on_playButtonCustomLevel_clicked();

private:
    Ui::Register *ui;
    User user;
    QSplitter* customLevelsSplitter = nullptr;
    QScrollArea* scrollAreaCustomLevel = nullptr;
};

#endif // REGISTER_H
