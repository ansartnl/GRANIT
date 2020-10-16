#include "Transliteration.h"

#include <QtCore/QTextCodec>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

TranslationMap Transliteration::commonV4444;
TranslationMap Transliteration::addressV4444;
TranslationMap Transliteration::point5V4444;
TranslationMap Transliteration::coordV4444;
TranslationMap Transliteration::aircraftTypeV4444;

TranslationMap Transliteration::commonVTc95;
TranslationMap Transliteration::addressVTc95;
TranslationMap Transliteration::point5VTc95;
TranslationMap Transliteration::coordVTc95;
TranslationMap Transliteration::aircraftTypeVTc95;

static int foo = Transliteration::initMaps();

const QString Transliteration::to4444(const QString &data)
{
    Transliteration t(ParseVersion::PV_LATIN);
    return t.trCommon(data);
}

const QString Transliteration::toTC95(const QString &data)
{
    Transliteration t(ParseVersion::PV_CYRILLIC);
    return t.trCommon(data);
}

const QString Transliteration::toVersion(const QString &data, ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return toTC95(data);
    if (v == ParseVersion::PV_LATIN)
        return to4444(data);
    return data;
}

const QString Transliteration::addressTo4444(const QString &data)
{
    Transliteration t(ParseVersion::PV_LATIN);
    return t.trAddress(data);
}

const QString Transliteration::addressToTC95(const QString &data)
{
    Transliteration t(ParseVersion::PV_CYRILLIC);
    return t.trAddress(data);
}

const QString Transliteration::addressToVersion(const QString &data, ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return addressToTC95(data);
    if (v == ParseVersion::PV_LATIN)
        return addressTo4444(data);
    return data;
}

const QString Transliteration::acidTo4444(const QString &data)
{
    return addressTo4444(data);
}

const QString Transliteration::acidToTC95(const QString &data)
{
    return addressToTC95(data);
}

const QString Transliteration::acidToVersion(const QString &data, ParseVersion::Version v)
{
    return addressToVersion(data, v);
}

const QString Transliteration::routeTo4444(const QString &data)
{
    Transliteration t(ParseVersion::PV_LATIN);
    return t.trRoute(data);
}

const QString Transliteration::routeToTC95(const QString &data)
{
    Transliteration t(ParseVersion::PV_CYRILLIC);
    return t.trRoute(data);
}

const QString Transliteration::routeToVersion(const QString &data, ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return routeToTC95(data);
    if (v == ParseVersion::PV_LATIN)
        return routeTo4444(data);
    return data;
}

const QString Transliteration::aircraftTypeTo4444(const QString &data)
{
    Transliteration t(ParseVersion::PV_LATIN);
    return t.trAircraftType(data);
}

const QString Transliteration::aircraftTypeToTC95(const QString &data)
{
    Transliteration t(ParseVersion::PV_CYRILLIC);
    return t.trAircraftType(data);
}

const QString Transliteration::aircraftTypeToVersion(const QString &data, ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return aircraftTypeToTC95(data);
    if (v == ParseVersion::PV_LATIN)
        return aircraftTypeTo4444(data);
    return data;
}

const QString Transliteration::nil(ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return QString::fromUtf8("НЕТ");
    return QString("NIL");
}

const QString Transliteration::notKnown(ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return QString::fromUtf8("НЕИЗВЕСТНО");
    return QString("NOT KNOWN");
}

bool Transliteration::isNil(const QString &data)
{
    if (data == nil(ParseVersion::PV_LATIN))
        return true;
    if (data == nil(ParseVersion::PV_CYRILLIC))
        return true;
    return false;
}

bool Transliteration::isNotKnown(const QString &data)
{
    if (data == notKnown(ParseVersion::PV_LATIN))
        return true;
    if (data == notKnown(ParseVersion::PV_CYRILLIC))
        return true;
    return false;
}

const QString Transliteration::rfiTo4444(const QString &data)
{
    Transliteration t(ParseVersion::PV_LATIN);
    return t.trRfi(data);
}

const QString Transliteration::rfiToTC95(const QString &data)
{
    Transliteration t(ParseVersion::PV_CYRILLIC);
    return t.trRfi(data);
}

const QString Transliteration::rfiToVersion(const QString &data, ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return rfiToTC95(data);
    if (v == ParseVersion::PV_LATIN)
        return rfiTo4444(data);
    return data;
}

