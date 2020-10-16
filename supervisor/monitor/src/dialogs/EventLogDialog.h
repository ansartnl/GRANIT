#ifndef EVENTLOGDIALOG_H
#define EVENTLOGDIALOG_H

#include <QDialog>
#include <QDateTime>

namespace Ui {
class EventLogDialog;
}

class EventLogModel;

class EventLogDialog : public QDialog
{
    Q_OBJECT
    static const QString DB_CONNECTION_NAME;

public:
    explicit EventLogDialog(QWidget *parent = 0);
    ~EventLogDialog();

    void enableActiveControl(bool enable);

public slots:
    void refresh();
    
signals:
    void print();

protected:
    virtual void closeEvent(QCloseEvent * event);

private slots:
    void onFilterFailureToggled(bool checked);
    void onClearFilter();
    void onPrint();
    void onColumnResized(int logicalIndex, int oldSize, int newSize);
    void updateDateEdit();
    void loadDatabase(const QDate &date);

private:
    Ui::EventLogDialog *ui;
    EventLogModel* mModel;

    QString mEventFilterBuffer;
    QString mHostFilterBuffer;
    QString mServiceFilterBuffer;
};

#endif // EVENTLOGDIALOG_H
