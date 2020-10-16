#include "SnmpHelper.h"

#include <QtCore/QDateTime>
#include <QtCore/QRegExp>

namespace snmp_helper
{

const snmp::BaseSnmpVariable getSnmpInfo(const QString &host, const oid *objid, size_t objidSize)
{
    snmp::BaseSnmpVariable ret;

    static QRegExp rexp1("(.*)::(.*)\\.(\\d{1,3})");
    static QRegExp rexp2("(.*)::(.*)");
    static const size_t buffLen = 30000;
    static char buff[buffLen];

    snprint_objid(buff, buffLen, objid, objidSize);
    QString objidString(buff);

    if (rexp1.exactMatch(objidString)) {
        ret.host = host;
        ret.mib = rexp1.cap(1);
        ret.name = rexp1.cap(2);
        ret.index = rexp1.cap(3).toInt();
        return ret;
    }

    if (rexp2.exactMatch(objidString)) {
        ret.host = host;
        ret.mib = rexp2.cap(1);
        ret.name = rexp2.cap(2);
        return ret;
    }

    ret.name = objidString;
    return ret;
}

bool isWritable(const oid *objid, size_t objidSize)
{
    bool ret = false;

    tree *t = get_tree(objid, objidSize, get_tree_head());

    switch (t->access) {
    case MIB_ACCESS_READWRITE:
    case MIB_ACCESS_WRITEONLY:
        ret = true;
        break;
    }
    return ret;
}

const Value valueByType(const QString &value, int type)
{
    switch (type) {
    case ASN_INTEGER: {
        QRegExp rexp("^INTEGER: (-?\\d{1,10})$");
        if (rexp.exactMatch(value)) {
            return Value(rexp.cap(1).toInt(), snmp::SnmpVariable::T_INTEGER);
        }
        rexp.setPattern("^INTEGER: (Stopped\\(|Run\\(|Running\\()(1|2|3)(\\))$");
        if (rexp.exactMatch(value)) {
            return Value(QVariant::fromValue(snmp_private::RunScript(
                                                 snmp::SnmpScript::RunScript(rexp.cap(2).toInt()))
                                             )
                         , snmp::SnmpVariable::T_INTEGER);
        }
        rexp.setPattern("^INTEGER: (Down\\(|Up\\()(1|2)(\\))$");
        if (rexp.exactMatch(value)) {
            return Value(QVariant::fromValue(
                             snmp_private::ProgramState(
                                 snmp::SnmpProgramState::ProgramState(rexp.cap(2).toInt())
                                 )
                             )
                         , snmp::SnmpVariable::T_INTEGER);
        }

        return Value(SnmpTr::tr("Cannot process integer value '%1'.").arg(value)
                     , snmp::SnmpVariable::T_UNKNOWN);
    }
    case ASN_OCTET_STR: {
        QRegExp dateAndTimeRexp(
                    "^STRING: "
                    // year: 0..65536
                    "(\\d{1,4}|[0-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d{1}|6553[0-5])"
                    "-"
                    // month: 1..12
                    "(\\d{1}|11|12)"
                    "-"
                    // day: 1..31
                    "(\\d{1}|[0-2]\\d{1}|3[0-1])"
                    ","
                    // hour: 0..23
                    "(\\d{1}|[0-1]\\d{1}|2[0-3])"
                    ":"
                    // minutes: 0..59
                    "(\\d{1}|[0-5]\\d{1})"
                    ":"
                    // seconds: 0..60
                    "(\\d{1}|[0-5]\\d{1}|60)"
                    "."
                    // deci-seconds: 0..9
                    "(\\d{1})"
                    "$");
        QRegExp dateAndTimeWithZoneRexp(
                    "^STRING: "
                    // year: 0..65536
                    "(\\d{1,4}|[0-5]\\d{4}|6[0-4]\\d{3}|65[0-4]\\d{2}|655[0-2]\\d{1}|6553[0-5])"
                    "-"
                    // month: 1..12
                    "(\\d{1}|11|12)"
                    "-"
                    // day: 1..31
                    "(\\d{1}|[0-2]\\d{1}|3[0-1])"
                    ","
                    // hour: 0..23
                    "(\\d{1}|[0-1]\\d{1}|2[0-3])"
                    ":"
                    // minutes: 0..59
                    "(\\d{1}|[0-5]\\d{1})"
                    ":"
                    // seconds: 0..60
                    "(\\d{1}|[0-5]\\d{1}|60)"
                    "."
                    // deci-seconds: 0..9
                    "(\\d{1})"
                    ","
                    // direction from UTC: '+'|'-'
                    "(\\+|-)"
                    // hours from UTC: 0..13
                    "(\\d{1}|1[0-3])"
                    ":"
                    // minutes from UTC: 0..59
                    "(\\d{1}|[0-5]\\d{1})"
                    "$");
        QRegExp rexp("^STRING: (.*)$");
        if (dateAndTimeRexp.exactMatch(value)) {
            QDateTime d(QDate(dateAndTimeRexp.cap(1).toInt()
                              , dateAndTimeRexp.cap(2).toInt()
                              , dateAndTimeRexp.cap(3).toInt())
                        , QTime(dateAndTimeRexp.cap(4).toInt()
                                , dateAndTimeRexp.cap(5).toInt()
                                , dateAndTimeRexp.cap(6).toInt()
                                , dateAndTimeRexp.cap(7).toInt() * 100)
                        , Qt::UTC);
            return Value(d, snmp::SnmpVariable::T_DATEANDTIME);
        } else if (dateAndTimeWithZoneRexp.exactMatch(value))
        {
            QDateTime d(QDate(dateAndTimeWithZoneRexp.cap(1).toInt()
                              , dateAndTimeWithZoneRexp.cap(2).toInt()
                              , dateAndTimeWithZoneRexp.cap(3).toInt())
                        , QTime(dateAndTimeWithZoneRexp.cap(4).toInt()
                                , dateAndTimeWithZoneRexp.cap(5).toInt()
                                , dateAndTimeWithZoneRexp.cap(6).toInt()
                                , dateAndTimeWithZoneRexp.cap(7).toInt() * 100)
                        , Qt::UTC);
            if (dateAndTimeWithZoneRexp.cap(8) == "+")
                d.time().addSecs((dateAndTimeWithZoneRexp.cap(9).toInt() * 60
                                  + dateAndTimeWithZoneRexp.cap(10).toInt()) * -60);
            else if (dateAndTimeWithZoneRexp.cap(8) == "-")
                d.time().addSecs((dateAndTimeWithZoneRexp.cap(9).toInt() * 60
                                  + dateAndTimeWithZoneRexp.cap(10).toInt()) * 60);
            return Value(d, snmp::SnmpVariable::T_DATEANDTIME);
        } else if (rexp.exactMatch(value)) {
            return Value(rexp.cap(1), snmp::SnmpVariable::T_STRING);
        }

        return Value(SnmpTr::tr("Cannot process string value '%1'.").arg(value)
                     , snmp::SnmpVariable::T_UNKNOWN);
    }
    case ASN_IPADDRESS: {
        QRegExp rexp("^IpAddress: ((\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3})\\.(\\d{1,3}))$");
        if (rexp.exactMatch(value)) {
            return Value(rexp.cap(1), snmp::SnmpVariable::T_IPADDRESS);
        }

        return Value(SnmpTr::tr("Cannot process ip address value '%1'.").arg(value)
                     , snmp::SnmpVariable::T_UNKNOWN);
    }
    case ASN_COUNTER64: {
        QRegExp rexp("^Counter64: (\\d+)$");
        if (rexp.exactMatch(value)) {
            return Value(rexp.cap(1).toULongLong(), snmp::SnmpVariable::T_COUNTER64);
        }

        return Value(SnmpTr::tr("Cannot process counter64 value '%1'.").arg(value)
                     , snmp::SnmpVariable::T_UNKNOWN);
    }
    case ASN_COUNTER: {
        QRegExp rexp("^Counter32: (\\d+)$");
        if (rexp.exactMatch(value)) {
            return Value(rexp.cap(1).toUInt(), snmp::SnmpVariable::T_COUNTER32);
        }

        return Value(SnmpTr::tr("Cannot process counter64 value '%1'.").arg(value)
                     , snmp::SnmpVariable::T_UNKNOWN);
    }
    }
    return Value(SnmpTr::tr("Type '%1' is not supported for value '%2'.").arg(type).arg(value)
                 , snmp::SnmpVariable::T_UNKNOWN);
}

} // namespace snmp_helper
