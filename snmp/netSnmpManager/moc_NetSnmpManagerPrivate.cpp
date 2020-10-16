/****************************************************************************
** Meta object code from reading C++ file 'NetSnmpManagerPrivate.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/NetSnmpManagerPrivate.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetSnmpManagerPrivate.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_snmp_private__NetSnmpManagerPrivate_t {
    QByteArrayData data[20];
    char stringdata0[217];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_snmp_private__NetSnmpManagerPrivate_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_snmp_private__NetSnmpManagerPrivate_t qt_meta_stringdata_snmp_private__NetSnmpManagerPrivate = {
    {
QT_MOC_LITERAL(0, 0, 35), // "snmp_private::NetSnmpManagerP..."
QT_MOC_LITERAL(1, 36, 7), // "started"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 7), // "stopped"
QT_MOC_LITERAL(4, 53, 9), // "connected"
QT_MOC_LITERAL(5, 63, 4), // "host"
QT_MOC_LITERAL(6, 68, 14), // "lostConnection"
QT_MOC_LITERAL(7, 83, 17), // "variablesReceived"
QT_MOC_LITERAL(8, 101, 9), // "variables"
QT_MOC_LITERAL(9, 111, 4), // "trap"
QT_MOC_LITERAL(10, 116, 8), // "hostname"
QT_MOC_LITERAL(11, 125, 2), // "ip"
QT_MOC_LITERAL(12, 128, 5), // "error"
QT_MOC_LITERAL(13, 134, 4), // "text"
QT_MOC_LITERAL(14, 139, 8), // "pAddHost"
QT_MOC_LITERAL(15, 148, 9), // "community"
QT_MOC_LITERAL(16, 158, 10), // "onPingCame"
QT_MOC_LITERAL(17, 169, 19), // "onVariablesReceived"
QT_MOC_LITERAL(18, 189, 11), // "onHostError"
QT_MOC_LITERAL(19, 201, 15) // "onTrapReadyRead"

    },
    "snmp_private::NetSnmpManagerPrivate\0"
    "started\0\0stopped\0connected\0host\0"
    "lostConnection\0variablesReceived\0"
    "variables\0trap\0hostname\0ip\0error\0text\0"
    "pAddHost\0community\0onPingCame\0"
    "onVariablesReceived\0onHostError\0"
    "onTrapReadyRead"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_snmp_private__NetSnmpManagerPrivate[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      12,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   74,    2, 0x06 /* Public */,
       3,    0,   75,    2, 0x06 /* Public */,
       4,    1,   76,    2, 0x06 /* Public */,
       6,    1,   79,    2, 0x06 /* Public */,
       7,    2,   82,    2, 0x06 /* Public */,
       9,    3,   87,    2, 0x06 /* Public */,
      12,    1,   94,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    2,   97,    2, 0x08 /* Private */,
      16,    0,  102,    2, 0x08 /* Private */,
      17,    1,  103,    2, 0x08 /* Private */,
      18,    1,  106,    2, 0x08 /* Private */,
      19,    0,  109,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,    5,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QVariantMap,   10,   11,    8,
    QMetaType::Void, QMetaType::QString,   13,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,   15,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QVariantMap,    8,
    QMetaType::Void, QMetaType::QString,   13,
    QMetaType::Void,

       0        // eod
};

void snmp_private::NetSnmpManagerPrivate::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetSnmpManagerPrivate *_t = static_cast<NetSnmpManagerPrivate *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->started(); break;
        case 1: _t->stopped(); break;
        case 2: _t->connected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->lostConnection((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->variablesReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 5: _t->trap((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariantMap(*)>(_a[3]))); break;
        case 6: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->pAddHost((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 8: _t->onPingCame(); break;
        case 9: _t->onVariablesReceived((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 10: _t->onHostError((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->onTrapReadyRead(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (NetSnmpManagerPrivate::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::started)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (NetSnmpManagerPrivate::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::stopped)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (NetSnmpManagerPrivate::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::connected)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (NetSnmpManagerPrivate::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::lostConnection)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (NetSnmpManagerPrivate::*_t)(QString , QVariantMap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::variablesReceived)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (NetSnmpManagerPrivate::*_t)(QString , QString , QVariantMap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::trap)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (NetSnmpManagerPrivate::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManagerPrivate::error)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject snmp_private::NetSnmpManagerPrivate::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_snmp_private__NetSnmpManagerPrivate.data,
      qt_meta_data_snmp_private__NetSnmpManagerPrivate,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *snmp_private::NetSnmpManagerPrivate::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *snmp_private::NetSnmpManagerPrivate::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_snmp_private__NetSnmpManagerPrivate.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int snmp_private::NetSnmpManagerPrivate::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 12)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 12;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 12)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 12;
    }
    return _id;
}

// SIGNAL 0
void snmp_private::NetSnmpManagerPrivate::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void snmp_private::NetSnmpManagerPrivate::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void snmp_private::NetSnmpManagerPrivate::connected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void snmp_private::NetSnmpManagerPrivate::lostConnection(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void snmp_private::NetSnmpManagerPrivate::variablesReceived(QString _t1, QVariantMap _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void snmp_private::NetSnmpManagerPrivate::trap(QString _t1, QString _t2, QVariantMap _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void snmp_private::NetSnmpManagerPrivate::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
