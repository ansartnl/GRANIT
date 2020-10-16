#include "aftnclient.h"
#include "private/aftnclient_p.h"

#include "aftnproto.h"

#include <QtCore/QDateTime>

#include <QtCore/QFile>
#include <QtCore/QCoreApplication>
#include <QtCore/QTextEncoder>
#include <QtCore/QTextDecoder>
#include <QtCore/QRegExp>
#include <QtCore/QStringList>

//qDebug() << Q_FUNC_INFO;
#define QDEBUG(X) qDebug() << (X)

AFTNClientMessage::AFTNClientMessage(QString sText, QString sAddresses, QString sHeader, QString sMessage)
    : mDBID(0), mText(sText), mHeader(sHeader), mMessage(sMessage), mAddresses(sAddresses)
{

}

QString AFTNClientMessage::text() const
{
    return mText;
}

void AFTNClientMessage::setText(const QString &sText)
{
    mText = sText;
}

QString AFTNClientMessage::header() const
{
    return mHeader;
}

void AFTNClientMessage::setHeader(const QString &sHeader)
{
    mHeader = sHeader;
}

QString AFTNClientMessage::message() const
{
    return mMessage;
}

void AFTNClientMessage::setMessage(const QString &sMessage)
{
    mMessage = sMessage;
}

QString AFTNClientMessage::addresses() const
{
    return mAddresses;
}

void AFTNClientMessage::setAddresses(const QString &sAddresses)
{
    mAddresses = sAddresses;
}

quint32 AFTNClientMessage::getDBID() const
{
    return mDBID;
}

void AFTNClientMessage::setDBID(quint32 DBID)
{
    mDBID = DBID;
}

bool AFTNClientMessage::isEmpty() const
{
    return mText.isEmpty() && mMessage.isEmpty();
}


void AFTNClient::init()
{
    Q_D( AFTNClient);

    d->pingTimeoutTimer = new QTimer(this);
    d->pingTimer = new QTimer(this);

    connect(this, SIGNAL(stateChanged(QAbstractSocket::SocketState)), this, SLOT(
            socketStateChanged(QAbstractSocket::SocketState)));
    connect(this, SIGNAL(readyRead()), this, SLOT(readSocket()));
    connect(d->pingTimeoutTimer, SIGNAL(timeout()), this,
            SLOT(pingTimeoutTimer()));

    connect(d->pingTimer, SIGNAL(timeout()), this, SLOT(ping()));

    setTextCodec(QTextCodec::codecForName("ITA2"));
    d->aftnNamesCodec = QTextCodec::codecForName("IBM 866");
}

AFTNClient::AFTNClient(QObject *parent) :
        QTcpSocket(parent), d_ptr(new AFTNClientPrivate)
{
    init();
}


AFTNClient::~AFTNClient()
{
}

QString AFTNClient::aftnName() const
{
    Q_D(const AFTNClient);
    QString sName;
    if(d->aftnNamesCodec)
        sName = d->aftnNamesCodec->toUnicode(d->aftnName);
    else
        sName = QString(d->aftnName.data());
    return sName;
}

void AFTNClient::setAftnName(const QString& name)
{
    Q_D(AFTNClient);
    if(d->aftnNamesCodec)
        d->aftnName = d->aftnNamesCodec->fromUnicode(name);
    else
        d->aftnName = name.toLocal8Bit();
}

QString AFTNClient::aftnServerName() const
{
    Q_D(const AFTNClient);
    QString sName;
    QByteArray aName;
    if(d->aftnServerName.isEmpty())
        aName = d->aftnServerNameReceived;
    else
        aName = d->aftnServerName;
    if(d->aftnNamesCodec)
        sName = d->aftnNamesCodec->toUnicode(aName);
    else
        sName = QString(aName.data());
    return sName;
}

void AFTNClient::setAftnServerName(const QString& name)
{
    Q_D(AFTNClient);
    if(d->aftnNamesCodec)
        d->aftnServerName = d->aftnNamesCodec->fromUnicode(name);
    else
        d->aftnServerName = name.toLocal8Bit();
}

