#include "AdexpParser.h"
#include "private/AftnMessageParserPrivate.h"

#include "ParseErrors.h"
#include "TelegramTypes.h"
#include "Transliteration.h"

#include <QStringList>

#include <QtCore/QTextCodec>

#ifdef AFTN_NAMESPACE
namespace Aftn
{
#endif

static QString fplSignature("FPL");
static QString chgSignature("CHG");
static QString cnlSignature("CNL");
static QString dlaSignature("DLA");
static QString depSignature("DEP");
static QString arrSignature("ARR");

static QString ackSignature("ACK");
static QString rejSignature("REJ");

static QString manSignature("MAN");

static QString alrSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АЛР"));

static QString fplSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ФПЛ"));
static QString chgSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ЦХГ"));
static QString cnlSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ЦНЛ"));
static QString dlaSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ДЛА"));
static QString depSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ДЕП"));
static QString arrSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АРР"));

static QString plnSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ПЛН"));
static QString ppnSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ППН"));
static QString ulsSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("УЛС"));
static QString flaSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ФЛА"));
static QString anpSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АНП"));
static QString rgmSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РЖМ"));
static QString rgiSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РЖИ"));
static QString apzSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АПЗ"));
static QString apvSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АПВ"));

static QString spsSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("СПС"));
static QString anmSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АНМ"));
static QString scnSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("СЦН"));
static QString rrpSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РРП"));
static QString samSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("САМ"));
static QString slcSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("СЛЦ"));
static QString sltSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("СЛТ"));
static QString cslSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("ЦСЛ"));

static QString rplSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РПЛ"));
static QString ackSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("АЦК"));
static QString rejSignatureR(QTextCodec::codecForName("UTF-8")->toUnicode("РЕЙ"));

int initConstants()
{
    AftnMessageParserPrivate::saveOnlyFir = true;
    AftnMessageParserPrivate::validateRoute = true;
    AftnMessageParserPrivate::firName = "UMMV";

    AftnMessageParserPrivate::turbulenceCategories << "H" << "M" << "L" << "J";
    AftnMessageParserPrivate::turbulenceCategoriesR << "Х" << "М" << "Л" << "Й";
    AftnMessageParserPrivate::flightRules << "I" << "V" << "Y" << "Z";
    AftnMessageParserPrivate::flightRulesR << "И" << "Ж" << "Ы" << "З";
    AftnMessageParserPrivate::flightTypes << "S" << "N" << "G" << "M" << "X";
    AftnMessageParserPrivate::flightTypesR << "С" << "Н" << "Г" << "М" << "Ь";
    AftnMessageParserPrivate::radioDevices << "N" << "S" << "A" << "B" << "C" << "D" << "E" << "F" << "J" << "H" << "I"
                    << "G" << "K" << "L" << "M" << "O" << "P" << "Q" << "R" << "T" << "U" << "V"
                    << "W" << "X" << "Y" << "Z";
    AftnMessageParserPrivate::radioDevicesR << "Н" << "С" << "Ц" << "Й" << "Х" << "И" << "Л" << "М" << "О" << "У" << "Ж"
                     << "В" << "Ы" << "З";
    AftnMessageParserPrivate::equipment << "N" << "A" << "C" << "X" << "P" << "I" << "S" << "D";
    AftnMessageParserPrivate::equipmentR << "А" << "Ц" << "Ь" << "П" << "И" << "С";

    AftnMessageParserPrivate::standardDevice12 << "N" << "S";
    AftnMessageParserPrivate::radioDevices12 << "A" << "B" << "C" << "D" << "E1" << "E2" << "E3" << "F" << "G" << "H" << "I"
                      << "J1" << "J2" << "J3" << "J4" << "J5" << "J6" << "J7" << "K" << "L" << "M1"
                      << "M2" << "M3" << "O" << "P1" << "P2" << "P3" << "P4" << "P5" << "P6" << "P7"
                      << "P8" << "P9" << "R" << "T" << "U" << "V" << "W" << "X" << "Y" << "Z";
    AftnMessageParserPrivate::equipment12 << "A" << "C" << "E" << "H" << "I" << "L" << "P" << "S" << "X" << "B1" << "B2"
                   << "U1" << "U2" << "V1" << "V2" << "D1" << "G1";

    AftnMessageParserPrivate::standardDevice12R << "Н" << "С";
    AftnMessageParserPrivate::radioDevices12R << "А" << "Б" << "Ц" << "Д" << "Е1" << "Е2" << "Е3" << "Ф" << "Г" << "Х" << "И"
                       << "Й1" << "Й2" << "Й3" << "Й4" << "Й5" << "Й6" << "Й7" << "К" << "Л" << "М1"
                       << "М2" << "М3" << "О" << "П1" << "П2" << "П3" << "П4" << "П5" << "П6" << "П7"
                       << "П8" << "П9" << "Р" << "Т" << "У" << "Ж" << "В" << "Ь" << "Ы" << "З";
    AftnMessageParserPrivate::equipment12R << "А" << "Ц" << "Е" << "Х" << "И" << "Л" << "П" << "С" << "Ь" << "Б1" << "Б2"
                    << "У1" << "У2" << "Ж1" << "Ж2" << "Д1" << "Г1";

    AftnMessageParserPrivate::borderTerms << "A" << "B";
    AftnMessageParserPrivate::borderTermsR << "А" << "Б"; //???

    return 0;
}

