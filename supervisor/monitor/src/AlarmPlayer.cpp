#include "AlarmPlayer.h"

AlarmPlayer::AlarmPlayer()
    : mMediaObject(0)
{
}

AlarmPlayer::~AlarmPlayer()
{
    if(mMediaObject)
        mMediaObject->deleteLater();
}

void AlarmPlayer::setFileName(const QString filename)
{
    if(mMediaObject)
        mMediaObject->deleteLater();

    mMediaObject = Phonon::createPlayer(Phonon::NotificationCategory, Phonon::MediaSource(filename));
    connect(mMediaObject, SIGNAL(finished()), this, SLOT(replay()));
}

void AlarmPlayer::play()
{
    if(mMediaObject)
        mMediaObject->play();
}

void AlarmPlayer::stop()
{
    if(mMediaObject)
        mMediaObject->stop();
}

void AlarmPlayer::replay()
{
    if(mMediaObject)
    {
        mMediaObject->stop();
        mMediaObject->play();
    }
}
