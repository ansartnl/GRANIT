#include "ClickableLabel.h"

#include <QMouseEvent>
#include <QMenu>

#include <QWidgetAction>

ClickableLabel::ClickableLabel(QWidget *parent) :
    QLabel(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
}


void ClickableLabel::mousePressEvent(QMouseEvent *ev)
{
    if(ev->button() == Qt::LeftButton)
        emit clicked();
    else
        QLabel::mousePressEvent(ev);
}


void ClickableLabel::showContextMenu(const QPoint &pos)
{
    QMenu* menu = new QMenu(this);
    menu->addActions(actions());
    menu->exec(pos);

    menu->deleteLater();
}
