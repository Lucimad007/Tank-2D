#ifndef USER_H
#define USER_H
#include <QString>

class User
{
public:
    User();
    User(QString username, int highScore, int gamesPlayed);
    const QString &getUsername() const;
    void setUsername(const QString &newUsername);

    int getHighScore() const;
    void setHighScore(int newHighScore);

    int getGamesPlayed() const;
    void setGamesPlayed(int newGamesPlayed);


private:
    QString username;
    int highScore;
    int gamesPlayed;
};

#endif // USER_H