int Transliteration::initMaps()
{
    commonV4444.clear();
    commonV4444.insert(L'А', "A");
    commonV4444.insert(L'Б', "B");
    commonV4444.insert(L'В', "W");
    commonV4444.insert(L'Г', "G");
    commonV4444.insert(L'Д', "D");
    commonV4444.insert(L'Е', "E");
    commonV4444.insert(L'Ё', "E");
    commonV4444.insert(L'Э', "E");
    commonV4444.insert(L'Ж', "V");
    commonV4444.insert(L'З', "Z");
    commonV4444.insert(L'И', "I");
    commonV4444.insert(L'Й', "J");
    commonV4444.insert(L'К', "K");
    commonV4444.insert(L'Л', "L");
    commonV4444.insert(L'М', "M");
    commonV4444.insert(L'Н', "N");
    commonV4444.insert(L'О', "O");
    commonV4444.insert(L'П', "P");
    commonV4444.insert(L'Р', "R");
    commonV4444.insert(L'С', "S");
    commonV4444.insert(L'Т', "T");
    commonV4444.insert(L'У', "U");
    commonV4444.insert(L'Ф', "F");
    commonV4444.insert(L'Х', "H");
    commonV4444.insert(L'Ц', "C");
    commonV4444.insert(L'Ч', "CH");
    commonV4444.insert(L'Ш', "SH");
    commonV4444.insert(L'Щ', "Q");
    commonV4444.insert(L'Ы', "Y");
    commonV4444.insert(L'Ъ', "X");
    commonV4444.insert(L'Ь', "X");
    commonV4444.insert(L'Ю', "IU");
    commonV4444.insert(L'Я', "IA");

    addressV4444.clear();
    addressV4444.insert(L'А', "A");
    addressV4444.insert(L'Б', "B");
    addressV4444.insert(L'В', "W");
    addressV4444.insert(L'Г', "G");
    addressV4444.insert(L'Д', "D");
    addressV4444.insert(L'Е', "E");
    addressV4444.insert(L'Ё', "E");
    addressV4444.insert(L'Э', "E");
    addressV4444.insert(L'Ж', "V");
    addressV4444.insert(L'З', "Z");
    addressV4444.insert(L'И', "I");
    addressV4444.insert(L'Й', "J");
    addressV4444.insert(L'К', "K");
    addressV4444.insert(L'Л', "L");
    addressV4444.insert(L'М', "M");
    addressV4444.insert(L'Н', "N");
    addressV4444.insert(L'О', "O");
    addressV4444.insert(L'П', "P");
    addressV4444.insert(L'Р', "R");
    addressV4444.insert(L'С', "S");
    addressV4444.insert(L'Т', "T");
    addressV4444.insert(L'У', "U");
    addressV4444.insert(L'Ф', "F");
    addressV4444.insert(L'Х', "H");
    addressV4444.insert(L'Ц', "C");
    addressV4444.insert(L'Ч', "C");
    addressV4444.insert(L'Ш', "S");
    addressV4444.insert(L'Щ', "Q");
    addressV4444.insert(L'Ы', "Y");
    addressV4444.insert(L'Ъ', "X");
    addressV4444.insert(L'Ь', "X");
    addressV4444.insert(L'Ю', "U");
    addressV4444.insert(L'Я', "Q");

    point5V4444.clear();
    point5V4444.insert(L'А', "A");
    point5V4444.insert(L'Б', "B");
    point5V4444.insert(L'В', "V");
    point5V4444.insert(L'Г', "G");
    point5V4444.insert(L'Д', "D");
    point5V4444.insert(L'Е', "E");
    point5V4444.insert(L'Ё', "E");
    point5V4444.insert(L'Э', "E");
    point5V4444.insert(L'Ж', "V");
    point5V4444.insert(L'З', "Z");
    point5V4444.insert(L'И', "I");
    point5V4444.insert(L'Й', "J");
    point5V4444.insert(L'К', "K");
    point5V4444.insert(L'Л', "L");
    point5V4444.insert(L'М', "M");
    point5V4444.insert(L'Н', "N");
    point5V4444.insert(L'О', "O");
    point5V4444.insert(L'П', "P");
    point5V4444.insert(L'Р', "R");
    point5V4444.insert(L'С', "S");
    point5V4444.insert(L'Т', "T");
    point5V4444.insert(L'У', "U");
    point5V4444.insert(L'Ф', "F");
    point5V4444.insert(L'Х', "H");
    point5V4444.insert(L'Ц', "C");
    point5V4444.insert(L'Ч', "C");
    point5V4444.insert(L'Ш', "S");
    point5V4444.insert(L'Щ', "Q");
    point5V4444.insert(L'Ы', "Y");
    point5V4444.insert(L'Ъ', "X");
    point5V4444.insert(L'Ь', "X");
    point5V4444.insert(L'Ю', "U");
    point5V4444.insert(L'Я', "A");

    coordV4444.clear();
    coordV4444.insert(L'С', "N");
    coordV4444.insert(L'Ю', "S");
    coordV4444.insert(L'З', "W");
    coordV4444.insert(L'В', "E");

    aircraftTypeV4444.clear();
    aircraftTypeV4444.insert(L'А', "A");
    aircraftTypeV4444.insert(L'Б', "B");
    aircraftTypeV4444.insert(L'В', "W");
    aircraftTypeV4444.insert(L'Г', "G");
    aircraftTypeV4444.insert(L'Д', "D");
    aircraftTypeV4444.insert(L'Е', "E");
    aircraftTypeV4444.insert(L'Ё', "E");
    aircraftTypeV4444.insert(L'Э', "E");
    aircraftTypeV4444.insert(L'Ж', "V");
    aircraftTypeV4444.insert(L'З', "Z");
    aircraftTypeV4444.insert(L'И', "I");
    aircraftTypeV4444.insert(L'Й', "J");
    aircraftTypeV4444.insert(L'К', "K");
    aircraftTypeV4444.insert(L'Л', "L");
    aircraftTypeV4444.insert(L'М', "M");
    aircraftTypeV4444.insert(L'Н', "N");
    aircraftTypeV4444.insert(L'О', "O");
    aircraftTypeV4444.insert(L'П', "P");
    aircraftTypeV4444.insert(L'Р', "R");
    aircraftTypeV4444.insert(L'С', "S");
    aircraftTypeV4444.insert(L'Т', "T");
    aircraftTypeV4444.insert(L'У', "U");
    aircraftTypeV4444.insert(L'Ф', "F");
    aircraftTypeV4444.insert(L'Х', "H");
    aircraftTypeV4444.insert(L'Ц', "C");
    aircraftTypeV4444.insert(L'Ч', "CH");
    aircraftTypeV4444.insert(L'Ш', "SH");
    aircraftTypeV4444.insert(L'Щ', "Q");
    aircraftTypeV4444.insert(L'Ы', "Y");
    aircraftTypeV4444.insert(L'Ъ', "X");
    aircraftTypeV4444.insert(L'Ь', "X");
    aircraftTypeV4444.insert(L'Ю', "IU");
    aircraftTypeV4444.insert(L'Я', "Y");

    QTextCodec *utf8 = QTextCodec::codecForName("UTF-8");

    commonVTc95.clear();
    commonVTc95.insert(L'A', utf8->toUnicode("А"));
    commonVTc95.insert(L'B', utf8->toUnicode("Б"));
    commonVTc95.insert(L'C', utf8->toUnicode("Ц"));
    commonVTc95.insert(L'D', utf8->toUnicode("Д"));
    commonVTc95.insert(L'E', utf8->toUnicode("Е"));
    commonVTc95.insert(L'F', utf8->toUnicode("Ф"));
    commonVTc95.insert(L'G', utf8->toUnicode("Г"));
    commonVTc95.insert(L'H', utf8->toUnicode("Х"));
    commonVTc95.insert(L'I', utf8->toUnicode("И"));
    commonVTc95.insert(L'J', utf8->toUnicode("Й"));
    commonVTc95.insert(L'K', utf8->toUnicode("К"));
    commonVTc95.insert(L'L', utf8->toUnicode("Л"));
    commonVTc95.insert(L'M', utf8->toUnicode("М"));
    commonVTc95.insert(L'N', utf8->toUnicode("Н"));
    commonVTc95.insert(L'O', utf8->toUnicode("О"));
    commonVTc95.insert(L'P', utf8->toUnicode("П"));
    commonVTc95.insert(L'Q', utf8->toUnicode("Щ"));
    commonVTc95.insert(L'R', utf8->toUnicode("Р"));
    commonVTc95.insert(L'S', utf8->toUnicode("С"));
    commonVTc95.insert(L'T', utf8->toUnicode("Т"));
    commonVTc95.insert(L'U', utf8->toUnicode("У"));
    commonVTc95.insert(L'V', utf8->toUnicode("Ж"));
    commonVTc95.insert(L'W', utf8->toUnicode("В"));
    commonVTc95.insert(L'X', utf8->toUnicode("Ь"));
    commonVTc95.insert(L'Y', utf8->toUnicode("Ы"));
    commonVTc95.insert(L'Z', utf8->toUnicode("З"));

    addressVTc95.clear();
    addressVTc95.insert(L'A', utf8->toUnicode("А"));
    addressVTc95.insert(L'B', utf8->toUnicode("Б"));
    addressVTc95.insert(L'C', utf8->toUnicode("Ц"));
    addressVTc95.insert(L'D', utf8->toUnicode("Д"));
    addressVTc95.insert(L'E', utf8->toUnicode("Е"));
    addressVTc95.insert(L'F', utf8->toUnicode("Ф"));
    addressVTc95.insert(L'G', utf8->toUnicode("Г"));
    addressVTc95.insert(L'H', utf8->toUnicode("Х"));
    addressVTc95.insert(L'I', utf8->toUnicode("И"));
    addressVTc95.insert(L'J', utf8->toUnicode("Й"));
    addressVTc95.insert(L'K', utf8->toUnicode("К"));
    addressVTc95.insert(L'L', utf8->toUnicode("Л"));
    addressVTc95.insert(L'M', utf8->toUnicode("М"));
    addressVTc95.insert(L'N', utf8->toUnicode("Н"));
    addressVTc95.insert(L'O', utf8->toUnicode("О"));
    addressVTc95.insert(L'P', utf8->toUnicode("П"));
    addressVTc95.insert(L'Q', utf8->toUnicode("Я"));
    addressVTc95.insert(L'R', utf8->toUnicode("Р"));
    addressVTc95.insert(L'S', utf8->toUnicode("С"));
    addressVTc95.insert(L'T', utf8->toUnicode("Т"));
    addressVTc95.insert(L'U', utf8->toUnicode("У"));
    addressVTc95.insert(L'V', utf8->toUnicode("Ж"));
    addressVTc95.insert(L'W', utf8->toUnicode("В"));
    addressVTc95.insert(L'X', utf8->toUnicode("Ь"));
    addressVTc95.insert(L'Y', utf8->toUnicode("Ы"));
    addressVTc95.insert(L'Z', utf8->toUnicode("З"));

    point5VTc95.clear();
    point5VTc95.insert(L'A', utf8->toUnicode("А"));
    point5VTc95.insert(L'B', utf8->toUnicode("Б"));
    point5VTc95.insert(L'C', utf8->toUnicode("Ц"));
    point5VTc95.insert(L'D', utf8->toUnicode("Д"));
    point5VTc95.insert(L'E', utf8->toUnicode("Е"));
    point5VTc95.insert(L'F', utf8->toUnicode("Ф"));
    point5VTc95.insert(L'G', utf8->toUnicode("Г"));
    point5VTc95.insert(L'H', utf8->toUnicode("Х"));
    point5VTc95.insert(L'I', utf8->toUnicode("И"));
    point5VTc95.insert(L'J', utf8->toUnicode("Й"));
    point5VTc95.insert(L'K', utf8->toUnicode("К"));
    point5VTc95.insert(L'L', utf8->toUnicode("Л"));
    point5VTc95.insert(L'M', utf8->toUnicode("М"));
    point5VTc95.insert(L'N', utf8->toUnicode("Н"));
    point5VTc95.insert(L'O', utf8->toUnicode("О"));
    point5VTc95.insert(L'P', utf8->toUnicode("П"));
    point5VTc95.insert(L'Q', utf8->toUnicode("Щ"));
    point5VTc95.insert(L'R', utf8->toUnicode("Р"));
    point5VTc95.insert(L'S', utf8->toUnicode("С"));
    point5VTc95.insert(L'T', utf8->toUnicode("Т"));
    point5VTc95.insert(L'U', utf8->toUnicode("У"));
    point5VTc95.insert(L'V', utf8->toUnicode("В"));
    point5VTc95.insert(L'W', utf8->toUnicode("В"));
    point5VTc95.insert(L'X', utf8->toUnicode("Ь"));
    point5VTc95.insert(L'Y', utf8->toUnicode("Ы"));
    point5VTc95.insert(L'Z', utf8->toUnicode("З"));

    coordVTc95.clear();
    coordVTc95.insert(L'N', utf8->toUnicode("С"));
    coordVTc95.insert(L'S', utf8->toUnicode("Ю"));
    coordVTc95.insert(L'W', utf8->toUnicode("З"));
    coordVTc95.insert(L'E', utf8->toUnicode("В"));

    aircraftTypeVTc95.clear();
    aircraftTypeVTc95.insert(L'A', utf8->toUnicode("А"));
    aircraftTypeVTc95.insert(L'B', utf8->toUnicode("Б"));
    aircraftTypeVTc95.insert(L'C', utf8->toUnicode("Ц"));
    aircraftTypeVTc95.insert(L'D', utf8->toUnicode("Д"));
    aircraftTypeVTc95.insert(L'E', utf8->toUnicode("Е"));
    aircraftTypeVTc95.insert(L'F', utf8->toUnicode("Ф"));
    aircraftTypeVTc95.insert(L'G', utf8->toUnicode("Г"));
    aircraftTypeVTc95.insert(L'H', utf8->toUnicode("Х"));
    aircraftTypeVTc95.insert(L'I', utf8->toUnicode("И"));
    aircraftTypeVTc95.insert(L'J', utf8->toUnicode("Й"));
    aircraftTypeVTc95.insert(L'K', utf8->toUnicode("К"));
    aircraftTypeVTc95.insert(L'L', utf8->toUnicode("Л"));
    aircraftTypeVTc95.insert(L'M', utf8->toUnicode("М"));
    aircraftTypeVTc95.insert(L'N', utf8->toUnicode("Н"));
    aircraftTypeVTc95.insert(L'O', utf8->toUnicode("О"));
    aircraftTypeVTc95.insert(L'P', utf8->toUnicode("П"));
    aircraftTypeVTc95.insert(L'Q', utf8->toUnicode("Щ"));
    aircraftTypeVTc95.insert(L'R', utf8->toUnicode("Р"));
    aircraftTypeVTc95.insert(L'S', utf8->toUnicode("С"));
    aircraftTypeVTc95.insert(L'T', utf8->toUnicode("Т"));
    aircraftTypeVTc95.insert(L'U', utf8->toUnicode("У"));
    aircraftTypeVTc95.insert(L'V', utf8->toUnicode("Ж"));
    aircraftTypeVTc95.insert(L'W', utf8->toUnicode("В"));
    aircraftTypeVTc95.insert(L'X', utf8->toUnicode("Ь"));
    aircraftTypeVTc95.insert(L'Y', utf8->toUnicode("Я"));
    aircraftTypeVTc95.insert(L'Z', utf8->toUnicode("З"));

    return 0;
}

