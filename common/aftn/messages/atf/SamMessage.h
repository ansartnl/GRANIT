#ifndef SAMMESSAGE_H
#define SAMMESSAGE_H

#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \class SamMessage
 * \brief Класс сообщения SAM/САМ
 */

class SamMessage : public AtfFplMessage
{
public:
    SamMessage();
    SamMessage(const AtfFplStruct &s);
    virtual ~SamMessage();
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SAMMESSAGE_H
