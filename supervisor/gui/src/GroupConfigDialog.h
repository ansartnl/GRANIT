#ifndef GROUPCONFIGDIALOG_H
#define GROUPCONFIGDIALOG_H

#include <QDialog>

class QAbstractButton;

namespace Ui {
class GroupConfigDialog;
}

#include <QVariant>
#include <QDataStream>

struct Config{
    bool isGroup;
    QString value;
};

Q_DECLARE_METATYPE(Config)

QDataStream &operator<<(QDataStream &out, const Config &config);
QDataStream &operator>>(QDataStream &in, Config &config);

class GroupConfigDialog : public QDialog
{
    Q_OBJECT
    
public:

    typedef QMap<QString, QHash<QString, Config> > TConfigMap;

    static const char COMMAND_TAG[];
    static QStringList getConfigNames();
    static QHash<QString, Config> getConfig(const QString& name);

    explicit GroupConfigDialog(QWidget *parent = 0);
    ~GroupConfigDialog();

    void init();

public slots:
    void setServers(const QStringList& servers);
    void setAvailableOptions(const QStringList& options);

protected:
    virtual void changeEvent(QEvent * event);

signals:
    void requestServers();
    void requestGroups(QString host);
    void requestScripts(QString host);

private slots:
    void onAddConfig();
    void onRemoveConfig();
    void onConfigSelected(QString configName);
    void onAdd();
    void onRemove();
    void onHostSelected();

    void onActionsChaned(bool isGroupsSelected);

    void onControlButtonsClicked(QAbstractButton*pButton);

private:
    Ui::GroupConfigDialog *ui;
    TConfigMap     mConfigs;
    QString mCurrentHost;
};

#endif // GROUPCONFIGDIALOG_H
