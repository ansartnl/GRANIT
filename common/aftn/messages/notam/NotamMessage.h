#ifndef NOTAMMESSAGE_H
#define NOTAMMESSAGE_H

#include "AftnMessage.h"

#include <QtCore/QDateTime>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

const QString validPerm[2] = {
    "PERM"
    , QString::fromUtf8("ПОСТ")
};

const QString validEst[2] = {
    "EST"
    , QString::fromUtf8("РАС4")
};

/*!
 * \struct NotamStruct
 * \brief Структура сообщения NOTAM
 */

struct NotamStruct
{
    QString series;
    int number;
    int year;

    QString type;

    QString replaceSeries;
    int replaceNumber;
    int replaceYear;

    QString fir;
    QString subject;
    QString condition;
    QString traffic;
    QString purpose;
    QString scope;
    QString qLowerLimit;
    QString qUpperLimit;
    QString location;
    int radius;

    QString aerodrome;
    QDateTime validFrom;
    QDateTime validTill;
    QString validType;

    QString schedule;
    QString description;

    QString lowerLimit;
    QString upperLimit;

    ParseVersion::Version version;

    inline int baseYear() const
    {
        return (QDateTime::currentDateTime().toUTC().date().year() / 100) * 100;
    }

    NotamStruct() :
        number(0), year(baseYear()), replaceNumber(0), replaceYear(baseYear())
      , qLowerLimit(""), qUpperLimit(""), radius(0), version(ParseVersion::PV_LATIN)
    {}

    inline int notamYear() const
    {
        return year - baseYear();
    }

    inline int notamReplaceYear() const
    {
        return replaceYear - baseYear();
    }


    static inline bool isPerm(const QString &validType)
    {
        return ((Transliteration::to4444(validType) == validPerm[0])
                || (Transliteration::toTC95(validType) == validPerm[1]));
    }

    static inline bool isEst(const QString &validType)
    {
        return ((Transliteration::to4444(validType) == validEst[0])
                || (Transliteration::toTC95(validType) == validEst[1]));
    }

    static inline const QString perm(const ParseVersion::Version v)
    {
        if (v == ParseVersion::PV_CYRILLIC)
            return validPerm[1];
        return validPerm[0];
    }

    static inline const QString est(const ParseVersion::Version v)
    {
        if (v == ParseVersion::PV_CYRILLIC)
            return validEst[1];
        return validEst[0];
    }
};

inline QDataStream & operator<< (QDataStream &stream, const NotamStruct &msg)
{
    stream << msg.series;
    stream << msg.number;
    stream << msg.year;
    stream << msg.type;
    stream << msg.replaceSeries;
    stream << msg.replaceNumber;
    stream << msg.replaceYear;
    stream << msg.fir;
    stream << msg.subject;
    stream << msg.condition;
    stream << msg.traffic;
    stream << msg.purpose;
    stream << msg.scope;
    stream << msg.qLowerLimit;
    stream << msg.qUpperLimit;
    stream << msg.location;
    stream << msg.radius;
    stream << msg.aerodrome;
    stream << msg.validFrom;
    stream << msg.validTill;
    stream << msg.validType;
    stream << msg.schedule;
    stream << msg.description;
    stream << msg.lowerLimit;
    stream << msg.upperLimit;
    stream << msg.version;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, NotamStruct &msg)
{
    stream >> msg.series;
    stream >> msg.number;
    stream >> msg.year;
    stream >> msg.type;
    stream >> msg.replaceSeries;
    stream >> msg.replaceNumber;
    stream >> msg.replaceYear;
    stream >> msg.fir;
    stream >> msg.subject;
    stream >> msg.condition;
    stream >> msg.traffic;
    stream >> msg.purpose;
    stream >> msg.scope;
    stream >> msg.qLowerLimit;
    stream >> msg.qUpperLimit;
    stream >> msg.location;
    stream >> msg.radius;
    stream >> msg.aerodrome;
    stream >> msg.validFrom;
    stream >> msg.validTill;
    stream >> msg.validType;
    stream >> msg.schedule;
    stream >> msg.description;
    stream >> msg.lowerLimit;
    stream >> msg.upperLimit;
    stream >> msg.version;
    return stream;
}

/*!
 * \struct NotamId
 * \brief Структура идентифицирующая NOTAM
 */

struct NotamId
{
    int year;
    int number;
    QString series;
    inline bool operator ==(const NotamId &other)
    {
        if (this->year != other.year)
            return false;
        if (this->number != other.number)
            return false;

        if ((this->series != Transliteration::to4444(other.series))
                && (this->series != Transliteration::toTC95(other.series)))
            return false;
        return true;
    }
};

class NotamIdList : public QList < NotamId >
{
public:
    QList < int > years() const;
    QList < int > numbersForYear(int year) const;
};

/*!
 * \class NotamMessage
 * \brief Класс NOTAM сообщения
 */

class NotamMessage : public AftnMessage
{
public:
    NotamMessage();
    NotamMessage(const NotamStruct &s);
    virtual ~NotamMessage();

    QString data() const;

    ParseVersion::Version version() const;
    void setVersion(ParseVersion::Version v);

    bool isCheckList() const;

    NotamIdList activeNotams() const;

    NotamStruct mNotam;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // NOTAMMESSAGE_H
