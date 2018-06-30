#include "level.h"

#include <fstream>
#include <QMessageBox>
#include <QPixmap>
#include <QDebug>
#include <random>
#include <ctime>

#include "tile.h"
#include "game/game.h"
#include "core/window.h"
#include "game/entities/mobs/zombie.h"
#include "game/game.h"
#include "game/entities/mobs/scientist.h"

extern Window* w;
Level::Level()
{
    isInitialized = false;
}

void Level::init(QString levelPath)
{
    levelData.clear();

    // We can only load .txt or .lvl files
    FileType fileType = FileType::INVALID;
    QStringList list = levelPath.split(".");
    if(list.last() == "txt") fileType = FileType::TEXT;
    else if(list.last() == "lvl") fileType = FileType::BINARY;

    // Load the data into levelData or write out the error
    if(fileType == FileType::TEXT) readFromTxt(levelPath);
    else if(fileType == FileType::BINARY) readFromLvl(levelPath);
    else
    {
        QString text = "Level: " + levelPath + " is the wrong file type!";
        QMessageBox::warning(w, "Level load error", text, QMessageBox::Ok);
        w->close();
        return;
    }

    // After all that level is initialized and can be loaded
    isInitialized = true;
}

void Level::init(QList<QString> levelData)
{
    // Or we can just do it directly
    this->levelData = levelData;

    isInitialized = true;
}

void Level::readFromTxt(QString path)
{
    std::fstream level;
    level.open(path.toStdString(), std::ios::in);
    if(!level.is_open())
    {
        QString text = "Level: " + path + " could not be loaded!";
        QMessageBox::warning(w, "Level load error", text, QMessageBox::Ok);
        w->close();
        return;
    }

    std::string line;
    while(std::getline(level, line))
        levelData.push_back(QString::fromStdString(line));

    level.close();
}

void Level::readFromLvl(QString path)
{
    std::fstream level;
    level.open(path.toStdString(), std::ios::binary | std::ios::in);
    if(!level.is_open())
    {
        QString text = "Level: " + path + " could not be loaded!";
        QMessageBox::warning(w, "Level load error", text, QMessageBox::Ok);
        w->close();
        return;
    }

    // [listSize][lineSize](data)[lineSize](data).....
    int listLength;
    level.read((char*)&listLength, sizeof(int));
    levelData.reserve(listLength);
    int lineLength;
    while(level.read((char*)&lineLength, sizeof(int)))
    {
          std::string line;
          line.resize(lineLength);
          level.read((char*)&line[0], lineLength);

          levelData.push_back(QString::fromStdString(line));
    }

    level.close();
}

void Level::save(QString levelPath)
{
    if(!isInitialized)
    {
        qDebug() << "level is not initialized!";
        return;
    }

    std::fstream level;
    level.open(levelPath.toStdString(), std::ios::out | std::ios::binary);
    level.close();
    level.open(levelPath.toStdString(), std::ios::out | std::ios::binary);

    int listLength = levelData.length();
    level.write((char*)&listLength, sizeof(int));
    for(int i = 0; i < levelData.length(); i++)
    {
        int lineLength = levelData[i].length();
        level.write((char*)&lineLength, sizeof(int));
        level.write(levelData[i].toStdString().c_str(), lineLength);
    }

    // [listSize][lineSize](data)[lineSize](data).....

    level.close();
}

void Level::loadLevel(Game* game)
{
    if(!isInitialized)
    {
        qDebug() << "Level is not initialized!";
        return;
    }

    // Go through each character in the levelData and load the appropriate tile,save spawn a position or spawn a zombie
    int y = 0;
    int x = 0;
    for(y = 0; y < levelData.size(); y++)
    {
        for(x = 0; x < levelData[y].length(); x++)
        {
            switch(levelData[y][x].toLatin1())
            {
            case 'W':
            {
                Tile* wall = new Tile(QPixmap(":/images/textures/wall_tile.png"), QPointF(x * TILE_SIZE, y * TILE_SIZE), TileType::WALL);
                game->tiles.push_back(wall);
                game->addItem(wall);
                break;
            }
            case 'B':
            {
                Tile* box = new Tile(QPixmap(":/images/textures/box_tile.png"), QPointF(x * TILE_SIZE, y * TILE_SIZE), TileType::BOX);
                game->tiles.push_back(box);
                game->addItem(box);
                break;
            }
            case 'C':
            {
                Tile* computor = new Tile(QPixmap(":/images/textures/computor_tile.png"), QPointF(x * TILE_SIZE, y * TILE_SIZE), TileType::COMPUTER);
                game->tiles.push_back(computor);
                game->addItem(computor);
                break;
            }
            case 'F':
            {
                Tile* bookshelf = new Tile(QPixmap(":/images/textures/bookshelf_tile.png"), QPointF(x * TILE_SIZE, y * TILE_SIZE), TileType::BOOKSHELF);
                game->tiles.push_back(bookshelf);
                game->addItem(bookshelf);
                break;
            }

            case 'P':
            {
                playerStartPos = QPointF(x * TILE_SIZE, y * TILE_SIZE);
                break;
            }
            case 'Z':
            {
                if(game->getSceneType() == SceneType::MULTIPLAYER_GAME) break;
                Zombie* zombie = new Zombie(QPointF(x * TILE_SIZE, y * TILE_SIZE));
                game->zombies.push_back(zombie);
                game->addItem(zombie);
                break;
            }
            case 'S':
            {
                if(game->getSceneType() == SceneType::MULTIPLAYER_GAME) break;
                static std::mt19937 randEngine(time(nullptr));
                std::uniform_int_distribution<int> randGender(0, 1);
                ScientistGender gender = ScientistGender::MALE;
                if(randGender(randEngine)) gender = ScientistGender::FEMALE;
                Scientist* scientist = new Scientist(gender, QPointF(x * TILE_SIZE, y * TILE_SIZE));
                game->scientists.push_back(scientist);
                game->addItem(scientist);
                break;
            }
            default:
            {
                break;
            }
            }
        }
    }

    levelWidth = x * TILE_SIZE;
    levelHeight = y * TILE_SIZE;
}
