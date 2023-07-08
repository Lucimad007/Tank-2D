#ifndef REGISTER_H
#define REGISTER_H

#include <QWidget>

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
    void setMenuUI();
    void setSelectLevelUI();
    ~Register();

private slots:
    void on_loginButton_clicked();

    void on_registerButton_clicked();

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
};

#endif // REGISTER_H
