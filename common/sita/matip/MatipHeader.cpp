#include "MatipHeader.h"

#include <QtCore/QtDebug>
#include <QtCore/QtEndian>

MatipHeader::MatipHeader(const uchar *data)
    : mRawData(reinterpret_cast<const char *>(data), 4)
    , mCommand(NoCommand), mError(NoError)
{
    /*
     * parse MATIP header
     *
     *  byte0    byte1    byte3    byte4
     * 00000aaa bccccccc dddddddd dddddddd
     *
     * aaa:              version, must be 001 or packet is invalid
     * b:                1 -- control packet, 0 -- data packet
     * ccccccc:          command
     * dddddddd ddddddd: packet length
     *
     */

    // check if byte0 == 00000xxx
    if ((data[0] >> 3) != 0)
    {
        mError = InvalidHeaderError;
        return;
    }

    // check if byte0 == xxxxx001
    if ((data[0] & 0x07) != 1)
    {
        mError = VersionMismatchError;
        return;
    }

    // control packet if byte1 == 1xxxxxxx
    if ((data[1] >> 7) == 1)
    {
        switch ((data[1] & 0x7F))
        {
        case 0x7E:
            mCommand = SessionOpenCommand;
            break;
        case 0x7C:
            mCommand = SessionCloseCommand;
            break;
        case 0x7D:
            mCommand = OpenConfirmCommand;
            break;
        case 0x7B:
            mCommand = SessionStatusQueryCommand;
            break;
        case 0x7A:
            mCommand = SessionStatusResponseCommand;
            break;
        case 0x79:
            mCommand = StopTransmissionCommand;
            break;
        case 0x78:
            mCommand = ResumeTransmissionCommand;
            break;
        default:
            mCommand = UnknownCommand;
        }
    }

    mDataSize = qFromBigEndian<quint16>(data + 2) - 4;
}


QByteArray MatipHeader::rawData() const
{
    return mRawData;
}


bool MatipHeader::isValid() const
{
    return error() == NoError;
}


bool MatipHeader::isControlPacket() const
{
    return command() != NoCommand;
}


bool MatipHeader::isDataPacket() const
{
    return command() == NoCommand;
}


quint16 MatipHeader::dataSize() const
{
    return mDataSize;
}


MatipHeader::Command MatipHeader::command() const
{
    return mCommand;
}


MatipHeader::Error MatipHeader::error() const
{
    return mError;
}


void MatipHeader::make(uchar *dest, Command command, quint16 size)
{
    /*
     * make MATIP header
     *
     * byte0: 00000001
     * byte1: 1xxxxxxx (control packet) or 00000000 (data packet)
     * byte2: length
     * byte3: length
     *
     */

    dest[0] = 0x01;
    dest[1] = 0x80;
    qToBigEndian(size, dest + 2);

    switch (command)
    {
    case SessionOpenCommand:
        dest[1] |= 0x7E;
        break;
    case SessionCloseCommand:
        dest[1] |= 0x7C;
        break;
    case OpenConfirmCommand:
        dest[1] |= 0x7D;
        break;
    case SessionStatusQueryCommand:
        dest[1] |= 0x7B;
        break;
    case SessionStatusResponseCommand:
        dest[1] |= 0x7A;
        break;
    case StopTransmissionCommand:
        dest[1] |= 0x79;
        break;
    case ResumeTransmissionCommand:
        dest[1] |= 0x78;
        break;
    default:
        dest[1] = 0x00;
    }
}
