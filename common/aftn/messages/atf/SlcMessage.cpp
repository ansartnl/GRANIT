#include "SlcMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SlcMessage::SlcMessage()
{
}

SlcMessage::SlcMessage(const AtfFplStruct &s) :
    AtfFplMessage(s)
{
}

SlcMessage::~SlcMessage()
{
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
