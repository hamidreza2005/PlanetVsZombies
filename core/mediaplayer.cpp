#include "mediaplayer.h"

MediaPlayer::MediaPlayer(QObject *parent)
    : QObject(parent),
    player(new QMediaPlayer(this)),
    audioOutput(new QAudioOutput(this)) {
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(100.0);
}

MediaPlayer::~MediaPlayer() {
    delete player;
    delete audioOutput;
}

void MediaPlayer::playBackgroundMusic(const QString &filePath) {
    player->setSource(QUrl::fromLocalFile(filePath));
    player->setLoops(QMediaPlayer::Infinite);
    player->play();
}

void MediaPlayer::stopBackgroundMusic() {
    player->stop();
}

QMediaPlayer* MediaPlayer::getMediaPlayer() {
    return player;
}

void MediaPlayer::playRoundMusic(const QString &roundMusic, const QString &backgroundMusic) {
    player->setSource(QUrl::fromLocalFile(roundMusic));
    player->setLoops(1);
    player->play();
    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this,backgroundMusic](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
           this->playBackgroundMusic(backgroundMusic);
        }
    });
}
