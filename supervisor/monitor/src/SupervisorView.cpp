#include "SupervisorView.h"

#include <QMenuBar>
#include <QGraphicsProxyWidget>

#include "SupervisorWidget.h"


SupervisorView::SupervisorView(QWidget *parent)
    : QGraphicsView(parent), mAdminMode(true)
{
    QGraphicsScene *scene = new QGraphicsScene(this);
    setScene(scene);
    setRenderHint(QPainter::Antialiasing);
}

SupervisorView::~SupervisorView()
{
}

void SupervisorView::clearWidgets()
{
    QList<SupervisorWidget*> items = findChildren<SupervisorWidget*>();
    foreach(SupervisorWidget* item, items)
        item->deleteLater();

    mWidgets.clear();
}

const SupervisorView::TWidgetsHash &SupervisorView::supervisorWidgets() const
{
    return mWidgets;
}

void SupervisorView::addSupervisorWidget(SupervisorWidget *widget)
{
    if(widget->scene() != scene())
    {
        mWidgets.insert(widget->id(), widget);
        widget->enableMoving(mAdminMode);
        scene()->addItem(widget);
    }
}

void SupervisorView::updateWidgets()
{
    TWidgetsHash::iterator itWidget = mWidgets.begin();
    TWidgetsHash::iterator itEnd = mWidgets.end();
    for(; itWidget != itEnd; itWidget++)
    {
        if(mWidgets.contains(itWidget.value()->parentID()))
            itWidget.value()->setParentItem(mWidgets[itWidget.value()->parentID()]);
        else
            itWidget.value()->setParent(this);
    }
    update();
}

void SupervisorView::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event)
    scene()->setSceneRect(contentsRect());
}

void SupervisorView::enableAdminMode(bool enable)
{
    mAdminMode = enable;
    foreach(SupervisorWidget* item, mWidgets.values())
        item->enableMoving(enable);
}
