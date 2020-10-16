#ifndef TRANSLITERATION_H
#define TRANSLITERATION_H

#include "ParseVersion.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

typedef ushort Letter;
typedef QString Replace;
typedef QMap < Letter, Replace > TranslationMap;

class Transliteration
{
public:
    static const QString to4444(const QString &data);
    static const QString toTC95(const QString &data);
    static const QString toVersion(const QString &data, ParseVersion::Version v);

    static const QString addressTo4444(const QString &data);
    static const QString addressToTC95(const QString &data);
    static const QString addressToVersion(const QString &data, ParseVersion::Version v);

    static const QString acidTo4444(const QString &data);
    static const QString acidToTC95(const QString &data);
    static const QString acidToVersion(const QString &data, ParseVersion::Version v);

    static const QString routeTo4444(const QString &data);
    static const QString routeToTC95(const QString &data);
    static const QString routeToVersion(const QString &data, ParseVersion::Version v);

    static const QString aircraftTypeTo4444(const QString &data);
    static const QString aircraftTypeToTC95(const QString &data);
    static const QString aircraftTypeToVersion(const QString &data, ParseVersion::Version v);

    static const QString nil(ParseVersion::Version v);
    static const QString notKnown(ParseVersion::Version v);

    static bool isNil(const QString &data);
    static bool isNotKnown(const QString &data);

    static const QString rfiTo4444(const QString &data);
    static const QString rfiToTC95(const QString &data);
    static const QString rfiToVersion(const QString &data, ParseVersion::Version v);

public:
    static int initMaps();

    Transliteration(ParseVersion::Version v);

    void setVersion(ParseVersion::Version v);

    const QString trCommon(const QString &data) const;
    const QString trAddress(const QString &data) const;
    const QString trRoute(const QString &data) const;
    const QString trPoint5(const QString &data) const;
    const QString trCoord(const QString &data) const;
    const QString trAircraftType(const QString &data) const;

    const QString trRfi(const QString &data) const;

private:
    const QString transliterate(const QString &data, const TranslationMap &map) const;

    const TranslationMap & common() const;
    const TranslationMap & address() const;
    const TranslationMap & point5() const;
    const TranslationMap & coord() const;
    const TranslationMap & aircraftType() const;

private:
    ParseVersion::Version mVersion;

    static TranslationMap commonV4444;
    static TranslationMap addressV4444;
    static TranslationMap point5V4444;
    static TranslationMap coordV4444;
    static TranslationMap aircraftTypeV4444;

    static TranslationMap commonVTc95;
    static TranslationMap addressVTc95;
    static TranslationMap point5VTc95;
    static TranslationMap coordVTc95;
    static TranslationMap aircraftTypeVTc95;
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TRANSLITERATION_H
