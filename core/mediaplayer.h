#ifndef MEDIAPLAYER_H
#define MEDIAPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class MediaPlayer : public QObject {
    Q_OBJECT

public:
    explicit MediaPlayer(QObject *parent = nullptr);
    ~MediaPlayer();
    QMediaPlayer* getMediaPlayer();
    void playBackgroundMusic(const QString &filePath);
    void stopBackgroundMusic();
    void playRoundMusic(const QString &roundMusic, const QString &backgroundMusic);

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif // MEDIAPLAYER_H