AdexpParser::AdexpParser(QObject *parent) :
    QObject(parent)
{
    static int iC = initConstants();
    Q_UNUSED(iC)

    setStandard();
}

AdexpParser::~AdexpParser()
{
}

const AdexpParser::FieldProcessor AdexpParser::fieldProcessors[] =
{
    {fplSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType8,
     &AftnMessageParser::processType9, &AftnMessageParser::processType10,
      &AftnMessageParser::processType13, &AftnMessageParser::processType15,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType8,
     &AftnMessageParser::validateType9, &AftnMessageParser::validateType10,
     &AftnMessageParser::validateType13, &AftnMessageParser::validateType15,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {fplSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType8,
      &AftnMessageParser::processType9R, &AftnMessageParser::processType10,
      &AftnMessageParser::processType13, &AftnMessageParser::processType15,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType8,
     &AftnMessageParser::validateType9R, &AftnMessageParser::validateType10,
     &AftnMessageParser::validateType13, &AftnMessageParser::validateType15,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {chgSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, &AftnMessageParser::processType22, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, &AftnMessageParser::validateType22, 0, } },

    {chgSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, &AftnMessageParser::processType22, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, &AftnMessageParser::validateType22, 0, } },

    {cnlSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {cnlSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {dlaSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {dlaSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {depSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {depSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {arrSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16Arr, &AftnMessageParser::processType17, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16Arr, &AftnMessageParser::validateType17, 0, } },

    {arrSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16Arr, &AftnMessageParser::processType17, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16Arr, &AftnMessageParser::validateType17, 0, } },

    {ackSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {ackSignatureR,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {rejSignature,
     {&AftnMessageParser::processType3, 0, },
    {&AftnMessageParser::validateType3, 0, } },

    {rejSignatureR,
     {&AftnMessageParser::processType3, 0, },
    {&AftnMessageParser::validateType3, 0, } },

    {manSignature,
     {&AftnMessageParser::processType3, &AftnMessageParser::processType7, &AftnMessageParser::processType13,
      &AftnMessageParser::processType16, &AftnMessageParser::processType18, 0, },
    {&AftnMessageParser::validateType3, &AftnMessageParser::validateType7, &AftnMessageParser::validateType13,
     &AftnMessageParser::validateType16, &AftnMessageParser::validateType18, 0, } },

    {QString::null, {0}, {0} },
};

bool AdexpParser::isSaveOnlyFIR() const
{
    return AftnMessageParserPrivate::saveOnlyFir;
}

void AdexpParser::setSaveOnlyFIR(bool enable)
{
    AftnMessageParserPrivate::saveOnlyFir = enable;
}

QString AdexpParser::FIRName() const
{
    return AftnMessageParserPrivate::firName;
}

void AdexpParser::setFIRName(const QString& firName)
{
    AftnMessageParserPrivate::firName = firName;
}

bool AdexpParser::isValidateRoute() const
{
    return AftnMessageParserPrivate::validateRoute;
}

void AdexpParser::setValidateRoute(bool validate)
{
    AftnMessageParserPrivate::validateRoute = validate;
}

QStringList AdexpParser::createFpl(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //8
    messageFields.append(createField8(splittedMessage));
    //9
    messageFields.append(createField9(splittedMessage));
    //10
    messageFields.append(createField10(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //15
    messageFields.append(createField15(splittedMessage));
    //16
    messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));

    return messageFields;
}

QStringList AdexpParser::createArr(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //16
    QString field16 = createField16(splittedMessage);
    if (!field16.isEmpty())
        messageFields.append(createField16(splittedMessage));
    //17
    messageFields.append(createField17(splittedMessage));

    return messageFields;
}

QStringList AdexpParser::createDep(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //16
    QString field16 = createField16(splittedMessage);
    if (!field16.isEmpty())
        messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));
    return messageFields;
}

QStringList AdexpParser::createCnl(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //16
    QString field16 = createField16(splittedMessage);
    if (!field16.isEmpty())
        messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));

    return messageFields;
}

QStringList AdexpParser::createDla(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //16
    QString field16 = createField16(splittedMessage);
    if (!field16.isEmpty())
        messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));

    return messageFields;
}

