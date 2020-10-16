#ifndef SLTMESSAGE_H
#define SLTMESSAGE_H

#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct SltStruct
 * \brief Структура SLT/СЛТ сообщения
 *
 * Поле \a slotTime содержит назначенное время прибытия.
 */

struct SltStruct : AtfFplStruct
{
    QDateTime departureTime;

    QDateTime slotTime;

    SltStruct()
    {}
    explicit SltStruct(const AtfFplStruct &s) :
        AtfFplStruct(s)
    {}
};

inline QDataStream & operator<< (QDataStream &stream, const SltStruct &msg)
{
    stream << (const AtfFplStruct &)msg;
    stream << msg.departureTime;
    stream << msg.slotTime;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, SltStruct &msg)
{
    stream >> (AtfFplStruct &)msg;
    stream >> msg.departureTime;
    stream >> msg.slotTime;
    return stream;
}

/*!
 * \class SltMessage
 * \brief Класс сообщения SLT/СЛТ
 */

class SltMessage : public AtfFplMessage
{
public:
    SltMessage();
    SltMessage(const SltStruct &s);
    virtual ~SltMessage();

    QString data() const;

    const SltStruct slt() const;

    const QDateTime & departureTime() const;
    void setDepartureTime(const QDateTime &time);

    const QDateTime & slotTime() const;
    void setSlotTime(const QDateTime &time);

private:
    QDateTime mDepartureTime;
    QDateTime mSlotTime;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SLTMESSAGE_H
