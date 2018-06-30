#ifndef MUSICMANAGER_H
#define MUSICMANAGER_H

#include <QMediaPlayer>
#include <QMediaPlaylist>

enum class MusicType
{
    MAIN_MENU,
    LEVEL_CREATOR
};

class MusicManager
{
    static QMediaPlayer mediaPlayer;
public:
    MusicManager();

    static void play(MusicType type);
    static void stop();
};

#endif // MUSICMANAGER_H
