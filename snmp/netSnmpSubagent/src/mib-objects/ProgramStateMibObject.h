#ifndef PROGRAMSTATEMIBOBJECT_H
#define PROGRAMSTATEMIBOBJECT_H

#include "BaseMibObject.h"

#include <QtCore/QScopedPointer>

class ProgramStateMibObject : public BaseMibObject
{
    Q_OBJECT
public:
    ProgramStateMibObject(const QString &name_, const QString &oidString_);

    u_char asnType() const;
    char type() const;

    const QByteArray value() const;
    void setValue(const QVariant &value);

    enum ProgramState {
        PS_DOWN = 1
        , PS_UP = 2
    };

signals:
    void programIsDown();

protected:
    void init();

private:
    int mValue;
};

#endif // PROGRAMSTATEMIBOBJECT_H