QString AFTNClient::channelId() const
{
    Q_D(const AFTNClient);
    QString sName;
    QByteArray aName;
    if(d->channelId.isEmpty())
        aName = d->channelIdReceived;
    else
        aName = d->channelId;
    if(d->aftnNamesCodec)
        sName = d->aftnNamesCodec->toUnicode(aName);
    else
        sName = QString(aName.data());
    return sName;
}

void AFTNClient::setChannelId(const QString& name)
{
    Q_D(AFTNClient);
    if(d->aftnNamesCodec)
        d->channelId = d->aftnNamesCodec->fromUnicode(name);
    else
        d->channelId = name.toLocal8Bit();
}

quint8 AFTNClient::pingTimeout() const
{
    Q_D(const AFTNClient);
    return d->pingTimeout;
}

void AFTNClient::setPingTimeout(quint8 timeout)
{
    Q_D(AFTNClient);
    d->pingTimeout = timeout;
    d->validPingTimeout = d->pingTimeout ? d->pingTimeout : 10;
}

bool AFTNClient::isEnableCH() const
{
    Q_D(const AFTNClient);
    return d->enableCH;
}

void AFTNClient::setEnableCH(bool enable)
{
    Q_D(AFTNClient);
    d->enableCH = enable;
}

QTextCodec* AFTNClient::textCodec() const
{
    Q_D(const AFTNClient);
    return d->codec;
}

void AFTNClient::setTextCodec(QTextCodec* codec)
{
    Q_D(AFTNClient);
    d->codec = codec;

    // default initialization
    if (!d->codec)
        d->codec = QTextCodec::codecForName("ASCII");

    d->decoder.reset(d->codec->makeDecoder());
    d->encoder.reset(d->codec->makeEncoder());
}

AFTNClient::AftnState AFTNClient::aftnState() const
{
    Q_D(const AFTNClient);
    return d->aftnState;
}

void AFTNClient::socketStateChanged(QAbstractSocket::SocketState state)
{
    Q_D(AFTNClient);
    switch (state) {
    case QAbstractSocket::HostLookupState:
        setAftnState( AftnHostLookup);
        break;
    case QAbstractSocket::ConnectingState:
        setAftnState( AftnConnecting);
        break;
    case QAbstractSocket::ConnectedState:
        d->buffer.clear();
        setAftnState( AftnConnected);
        d->pingTimer->start(d->validPingTimeout * 1000);
        sendAuthorizationRequest();
        break;
    case QAbstractSocket::ClosingState:
        setAftnState( AftnClosing);
        break;
    case QAbstractSocket::UnconnectedState:
        setAftnState( AftnUnconnected);
        closeChannel();
        break;
    default:
        break;
    }
}

void AFTNClient::closeChannel()
{
    Q_D(AFTNClient);
    d->channelId.clear();
    d->pingTimeoutTimer->stop();
    d->pingTimer->stop();
    d->buffer.clear();
}

void AFTNClient::startPingTimer(int msesc)
{
    Q_D(AFTNClient);
    d->pingTimer->stop();
    d->pingTimer->start(msesc);
}

void AFTNClient::setAftnState(AftnState state)
{
    Q_D(AFTNClient);
    if (d->aftnState == state)
        return;

    d->aftnState = state;
    emit aftnStateChanged(d->aftnState);
}

void AFTNClient::sendAuthorizationRequest()
{
    Q_D(AFTNClient);
    AftnAuthorizationRequest request;
    memset(&request, 0, sizeof(request));
    int size = d->aftnName.size() > AFTN_NAME_SIZE ? AFTN_NAME_SIZE
        : d->aftnName.size();
    memcpy(request.aftnName, d->aftnName.data(), size);
    request.pingTimeout = d->pingTimeout;
    write((const char*) &request, sizeof(request));
    flush();
    setAftnState(AftnAuthorizating);
}

void AFTNClient::readSocket()
{
    Q_D(AFTNClient);

    d->pingTimeoutTimer->stop();

    QByteArray array = readAll();

    qDebug() << QString("AFTNClient: received %1 bytes").arg(array.size());

    // TODO difference with documentation
//    while (array.startsWith(QByteArray("\x00\x00\x00\x00\x00\x00", 6)))
//        array = array.mid(6);

    d->buffer.append(array);

    // loop fully received data
    while (parseBuffer());

    d->pingTimeoutTimer->start(d->validPingTimeout * 1000 * 3);
}

