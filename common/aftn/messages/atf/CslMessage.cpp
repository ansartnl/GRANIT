#include "CslMessage.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

CslMessage::CslMessage()
{
}

CslMessage::CslMessage(const AtfFplStruct &s) :
    AtfFplMessage(s)
{
}

CslMessage::~CslMessage()
{
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
