#ifndef SVCPARSER_H
#define SVCPARSER_H

#include <QtCore/QString>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class SvcAdsMessage;
class SvcLrExpMessage;
class SvcLrLsMessage;
class SvcMisChMessage;
class SvcQtaAdsMessage;
class SvcQtaMisMessage;
class SvcQtaMsrMessage;
class SvcQtaOgnMessage;
class SvcQtaRptMessage;
class SvcQtaRptMessage2;

namespace SvcParser
{

bool isSvc(const QString &data);

SvcAdsMessage * svcAds(const QString &data);
SvcLrLsMessage * svcLrLs(const QString &data);
SvcLrExpMessage * svcLrExp(const QString &data);
SvcMisChMessage * svcMisCh(const QString &data);
SvcQtaAdsMessage * svcQtaAds(const QString &data);
SvcQtaMisMessage * svcQtaMis(const QString &data);
SvcQtaMsrMessage * svcQtaMsr(const QString &data);
SvcQtaOgnMessage * svcQtaOgn(const QString &data);
SvcQtaRptMessage * svcQtaRpt(const QString &data);
SvcQtaRptMessage2 * svcQtaRpt2(const QString &data);

} // namespace SvcParser

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // SVCPARSER_H