void AFTNClient::pingTimeoutTimer()
{
    qWarning() << "AFTNClient: ping timeout";
    disconnectFromHost();
}

void AFTNClient::ping(bool bSend)
{
    //  QDEBUG("Send: ping.");
    AFTNClientMessage message;
    if(bSend)
        emit canSend(message);
    if(!message.isEmpty())
    {
        sendMessage(message);
    }
    else
    {
        AftnPing ping;
        ping.code = AFTN_PING_CODE_PING;
        qDebug() << "Send ping";
        write((const char*) &ping, sizeof(ping));
        flush();
    }
}


int AFTNClient::parseBuffer()
{
    Q_D(AFTNClient);
    if (d->buffer.isEmpty())
        return 0;

    switch (d->aftnState) {
    case AftnAuthorizating:
        if (d->buffer.size() < int(sizeof(AftnAuthorizationReply)))
            return 0;
        AftnAuthorizationReply *reply;
        reply = (AftnAuthorizationReply *) d->buffer.data();
        switch (reply->code) {
        case AFTN_AUTHORIZATION_REPLY_CODE_FAILED:
            disconnectFromHost();
            return 0;
        case AFTN_AUTHORIZATION_REPLY_CODE_SUCCESS:
            setAftnState( AftnWaitForAuthorizationInfo);
            break;
        }
        // cut processed packet
        d->buffer = d->buffer.mid(sizeof(AftnAuthorizationReply));
        // parse end of buffer
        return d->buffer.size();
        break;
    case AftnWaitForAuthorizationInfo:
        // await for ends of packet
        if (d->buffer.size() < int(sizeof(AftnAuthorizationInfo)))
            return 0;
        AftnAuthorizationInfo *authInfo;
        authInfo = (AftnAuthorizationInfo *) d->buffer.data();
        d->aftnServerNameReceived = QByteArray((const char*) &authInfo->aftnName, AFTN_NAME_SIZE);
        d->channelIdReceived = QByteArray((const char*) &authInfo->channelId, AFTN_CHANNEL_ID_SIZE);

        setAftnState( AftnReceiving);
        // cut processed packet
        d->buffer = d->buffer.mid(sizeof(AftnAuthorizationInfo));
        // send GOTOW K PRIEMU
        sendGKP();
        // parse end of buffer
        return d->buffer.size();
        break;
    case AftnReceiving:
        // It's message
        return processMessages();
        break;
    case AftnInterrupted:
        break;
    default:
        break;
    }
    return 0;
}

int AFTNClient::processMessages()
{
    Q_D(AFTNClient);
    QString string;

    if (d->buffer.size() == 1 && !(*d->buffer.data()))
    {
        d->buffer.clear();
        return 0;
    }

    //d->buffer.replace(QByteArray("\x00\x00\x00\x00\x00\x00", 6), QByteArray(""));
    string = d->decoder->toUnicode(d->buffer);
    //string = string.simplified();

    emit messages(string);

    d->buffer.clear();
    ping(false);
    qWarning() << "AFTNClient: send confirmation " << string.count("NNNN");
    for (int i = 0; i < string.count("NNNN"); ++i)
        write(QByteArray("\xFD\x01", 2));

    return 0;
}

QByteArray& AFTNClient::getBuffer()
{
    Q_D(AFTNClient);
    return d->buffer;
}

qint64 AFTNClient::readData(char *data, qint64 maxlen)
{
    qint64 size = QTcpSocket::readData(data, maxlen);
    return size;
}

qint64 AFTNClient::writeData(const char *data, qint64 len)
{
    qint64 size = QTcpSocket::writeData(data, len);
    return size;
}

