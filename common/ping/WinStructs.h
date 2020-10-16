#ifndef WINSTRUCTS
#define WINSTRUCTS

#include "qglobal.h"

/* Structure for scatter/gather I/O.  */
struct iovec
  {
    void *iov_base;	/* Pointer to data.  */
    size_t iov_len;	/* Length of data.  */
  };

/* Structure describing messages sent by
   `sendmsg' and received by `recvmsg'.  */
struct msghdr
  {
    void *msg_name;		/* Address to send to/receive from.  */
    quint32 msg_namelen;	/* Length of address data.  */

    struct iovec *msg_iov;	/* Vector of data to send/receive into.  */
    size_t msg_iovlen;		/* Number of elements in the vector.  */

    void *msg_control;		/* Ancillary data (eg BSD filedesc passing). */
    size_t msg_controllen;	/* Ancillary data buffer length.
                   !! The type should be socklen_t but the
                   definition of the kernel is incompatible
                   with this.  */

    int msg_flags;		/* Flags on received message.  */
  };

struct iphdr
  {
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl:4;
    unsigned int version:4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version:4;
    unsigned int ihl:4;
#else
# error	"Please fix <WinStructs.h>"
#endif
    quint8 tos;
    quint16 tot_len;
    quint16 id;
    quint16 frag_off;
    quint8 ttl;
    quint8 protocol;
    quint16 check;
    quint32 saddr;
    quint32 daddr;
    /*The options start here. */
  };

struct icmphdr
{
  quint8 type;		/* message type */
  quint8 code;		/* type sub-code */
  quint16 checksum;
  union
  {
    struct
    {
      quint16	id;
      quint16	sequence;
    } echo;			/* echo datagram */
    quint32 	gateway;	/* gateway address */
    struct
    {
      quint16	__glibc_reserved;
      quint16	mtu;
    } frag;			/* path mtu discovery */
  } un;
};

/* Structure used for storage of ancillary data object information.  */
struct cmsghdr
  {
    size_t cmsg_len;		/* Length of data in cmsg_data plus length
                   of cmsghdr structure.
                   !! The type should be socklen_t but the
                   definition of the kernel is incompatible
                   with this.  */
    int cmsg_level;		/* Originating protocol.  */
    int cmsg_type;		/* Protocol specific type.  */
#if (!defined __STRICT_ANSI__ && __GNUC__ >= 2) || __STDC_VERSION__ >= 199901L
    __extension__ unsigned char __cmsg_data[]; /* Ancillary data.  */
#endif
  };

/* Structure used for IP_PKTINFO.  */
struct in_pktinfo
  {
    int ipi_ifindex;			/* Interface index  */
    struct in_addr ipi_spec_dst;	/* Routing destination address  */
    struct in_addr ipi_addr;		/* Header destination address  */
  };

#define SOL_IP	0

#define IP_PKTINFO	8	/* bool */

#define	MSG_CONFIRM	0x800

#define ICMP_ECHOREPLY		0	/* Echo Reply			*/
#define ICMP_DEST_UNREACH	3	/* Destination Unreachable	*/
#define ICMP_SOURCE_QUENCH	4	/* Source Quench		*/
#define ICMP_REDIRECT		5	/* Redirect (change route)	*/
#define ICMP_ECHO		8	/* Echo Request			*/
#define ICMP_TIME_EXCEEDED	11	/* Time Exceeded		*/
#define ICMP_PARAMETERPROB	12	/* Parameter Problem		*/
#define ICMP_TIMESTAMP		13	/* Timestamp Request		*/
#define ICMP_TIMESTAMPREPLY	14	/* Timestamp Reply		*/
#define ICMP_INFO_REQUEST	15	/* Information Request		*/
#define ICMP_INFO_REPLY		16	/* Information Reply		*/
#define ICMP_ADDRESS		17	/* Address Mask Request		*/
#define ICMP_ADDRESSREPLY	18	/* Address Mask Reply		*/
#define NR_ICMP_TYPES		18

inline ssize_t recvmsg(int sd, struct msghdr *msg, int flags)
{
    ssize_t bytes_read;
    size_t expected_recv_size;
    size_t left2move;
    char *tmp_buf;
    char *tmp;
    int i;

    expected_recv_size = 0;
    for(i = 0; i < msg->msg_iovlen; i++)
        expected_recv_size += msg->msg_iov[i].iov_len;
    tmp_buf = (char *)malloc(expected_recv_size);
    if(!tmp_buf)
        return -1;

    left2move = bytes_read = recvfrom(sd,
                                      tmp_buf,
                                      expected_recv_size,
                                      flags,
                                      (struct sockaddr *)msg->msg_name,
                                      (int *)&msg->msg_namelen
                                      );

    for(tmp = tmp_buf, i = 0; i < msg->msg_iovlen; i++)
    {
        if(left2move <= 0) break;
        memcpy(
                    msg->msg_iov[i].iov_base,
                    tmp,
                    qMin(msg->msg_iov[i].iov_len,left2move)
                    );
        left2move -= msg->msg_iov[i].iov_len;
        tmp += msg->msg_iov[i].iov_len;
    }

    free(tmp_buf);

    return bytes_read;
}

inline ssize_t sendmsg(int sd, struct msghdr *msg, int flags)
{
    ssize_t bytes_send;
    size_t expected_send_size;
    size_t left2move;
    char *tmp_buf;
    char *tmp;
    int i;

    expected_send_size = 0;
    for(i = 0; i < msg->msg_iovlen; i++)
        expected_send_size += msg->msg_iov[i].iov_len;
    tmp_buf = (char *)malloc(expected_send_size);
    if(!tmp_buf)
        return -1;

    for(tmp = tmp_buf, left2move = expected_send_size, i = 0; i <
        msg->msg_iovlen; i++)
    {
        if(left2move <= 0) break;
        memcpy(
                    tmp,
                    msg->msg_iov[i].iov_base,
                    qMin(msg->msg_iov[i].iov_len,left2move));
        left2move -= msg->msg_iov[i].iov_len;
        tmp += msg->msg_iov[i].iov_len;
    }

    bytes_send = sendto(sd,
                        tmp_buf,
                        expected_send_size,
                        flags,
                        (struct sockaddr *)msg->msg_name,
                        msg->msg_namelen
                        );

    free(tmp_buf);

    return bytes_send;
}

#endif // WINSTRUCTS
