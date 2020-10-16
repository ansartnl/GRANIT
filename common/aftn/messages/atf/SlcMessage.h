#ifndef SLCMESSAGE_H
#define SLCMESSAGE_H

#include "AtfFplMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

/*!
 * \class SlcMessage
 * \brief Класс сообщения SLC/СЛЦ
 */

class SlcMessage : public AtfFplMessage
{
public:
    SlcMessage();
    SlcMessage(const AtfFplStruct &s);
    virtual ~SlcMessage();
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SLCMESSAGE_H