Transliteration::Transliteration(ParseVersion::Version v)
{
    setVersion(v);
}

void Transliteration::setVersion(ParseVersion::Version v)
{
    mVersion = v;
}

const QString Transliteration::trCommon(const QString &data) const
{
    return transliterate(data, common());
}

const QString Transliteration::trAddress(const QString &data) const
{
    return transliterate(data, address());
}

const QString Transliteration::trRoute(const QString &data) const
{
    QStringList ret;

    QRegExp point5Rexp("(^|/)([A-Z|А-Я]{5})(/|$)");
    QRegExp coordRexp("(\\d{4}|\\d{2})(N|S|С|Ю)(\\d{5}|\\d{3})(E|W|В|З)");

    QStringList points = data.split(" ", QString::SkipEmptyParts);
    foreach (const QString &point, points)
    {
        if (point5Rexp.indexIn(point) != -1)
            ret << trPoint5(point);
        else if (coordRexp.indexIn(point) != -1)
            ret << trCoord(point);
        else
            ret << trCommon(point);
    }

    return ret.join(" ");
}

const QString Transliteration::trPoint5(const QString &data) const
{
    return transliterate(data, point5());
}

const QString Transliteration::trCoord(const QString &data) const
{
    return transliterate(data, coord());
}

const QString Transliteration::trAircraftType(const QString &data) const
{
    return transliterate(data, aircraftType());
}

