#ifndef ADEXPPARSERPRIVATE_H
#define ADEXPPARSERPRIVATE_H

#include "AdexpParser.h"

#include <QtCore/QStringList>

#include <QtSql/QSqlDatabase>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AdexpParserPrivate {

public:
  AdexpParserPrivate();
  virtual ~AdexpParserPrivate();

  QSqlDatabase database;

  bool saveOnlyFir;
  bool validateRoute;
  QString firName;

  int firId;

  QStringList flightRules;
  QStringList flightRulesR;
  QStringList flightTypes;
  QStringList flightTypesR;

  QStringList turbulenceCategories;
  QStringList turbulenceCategoriesR;
  QStringList radioDevices;
  QStringList radioDevicesR;
  QStringList equipment;
  QStringList equipmentR;

  QStringList standardDevice12;
  QStringList radioDevices12;
  QStringList equipment12;

  QStringList standardDevice12R;
  QStringList radioDevices12R;
  QStringList equipment12R;

  QStringList borderTerms;
  QStringList borderTermsR;

  QHash<QString, int> sids;
  QHash<QString, int> stars;

  QHash<QString, int> routes;
  QHash<QString, int> points;

  QHash<QString, QStringList > routepoint;
  QStringList airports;

};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ADEXPPARSERPRIVATE_H
