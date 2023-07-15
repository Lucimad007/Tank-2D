#include "user.h"

User::User()
{
    username = "";
    gamesPlayed = 0;
    highScore = 0;
}

User::User(QString username, int highScore, int gamesPlayed){
    this->username = username;
    this->highScore = highScore;
    this->gamesPlayed = gamesPlayed;
}

const QString &User::getUsername() const
{
    return username;
}

void User::setUsername(const QString &newUsername)
{
    username = newUsername;
}

int User::getHighScore() const
{
    return highScore;
}

void User::setHighScore(int newHighScore)
{
    highScore = newHighScore;
}

int User::getGamesPlayed() const
{
    return gamesPlayed;
}

void User::setGamesPlayed(int newGamesPlayed)
{
    gamesPlayed = newGamesPlayed;
}
