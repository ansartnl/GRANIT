#ifndef FPLMESSAGE_H
#define FPLMESSAGE_H

#include "AtsMessage.h"
#include <AlrMessageShort.h>
#include <AlrMessage.h>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \struct FplStruct
 * \brief Структура сообщения FPL
 */

struct FplStruct : FplFields
{

};

inline QDataStream & operator<< (QDataStream &stream, const FplStruct &msg)
{
    stream << (const FplFields &)msg;
    return stream;
}

inline QDataStream & operator>> (QDataStream &stream, FplStruct &msg)
{
    stream >> (FplFields &)msg;
    return stream;
}

inline bool operator ==(const FplStruct &s1, const FplStruct &s2)
{
    return (operator ==(static_cast<FplFields>(s1), static_cast<FplFields>(s2))
            &&(s1.additionalInfo == s2.additionalInfo));
}

class ArrMessage;
class ChgMessage;
class CnlMessage;
class DepMessage;
class DlaMessage;
class FlaMessage;
class PpnMessage;
class UlsMessage;
class RqpMessage;
class RqsMessage;
class SplMessage;

class AnpMessage;
class AlrMessageShort;

class AtfFplMessage;
class SltMessage;

struct AldStruct;
struct ArrStruct;
struct ChgStruct;
struct CnlStruct;
struct DepStruct;
struct DlaStruct;
struct FlaStruct;
struct PpnStruct;
struct RcfStruct;
struct RqpStruct;
struct RqsStruct;
struct SplStruct;
struct UlsStruct;
struct UtpStruct;

struct AnpStruct;
struct AlrStructShort;
struct SltStruct;

struct AtfFplStruct;

/*!
 * \class FplMessage
 * \brief Класс FPL сообщения
 */

class FplMessage : public AtsMessage
{
public:
    FplMessage();
    FplMessage(const FplStruct &s);
    virtual ~FplMessage();

    QString data() const;
    QString adexp() const;

    void setVersion(ParseVersion::Version v);

    FplChange changes() const;
    AtsStruct ats() const;

    ArrMessage * arr() const;
    ChgMessage * chg() const;
    CnlMessage * cnl() const;
    DepMessage * dep() const;
    DlaMessage * dla() const;
    FlaMessage * fla() const;
    PpnMessage * ppn() const;
    UlsMessage * uls() const;
    RqpMessage * rqp() const;
    RqsMessage * rqs() const;
    SplMessage * spl() const;

    AnpMessage * anp() const;
    AlrMessageShort * alrShort() const;
    AlrMessage *alr() const;

    AtfFplMessage * atf() const;
    SltMessage * slt() const;

    const ArrStruct arrStruct() const;
    const ChgStruct chgStruct() const;
    const CnlStruct cnlStruct() const;
    const DepStruct depStruct() const;
    const DlaStruct dlaStruct() const;
    const FlaStruct flaStruct() const;
    const PpnStruct ppnStruct() const;
    const UlsStruct ulsStruct() const;
    const RqpStruct rqpStruct() const;
    const RqsStruct rqsStruct() const;
    const SplStruct splStruct() const;
    const AldStruct aldStruct() const;
    const RcfStruct rcfStruct() const;

    const AnpStruct anpStruct() const;
    const AlrStructShort alrStructShort() const;
    const AlrStruct alrStruct() const;

    const AtfFplStruct atfStruct() const;
    const SltStruct sltStruct() const;

    FplStruct mFpl;

    inline friend QDataStream & operator<< (QDataStream &stream, const FplMessage &msg)
    {
        stream << (const AtsMessage &)msg;
        stream << msg.mFpl;
        return stream;
    }

    inline friend QDataStream & operator>> (QDataStream &stream, FplMessage &msg)
    {
        stream >> (AtsMessage &)msg;
        stream >> msg.mFpl;
        return stream;
    }
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
Q_DECLARE_METATYPE(Aftn::FplStruct)
#endif
#ifndef AFTN_NAMESPACE
Q_DECLARE_METATYPE(FplStruct)
#endif

#endif // FPLMESSAGE_H