QByteArray AFTNClient::toByteArray(const AFTNClientMessage& message) const
{
    Q_D(const AFTNClient);
    QString sRet;
    if(!message.message().isEmpty())
    {
        sRet = message.message();
    }
    else if(!message.header().isEmpty())
    {
        sRet = message.header();
        sRet.replace("@@@", mesNumToString());
        sRet.replace("@SEND_TIME@", QDateTime::currentDateTimeUtc().toString("hhmm"));
        sRet += "\n\r";
        sRet += message.text();
        sRet += "\n\n\r";
        sRet += "\r\r\r\r\r\r\r";
        sRet += "NNNN";
    }
    else if(!message.text().isEmpty())
    {
        QDateTime time = QDateTime::currentDateTimeUtc();

        sRet = "ZCZC";
        sRet += " ";
        sRet += channelId() + mesNumToString();
        sRet += "     ";
        sRet += "\n\n\r";
        sRet += "FF";
        sRet += " ";
        sRet += message.addresses().isEmpty() ? aftnServerName() : message.addresses();
        sRet += "\n\n\r";
        sRet += time.toString("ddhhmm");
        sRet += " ";
        sRet += aftnName();
        sRet += "\n\n\r";
        sRet += message.text();
        sRet += "\n\n\r";
        sRet += "\r\r\r\r\r\r\r";
        sRet += "NNNN";
    }
    QByteArray ret = d->encoder->fromUnicode(sRet);
    ret.prepend((char)0);
    return ret;
}
/*
QByteArray AFTNClient::toByteArray(const AFTNClientMessage& message) const
{
    Q_D(const AFTNClient);
    QString sRet;
    if(!message.message().isEmpty())
    {
        sRet = message.message();
    }
    else if(!message.header().isEmpty())
    {
        sRet = message.header();
        sRet.replace("@@@", mesNumToString());
        sRet.replace("@SEND_TIME@", QDateTime::currentDateTimeUtc().toString("hhmm"));
        sRet += "\r\n";
        sRet += message.text();
        sRet += "\r\n";
        sRet += "\n\n\n\n\n\n\n";
        sRet += "NNNN";
    }
    else if(!message.text().isEmpty())
    {
        QDateTime time = QDateTime::currentDateTimeUtc();

        sRet = "ZCZC";
        sRet += " ";
        sRet += channelId() + mesNumToString();
        sRet += "     ";
        sRet += "\r\n";
        sRet += "FF";
        sRet += " ";
        sRet += message.addresses().isEmpty() ? aftnServerName() : message.addresses();
        sRet += "\r\n";
        sRet += time.toString("ddhhmm");
        sRet += " ";
        sRet += aftnName();
        sRet += "\r\n";
        sRet += message.text();
        sRet += "\r\n";
        sRet += "\n\n\n\n\n\n\n";
        sRet += "NNNN";
    }
    QByteArray ret = d->encoder->fromUnicode(sRet);
    ret.prepend((char)0);
    return ret;
}
*/
QByteArray AFTNClient::gotow_k_priemu() const
{
    Q_D(const AFTNClient);
    QString sRet;
    QDateTime time = QDateTime::currentDateTimeUtc();

    sRet = "ZCZC";
    sRet += " ";
    sRet += channelId() + mesNumToString();
    sRet += " ";
    sRet += time.toString("hhmm");
    sRet += "     ";
    sRet += "\n\n\r";
    sRet += "FF";
    sRet += " ";
    sRet += aftnServerName();
    sRet += "\n\n\r";
    sRet += time.toString("ddhhmm");
    sRet += " ";
    sRet += aftnName();
    sRet += "\n\n\r";
    sRet += "SVC RECEIVE READY";//"SVC GOTOW K PRIEMU";
    sRet += "\n\n\r";
    sRet += "\r\r\r\r\r\r\r";
    sRet += "NNNN";
#warning("FOR TASHKENT")
    {
//        QDateTime time = QDateTime::currentDateTimeUtc();
//        QByteArray arr = d->encoder->fromUnicode("ZCZC PTB061 " + time.toString("hhmm") + "     \n\n\rFF UTTTYFYX\n\n\r"
//                                                 + time.toString("ddhhmm") + " UTTTZRZP" "\n\n\rSVC GOTOW K PRIEMU\n\n\r\r\r\r\r\r\r\rNNNN");
//        arr.prepend((char)0);
//        qDebug() << d->decoder->toUnicode(arr);
//        arr = QByteArray::fromHex("00110e110e041610191b16181304170601170404040404000808020d0d0407101010150d151d0808021b160717060117000407101010110a1116080802051e0e041a18101813040f04160a06011c0700080802020202020202020c0c0c0c");
    }
    QByteArray ret = d->encoder->fromUnicode(sRet);
    ret.prepend((char)0);
    return ret;
}

