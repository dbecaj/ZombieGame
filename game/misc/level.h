#ifndef LEVEL_H
#define LEVEL_H

#include <QString>
#include <QList>
#include <QPointF>

class Game;

enum class FileType
{
    INVALID,
    TEXT,
    BINARY
};

class Level
{
    QList<QString> levelData;
    bool isInitialized; // Keeps track if the level is initialized

    void readFromTxt(QString path); // Read from txt file and fill the levelData with data
    void readFromLvl(QString path); // Read from lvl file and fill the levelData with data
public:
    Level();
    void init(QString levelPath);
    void init(QList<QString> levelData);
    void save(QString levelPath); // Saves the level to a levelPath in .dat format

    void loadLevel(Game* game); // Load the game,decide the player spawn,spawn zombies and populate the game with tiles
    QList<QString> getLevelData() { return levelData; }

    QPointF playerStartPos;

    float levelWidth;
    float levelHeight;
};

#endif // LEVEL_H
