#include "AtsMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

const QMap < QString, QString > field18Subfields(ParseVersion::Version v)
{
    QMap < QString, QString > ret;

    ret.insert(Transliteration::toVersion("ALTN", v), "ALTNZ");
    ret.insert(Transliteration::toVersion("CODE", v), "ARCADDR");
    ret.insert(Transliteration::toVersion("COM", v), "COM");
    ret.insert(Transliteration::toVersion("DAT", v), "DAT");
    ret.insert(Transliteration::toVersion("DEP", v), "DEPZ");
    ret.insert(Transliteration::toVersion("DEST", v), "DESTZ");
    ret.insert(Transliteration::toVersion("EET", v), "EETFIR");
    ret.insert(Transliteration::toVersion("DOF", v), "EOBD");
    ret.insert(Transliteration::toVersion("NAV", v), "NAV");
    ret.insert(Transliteration::toVersion("OPR", v), "OPR");
    ret.insert(Transliteration::toVersion("ORGN", v), "ORGN");
    ret.insert(Transliteration::toVersion("PER", v), "PER");
    ret.insert(Transliteration::toVersion("RALT", v), "RALT");
    ret.insert(Transliteration::toVersion("REG", v), "REG");
    ret.insert(Transliteration::toVersion("RFP", v), "RFP");
    ret.insert(Transliteration::toVersion("RIF", v), "RIF");
    ret.insert(Transliteration::toVersion("RMK", v), "RMK");
    ret.insert(Transliteration::toVersion("RVR", v), "RVR");
    ret.insert(Transliteration::toVersion("SEL", v), "SEL");
    ret.insert(Transliteration::toVersion("SRC", v), "SRC");
    ret.insert(Transliteration::toVersion("STS", v), "STS");
    ret.insert(Transliteration::toVersion("TYP", v), "TYPZ");

    return ret;
}

AtsMessage::AtsMessage()
{
}

AtsMessage::~AtsMessage()
{
}

ParseVersion::Version AtsMessage::version() const
{
    return ats().version;
}

QString AtsMessage::otherToAdexp(const QString &other) const
{
    QString ret;

    QString tmp = other;
    QMap < QString, QString > subFields = field18Subfields(ats().version);
    QRegExp rexp(QString("((%1)/(.*))((%1)/|$)").arg(QStringList(subFields.keys()).join("|")));
    rexp.setMinimal(true);
    int index = -1;
    while ((index = rexp.indexIn(tmp)) != -1) {
        ret += QString(" -%1 %2").arg(subFields[rexp.cap(2)]).arg(rexp.cap(3).trimmed());
        tmp.remove(index, rexp.cap(1).size());
    }

    return ret;
}

//void AtsMessage::setVersion(ParseVersion::Version v)
//{
//    AftnMessage::setVersion(v);
//}

QString removeColonFromTime(const QString &time)
{
    if (time.size() < 5)
        return time;
    return time.mid(0, 2) + time.mid(3, 2);
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