QStringList AdexpParser::createChg(QMultiMap<QString, QString> splittedMessage)
{
    Q_UNUSED(splittedMessage);
    return QStringList();
}

QStringList AdexpParser::createAck(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //16
    QString field16 = createField16(splittedMessage);
    if (!field16.isEmpty())
        messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));

    return messageFields;
}

QStringList AdexpParser::createRej(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //8
    messageFields.append(createField8(splittedMessage));
    //9
    messageFields.append(createField9(splittedMessage));
    //10
    messageFields.append(createField10(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //15
    messageFields.append(createField15(splittedMessage));
    //16
    messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));

    return messageFields;
}

QStringList AdexpParser::createMan(QMultiMap<QString, QString> splittedMessage)
{
    QStringList messageFields;

    //3
    QString title = createField3(splittedMessage);
    messageFields.append(title);
    //7
    messageFields.append(createField7(splittedMessage));
    //8
    messageFields.append(createField8(splittedMessage));
    //9
    messageFields.append(createField9(splittedMessage));
    //10
    messageFields.append(createField10(splittedMessage));
    //13
    messageFields.append(createField13(splittedMessage));
    //15
    messageFields.append(createField15(splittedMessage));
    //16
    messageFields.append(createField16(splittedMessage));
    //18
    QString other = createField18(splittedMessage);
    if (!other.isEmpty())
        messageFields.append(createField18(splittedMessage));

    return messageFields;
}

QString AdexpParser::createField3(QMultiMap<QString, QString> splittedMessage)
{
    QString field3 = splittedMessage.value("TITLE");
    if (field3.contains("I"))
        field3 = field3.right(field3.length() - 1);

    return field3;
}

QString AdexpParser::createField7(QMultiMap<QString, QString> splittedMessage)
{
    return splittedMessage.value("ARCID");
}

QString AdexpParser::createField8(QMultiMap<QString, QString> splittedMessage)
{
    QString rul = "FLTRUL";
    QString typ = "FLTTYP";
    if (splittedMessage.contains("FLIRUL"))
        rul = "FLIRUL";
    if (splittedMessage.contains("FLITYP"))
        typ = "FLITYP";
    return splittedMessage.value(rul) + splittedMessage.value(typ);
}

QString AdexpParser::createField9(QMultiMap<QString, QString> splittedMessage)
{
    QStringList field9(splittedMessage.value("NBARC") + splittedMessage.value("ARCTYP"));
    field9 << (splittedMessage.value("WKTRC"));
    field9.removeAll("");
    return field9.join("/");
}

QString AdexpParser::createField10(QMultiMap<QString, QString> splittedMessage)
{
    QStringList field10(splittedMessage.value("CEQPT"));
    field10 << splittedMessage.value("SEQPT");
    field10.removeAll("");
    return field10.join("/");
}

QString AdexpParser::createField13(QMultiMap<QString, QString> splittedMessage)
{
    return (splittedMessage.value("ADEP") + splittedMessage.value("EOBT"));
}

QString AdexpParser::createField15(QMultiMap<QString, QString> splittedMessage)
{
    return QString("%1").arg(splittedMessage.value("ROUTE"));
}

