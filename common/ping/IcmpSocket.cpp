#include "IcmpSocket.h"

#include <QtCore/QTimer>

// FIXME: not working for windows
#ifdef Q_OS_WIN
#include "Winsock2.h"
#include "WinStructs.h"
#else
#include <netinet/ip_icmp.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#endif
#include <errno.h>
#include <unistd.h>

#define MAXIPLEN 60
#define MAXICMPLEN 76

#define MAX_PING_PACKET_SIZE (MAX_PING_DATA_SIZE + sizeof(icmphdr))
#define MAX_RECEIVE_PACKET_SIZE (MAX_PING_PACKET_SIZE + MAXIPLEN + MAXICMPLEN)

// NOTE: filter
//#ifndef ICMP_FILTER
//#define ICMP_FILTER 1
//struct icmp_filter
//{
//    quint32 data;
//};
//#endif

static struct
{
    cmsghdr cm;
    in_pktinfo ipi;
} cmsg = {
{ sizeof(cmsghdr) + sizeof(in_pktinfo), SOL_IP, IP_PKTINFO
#if (!defined __STRICT_ANSI__ && __GNUC__ >= 2) || __STDC_VERSION__ >= 199901L
    , {}
#endif
}
, { 0, 0, 0 }
  };

static quint16 ip_checksum(quint16 *buffer, int size)
{
    unsigned long cksum = 0;

    // Sum all the words together, adding the final byte if size is odd
    while (size > 1) {
        cksum += *buffer++;
        size -= sizeof(quint16);
    }
    if (size) {
        cksum += *(quint8*)buffer;
    }

    // Do a little shuffling
    cksum = (cksum >> 16) + (cksum & 0xffff);
    cksum += (cksum >> 16);

    // Return the bitwise complement of the resulting mishmash
    return (quint16)(~cksum);
}

IcmpSocket::IcmpSocket(QObject *parent) :
    QObject(parent)
  , mError(ISE_NO_ERROR)
  , mReceiveTimer(new QTimer(this))
  , icmp_sock(-1)
  , num_sent(0)
  , ident(htons(getpid() & 0xFFFF))
{
    mReceiveTimer->setInterval(5);
    connect(mReceiveTimer, SIGNAL(timeout()), SLOT(onReceiveTimer()));
}

bool IcmpSocket::open(const QString &address)
{
    if (icmp_sock >= 0)
        close();

    icmp_sock = ::socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (icmp_sock < 0) {
        mError = ISE_ERROR_CREATING_SOCKET;
        return false;
    }
    mAddress = address;

    // NOTE: filter
//    icmp_filter filt;
//    filt.data = ~((1<<ICMP_SOURCE_QUENCH)|
//                  (1<<ICMP_DEST_UNREACH)|
//                  (1<<ICMP_TIME_EXCEEDED)|
//                  (1<<ICMP_PARAMETERPROB)|
//                  (1<<ICMP_REDIRECT)|
//                  (1<<ICMP_ECHOREPLY));
//    if (setsockopt(icmp_sock, SOL_RAW, ICMP_FILTER, &filt, sizeof(filt)) == -1)
//        emit warning(tr("WARNING: setsockopt(ICMP_FILTER)"));

//    int hold = 1;
//    setsockopt(icmp_sock, SOL_IP, IP_RECVERR, &hold, sizeof(hold));

    num_sent = 0;

    mReceiveTimer->start();

    return true;
}

bool IcmpSocket::close()
{
    mReceiveTimer->start();

    bool ret = (::close(icmp_sock) > 0);
    icmp_sock = -1;
    if (ret < 0)
        mError = ISE_ERROR_CLOSING_SOCKET;
    return ret;
}