/*
QByteArray AFTNClient::gotow_k_priemu() const
{
    Q_D(const AFTNClient);
    QString sRet;
    QDateTime time = QDateTime::currentDateTimeUtc();

    sRet = "ZCZC";
    sRet += " ";
    sRet += channelId() + mesNumToString();
    sRet += " ";
    sRet += time.toString("hhmm");
    sRet += "     ";
    sRet += "\r\n";
    sRet += "FF";
    sRet += " ";
    sRet += aftnServerName();
    sRet += "\r\n";
    sRet += time.toString("ddhhmm");
    sRet += " ";
    sRet += aftnName();
    sRet += "\r\n";
    sRet += "SVC RECEIVE READY";//"SVC GOTOW K PRIEMU";
    sRet += "\r\n";
    sRet += "\n\n\n\n\n\n\n";
    sRet += "NNNN";
#warning("FOR TASHKENT")
    {
//        QDateTime time = QDateTime::currentDateTimeUtc();
//        QByteArray arr = d->encoder->fromUnicode("ZCZC PTB061 " + time.toString("hhmm") + "     \n\n\rFF UTTTYFYX\n\n\r"
//                                                 + time.toString("ddhhmm") + " UTTTZRZP" "\n\n\rSVC GOTOW K PRIEMU\n\n\r\r\r\r\r\r\r\rNNNN");
//        arr.prepend((char)0);
//        qDebug() << d->decoder->toUnicode(arr);
//        arr = QByteArray::fromHex("00110e110e041610191b16181304170601170404040404000808020d0d0407101010150d151d0808021b160717060117000407101010110a1116080802051e0e041a18101813040f04160a06011c0700080802020202020202020c0c0c0c");
    }
    QByteArray ret = d->encoder->fromUnicode(sRet);
    //ret.prepend((char)0);
    return ret;
}
*/
quint32 AFTNClient::nextMesNum()
{
    Q_D(AFTNClient);
    ++d->mesNum %= 1000;
    emit checkNextNum(d->mesNum);
    return d->mesNum;
}

QString AFTNClient::mesNumToString() const
{
    Q_D(const AFTNClient);
    return QString("%1").arg(d->mesNum, 3, 10, QLatin1Char('0'));
}

void AFTNClient::sendMessage(const AFTNClientMessage& message, bool bNextNum)
{
    Q_D(AFTNClient);
    if(!message.isEmpty() && (aftnState() == AftnReceiving))
    {
        if(bNextNum)
            nextMesNum();
        QByteArray msg = toByteArray(message);
        QString sMes = d->decoder->toUnicode(msg);
        qDebug() << "Send message\n" << sMes;
        write(msg, msg.size());
        flush();
        AFTNClientMessage sSendedMessage(message);
        sSendedMessage.setMessage(sMes);
        QString header = message.header();
        QString serialNumber = mesNumToString();
        header.replace("@@@", serialNumber);
        header.replace("@SEND_TIME@", QDateTime::currentDateTimeUtc().toString("hhmm"));
        sSendedMessage.setHeader(header);
        emit sendedMessage(sSendedMessage, serialNumber);
    }
}

void AFTNClient::sendGKP()
{
    Q_D(AFTNClient);
    if(aftnState() == AftnReceiving)
    {
        nextMesNum();
        QByteArray arrGKP = gotow_k_priemu();
        qDebug() << "Send GOTOW K PRIEMU\n" << d->decoder->toUnicode(arrGKP);
        write(arrGKP);
    }
}

//------------------------------------------------------------------
// private
AFTNClientPrivate::AFTNClientPrivate() :
        pingTimeout(0), validPingTimeout(10), mesNum(0),
        aftnState(AFTNClient::AftnUnconnected), codec(0), aftnNamesCodec(0),
        enableCH(true)
{
}

AFTNClientPrivate::~AFTNClientPrivate()
{
}
