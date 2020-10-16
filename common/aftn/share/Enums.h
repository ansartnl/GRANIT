#ifndef ENUMS_H
#define ENUMS_H

#include "enumstreaming.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

namespace Emergency
{

enum Phase {
    INCERFA     // для стадии неопределенности
    , ALERFA    // для стадии тревоги
    , DETRESFA  // для стадии бедствия
};
QT_DECLARE_ENUM_STREAMING(Phase)

} // namespace Emergency

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ENUMS_H
