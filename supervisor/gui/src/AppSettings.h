#ifndef APPSETTINGS_H
#define APPSETTINGS_H

#include <QColor>
#include <QByteArray>
#include <QMap>
#include <QFont>

//! Class contains settings for application
class AppSettings
{
public:
    //! Constructor
    AppSettings();
    ~AppSettings();

    //! Colors
    //@{
    enum ColorType { WORK = 0, NOTWORK, PARTWORK, INIT, MODIFY };

    //! Get current color
    /*!
      \param type a type of color
    */
    QColor getColor(ColorType type) const;
    //! Set current color
    /*!
      \param type a type of color
    */
    void setColor(ColorType type, const QColor& color);
    //@}

    //! Font
    //@{
    QFont getFont() const;
    void setFont(const QFont& font);
    //@}

    void fromByteArray(const QByteArray& arr);
    QByteArray toByteArray() const;

private:
    void Init();

private:
    typedef QMap<int, QColor>   TColorMap;
    TColorMap   mMapColor;
    QFont       mFont;
};

#endif // APPSETTINGS_H
