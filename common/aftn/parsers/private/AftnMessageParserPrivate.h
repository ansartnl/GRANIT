#ifndef AFTNMESSAGEPARSERPRIVATE_H
#define AFTNMESSAGEPARSERPRIVATE_H

#include "AftnMessageParser.h"

#include <QtCore/QStringList>

#include <QtSql/QSqlDatabase>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

class AftnMessageParserPrivate {

public:
  AftnMessageParserPrivate();
  virtual ~AftnMessageParserPrivate();

  static bool saveOnlyFir;
  static bool validateRoute;
  static QString firName;

  static int firIdPrivate;

  static QStringList flightRules;
  static QStringList flightRulesR;
  static QStringList flightTypes;
  static QStringList flightTypesR;

  static QStringList turbulenceCategories;
  static QStringList turbulenceCategoriesR;
  static QStringList radioDevices;
  static QStringList radioDevicesR;
  static QStringList equipment;
  static QStringList equipmentR;

  static QStringList standardDevice12;
  static QStringList radioDevices12;
  static QStringList equipment12;

  static QStringList standardDevice12R;
  static QStringList radioDevices12R;
  static QStringList equipment12R;

  static QStringList borderTerms;
  static QStringList borderTermsR;

  static QHash<QString, int> sids;
  static QHash<QString, int> stars;

  static QHash<QString, int> routes;
  static QHash<QString, int> points;

  static QHash<QString, QStringList > routepoint;
  static QStringList airports;

};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // AFTNMESSAGEPARSERPRIVATE_H
