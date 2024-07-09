// mediaplayer.h
#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QDebug>

class MediaPlayer : public QObject {
Q_OBJECT

public:
    explicit MediaPlayer(QObject *parent = nullptr);
    ~MediaPlayer() override;
    QMediaPlayer* getMediaPlayer();
    bool playBackgroundMusic(const QString &resourcePath);
    void stopBackgroundMusic();
    bool playRoundMusic(const QString &roundMusicResource, const QString &backgroundMusicResource);
    static MediaPlayer* getInstance();

    MediaPlayer(MediaPlayer const&) = delete;
    void operator=(MediaPlayer const&) = delete;
private slots:
    void handleMediaStatusChanged(QMediaPlayer::MediaStatus status);
private:
    static MediaPlayer* instance;
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
    bool checkResource(const QString &resourcePath);
};

#endif // MEDIAPLAYER_H