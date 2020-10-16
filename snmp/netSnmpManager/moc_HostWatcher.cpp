/****************************************************************************
** Meta object code from reading C++ file 'HostWatcher.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.9.5)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "src/HostWatcher.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'HostWatcher.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.9.5. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_snmp_private__HostWatcher_t {
    QByteArrayData data[13];
    char stringdata0[116];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_snmp_private__HostWatcher_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_snmp_private__HostWatcher_t qt_meta_stringdata_snmp_private__HostWatcher = {
    {
QT_MOC_LITERAL(0, 0, 25), // "snmp_private::HostWatcher"
QT_MOC_LITERAL(1, 26, 17), // "variablesReceived"
QT_MOC_LITERAL(2, 44, 0), // ""
QT_MOC_LITERAL(3, 45, 5), // "error"
QT_MOC_LITERAL(4, 51, 4), // "text"
QT_MOC_LITERAL(5, 56, 5), // "start"
QT_MOC_LITERAL(6, 62, 4), // "stop"
QT_MOC_LITERAL(7, 67, 16), // "requestVariables"
QT_MOC_LITERAL(8, 84, 4), // "oids"
QT_MOC_LITERAL(9, 89, 11), // "setVariable"
QT_MOC_LITERAL(10, 101, 3), // "oid"
QT_MOC_LITERAL(11, 105, 5), // "value"
QT_MOC_LITERAL(12, 111, 4) // "type"

    },
    "snmp_private::HostWatcher\0variablesReceived\0"
    "\0error\0text\0start\0stop\0requestVariables\0"
    "oids\0setVariable\0oid\0value\0type"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_snmp_private__HostWatcher[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       3,    1,   47,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       5,    0,   50,    2, 0x0a /* Public */,
       6,    0,   51,    2, 0x0a /* Public */,
       7,    1,   52,    2, 0x0a /* Public */,
       9,    3,   55,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QVariantMap,    2,
    QMetaType::Void, QMetaType::QString,    4,

 // slots: parameters
    QMetaType::Bool,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QStringList,    8,
    QMetaType::Void, QMetaType::QString, QMetaType::QString, QMetaType::Char,   10,   11,   12,

       0        // eod
};

void snmp_private::HostWatcher::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        HostWatcher *_t = static_cast<HostWatcher *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->variablesReceived((*reinterpret_cast< QVariantMap(*)>(_a[1]))); break;
        case 1: _t->error((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 2: { bool _r = _t->start();
            if (_a[0]) *reinterpret_cast< bool*>(_a[0]) = std::move(_r); }  break;
        case 3: _t->stop(); break;
        case 4: _t->requestVariables((*reinterpret_cast< const QStringList(*)>(_a[1]))); break;
        case 5: _t->setVariable((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2])),(*reinterpret_cast< char(*)>(_a[3]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            typedef void (HostWatcher::*_t)(QVariantMap );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HostWatcher::variablesReceived)) {
                *result = 0;
                return;
            }
        }
        {
            typedef void (HostWatcher::*_t)(const QString & );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&HostWatcher::error)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject snmp_private::HostWatcher::staticMetaObject = {
    { &QObject::staticMetaObject, qt_meta_stringdata_snmp_private__HostWatcher.data,
      qt_meta_data_snmp_private__HostWatcher,  qt_static_metacall, nullptr, nullptr}
};


const QMetaObject *snmp_private::HostWatcher::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *snmp_private::HostWatcher::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_snmp_private__HostWatcher.stringdata0))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int snmp_private::HostWatcher::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void snmp_private::HostWatcher::variablesReceived(QVariantMap _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void snmp_private::HostWatcher::error(const QString & _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
