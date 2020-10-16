#ifndef CATALOGORGANIZER_H
#define CATALOGORGANIZER_H

#include <QtCore/QDateTime>
#include <QtCore/QDir>
#include <QtCore/QLocale>
#include <QtCore/QString>

class CatalogOrganizer
{
public:
    //! constructor.
    /*!
        \param check identifies if check for directory existance is needed
    */
    CatalogOrganizer(bool check = false);

    //! Set if directory check is needed
    /*!
        \param check true == check needed
      */
    void SetCheck(bool check){bCheckNeeded = check;}

    //! returns directory for today's incoming messages
    QString GetIncomingPath();

    //! returns directory for today's outgoing messages
    QString GetOutgoingPath();

    //! gets todays subdir way (oct-12/D12/)
    QString GetTodaysPath();

    //! set paths for incoming and outgoing directories
    void SetPaths(QString incoming, QString outgoing);

private:

    //! create directory if it doesn't exits
    bool CreateIfNeeded(QString path);

    QString mDirInc;
    QString mDirOut;
    bool bCheckNeeded;
};

#endif // CATALOGORGANIZER_H
