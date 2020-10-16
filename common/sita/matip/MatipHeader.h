#ifndef MATIPHEADER_H
#define MATIPHEADER_H

#include <QtCore/QByteArray>

class MatipHeader
{
public:
    //! MATIP commands
    enum Command
    {
        NoCommand,
        SessionOpenCommand,
        SessionCloseCommand,
        OpenConfirmCommand,
        SessionStatusQueryCommand,
        SessionStatusResponseCommand,
        StopTransmissionCommand,
        ResumeTransmissionCommand,
        UnknownCommand
    };

    //! MATIP header errors
    enum Error
    {
        NoError,
        InvalidHeaderError,
        VersionMismatchError
    };

    /*!
     * Parses MATIP header
     * \param data  The memory area containing four-byte MATIP header
     */
    explicit MatipHeader(const uchar *data);

    //! Returns raw header data
    QByteArray rawData() const;

    //! Returns true if this header is valid
    bool    isValid() const;

    //! Returns true if packet contains MATIP command
    bool    isControlPacket() const;

    //! Returns true if packet contains payload
    bool    isDataPacket() const;

    //! Returns payloads size
    quint16 dataSize() const;

    //! Returns MATIP command
    Command command() const;

    //! Returns MATIP error
    Error   error() const;

    /*!
     * Makes MATIP header
     * \param[out] dest The memory area to save header
     * \param[in] cmd MATIP command
     * \param[in] size Packet size
     */
    static void make(uchar *dest, Command cmd, quint16 size);

private:
    QByteArray mRawData;
    quint16 mDataSize;
    Command mCommand;
    Error   mError;

};

#endif // MATIPHEADER_H
