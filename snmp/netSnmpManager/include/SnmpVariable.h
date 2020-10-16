#ifndef SNMPVARIABLE_H
#define SNMPVARIABLE_H

#include "enumstreaming.h"

class SnmpTr : public QObject
{
    Q_OBJECT
};

namespace snmp
{

/*!
 * \struct BaseSnmpVariable
 * \brief Базовая структура для всех SNMP переменных
 */

struct BaseSnmpVariable
{
    QString host;
    QString mib;
    QString name;
    int index;

    BaseSnmpVariable() :
        index(0)
    {}

    inline const QString oidString() const
    {
        return QString("%1::%2.%3").arg(mib).arg(name).arg(index); // example: 'TEST-MIB::value.0'
    }
};

inline QDataStream & operator <<(QDataStream &stream, const BaseSnmpVariable &msg)
{
    stream << msg.host;
    stream << msg.mib;
    stream << msg.name;
    stream << msg.index;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, BaseSnmpVariable &msg)
{
    stream >> msg.host;
    stream >> msg.mib;
    stream >> msg.name;
    stream >> msg.index;

    return stream;
}

/*!
 * \struct SnmpVariable
 * \brief Стандартная SNMP переменная, содержащая данные одного из базовых типов
 */

struct SnmpVariable : public BaseSnmpVariable
{
    QVariant value;
    bool writable;
    enum Type {
        T_UNKNOWN
        , T_INTEGER
        , T_STRING
        , T_IPADDRESS
        , T_DATEANDTIME
        , T_COUNTER32
        , T_COUNTER64
    } type;

    SnmpVariable() :
        BaseSnmpVariable(), writable(false), type(T_UNKNOWN)
    {}
    SnmpVariable(const BaseSnmpVariable &base) :
        BaseSnmpVariable(base), writable(false), type(T_UNKNOWN)
    {}

    inline const QString typeString() const
    {
        switch (type) {
        case T_UNKNOWN:
            return SnmpTr::tr("Unknown");
        case T_INTEGER:
            return SnmpTr::tr("Integer");
        case T_STRING:
            return SnmpTr::tr("String");
        case T_IPADDRESS:
            return SnmpTr::tr("Ip Address");
        case T_DATEANDTIME:
            return SnmpTr::tr("Date and Time");
        case T_COUNTER32:
            return SnmpTr::tr("Counter32");
        case T_COUNTER64:
            return SnmpTr::tr("Counter64");
        }
        return SnmpTr::tr("Unhandled");
    }

    inline char typeChar() const
    {
        switch (type) {
        case T_UNKNOWN:
            return 's';
        case T_INTEGER:
            return 'i';
        case T_STRING:
            return 's';
        case T_IPADDRESS:
            return 'a';
        case T_DATEANDTIME:
            return 's'; // 'x' - hex. 'd' - decimal
        case T_COUNTER32:
            return 'i';
        case T_COUNTER64:
            return 'i';
        }
        return 's';
    }
};
QT_DECLARE_ENUM_STREAMING(snmp::SnmpVariable::Type)

inline QDataStream & operator <<(QDataStream &stream, const SnmpVariable &msg)
{
    stream << (const BaseSnmpVariable &)msg;
    stream << msg.value;
    stream << msg.writable;
    stream << msg.type;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, SnmpVariable &msg)
{
    stream >> (BaseSnmpVariable &)msg;
    stream >> msg.value;
    stream >> msg.writable;
    stream >> msg.type;

    return stream;
}

/*!
 * \struct SnmpScript
 * \brief Состояние скрипта
 */

struct SnmpScript : public BaseSnmpVariable
{

    enum RunScript {
        RS_STOPPED = 1
        , RS_RUN = 2
        , RS_RUNNING = 3
    } state;

    SnmpScript() :
        BaseSnmpVariable(), state(RS_STOPPED)
    {}
    SnmpScript(const BaseSnmpVariable &base) :
        BaseSnmpVariable(base), state(RS_STOPPED)
    {}

