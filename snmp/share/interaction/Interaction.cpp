#include "Interaction.h"

namespace snmp_interaction
{

static int i00[] = {

    // Handshake.h
    qRegisterMetaTypeStreamOperators < snmp_interaction::Handshake >()

    // PingPong.h
    , qRegisterMetaTypeStreamOperators < snmp_interaction::Ping >()

    // GetValue.h
    , qRegisterMetaTypeStreamOperators < snmp_interaction::GetValue >()
    , qRegisterMetaTypeStreamOperators < snmp_interaction::Value >()

    // SetValue.h
    , qRegisterMetaTypeStreamOperators < snmp_interaction::SetValue >()
    , qRegisterMetaTypeStreamOperators < snmp_interaction::SetValueResult >()
    , qRegisterMetaTypeStreamOperators < snmp_interaction::SetValueResult::SetResult >()

    // Notify
    , qRegisterMetaType < snmp_interaction::Vars >()
    , qRegisterMetaTypeStreamOperators < snmp_interaction::Notify >()

};

} // namespace snmp_interaction

