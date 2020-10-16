#ifndef CSLMESSAGE_H
#define CSLMESSAGE_H

#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \class CslMessage
 * \brief Класс сообщения CSL/ЦСЛ
 */

class CslMessage : public AtfFplMessage
{
public:
    CslMessage();
    CslMessage(const AtfFplStruct &s);
    virtual ~CslMessage();
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // CSLMESSAGE_H
