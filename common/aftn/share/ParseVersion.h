#ifndef PARSEVERSION_H
#define PARSEVERSION_H

#include "enumstreaming.h"

#include <QtCore/QStringList>

/*
 * Терминология.
 *
 * Telegram - вся АФТН телеграмма вместе с заголовком и окончанием, как она приходит.
 * Message - отдельное сообщение в пачке сообщений, или все поле text телеграммы.
 *
 *
 * Примеры.
 *
 * Телеграмма:
 *
 * ZCZC UVA103
 * FF EUCHZMFP
 * 120856 UMMVZQZX
 * (ACK-NJE319-UUWW-EGLF)
 * NNNN
 *
 * Сообщение:
 *
 * (ACK-NJE319-UUWW-EGLF)
 */

/*!
 * \struct VersionTr
 * \brief Класс переводов
 *
 * Фиктивный класс, для вынесения переводов в отдельную запись.
 * Вынесен из пространства имен \a ParseVersion, потому что lupdate некорректно обрабатывает конструкцию:
 * #ifdef AFTN_NAMESPACE
 * namespace Aftn
 * {
 * #endif
 */

class VersionTr : public QObject
{
    Q_OBJECT
};

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

namespace ParseVersion
{

const int n_Unknown = -1; // default
const int n_4444 = 0;
const int n_TC95 = 1;

enum Version {
    V_UNKNOWN = n_Unknown
    , V_4444 = n_4444
    , V_TC95 = n_TC95

