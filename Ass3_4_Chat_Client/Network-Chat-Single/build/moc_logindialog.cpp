/****************************************************************************
** Meta object code from reading C++ file 'logindialog.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.5.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../logindialog.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'logindialog.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.5.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_LoginDialog_t {
    QByteArrayData data[11];
    char stringdata0[128];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_LoginDialog_t, stringdata0) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_LoginDialog_t qt_meta_stringdata_LoginDialog = {
    {
QT_MOC_LITERAL(0, 0, 11), // "LoginDialog"
QT_MOC_LITERAL(1, 12, 5), // "login"
QT_MOC_LITERAL(2, 18, 0), // ""
QT_MOC_LITERAL(3, 19, 6), // "signup"
QT_MOC_LITERAL(4, 26, 15), // "returnedPressed"
QT_MOC_LITERAL(5, 42, 8), // "loggedin"
QT_MOC_LITERAL(6, 51, 18), // "loginButtonPressed"
QT_MOC_LITERAL(7, 70, 19), // "signupButtonPressed"
QT_MOC_LITERAL(8, 90, 15), // "responseHandler"
QT_MOC_LITERAL(9, 106, 7), // "exitApp"
QT_MOC_LITERAL(10, 114, 13) // "disconnectAll"

    },
    "LoginDialog\0login\0\0signup\0returnedPressed\0"
    "loggedin\0loginButtonPressed\0"
    "signupButtonPressed\0responseHandler\0"
    "exitApp\0disconnectAll"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_LoginDialog[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       9,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       4,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    2,   59,    2, 0x06 /* Public */,
       3,    2,   64,    2, 0x06 /* Public */,
       4,    0,   69,    2, 0x06 /* Public */,
       5,    0,   70,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       6,    0,   71,    2, 0x08 /* Private */,
       7,    0,   72,    2, 0x08 /* Private */,
       8,    1,   73,    2, 0x08 /* Private */,
       9,    0,   76,    2, 0x08 /* Private */,
      10,    0,   77,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void, QMetaType::QString, QMetaType::QString,    2,    2,
    QMetaType::Void,
    QMetaType::Void,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, QMetaType::QString,    2,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void LoginDialog::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        LoginDialog *_t = static_cast<LoginDialog *>(_o);
        Q_UNUSED(_t)
        switch (_id) {
        case 0: _t->login((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 1: _t->signup((*reinterpret_cast< QString(*)>(_a[1])),(*reinterpret_cast< QString(*)>(_a[2]))); break;
        case 2: _t->returnedPressed(); break;
        case 3: _t->loggedin(); break;
        case 4: _t->loginButtonPressed(); break;
        case 5: _t->signupButtonPressed(); break;
        case 6: _t->responseHandler((*reinterpret_cast< QString(*)>(_a[1]))); break;
        case 7: _t->exitApp(); break;
        case 8: _t->disconnectAll(); break;
        default: ;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (LoginDialog::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::login)) {
                *result = 0;
            }
        }
        {
            typedef void (LoginDialog::*_t)(QString , QString );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::signup)) {
                *result = 1;
            }
        }
        {
            typedef void (LoginDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::returnedPressed)) {
                *result = 2;
            }
        }
        {
            typedef void (LoginDialog::*_t)();
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&LoginDialog::loggedin)) {
                *result = 3;
            }
        }
    }
}

const QMetaObject LoginDialog::staticMetaObject = {
    { &QDialog::staticMetaObject, qt_meta_stringdata_LoginDialog.data,
      qt_meta_data_LoginDialog,  qt_static_metacall, Q_NULLPTR, Q_NULLPTR}
};


const QMetaObject *LoginDialog::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *LoginDialog::qt_metacast(const char *_clname)
{
    if (!_clname) return Q_NULLPTR;
    if (!strcmp(_clname, qt_meta_stringdata_LoginDialog.stringdata0))
        return static_cast<void*>(const_cast< LoginDialog*>(this));
    return QDialog::qt_metacast(_clname);
}

int LoginDialog::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QDialog::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 9)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 9;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 9)
            *reinterpret_cast<int*>(_a[0]) = -1;
        _id -= 9;
    }
    return _id;
}

// SIGNAL 0
void LoginDialog::login(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void LoginDialog::signup(QString _t1, QString _t2)
{
    void *_a[] = { Q_NULLPTR, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void LoginDialog::returnedPressed()
{
    QMetaObject::activate(this, &staticMetaObject, 2, Q_NULLPTR);
}

// SIGNAL 3
void LoginDialog::loggedin()
{
    QMetaObject::activate(this, &staticMetaObject, 3, Q_NULLPTR);
}
QT_END_MOC_NAMESPACE
