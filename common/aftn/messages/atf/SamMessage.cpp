#include "SamMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

SamMessage::SamMessage()
{
}

SamMessage::SamMessage(const AtfFplStruct &s) :
    AtfFplMessage(s)
{
}

SamMessage::~SamMessage()
{
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