QString AdexpParser::createField16(QMultiMap<QString, QString> splittedMessage)
{
    QStringList field16(splittedMessage.value("ADES") + splittedMessage.value("TTLEET"));
    field16 << splittedMessage.value("ALTRNT1");
    field16 << splittedMessage.value("ALTRNT2");
    field16.removeAll("");
    return field16.join(" ");
}

QString AdexpParser::createField17(QMultiMap<QString, QString> splittedMessage)
{
    QStringList field17(splittedMessage.value("ADARR") + splittedMessage.value("ATA"));
    field17 << splittedMessage.value("ADARRZ");
    field17.removeAll("");
    return field17.join(" ");
}

QString AdexpParser::createField18(QMultiMap<QString, QString> splittedMessage)
{
    ParseVersion::Version version = ParseVersion::PV_LATIN;
    if (TelegramType::isTC95(createField3(splittedMessage)))
        version = ParseVersion::PV_CYRILLIC;

    QMap < QString, QString > subFields = field18Subfields(version);

    QStringList field18(getField(splittedMessage.values("ALTNZ"), subFields.value("ALTNZ")));
    field18 << getField(splittedMessage.values("ARCADDR"), subFields.value("ARCADDR"));
    field18 << getField(splittedMessage.values("COM"), subFields.value("COM"));
    field18 << getField(splittedMessage.values("DAT"), subFields.value("DAT"));
    field18 << getField(splittedMessage.values("DEPZ"), subFields.value("DEPZ"));
    field18 << getField(splittedMessage.values("DESTZ"), subFields.value("DESTZ"));
    field18 << getField(splittedMessage.values("EETFIR"), subFields.value("EETFIR"));
    field18 << getField(splittedMessage.values("EOBD"), subFields.value("EOBD"));
    field18 << getField(splittedMessage.values("NAV"), subFields.value("NAV"));
    field18 << getField(splittedMessage.values("OPR"), subFields.value("OPR"));
    field18 << getField(splittedMessage.values("ORGN"), subFields.value("ORGN"));
    field18 << getField(splittedMessage.values("PER"), subFields.value("PER"));
    field18 << getField(splittedMessage.values("RALT"), subFields.value("RALT"));
    field18 << getField(splittedMessage.values("REG"), subFields.value("REG"));
    field18 << getField(splittedMessage.values("RFP"), subFields.value("RFP"));
    field18 << getField(splittedMessage.values("RIF"), subFields.value("RIF"));
    field18 << getField(splittedMessage.values("RMK"), subFields.value("RMK"));
    field18 << getField(splittedMessage.values("RVR"), subFields.value("RVR"));
    field18 << getField(splittedMessage.values("SEL"), subFields.value("SEL"));
    field18 << getField(splittedMessage.values("SRC"), subFields.value("SRC"));
    field18 << getField(splittedMessage.values("STS"), subFields.value("STS"));
    field18 << getField(splittedMessage.values("TYPZ"), subFields.value("TYPZ"));

    field18.removeAll("");
    return field18.join(" ");
}

const QMap < QString, QString > AdexpParser::field18Subfields(ParseVersion::Version v)
{
    QMap < QString, QString > ret;

    ret.insert(Transliteration::toVersion("ALTNZ", v), "ALTN");
    ret.insert(Transliteration::toVersion("ARCADDR", v), "CODE");
    ret.insert(Transliteration::toVersion("COM", v), "COM");
    ret.insert(Transliteration::toVersion("DAT", v), "DAT");
    ret.insert(Transliteration::toVersion("DEPZ", v), "DEP");
    ret.insert(Transliteration::toVersion("DESTZ", v), "DEST");
    ret.insert(Transliteration::toVersion("EETFIR", v), "EET");
    ret.insert(Transliteration::toVersion("EOBD", v), "DOF");
    ret.insert(Transliteration::toVersion("NAV", v), "NAV");
    ret.insert(Transliteration::toVersion("OPR", v), "OPR");
    ret.insert(Transliteration::toVersion("ORGN", v), "ORGN");
    ret.insert(Transliteration::toVersion("PER", v), "PER");
    ret.insert(Transliteration::toVersion("RALT", v), "RALT");
    ret.insert(Transliteration::toVersion("REG", v), "REG");
    ret.insert(Transliteration::toVersion("RFP", v), "RFP");
    ret.insert(Transliteration::toVersion("RIF", v), "RIF");
    ret.insert(Transliteration::toVersion("RMK", v), "RMK");
    ret.insert(Transliteration::toVersion("RVR", v), "RVR");
    ret.insert(Transliteration::toVersion("SEL", v), "SEL");
    ret.insert(Transliteration::toVersion("SRC", v), "SRC");
    ret.insert(Transliteration::toVersion("STS", v), "STS");
    ret.insert(Transliteration::toVersion("TYPZ", v), "TYP");

    return ret;
}

