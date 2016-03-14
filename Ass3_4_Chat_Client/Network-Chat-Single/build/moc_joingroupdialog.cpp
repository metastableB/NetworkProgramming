/****************************************************************************
** Meta object code from reading C++ file 'joingroupdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../joingroupdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'joingroupdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_joinGroupDialog_t {
    QByteArrayData data[6];
    char stringdata0[73];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_joinGroupDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_joinGroupDialog_t qt_meta_stringdata_joinGroupDialog = {
    {
QT_MOC_LITERAL(0, 0, 15), // "joinGroupDialog"
QT_MOC_LITERAL(1, 16, 9), // "joinGroup"
QT_MOC_LITERAL(2, 26, 0), // ""
QT_MOC_LITERAL(3, 27, 11), // "createGroup"
QT_MOC_LITERAL(4, 39, 15), // "slotJoinPressed"
QT_MOC_LITERAL(5, 55, 17) // "slotCreatePressed"

    },
    "joinGroupDialog\0joinGroup\0\0createGroup\0"
    "slotJoinPressed\0slotCreatePressed"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_joinGroupDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       4,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   34,    2, 0x06 /* Public */,
       3,    1,   37,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       4,    0,   40,    2, 0x08 /* Private */,
       5,    0,   41,    2, 0x08 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void, QMetaType::QString,    2,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void joinGroupDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        joinGroupDialog *_t = static_cast<joinGroupDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->joinGroup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 1: _t->createGroup((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 2: _t->slotJoinPressed(); break;
        case 3: _t->slotCreatePressed(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (joinGroupDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&joinGroupDialog::joinGroup)) {
                *result = 0;
            }
        }
        {
            typedef void (joinGroupDialog::*_t)(QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&joinGroupDialog::createGroup)) {
                *result = 1;
            }
        }
    }
}

const QMetaObject joinGroupDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_joinGroupDialog.data,
      qt_meta_data_joinGroupDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *joinGroupDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *joinGroupDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_joinGroupDialog.stringdata0))
        return static_cast<void*>(const_cast< joinGroupDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int joinGroupDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 4)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 4;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 4)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 4;
    }
    return _id;
}

// SIGNAL 0
void joinGroupDialog::joinGroup(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void joinGroupDialog::createGroup(QString _t1)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_END_MOC_NAMESPACE
