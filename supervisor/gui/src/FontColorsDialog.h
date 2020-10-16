#ifndef FONTCOLORSDIALOG_H
#define FONTCOLORSDIALOG_H

#include <QtGui/QDialog>
#include "AppSettings.h"

namespace Ui {
    class FontColorsDialog;
}

class FontColorsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FontColorsDialog(const AppSettings& appsettings, QWidget *parent = 0);
    ~FontColorsDialog();

    AppSettings getAppSettings() const;
	
protected slots:
    void on_pushButton_1_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_5_clicked();
    void on_pushButtonFont_clicked();

private:
    Ui::FontColorsDialog *ui;
    AppSettings mAppSettings;
};

#endif // FONTCOLORSDIALOG_H
