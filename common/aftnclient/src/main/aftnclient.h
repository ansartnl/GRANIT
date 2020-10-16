#ifndef AFTNCLIENT_H
#define AFTNCLIENT_H

#include <QtNetwork/QTcpSocket>

#include "aftnproto.h"

class AFTNClientPrivate;

class AFTNClientMessage
{
public:
    explicit AFTNClientMessage(QString sText = QString::null, QString sAddresses = QString::null, QString sHeader = QString::null, QString sMessage = QString::null);

    QString text() const;
    void setText(const QString& sText);

    QString header() const;
    void setHeader(const QString& sHeader);

    QString message() const;
    void setMessage(const QString& sMessage);

    QString addresses() const;
    void setAddresses(const QString& sAddresses);

    quint32 getDBID() const;
    void setDBID(quint32 DBID);

    bool isEmpty() const;

private:
    quint32 mDBID;
    QString mText;
    QString mHeader;
    QString mMessage;
    QString mAddresses;
};

/*!
 Class for connection to AFTN servers.
 Protocol started with authrequest packet;
 Authrequest packet contain AFTN abonent name, for "CKS Monitor" it can be
 encoded by cp866
 */
class AFTNClient: public QTcpSocket
{
Q_OBJECT
  /*!
   3 bytes
   \property channelId
   */
  Q_PROPERTY(QString channelId READ channelId)
  /*!
   8 ASCII characters. May be
   \property channelId
   */
  Q_PROPERTY(QString aftnName READ aftnName WRITE setAftnName)
  /*!
   8 ASCII characters. May be
   \property channelId
   */
  Q_PROPERTY(QString aftnServerName READ aftnServerName WRITE setAftnServerName)
  /*!
   * Ping timeout
   */
  Q_PROPERTY(quint8 pingTimeout READ pingTimeout WRITE setPingTimeout)
  /*!
   * Connection state
   */
  Q_PROPERTY(AftnState aftnState READ aftnState NOTIFY aftnStateChanged)
  /*!
   * Send or not CH telegrams
   */
  Q_PROPERTY(bool enableCH READ isEnableCH WRITE setEnableCH)

  void init();
protected:
  Q_DECLARE_PRIVATE(AFTNClient)
  const QScopedPointer<AFTNClientPrivate> d_ptr;

public:
  AFTNClient(QObject * parent = 0);
  virtual ~AFTNClient();

  QString aftnName() const;
  void setAftnName(const QString& name);

  QString aftnServerName() const;
  void setAftnServerName(const QString& name);

  QString channelId() const;
  void setChannelId(const QString& name);

  quint8 pingTimeout() const;
  void setPingTimeout(quint8 timeout);

  QTextCodec* textCodec() const;
  void setTextCodec(QTextCodec* codec);

  bool isEnableCH() const;
  void setEnableCH(bool enable);

  virtual QByteArray toByteArray(const AFTNClientMessage& message) const;
  void sendMessage(const AFTNClientMessage& message, bool bNextNum = true);
  void sendGKP();

  enum AftnState
  {
    AftnUnconnected //! Disconnected state.
    ,
    AftnHostLookup //! The socket is performing a host name lookup.
    ,
    AftnConnecting //! Process to connecting to one of servers.
    ,
    AftnConnected //! Connected to server.
    ,
    AftnAuthorizating //! Process of authorization.
    ,
    AftnWaitForAuthorizationInfo //! Wait for authorization info
    ,
    AftnReceiving //!     Receiving messages.
    ,
    AftnInterrupted //!   Receiving messages interrupted.
    ,
    AftnClosing
  //! Process of disconnecting from server.
  };

  Q_ENUMS(AftnState)
  AftnState aftnState() const;

signals:
  void aftnStateChanged(AFTNClient::AftnState status);
  void messages(const QString& message);
  void canSend(AFTNClientMessage& message);
  void sendedMessage(const AFTNClientMessage& message, const QString &serialNumber);
  void checkNextNum(quint16& nextNum);

protected:
  virtual qint64 readData(char *data, qint64 maxlen);
  virtual qint64 writeData(const char *data, qint64 len);
  void selectMessagesFromBuffer();
  virtual void sendAuthorizationRequest();
  virtual int parseBuffer();
  virtual int processMessages();
  virtual QByteArray gotow_k_priemu() const;
  quint32 nextMesNum();
  virtual QString mesNumToString() const;
  virtual void closeChannel();
  QByteArray& getBuffer();
  void startPingTimer(int msesc);

protected:
  Q_DISABLE_COPY(AFTNClient)

protected slots:
  void socketStateChanged(QAbstractSocket::SocketState state);

  void setAftnState(AftnState state);

  void readSocket();

  virtual void pingTimeoutTimer();

  virtual void ping(bool bSend = true);
};

#endif //#ifndef AFTNCLIENT_H
