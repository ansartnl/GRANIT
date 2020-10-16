#ifndef ADEXPPARSER_H
#define ADEXPPARSER_H

#include <QObject>

#include <QVariantMap>

#include "AftnMessageParser.h"

#include "ParseVersion.h"

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

// internal functions
typedef void (*ProcessFieldCallback)(
        const QString& field, QVariantMap& map, AftnMessageErrors& errorCodes);

typedef void (*ValidateFieldCallback)(
        QVariantMap& map, AftnMessageErrors& errorCodes);

class AdexpParser : public QObject
{
    Q_OBJECT

public:
    explicit AdexpParser(QObject *parent = 0);
    virtual ~AdexpParser();

    inline void setStandard(Standard s = FPL_2007)
    {
        standard = s;
    }

    QVariantMap processMessage(const QString &message, AftnMessageErrors &errorCodes);

    bool isSaveOnlyFIR() const;
    void setSaveOnlyFIR(bool enable);

    QString FIRName() const;
    void setFIRName(const QString& firName);

    bool isValidateRoute() const;
    void setValidateRoute(bool validate);

    #define MAXFIELDS               23
    struct FieldProcessor
    {
        const QString type;
        ProcessFieldCallback callbacks[MAXFIELDS];
        ValidateFieldCallback validateCallbacks[MAXFIELDS];
    };

    static const FieldProcessor fieldProcessors[];

    QList<int> errorLines;

protected:
    QStringList createFpl(QMultiMap < QString, QString > splittedMessage);
    QStringList createArr(QMultiMap < QString, QString > splittedMessage);
    QStringList createDep(QMultiMap < QString, QString > splittedMessage);
    QStringList createCnl(QMultiMap < QString, QString > splittedMessage);
    QStringList createDla(QMultiMap < QString, QString > splittedMessage);
    QStringList createChg(QMultiMap < QString, QString > splittedMessage);
    QStringList createAck(QMultiMap < QString, QString > splittedMessage);
    QStringList createRej(QMultiMap < QString, QString > splittedMessage);
    QStringList createMan(QMultiMap < QString, QString > splittedMessage);

    QString createField3(QMultiMap<QString, QString> splittedMessage);
    QString createField7(QMultiMap<QString, QString> splittedMessage);
    QString createField8(QMultiMap<QString, QString> splittedMessage);
    QString createField9(QMultiMap<QString, QString> splittedMessage);
    QString createField10(QMultiMap<QString, QString> splittedMessage);
    QString createField13(QMultiMap<QString, QString> splittedMessage);
    QString createField15(QMultiMap<QString, QString> splittedMessage);
    QString createField16(QMultiMap<QString, QString> splittedMessage);
    QString createField17(QMultiMap<QString, QString> splittedMessage);
    QString createField18(QMultiMap<QString, QString> splittedMessage);

    QString getField(QStringList fieldsMessage, QString key);
    const QMap < QString, QString > field18Subfields(ParseVersion::Version v);
};

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif

#endif // ADEXPPARSER_H
