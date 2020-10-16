#ifndef TELEGRAMTYPES_H
#define TELEGRAMTYPES_H

#include "ParseVersion.h"

#include <QtCore/QStringList>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

namespace TelegramType
{

const char Unknown[] = "???";
const char Part[] = "Part";
const char Pack[] = "Pack";

inline char isPart(const QString &type)
{
    return (type == Part);
}

inline char isPack(const QString &type)
{
    return (type == Pack);
}

inline char isUnknown(const QString &type)
{
    return (type == Unknown);
}

const QString FreeText = "Free text";
const QString FreeText1 = QString::fromUtf8("Текст");

inline bool isFreeText(const QString &type)
{
    return (type == FreeText) || (type == FreeText1);
}

inline const QString freeText(ParseVersion::Version v)
{
    if (v == ParseVersion::PV_CYRILLIC)
        return FreeText1;
    return FreeText;
}

// Служебные

const char CH[] = "CH";
const char CH1[] = "ЦХ";

inline bool isCh(const QString &type)
{
    return (type == CH) || (type == CH1);
}

const char SVC[] = "SVC";
const char SVC1[] = "СЖЦ";

inline bool isSvc(const QString &type)
{
    return (type == SVC) || (type == SVC1);
}

const char RTC[] = "RTC";
const char RES[] = "RES";

inline bool isRtc(const QString &type)
{
    return (type == RTC);
}

inline bool isRes(const QString &type)
{
    return (type == RES);
}

// ОВД

const char FPL[] = "FPL";

const char ARR[] = "ARR";
const char CHG[] = "CHG";
const char CNL[] = "CNL";
const char DEP[] = "DEP";
const char DLA[] = "DLA";

const char FPL1[] = "ФПЛ";

const char ARR1[] = "АРР";
const char CHG1[] = "ЦХГ";
const char CNL1[] = "ЦНЛ";
const char DEP1[] = "ДЕП";
const char DLA1[] = "ДЛА";

const char PLN[] = "PLN";
const char PLN1[] = "ПЛН";
const char PPN[] = "PPN";
const char PPN1[] = "ППН";

const char ULS[] = "ULS";
const char ULS1[] = "УЛС";
const char FLA[] = "FLA";
const char FLA1[] = "ФЛА";

const char PLR[] = "PLR";

const char RQP[] = "RQP";
const char RQS[] = "RQS";
const char SPL[] = "SPL";

const char UTP1[] = "УТП";

const char ALD[] = "ALD";
const char ALD1[] = "АЛД";

const char RCF[] = "RCF";
const char RCF1[] = "РЦФ";

inline bool isFpl(const QString &type)
{
    return ((type == FPL) || (type == FPL1));
}
inline bool isArr(const QString &type)
{
    return ((type == ARR) || (type == ARR1));
}
inline bool isChg(const QString &type)
{
    return ((type == CHG) || (type == CHG1));
}
inline bool isCnl(const QString &type)
{
    return ((type == CNL) || (type == CNL1));
}
inline bool isDep(const QString &type)
{
    return ((type == DEP) || (type == DEP1));
}
inline bool isDla(const QString &type)
{
    return ((type == DLA) || (type == DLA1));
}
inline bool isPln(const QString &type)
{
    return ((type == PLN) || (type == PLN1));
}
inline bool isPpn(const QString &type)
{
    return ((type == PPN) || (type == PPN1));
}
inline bool isUls(const QString &type)
{
    return (type == ULS) || (type == ULS1);
}
inline bool isFla(const QString &type)
{
    return (type == FLA) || (type == FLA1);
}
inline bool isPlr(const QString &type)
{
    return (type == PLR);
}
inline bool isRqp(const QString &type)
{
    return (type == RQP);
}
inline bool isRqs(const QString &type)
{
    return (type == RQS);
}
inline bool isSpl(const QString &type)
{
    return (type == SPL);
}
inline bool isUtp(const QString &type)
{
    return (type == UTP1);
}
inline bool isAld(const QString &type)
{
    return (type == ALD) || (type == ALD1);
}
inline bool isRcf(const QString &type)
{
    return (type == RCF) || (type == RCF1);
}

inline bool isAts(const QString &type)
{
    return (isFpl(type)
            || isArr(type)
            || isChg(type)
            || isCnl(type)
            || isDep(type)
            || isDla(type));
}

// RPL

const char RPL[] = "RPL";
const char RPL1[] = "РПЛ";
const char CNL_RPL1[] = "ЦНЛ РПЛ";

inline bool isRpl(const QString &type)
{
    return ((type == RPL) || (type == RPL1));
}

inline bool isCnlRpl(const QString &type)
{
    return (type == CNL_RPL1);
}

// НОТАМ

const char NOTAM[] = "NOTAM";
const char NOTAM1[] = "НОТАМ";

inline bool isNotam(const QString &type)
{
    return (type == NOTAM)
            || (type == NOTAM1);
}

const char RQN[] = "RQN";

inline bool isRqn(const QString &type)
{
    return type == RQN;
}

// ИВП
const char ANP[] = "ANP";
const char ANP1[] = "АНП";
const char RVM[] = "RVM";
const char RVM1[] = "РЖМ";
const char RVI[] = "RVI";
const char RVI1[] = "РЖИ";
const char APZ[] = "APZ";
const char APZ1[] = "АПЗ";
const char APW[] = "APW";
const char APW1[] = "АПВ";
const char ALR[] = "ALR";
const char ALR1[] = "АЛР";

inline bool isAlr(const QString &type)
{
    return ((type == ALR) || (type == ALR1));
}

inline bool isAnp(const QString &type)
{
    return ((type == ANP) || (type == ANP1));
}

inline bool isRvm(const QString &type)
{
    return ((type == RVM) || (type == RVM1));
}

inline bool isRvi(const QString &type)
{
    return ((type == RVI) || (type == RVI1));
}

inline bool isApz(const QString &type)
{
    return ((type == APZ) || (type == APZ1));
}

inline bool isApw(const QString &type)
{
    return ((type == APW) || (type == APW1));
}

inline bool isAtm(const QString &type)
{
    return (isAnp(type)
            || isRvm(type)
            || isRvi(type)
            || isApz(type)
            || isApw(type)
            || isAlr(type));
}

inline bool isRestriction(const QString &type)
{
    return (type == RVM1 || type == RVI1);

}
// ОПВД

const char SPS[] = "SPS";
const char SPS1[] = "СПС";

const char AIM[] = "AIM";

const char ANM[] = "ANM";
const char ANM1[] = "АНМ";

const char SCN[] = "SCN";
const char SCN1[] = "СЦН";

const char RRP[] = "RRP";
const char RRP1[] = "РРП";

const char SAM[] = "SAM";
const char SAM1[] = "САМ";

const char SLC[] = "SLC";
const char SLC1[] = "СЛЦ";

const char SLT[] = "SLT";
const char SLT1[] = "СЛТ";

const char CSL[] = "CSL";
const char CSL1[] = "ЦСЛ";

const char ACP[] = "ACP";
const char ACP1[] = "АЦП";

inline bool isSps(const QString &type)
{
    return ((type == SPS) || (type == SPS1));
}
inline bool isAim(const QString &type)
{
    return (type == AIM);
}
inline bool isAnm(const QString &type)
{
    return ((type == ANM) || (type == ANM1));
}
inline bool isScn(const QString &type)
{
    return ((type == SCN) || (type == SCN1));
}
inline bool isRrp(const QString &type)
{
    return ((type == RRP) || (type == RRP1));
}
inline bool isSam(const QString &type)
{
    return ((type == SAM) || (type == SAM1));
}
inline bool isSlc(const QString &type)
{
    return ((type == SLC) || (type == SLC1));
}
inline bool isSlt(const QString &type)
{
    return ((type == SLT) || (type == SLT1));
}
inline bool isCsl(const QString &type)
{
    return ((type == CSL) || (type == CSL1));
}

inline bool isAtfFpl(const QString &type)
{
    return isRrp(type)
            || isSam(type)
            || isSlc(type)
            || isSlt(type)
            || isCsl(type);
}

inline bool isAtf(const QString &type)
{
    return isSps(type)
            || isAim(type)
            || isAnm(type)
            || isScn(type)
            || isAtfFpl(type);
}

inline bool isAcp(const QString &type)
{
    return ((type == ACP) || (type == ACP1));
}

// Meteo

const char METAR[] = "METAR";

inline bool isMetar(const QString &type)
{
    return (type == METAR);
}

const char SPECI[] = "SPECI";

inline bool isSpeci(const QString &type)
{
    return (type == SPECI);
}

const char TAF[] = "TAF";

inline bool isTaf(const QString &type)
{
    return (type == TAF);
}

const char SIGMET[] = "SIGMET";

inline bool isSigmet(const QString &type)
{
    return (type == SIGMET);
}

const char AIRMET[] = "AIRMET";

inline bool isAirmet(const QString &type)
{
    return (type == AIRMET);
}

const char GAMET[] = "GAMET";

inline bool isGamet(const QString &type)
{
    return (type == GAMET);
}

inline bool isMeteo(const QString &type)
{
    return isMetar(type)
            || isSpeci(type)
            || isTaf(type)
            || isSigmet(type)
            || isAirmet(type)
            || isGamet(type);
}

// not standard messages

// get fpl list
const char GPL[] = "GPL";

inline bool isGpl(const QString &type)
{
    return (type == GPL);
}

const char MAN[] = "MAN";

inline bool isMan(const QString &type)
{
    return (type == MAN);
}

// test fpl
const char TPL[] = "TPL";

inline bool isTpl(const QString &type)
{
    return (type == TPL);
}

const char ACK[] = "ACK";
const char ACK1[] = "АЦК";
const char REJ[] = "REJ";
const char REJ1[] = "РЕЙ";

inline bool isAck(const QString &type)
{
    return (type == ACK) || (type == ACK1);
}

inline bool isRej(const QString &type)
{
    return (type == REJ) || (type == REJ1);
}

inline bool isNonStandard(const QString &type)
{
    return (isGpl(type)
            || isTpl(type)
            || isAck(type)
            || isRej(type));
}

// Принадлежность версии

inline bool isMixed(const QString &type)
{
    return (isTpl(type)
            || isAck(type)
            || isRej(type));
}

inline bool isTC95(const QString &type)
{
    return (type == CH1)
            || (type == SVC1)
            || (type == ARR1)
            || (type == CHG1)
            || (type == CNL1)
            || (type == DEP1)
            || (type == DLA1)
            || (type == FPL1)
            || (type == PLN1)
            || (type == PPN1)
            || (type == ULS1)
            || (type == FLA1)
            || (type == RPL1)
            || (type == NOTAM1)
            || (type == ANP1)
            || (type == RVM1)
            || (type == RVI1)
            || (type == APZ1)
            || (type == APW1)
            || (type == ALR1)
            || (type == SPS1)
            || (type == ANM1)
            || (type == SCN1)
            || (type == RRP1)
            || (type == SAM1)
            || (type == SLC1)
            || (type == SLT1)
            || (type == CSL1)
            || (type == CNL_RPL1)
            || (type == UTP1)
            || (type == ALD1)
            || (type == RCF1)
            || (type == ACP1);
}

inline bool is4444(const QString &type)
{
    return (type == CH)
            || (type == SVC)
            || (type == ARR)
            || (type == CHG)
            || (type == CNL)
            || (type == DEP)
            || (type == DLA)
            || (type == FPL)
            || (type == PLN)
            || (type == PPN)
            || (type == RPL)
            || (type == NOTAM)
            || (type == ALR)
            || (type == SPS)
            || (type == AIM)
            || (type == ANM)
            || (type == SCN)
            || (type == RRP)
            || (type == SAM)
            || (type == SLC)
            || (type == SLT)
            || (type == CSL)
            || (type == PLR)
            || (type == RQP)
            || (type == RQS)
            || (type == SPL)
            || (type == ALD)
            || (type == RCF)
            || (type == ACP);
}

inline bool isSupported(const QString &type)
{
    return isMixed(type)
            || isTC95(type)
            || is4444(type)
            || isGpl(type);
}

inline QStringList allTypes()
{
    return QStringList()
            << CH << CH1 << SVC << SVC1
            << ARR << ARR1 << CHG << CHG1 << CNL << CNL1 << DEP << DEP1 << DLA << DLA1 << FPL << FPL1
            << PLN << PLN1 << PPN << PPN1 << ULS1 << FLA1 << RPL << RPL1
            << ALD << ALD1 << RCF << RCF1
            << NOTAM << NOTAM1
            << ANP1 << RVM1 << RVI1 << APZ1 << APW1 << ALR << ALR1
            << SPS << SPS1 << AIM << ANM << ANM1 << SCN << SCN1 << RRP << RRP1
            << SAM << SAM1 << SLC << SLC1 << SLT << SLT1 << CSL << CSL1
            << ACP << ACP1
            << PLR << UTP1;
}

inline ParseVersion::Version typeVersion(const QString &type)
{
    if (isTC95(type))
        return ParseVersion::PV_CYRILLIC;
    if (is4444(type))
        return ParseVersion::PV_LATIN;
    if (isMixed(type))
        return ParseVersion::PV_MIXED;
    return ParseVersion::PV_UNKNOWN;
}

} // namespace TelegramType

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // TELEGRAMTYPES_H
