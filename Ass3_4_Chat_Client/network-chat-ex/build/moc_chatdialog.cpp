/****************************************************************************
** Meta object code from reading C++ file 'chatdialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../chatdialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'chatdialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_ChatDialog_t {
    QByteArrayData data[12];
    char stringdata0[131];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_ChatDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_ChatDialog_t qt_meta_stringdata_ChatDialog = {
    {
QT_MOC_LITERAL(0, 0, 10), // "ChatDialog"
QT_MOC_LITERAL(1, 11, 13), // "appendMessage"
QT_MOC_LITERAL(2, 25, 0), // ""
QT_MOC_LITERAL(3, 26, 4), // "from"
QT_MOC_LITERAL(4, 31, 7), // "message"
QT_MOC_LITERAL(5, 39, 13), // "returnPressed"
QT_MOC_LITERAL(6, 53, 14), // "newParticipant"
QT_MOC_LITERAL(7, 68, 4), // "nick"
QT_MOC_LITERAL(8, 73, 15), // "participantLeft"
QT_MOC_LITERAL(9, 89, 15), // "showInformation"
QT_MOC_LITERAL(10, 105, 21), // "authenticationMessage"
QT_MOC_LITERAL(11, 127, 3) // "msg"

    },
    "ChatDialog\0appendMessage\0\0from\0message\0"
    "returnPressed\0newParticipant\0nick\0"
    "participantLeft\0showInformation\0"
    "authenticationMessage\0msg"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_ChatDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags
       1,    2,   44,    2, 0x0a /* Public */,
       5,    0,   49,    2, 0x08 /* Private */,
       6,    1,   50,    2, 0x08 /* Private */,
       8,    1,   53,    2, 0x08 /* Private */,
       9,    0,   56,    2, 0x08 /* Private */,
      10,    1,   57,    2, 0x08 /* Private */,

 // slots: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    3,    4,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void, QMetaType::QString,    7,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,   11,

       0        // eod
};

void ChatDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        ChatDialog *_t = static_cast<ChatDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->appendMessage((*reinterpret_cast< const QString(*)>(_a[1])),(*reinterpret_cast< const QString(*)>(_a[2]))); break;
        case 1: _t->returnPressed(); break;
        case 2: _t->newParticipant((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 3: _t->participantLeft((*reinterpret_cast< const QString(*)>(_a[1]))); break;
        case 4: _t->showInformation(); break;
        case 5: _t->authenticationMessage((*reinterpret_cast< QString(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject ChatDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_ChatDialog.data,
      qt_meta_data_ChatDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *ChatDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *ChatDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_ChatDialog.stringdata0))
        return static_cast<void*>(const_cast< ChatDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int ChatDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
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
QT_END_MOC_NAMESPACE
