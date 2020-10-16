#ifndef ALARMPLAYER_H
#define ALARMPLAYER_H

#include <phonon/AudioOutput>
#include <phonon/MediaObject>

class AlarmPlayer : public QObject
{
    Q_OBJECT
public:
    AlarmPlayer();
    ~AlarmPlayer();

    void setFileName(const QString filename);

public slots:
    void play();
    void stop();

private slots:
    void replay();

private:
    Phonon::MediaObject* mMediaObject;
};

#endif // ALARMPLAYER_H
