#ifndef ASTERIX253STRUCT_H
#define ASTERIX253STRUCT_H

#include <QtCore/QMetaType>

struct Asterix253Struct
{
    enum State
    {
        NotSet,
        Master,
        Slave,
        Undefined
    };

    enum Mode
    {
        Mode0,
        Mode1
    };

    struct Source
    {
        quint8 sac;
        quint8 sic;
    };

    struct ChannelState
    {
        enum State
        {
            Valid,
            NotConnected,
            NotInitiated,
            Disabled
        };

        Source dataSource;
        State first;
        State second;
    };

    struct AdapterState
    {
        enum State
        {
            Valid,
            NotConnected,
            NotInitiated
        };

        State state;
    };

    // Идентификатор источника данных
    Source dataSource;

    // Тип сообщения
    quint8 messageType;

    // Сервисный идентификатор (номер МОРЛИ (1 или 2))
    quint8 serviceIdentifier;

    // Статус МОРЛИ
    State state;

    // Режим выдачи РЛИ
    Mode mode;

    // Состояние каналов РЛК
    QList<ChannelState> channelState;

    // Состояние ЛВС
    QList<AdapterState> adapterState;
};

namespace Asterix253Tools
{

    inline QString stateAsString(Asterix253Struct::State state)
    {
        switch(state)
        {
        case Asterix253Struct::NotSet:
            return "Not Set";
        case Asterix253Struct::Master:
            return "Master";
        case Asterix253Struct::Slave:
            return "Slave";
        case Asterix253Struct::Undefined:
            return "Undefined";
        default:
            return "Unknown";
        }
        return "Unknown";
    }

    inline QString stateAsString(Asterix253Struct::ChannelState::State state)
    {
        switch(state)
        {
        case Asterix253Struct::ChannelState::Valid:
            return "Valid";
        case Asterix253Struct::ChannelState::NotConnected:
            return "Not Connected";
        case Asterix253Struct::ChannelState::NotInitiated:
            return "Not Initiated";
        case Asterix253Struct::ChannelState::Disabled:
            return "Disabled";
        default:
            return "Unknown";
        }
        return "Unknown";
    }

}   // Asterix253Tools

inline bool operator ==(const Asterix253Struct::Source& s1, const Asterix253Struct::Source& s2)
{
    return s1.sac == s2.sac && s1.sic == s2.sic;
}

inline uint qHash(const Asterix253Struct::Source& source)
{
    return qHash(source.sac) ^ qHash(source.sic);
}

Q_DECLARE_METATYPE(Asterix253Struct)

#endif // ASTERIX253STRUCT_H
