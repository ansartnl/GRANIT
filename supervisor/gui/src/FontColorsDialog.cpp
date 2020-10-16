#include "FontColorsDialog.h"
#include "ui_FontColorsDialog.h"
#include <QColorDialog>
#include <QFontDialog>
#include <QPalette>

FontColorsDialog::FontColorsDialog(const AppSettings &appsettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FontColorsDialog), mAppSettings(appsettings)
{
    ui->setupUi(this);
    {
        QPalette pal(ui->lineEdit_1->palette());
        pal.setBrush(QPalette::Text, QBrush(mAppSettings.getColor(AppSettings::WORK)));
        ui->lineEdit_1->setPalette(pal);
    }
    {
        QPalette pal(ui->lineEdit_2->palette());
        pal.setBrush(QPalette::Text, QBrush(mAppSettings.getColor(AppSettings::NOTWORK)));
        ui->lineEdit_2->setPalette(pal);
    }
    {
        QPalette pal(ui->lineEdit_3->palette());
        pal.setBrush(QPalette::Text, QBrush(mAppSettings.getColor(AppSettings::PARTWORK)));
        ui->lineEdit_3->setPalette(pal);
    }
    {
        QPalette pal(ui->lineEdit_4->palette());
        pal.setBrush(QPalette::Text, QBrush(mAppSettings.getColor(AppSettings::INIT)));
        ui->lineEdit_4->setPalette(pal);
    }
    {
        QPalette pal(ui->lineEdit_5->palette());
        pal.setBrush(QPalette::Text, QBrush(mAppSettings.getColor(AppSettings::MODIFY)));
        ui->lineEdit_5->setPalette(pal);
    }
}

FontColorsDialog::~FontColorsDialog()
{
    delete ui;
}

AppSettings FontColorsDialog::getAppSettings() const
{
    return mAppSettings;
}

void FontColorsDialog::on_pushButton_1_clicked()
{
    QColor color = QColorDialog::getColor(mAppSettings.getColor(AppSettings::WORK), this);
    if(color.isValid())
    {
        mAppSettings.setColor(AppSettings::WORK, color);
        QPalette pal(ui->lineEdit_1->palette());
        pal.setBrush(QPalette::Text, QBrush(color));
        ui->lineEdit_1->setPalette(pal);
    }
}

void FontColorsDialog::on_pushButton_2_clicked()
{
    QColor color = QColorDialog::getColor(mAppSettings.getColor(AppSettings::NOTWORK), this);
    if(color.isValid())
    {
        mAppSettings.setColor(AppSettings::NOTWORK, color);
        QPalette pal(ui->lineEdit_2->palette());
        pal.setBrush(QPalette::Text, QBrush(color));
        ui->lineEdit_2->setPalette(pal);
    }
}

void FontColorsDialog::on_pushButton_3_clicked()
{
    QColor color = QColorDialog::getColor(mAppSettings.getColor(AppSettings::PARTWORK), this);
    if(color.isValid())
    {
        mAppSettings.setColor(AppSettings::PARTWORK, color);
        QPalette pal(ui->lineEdit_3->palette());
        pal.setBrush(QPalette::Text, QBrush(color));
        ui->lineEdit_3->setPalette(pal);
    }
}

void FontColorsDialog::on_pushButton_4_clicked()
{
    QColor color = QColorDialog::getColor(mAppSettings.getColor(AppSettings::INIT), this);
    if(color.isValid())
    {
        mAppSettings.setColor(AppSettings::INIT, color);
        QPalette pal(ui->lineEdit_4->palette());
        pal.setBrush(QPalette::Text, QBrush(color));
        ui->lineEdit_4->setPalette(pal);
    }
}

void FontColorsDialog::on_pushButton_5_clicked()
{
    QColor color = QColorDialog::getColor(mAppSettings.getColor(AppSettings::MODIFY), this);
    if(color.isValid())
    {
        mAppSettings.setColor(AppSettings::MODIFY, color);
        QPalette pal(ui->lineEdit_5->palette());
        pal.setBrush(QPalette::Text, QBrush(color));
        ui->lineEdit_5->setPalette(pal);
    }
}

void FontColorsDialog::on_pushButtonFont_clicked()
{
    bool ok(false);
    QFont font = QFontDialog::getFont(&ok, mAppSettings.getFont(), this);
    if(ok)
    {
        mAppSettings.setFont(font);
        setFont(mAppSettings.getFont());
    }
}
