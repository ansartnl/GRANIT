/****************************************************************************
** Meta object code from reading C++ file 'NetSnmpManager.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "include/NetSnmpManager.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NetSnmpManager.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_snmp__NetSnmpManager_t {
    QByteArrayData data[26];
    char stringdata0[227];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_snmp__NetSnmpManager_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_snmp__NetSnmpManager_t qt_meta_stringdata_snmp__NetSnmpManager = {
    {
QT_MOC_LITERAL(0, 0, 20), // "snmp::NetSnmpManager"
QT_MOC_LITERAL(1, 21, 7), // "started"
QT_MOC_LITERAL(2, 29, 0), // ""
QT_MOC_LITERAL(3, 30, 7), // "stopped"
QT_MOC_LITERAL(4, 38, 9), // "connected"
QT_MOC_LITERAL(5, 48, 4), // "host"
QT_MOC_LITERAL(6, 53, 14), // "lostConnection"
QT_MOC_LITERAL(7, 68, 17), // "variablesReceived"
QT_MOC_LITERAL(8, 86, 9), // "variables"
QT_MOC_LITERAL(9, 96, 4), // "trap"
QT_MOC_LITERAL(10, 101, 8), // "hostname"
QT_MOC_LITERAL(11, 110, 2), // "ip"
QT_MOC_LITERAL(12, 113, 5), // "error"
QT_MOC_LITERAL(13, 119, 4), // "text"
QT_MOC_LITERAL(14, 124, 5), // "start"
QT_MOC_LITERAL(15, 130, 4), // "stop"
QT_MOC_LITERAL(16, 135, 7), // "addHost"
QT_MOC_LITERAL(17, 143, 9), // "community"
QT_MOC_LITERAL(18, 153, 10), // "removeHost"
QT_MOC_LITERAL(19, 164, 14), // "removeAllHosts"
QT_MOC_LITERAL(20, 179, 11), // "setVariable"
QT_MOC_LITERAL(21, 191, 3), // "oid"
QT_MOC_LITERAL(22, 195, 5), // "value"
QT_MOC_LITERAL(23, 201, 4), // "type"
QT_MOC_LITERAL(24, 206, 15), // "requestVariable"
QT_MOC_LITERAL(25, 222, 4) // "oids"

    },
    "snmp::NetSnmpManager\0started\0\0stopped\0"
    "connected\0host\0lostConnection\0"
    "variablesReceived\0variables\0trap\0"
    "hostname\0ip\0error\0text\0start\0stop\0"
    "addHost\0community\0removeHost\0"
    "removeAllHosts\0setVariable\0oid\0value\0"
    "type\0requestVariable\0oids"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_snmp__NetSnmpManager[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
      14,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       7,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    0,   84,    2, 0x06 /* Public */,
       3,    0,   85,    2, 0x06 /* Public */,
       4,    1,   86,    2, 0x06 /* Public */,
       6,    1,   89,    2, 0x06 /* Public */,
       7,    2,   92,    2, 0x06 /* Public */,
       9,    3,   97,    2, 0x06 /* Public */,
      12,    1,  104,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
      14,    0,  107,    2, 0x0a /* Public */,
      15,    0,  108,    2, 0x0a /* Public */,
      16,    2,  109,    2, 0x0a /* Public */,
      18,    1,  114,    2, 0x0a /* Public */,
      19,    0,  117,    2, 0x0a /* Public */,
      20,    4,  118,    2, 0x0a /* Public */,
      24,    2,  127,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void, QMetaType::QString, QMetaType::QVariantMap,    5,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QVariantMap,   10,   11,    8,
    QMetaType::Void, QMetaType::QString,   13,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    5,   17,
    QMetaType::Void, QMetaType::QString,    5,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::QString, QMetaType::Char,    5,   21,   22,   23,
    QMetaType::Void, QMetaType::QString, QMetaType::QStringList,    5,   25,

       0        // eod
};

void snmp::NetSnmpManager::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        NetSnmpManager *_t = static_cast<NetSnmpManager *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->started(); break;
        case 1: _t->stopped(); break;
        case 2: _t->connected((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->lostConnection((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->variablesReceived((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QVariantMap(*)>(_a[2]))); break;
        case 5: _t->trap((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2])),(*reinterpret_cast< QVariantMap(*)>(_a[3]))); break;
        case 6: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 7: _t->start(); break;
        case 8: _t->stop(); break;
        case 9: _t->addHost((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 10: _t->removeHost((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 11: _t->removeAllHosts(); break;
        case 12: _t->setVariable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< const QString(*)>(_a[3])),(*reinterpret_cast< char(*)>(_a[4]))); break;
        case 13: _t->requestVariable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< QStringList(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (NetSnmpManager::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::started)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (NetSnmpManager::*_t)();
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::stopped)) {
                *result = 1;
                return;
            }
        }
        {
            typedef void (NetSnmpManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::connected)) {
                *result = 2;
                return;
            }
        }
        {
            typedef void (NetSnmpManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::lostConnection)) {
                *result = 3;
                return;
            }
        }
        {
            typedef void (NetSnmpManager::*_t)(QString , QVariantMap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::variablesReceived)) {
                *result = 4;
                return;
            }
        }
        {
            typedef void (NetSnmpManager::*_t)(QString , QString , QVariantMap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::trap)) {
                *result = 5;
                return;
            }
        }
        {
            typedef void (NetSnmpManager::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&NetSnmpManager::error)) {
                *result = 6;
                return;
            }
        }
    }
}

const QMetaObject snmp::NetSnmpManager::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_snmp__NetSnmpManager.data,
      qt_meta_data_snmp__NetSnmpManager,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *snmp::NetSnmpManager::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *snmp::NetSnmpManager::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_snmp__NetSnmpManager.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int snmp::NetSnmpManager::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 14)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 14;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 14)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 14;
    }
    return _id;
}

// SIGNAL 0
void snmp::NetSnmpManager::started()
{
    QMetaObject::activate(this, &staticMetaObject, 0, nullptr);
}

// SIGNAL 1
void snmp::NetSnmpManager::stopped()
{
    QMetaObject::activate(this, &staticMetaObject, 1, nullptr);
}

// SIGNAL 2
void snmp::NetSnmpManager::connected(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}

// SIGNAL 3
void snmp::NetSnmpManager::lostConnection(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 3, _a);
}

// SIGNAL 4
void snmp::NetSnmpManager::variablesReceived(QString _t1, QVariantMap _t2)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 4, _a);
}

// SIGNAL 5
void snmp::NetSnmpManager::trap(QString _t1, QString _t2, QVariantMap _t3)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)), const_cast<void*>(reinterpret_cast<const void*>(&_t3)) };
    QMetaObject::activate(this, &staticMetaObject, 5, _a);
}

// SIGNAL 6
void snmp::NetSnmpManager::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 6, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