const QString Transliteration::trRfi(const QString &data) const
{
    if (isNil(data))
        return nil(mVersion);
    if (isNotKnown(data))
        return notKnown(mVersion);
    return trCommon(data);
}

const QString Transliteration::transliterate(const QString &data, const TranslationMap &map) const
{
    QString ret;
    foreach (const QChar &letter, data)
    {
        if (map.contains(letter.unicode()))
            ret += map[letter.unicode()];
        else
            ret += letter;
    }
    return ret;
}

const TranslationMap & Transliteration::common() const
{
    if (mVersion == ParseVersion::PV_CYRILLIC)
        return commonVTc95;
    return commonV4444;
}

const TranslationMap & Transliteration::address() const
{
    if (mVersion == ParseVersion::PV_CYRILLIC)
        return addressVTc95;
    return addressV4444;
}

const TranslationMap & Transliteration::point5() const
{
    if (mVersion == ParseVersion::PV_CYRILLIC)
        return point5VTc95;
    return point5V4444;
}

const TranslationMap & Transliteration::coord() const
{
    if (mVersion == ParseVersion::PV_CYRILLIC)
        return coordVTc95;
    return coordV4444;
}

const TranslationMap & Transliteration::aircraftType() const
{
    if (mVersion == ParseVersion::PV_CYRILLIC)
        return aircraftTypeVTc95;
    return aircraftTypeV4444;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