bool IcmpSocket::send(const QByteArray &msg)
{
    if (icmp_sock < 0) {
        mError = ISE_SOCKET_CLOSED;
        return false;
    }

    sockaddr_in destination;
    memset((char *)&destination, 0, sizeof(destination));
    destination.sin_family = AF_INET;
    destination.sin_addr.s_addr = inet_addr(qPrintable(mAddress));

    char data[MAX_PING_PACKET_SIZE];

    icmphdr *icp = (icmphdr *)data;
    icp->type = ICMP_ECHO;
    icp->code = 0;
    icp->checksum = 0;
    icp->un.echo.id = htons(ident);
    icp->un.echo.sequence = htons(num_sent + 1);

    memcpy(data + sizeof(icmphdr), msg.data(), msg.size());

    icp->checksum = ip_checksum((quint16 *)&data, sizeof(icmphdr) + msg.size());

    iovec iov = { data, 0 };
    msghdr m = { &destination, sizeof(destination), &iov, 1, &cmsg, 0, 0 };
    m.msg_controllen = sizeof(cmsg);
    iov.iov_len = sizeof(icmphdr) + msg.size();

    int confirm = MSG_CONFIRM;
    const ssize_t res = sendmsg(icmp_sock, &m, confirm);

    if (res >= 0) {
        ++num_sent;
    } else {
        mError = ISE_SEND_ERROR;
    }

    return (res >= 0);
}

IcmpSocket::IcmpSocketError IcmpSocket::lastError() const
{
    return mError;
}

const QString IcmpSocket::errorString() const
{
    switch (lastError()) {
    case ISE_NO_ERROR:
        return tr("No error");
    case ISE_UNKNOWN_ERROR:
        return tr("Unknown error");
    case ISE_ERROR_CREATING_SOCKET:
        return tr("Error creating socket. Make sure, you have enough priveleges for opening raw socket");
    case ISE_ERROR_CLOSING_SOCKET:
        return tr("Error closing socket");
    case ISE_SOCKET_CLOSED:
        return tr("Socket is closed");
    case ISE_SEND_ERROR:
        return tr("Error sending data");
    case ISE_SELECT_ERROR:
        return tr("Error selecting data");
    case ISE_RECEIVE_ERROR:
        return tr("Error receiving data");
    case ISE_RECEIVED_PACKET_ERROR:
        return tr("Received package size is too small");
    }
    return tr("Unhandled error");
}

bool IcmpSocket::receive()
{
    char data[MAX_RECEIVE_PACKET_SIZE];

    timeval timeout = {0, 4 * 1000};    // 4 ms
    fd_set read_set;
    iphdr ip_hdr;
    icmphdr rcv_hdr;

    memset(&read_set, 0, sizeof(read_set));
    FD_SET(icmp_sock, &read_set);

    //wait for a reply with a timeout
    int rc = select(icmp_sock + 1, &read_set, NULL, NULL, &timeout);
    if (rc < 0) {
        mError = ISE_SELECT_ERROR;
        return false;
    }
    if (rc == 0)
        return true;

    char addrbuf[128];
    char ans_data[4096];
    iovec iov;
    msghdr msg;

    iov.iov_base = data;
    iov.iov_len = MAX_RECEIVE_PACKET_SIZE;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = addrbuf;
    msg.msg_namelen = sizeof(addrbuf);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;
    msg.msg_control = ans_data;
    msg.msg_controllen = sizeof(ans_data);

    rc = recvmsg(icmp_sock, &msg, 0);

    if (rc <= 0) {
        mError = ISE_RECEIVE_ERROR;
        return false;
    } else if (uint(rc) < (sizeof(ip_hdr) + sizeof(rcv_hdr))) {
        mError = ISE_RECEIVED_PACKET_ERROR;
        return false;
    }
    memcpy(&ip_hdr, data, sizeof(ip_hdr));
    memcpy(&rcv_hdr, data + sizeof(ip_hdr), sizeof(rcv_hdr));
    if (rcv_hdr.type == ICMP_ECHOREPLY
            && ntohs(rcv_hdr.un.echo.id) == ident
            && ip_hdr.saddr == inet_addr(qPrintable(mAddress))) {
        emit pongCame();
    }
    return true;
}

void IcmpSocket::onReceiveTimer()
{
    if (!receive()) {
        emit error(errorString());
    }
}
