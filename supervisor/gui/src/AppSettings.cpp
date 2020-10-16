#include "AppSettings.h"
#include <QDataStream>

AppSettings::AppSettings()
{
    Init();
}

AppSettings::~AppSettings()
{

}

QColor AppSettings::getColor(AppSettings::ColorType type) const
{
    QColor color;
    if(type >= WORK && type <= MODIFY)
        color = mMapColor.value(type);
    return color;
}

void AppSettings::setColor(ColorType type, const QColor& color)
{
    if(color.isValid() && type >= WORK && type <= MODIFY)
        mMapColor[type] = color;
}

QFont AppSettings::getFont() const
{
    return mFont;
}

void AppSettings::setFont(const QFont& font)
{
    mFont = font;
}

void AppSettings::fromByteArray(const QByteArray &arr)
{
    QDataStream ds(arr);
    ds >> mMapColor;
    ds >> mFont;
    Init();
}

QByteArray AppSettings::toByteArray() const
{
    QByteArray arr;
    QDataStream ds(&arr, QIODevice::WriteOnly);
    ds << mMapColor;
    ds << mFont;
    return arr;
}

void AppSettings::Init()
{
    if(!mMapColor.contains(WORK))
        mMapColor[WORK] = QColor(Qt::black);
    if(!mMapColor.contains(NOTWORK))
        mMapColor[NOTWORK] = QColor(Qt::red);
    if(!mMapColor.contains(PARTWORK))
        mMapColor[PARTWORK] = QColor::fromRgb(255, 128, 0);
    if(!mMapColor.contains(INIT))
        mMapColor[INIT] = QColor(Qt::darkGray);
    if(!mMapColor.contains(MODIFY))
        mMapColor[MODIFY] = QColor::fromRgb(255, 0, 255);
}
