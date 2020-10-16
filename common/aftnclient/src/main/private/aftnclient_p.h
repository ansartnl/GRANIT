
#include "aftnclient.h"

#include <QtCore/QTimer>

class QFile;
class QTextDecoder;
class QTextEncoder;

class AFTNClientPrivate {
public:
  AFTNClientPrivate();
  virtual ~AFTNClientPrivate();


  QByteArray aftnName;
  QByteArray aftnServerName;
  QByteArray aftnServerNameReceived;

  quint8 pingTimeout; // seconds
  quint8 validPingTimeout; // seconds

  QByteArray channelId;
  QByteArray channelIdReceived;

  quint16 mesNum;

  AFTNClient::AftnState aftnState;
  QTimer *pingTimeoutTimer;
  QTimer *pingTimer;

  QByteArray buffer;
  QTextCodec* codec;
  QScopedPointer<QTextDecoder> decoder;
  QScopedPointer<QTextEncoder> encoder;
  QTextCodec* aftnNamesCodec;
  bool enableCH;
};
