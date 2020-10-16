#ifndef SUPERVISORVIEW_H
#define SUPERVISORVIEW_H

#include <QGraphicsView>

#include "SupervisorWidget.h"

class SupervisorView : public QGraphicsView
{
    Q_OBJECT
    
public:
    typedef QHash<QString, SupervisorWidget* > TWidgetsHash;

    SupervisorView(QWidget *parent = 0);
    ~SupervisorView();

    void clearWidgets();
    const TWidgetsHash& supervisorWidgets() const;
    void addSupervisorWidget(SupervisorWidget* widget);

//    const TDatasourceHash& datasources() const;
//    void addDatasource(QObject* datasource);

    void updateWidgets();

protected:
    void resizeEvent(QResizeEvent *event);

public slots:
    void enableAdminMode(bool enable);

private:
    TWidgetsHash mWidgets;
    bool mAdminMode;
};

#endif // SUPERVISORVIEW_H