    // rebranding
    , PV_UNKNOWN = V_UNKNOWN
    , PV_LATIN = V_4444
    , PV_CYRILLIC = V_TC95
    , PV_MIXED = PV_UNKNOWN // Maybe will change someday
};

inline const QString toString(Version v)
{
    switch (v)
    {
    case V_UNKNOWN:
        return VersionTr::tr("Unknown");
    case V_4444:
        return VersionTr::tr("ICAO");
    case V_TC95:
        return VersionTr::tr("RF");
    }
    return VersionTr::tr("Unhandled");
}

const QString symbols4444 = QString::fromUtf8("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
const QString symbolsTC95 = QString::fromUtf8("АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ");

/*!
 * Определяет и возвращает версию строки \a data.
 */

inline Version textVersion(const QString &data, const Version defVer = V_4444)
{
    bool has4444 = false;
    bool hasTC95 = false;
    foreach (const QChar &c, data)
    {
        if (symbols4444.contains(c))
            has4444 = true;
        else if (symbolsTC95.contains(c))
            hasTC95 = true;
    }
    if (has4444 && hasTC95)
        return V_UNKNOWN;
    if (hasTC95)
        return V_TC95;
    if (has4444)
        return V_4444;
    return defVer;
}

/*!
 * Конвертирует \a data в латиницу.
 *
 * Устарела. Вместо нее используйте класс \a Transliteration.
 */

Q_DECL_DEPRECATED inline QString to4444(const QString &data);

/*!
 * Конвертирует \a data в кирилицу.
 *
 * Устарела. Вместо нее используйте класс \a Transliteration.
 */

Q_DECL_DEPRECATED inline QString toTC95(const QString &data);

/*!
 * Конвертирует \a data согласно \a version. Устаревшая.
 */

Q_DECL_DEPRECATED inline QString toVersion(const QString &data, int version);

/*!
 * Конвертирует \a data согласно \a version.
 *
 * Устарела. Вместо нее используйте класс \a Transliteration.
 */

Q_DECL_DEPRECATED inline QString toVersion(const QString &data, Version version);

inline QString to4444(const QString &data)
{
    QString ret;
    foreach (const QChar &letter, data)
    {
        switch (letter.unicode())
        {
        case L'А':
            ret += "A";
            break;
        case L'Б':
            ret += "B";
            break;
        case L'В':
            ret += "W";
            break;
        case L'Г':
            ret += "G";
            break;
        case L'Д':
            ret += "D";
            break;
        case L'Е':
        case L'Ё':
        case L'Э':
            ret += "E";
            break;
        case L'Ж':
            ret += "V";
            break;
        case L'З':
            ret += "Z";
            break;
        case L'И':
            ret += "I";
            break;
        case L'Й':
            ret += "J";
            break;
        case L'К':
            ret += "K";
            break;
        case L'Л':
            ret += "L";
            break;
        case L'М':
            ret += "M";
            break;
        case L'Н':
            ret += "N";
            break;
        case L'О':
            ret += "O";
            break;
        case L'П':
            ret += "P";
            break;
        case L'Р':
            ret += "R";
            break;
        case L'С':
            ret += "S";
            break;
        case L'Т':
            ret += "T";
            break;
        case L'У':
            ret += "U";
            break;
        case L'Ф':
            ret += "F";
            break;
        case L'Х':
            ret += "H";
            break;
        case L'Ц':
            ret += "C";
            break;
        case L'Ч':
            ret += "CH";
            break;
        case L'Ш':
            ret += "SH";
            break;
        case L'Щ':
            ret += "Q";
            break;
        case L'Ы':
            ret += "Y";
            break;
        case L'Ъ':
        case L'Ь':
            ret += "X";
            break;
        case L'Ю':
            ret += "IU";
            break;
        case L'Я':
            ret += "IA";
            break;
        default:
            ret += letter;
        }
    }
    return ret;
}

inline QString toTC95(const QString &data)
{
    QString ret;
    foreach (const QChar &letter, data)
    {
        switch (letter.toAscii())
        {
        case L'A':
            ret += "А";
            break;
        case L'B':
            ret += "Б";
            break;
        case L'C':
            ret += "Ц";
            break;
        case L'D':
            ret += "Д";
            break;
        case L'E':
            ret += "Е";
            break;
        case L'F':
            ret += "Ф";
            break;
        case L'G':
            ret += "Г";
            break;
        case L'H':
            ret += "Х";
            break;
        case L'I':
            ret += "И";
            break;
        case L'J':
            ret += "Й";
            break;
        case L'K':
            ret += "К";
            break;
        case L'L':
            ret += "Л";
            break;
        case L'M':
            ret += "М";
            break;
        case L'N':
            ret += "Н";
            break;
        case L'O':
            ret += "О";
            break;
        case L'P':
            ret += "П";
            break;
        case L'Q':
            ret += "Щ";
            break;
        case L'R':
            ret += "Р";
            break;
        case L'S':
            ret += "С";
            break;
        case L'T':
            ret += "Т";
            break;
        case L'U':
            ret += "У";
            break;
        case L'V':
            ret += "Ж";
            break;
        case L'W':
            ret += "В";
            break;
        case L'X':
            ret += "Ь";
            break;
        case L'Y':
            ret += "Ы";
            break;
        case L'Z':
            ret += "З";
            break;
        default:
            ret += letter;
        }
    }
    return ret;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

inline QString toVersion(const QString &data, int version)
{
    if (version == n_TC95)
        return toTC95(data);
    if (version == n_4444)
        return to4444(data);
    return data;
}

inline QString toVersion(const QString &data, Version version)
{
    if (version == V_TC95)
        return toTC95(data);
    if (version == V_4444)
        return to4444(data);
    return data;
}

#pragma GCC diagnostic pop

/*!
 * Конвертирует \a data в латиницу по правилам для адресов.
 *
 * Устарела. Вместо нее используйте класс \a Transliteration.
 */

Q_DECL_DEPRECATED inline QString addressTo4444(const QString &data);

/*!
 * Конвертирует \a data в кирилицу по правилам для адресов.
 *
 * Устарела. Вместо нее используйте класс \a Transliteration.
 */

Q_DECL_DEPRECATED inline QString addressToTC95(const QString &data);

/*!
 * Конвертирует \a data согласно \a version по правилам для адресов. Устаревшая.
 */

Q_DECL_DEPRECATED inline QString addressToVersion(const QString &data, int version);

/*!
 * Конвертирует \a data согласно \a version по правилам для адресов.
 *
 * Устарела. Вместо нее используйте класс \a Transliteration.
 */

Q_DECL_DEPRECATED inline QString addressToVersion(const QString &data, Version version);

inline QString addressTo4444(const QString &data)
{
    QString ret;
    foreach (const QChar &letter, data)
    {
        switch (letter.unicode())
        {
        case L'А':
            ret += "A";
            break;
        case L'Б':
            ret += "B";
            break;
        case L'В':
            ret += "W";
            break;
        case L'Г':
            ret += "G";
            break;
        case L'Д':
            ret += "D";
            break;
        case L'Е':
        case L'Ё':
        case L'Э':
            ret += "E";
            break;
        case L'Ж':
            ret += "V";
            break;
        case L'З':
            ret += "Z";
            break;
        case L'И':
            ret += "I";
            break;
        case L'Й':
            ret += "J";
            break;
        case L'К':
            ret += "K";
            break;
        case L'Л':
            ret += "L";
            break;
        case L'М':
            ret += "M";
            break;
        case L'Н':
            ret += "N";
            break;
        case L'О':
            ret += "O";
            break;
        case L'П':
            ret += "P";
            break;
        case L'Р':
            ret += "R";
            break;
        case L'С':
            ret += "S";
            break;
        case L'Т':
            ret += "T";
            break;
        case L'У':
            ret += "U";
            break;
        case L'Ф':
            ret += "F";
            break;
        case L'Х':
            ret += "H";
            break;
        case L'Ц':
            ret += "C";
            break;
        case L'Ч':
            ret += "C";
            break;
        case L'Ш':
            ret += "S";
            break;
        case L'Щ':
            ret += "Q";
            break;
        case L'Ы':
            ret += "Y";
            break;
        case L'Ъ':
        case L'Ь':
            ret += "X";
            break;
        case L'Ю':
            ret += "U";
            break;
        case L'Я':
            ret += "Q";
            break;
        default:
            ret += letter;
        }
    }
    return ret;
}

inline QString addressToTC95(const QString &data)
{
    QString ret;
    foreach (const QChar &letter, data)
    {
        switch (letter.toAscii())
        {
        case L'A':
            ret += "А";
            break;
        case L'B':
            ret += "Б";
            break;
        case L'C':
            ret += "Ц";
            break;
        case L'D':
            ret += "Д";
            break;
        case L'E':
            ret += "Е";
            break;
        case L'F':
            ret += "Ф";
            break;
        case L'G':
            ret += "Г";
            break;
        case L'H':
            ret += "Х";
            break;
        case L'I':
            ret += "И";
            break;
        case L'J':
            ret += "Й";
            break;
        case L'K':
            ret += "К";
            break;
        case L'L':
            ret += "Л";
            break;
        case L'M':
            ret += "М";
            break;
        case L'N':
            ret += "Н";
            break;
        case L'O':
            ret += "О";
            break;
        case L'P':
            ret += "П";
            break;
        case L'Q':
            ret += "Я";
            break;
        case L'R':
            ret += "Р";
            break;
        case L'S':
            ret += "С";
            break;
        case L'T':
            ret += "Т";
            break;
        case L'U':
            ret += "У";
            break;
        case L'V':
            ret += "Ж";
            break;
        case L'W':
            ret += "В";
            break;
        case L'X':
            ret += "Ь";
            break;
        case L'Y':
            ret += "Ы";
            break;
        case L'Z':
            ret += "З";
            break;
        default:
            ret += letter;
        }
    }
    return ret;
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

inline QString addressToVersion(const QString &data, int version)
{
    if (version == n_TC95)
        return addressToTC95(data);
    if (version == n_4444)
        return addressTo4444(data);
    return data;
}

inline QString addressToVersion(const QString &data, Version version)
{
    if (version == V_TC95)
        return addressToTC95(data);
    if (version == V_4444)
        return addressTo4444(data);
    return data;
}

#pragma GCC diagnostic pop

/*!
 * Запрещенные последовательности символов
 */

inline const QStringList forbiddenSequences4444()
{
    return QStringList() << "ZCZC" << "NNNN" << "+:+:" << ",,,,";
}

inline const QStringList forbiddenSequencesTc95()
{
    return QStringList() << "ЗЦЗЦ" << "НННН" << "+:+:" << ",,,,";
}

inline const QStringList forbiddenSequences()
{
    return QStringList() << "ZCZC" << "NNNN" << "ЗЦЗЦ" << "НННН" << "+:+:" << ",,,,";
}

inline const QStringList forbiddenSequences(Version version)
{
    if (version == V_TC95)
        return forbiddenSequencesTc95();
    if (version == V_4444)
        return forbiddenSequences4444();
    return forbiddenSequences();
}

/*!
 * Проверка на содержание запрещенных последовательностей символов
 */

inline bool hasForbiddenSequences4444(const QString &data)
{
    const QStringList list = forbiddenSequences4444();
    foreach (const QString &seq, list)
        if (data.contains(seq))
            return true;
    return false;
}

inline bool hasForbiddenSequencesTc95(const QString &data)
{
    const QStringList list = forbiddenSequencesTc95();
    foreach (const QString &seq, list)
        if (data.contains(seq))
            return true;
    return false;
}

inline bool hasForbiddenSequences(const QString &data)
{
    const QStringList list = forbiddenSequences();
    foreach (const QString &seq, list)
        if (data.contains(seq))
            return true;
    return false;
}

inline bool hasForbiddenSequences(const QString &data, Version version)
{
    const QStringList list = forbiddenSequences(version);
    foreach (const QString &seq, list)
        if (data.contains(seq))
            return true;
    return false;
}

} // namespace ParseVersion

QT_DECLARE_ENUM_STREAMING(ParseVersion::Version)

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // PARSEVERSION_H