    inline const QString toString() const
    {
        switch (state) {
        case RS_STOPPED:
            return SnmpTr::tr("Stopped");
        case RS_RUN:
            return SnmpTr::tr("Run");
        case RS_RUNNING:
            return SnmpTr::tr("Running");
        }
        return SnmpTr::tr("Unknown");
    }
};
QT_DECLARE_ENUM_STREAMING(snmp::SnmpScript::RunScript)

inline QDataStream & operator <<(QDataStream &stream, const SnmpScript &msg)
{
    stream << (const BaseSnmpVariable &)msg;
    stream << msg.state;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, SnmpScript &msg)
{
    stream >> (BaseSnmpVariable &)msg;
    stream >> msg.state;

    return stream;
}

/*!
 * \struct SnmpProgramState
 * \brief Состояние программы
 */

struct SnmpProgramState : public BaseSnmpVariable
{

    enum ProgramState {
        PS_DOWN = 1
        , PS_UP = 2
    } state;

    SnmpProgramState() :
        BaseSnmpVariable(), state(PS_DOWN)
    {}
    SnmpProgramState(const BaseSnmpVariable &base) :
        BaseSnmpVariable(base), state(PS_DOWN)
    {}

    inline const QString toString() const
    {
        switch (state) {
        case PS_DOWN:
            return SnmpTr::tr("Down");
        case PS_UP:
            return SnmpTr::tr("Up");
        }
        return SnmpTr::tr("Unknown");
    }
};
QT_DECLARE_ENUM_STREAMING(snmp::SnmpProgramState::ProgramState)

inline QDataStream & operator <<(QDataStream &stream, const SnmpProgramState &msg)
{
    stream << (const BaseSnmpVariable &)msg;
    stream << msg.state;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, SnmpProgramState &msg)
{
    stream >> (BaseSnmpVariable &)msg;
    stream >> msg.state;

    return stream;
}

/*!
 * \struct SnmpTrap
 * \brief Ловушка SNMP
 */

struct SnmpTrap
{
    QString host;
    QString name;
    QList < SnmpVariable > variables;

    SnmpTrap()
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const SnmpTrap &msg)
{
    stream << msg.host;
    stream << msg.name;
    stream << msg.variables;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, SnmpTrap &msg)
{
    stream >> msg.host;
    stream >> msg.name;
    stream >> msg.variables;

    return stream;
}

} // namespace snmp

Q_DECLARE_METATYPE(snmp::SnmpVariable)
Q_DECLARE_METATYPE(snmp::SnmpScript)
Q_DECLARE_METATYPE(snmp::SnmpProgramState)
Q_DECLARE_METATYPE(snmp::SnmpTrap)

namespace snmp_private
{

/*!
 * \struct RunScript
 * \brief Оболочка для \a snmp::SnmpScript::RunScript
 */

struct RunScript
{
    snmp::SnmpScript::RunScript value;

    RunScript() :
        value(snmp::SnmpScript::RS_STOPPED)
    {}
    RunScript(snmp::SnmpScript::RunScript value_) :
        value(value_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const RunScript &msg)
{
    stream << msg.value;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, RunScript &msg)
{
    stream >> msg.value;

    return stream;
}

/*!
 * \struct ProgramState
 * \brief Оболочка для \a snmp::SnmpProgramState::ProgramState
 */

struct ProgramState
{
    snmp::SnmpProgramState::ProgramState value;

    ProgramState() :
        value(snmp::SnmpProgramState::PS_DOWN)
    {}
    ProgramState(snmp::SnmpProgramState::ProgramState value_) :
        value(value_)
    {}
};

inline QDataStream & operator <<(QDataStream &stream, const ProgramState &msg)
{
    stream << msg.value;

    return stream;
}

inline QDataStream & operator >>(QDataStream &stream, ProgramState &msg)
{
    stream >> msg.value;

    return stream;
}

} // namespace snmp_private

Q_DECLARE_METATYPE(snmp_private::RunScript)
Q_DECLARE_METATYPE(snmp_private::ProgramState)

#endif // SNMPVARIABLE_H
