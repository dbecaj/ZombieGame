#include "musicmanager.h"

#include <QCoreApplication>

MusicManager::MusicManager()
{
}

QMediaPlayer MusicManager::mediaPlayer;

void MusicManager::play(MusicType type)
{
    stop();
    if(type == MusicType::MAIN_MENU) mediaPlayer.setMedia(QMediaContent(QUrl("qrc:/music/music/Defense Line.mp3")));
    else if(type == MusicType::LEVEL_CREATOR) mediaPlayer.setMedia(QMediaContent(QUrl("qrc:/music/music/80s Things.mp3")));
    mediaPlayer.setVolume(50);
    mediaPlayer.play();
}

void MusicManager::stop()
{
    mediaPlayer.stop();
}
