#ifndef REGISTER_H
#define REGISTER_H

#include "user.h"
#include <QWidget>
#include <map>

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
    std::map<int, User> loadAllUsers();
    ~Register();

    User getUser();
    void setUser(User newUser);
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

private:
    Ui::Register *ui;
    User user;
};

#endif // REGISTER_H