QVariantMap AdexpParser::processMessage(const QString &message, AftnMessageErrors &errorCodes)
{
    errorLines.clear();
    QVariantMap resultMap;
    QMultiMap < QString, QString > splittedMessage;
    QStringList messageFields;

    QString inMessage = message;//.mid(offset, size);
    inMessage.replace("&", " ");
    inMessage.replace("\n", " ");
    inMessage.replace("\t", "");
    inMessage.replace("–", "-"); // not equal !!
    QStringList fields = inMessage.split('-');

    if (!fields.count())
    {
        //errorCodes.append(AFTN_PARSER_ErrorInfo(AftnMessageParser::InvalidMessage, QString::null));
        return resultMap;
    }

    foreach (const QString &arg, fields)
        splittedMessage.insertMulti(arg.left(arg.indexOf(" ")).trimmed()
                                    , arg.right(arg.length() - arg.indexOf(" ")).trimmed());

    QString title = createField3(splittedMessage);

    if (TelegramType::isFpl(title))
    {
        QString ifplid = splittedMessage.value("IFPLID");
        if (!ifplid.isEmpty())
            resultMap.insert(AFTN_PARSER_MESSAGE_FPL_ID, ifplid);

        messageFields = createFpl(splittedMessage);
    } else if (TelegramType::isArr(title))
    {
        messageFields = createArr(splittedMessage);
    } else if (TelegramType::isDep(title))
    {
        messageFields = createDep(splittedMessage);
    } else if (TelegramType::isCnl(title))
    {
        messageFields = createCnl(splittedMessage);
    } else if (TelegramType::isDla(title))
    {
        messageFields = createDla(splittedMessage);
    } else if (TelegramType::isChg(title))
    {
        messageFields = createChg(splittedMessage);
    } else if (TelegramType::isAck(title))
    {
        messageFields = createAck(splittedMessage);
    } else if (TelegramType::isRej(title))
    {
        messageFields = createRej(splittedMessage);
    } else if (TelegramType::isMan(title))
    {
        messageFields = createMan(splittedMessage);
    }

    if (!fields.count())
        return resultMap;

    resultMap.insert(AFTN_PARSER_SOURCE_MESSAGE, message);

    int i = 0;

    while (!fieldProcessors[i].type.isNull())
    {
        if (fieldProcessors[i].type == title)
        {
            int g = 0;

            while (fieldProcessors[i].callbacks[g] && g < messageFields.count())
            {
                AftnMessageErrors errors;
                ((AdexpParser::fieldProcessors[i].callbacks[g]))(messageFields[g], resultMap, errors);
                ((AdexpParser::fieldProcessors[i].validateCallbacks[g]))(resultMap, errors);
                if (errors.count())
                    errorLines.append(g);
                errorCodes.append(errors);

                ++g;
            }
        }
        ++i;
    }

    bool success = false;
    AFTN_PARSER_ErrorInfo tmp;
    foreach (const AFTN_PARSER_ErrorInfo& error, errorCodes)
    {
        if (error.first == ParserErrors::InvalidFirEet)
        {
            success = true;
            tmp = error;
            break;
        }
    }

    if (success)
    {
        errorCodes.clear();
        errorCodes.append(tmp);
    }

    return resultMap;
}

QString AdexpParser::getField(QStringList fieldsMessage, QString key)
{
    QString val;
    foreach (const QString &value, fieldsMessage)
        val.append(QString("%1/%2").arg(key).arg(value));
    return val;
}

#ifdef AFTN_NAMESPACE
} // namespace Aftn
#endif
