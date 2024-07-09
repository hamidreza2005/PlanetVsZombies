// mediaplayer.cpp
#include "mediaplayer.h"
#include <QUrl>
#include <QFile>

MediaPlayer* MediaPlayer::instance = nullptr;
MediaPlayer::MediaPlayer(QObject *parent)
        : QObject(parent),
          player(new QMediaPlayer(this)),
          audioOutput(new QAudioOutput(this)) {
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(1.0);  // Set to maximum volume (1.0 = 100%)

    connect(player, &QMediaPlayer::mediaStatusChanged, this, &MediaPlayer::handleMediaStatusChanged);
}

MediaPlayer::~MediaPlayer() {
    delete player;
    delete audioOutput;
}

bool MediaPlayer::playBackgroundMusic(const QString &resourcePath) {
    if (!checkResource(resourcePath)) {
        return false;
    }

    QUrl resourceUrl("qrc" + resourcePath);
    player->setSource(resourceUrl);
    player->setLoops(QMediaPlayer::Infinite);
    player->play();

    return true;
}

void MediaPlayer::stopBackgroundMusic() {
    player->stop();
}

QMediaPlayer* MediaPlayer::getMediaPlayer() {
    return player;
}

bool MediaPlayer::playRoundMusic(const QString &roundMusicResource, const QString &backgroundMusicResource) {

    if (!checkResource(roundMusicResource)) {
        return false;
    }

    QUrl resourceUrl("qrc" + roundMusicResource);
    player->setSource(resourceUrl);
    player->setLoops(1);
    player->play();


    connect(player, &QMediaPlayer::mediaStatusChanged, this, [this, backgroundMusicResource](QMediaPlayer::MediaStatus status) {
        if (status == QMediaPlayer::EndOfMedia) {
            this->playBackgroundMusic(backgroundMusicResource);
        }
    });

    return true;
}

void MediaPlayer::handleMediaStatusChanged(QMediaPlayer::MediaStatus status) {
}

bool MediaPlayer::checkResource(const QString &resourcePath) {
    if (!QUrl("qrc" + resourcePath).isValid()) {
        return false;
    }
    return true;
}

MediaPlayer *MediaPlayer::getInstance() {
    if (!instance) {
        instance = new MediaPlayer();
    }
    return instance;
}
