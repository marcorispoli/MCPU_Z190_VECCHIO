/****************************************************************************
** Meta object code from reading C++ file 'tcpipclient.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../SOURCE/LIB/tcpipclient.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'tcpipclient.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_TcpIpClient_t {
    const uint offsetsAndSize[28];
    char stringdata0[159];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_TcpIpClient_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_TcpIpClient_t qt_meta_stringdata_TcpIpClient = {
    {
QT_MOC_LITERAL(0, 11), // "TcpIpClient"
QT_MOC_LITERAL(12, 16), // "clientConnection"
QT_MOC_LITERAL(29, 0), // ""
QT_MOC_LITERAL(30, 6), // "status"
QT_MOC_LITERAL(37, 6), // "rxData"
QT_MOC_LITERAL(44, 4), // "data"
QT_MOC_LITERAL(49, 6), // "txData"
QT_MOC_LITERAL(56, 7), // "timeout"
QT_MOC_LITERAL(64, 12), // "socketRxData"
QT_MOC_LITERAL(77, 11), // "socketError"
QT_MOC_LITERAL(89, 28), // "QAbstractSocket::SocketError"
QT_MOC_LITERAL(118, 5), // "error"
QT_MOC_LITERAL(124, 15), // "socketConnected"
QT_MOC_LITERAL(140, 18) // "socketDisconnected"

    },
    "TcpIpClient\0clientConnection\0\0status\0"
    "rxData\0data\0txData\0timeout\0socketRxData\0"
    "socketError\0QAbstractSocket::SocketError\0"
    "error\0socketConnected\0socketDisconnected"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_TcpIpClient[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       8,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       2,       // signalCount

 // signals: name, argc, parameters, tag, flags, initial metatype offsets
       1,    1,   62,    2, 0x06,    1 /* Public */,
       4,    1,   65,    2, 0x06,    3 /* Public */,

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       6,    2,   68,    2, 0x0a,    5 /* Public */,
       6,    1,   73,    2, 0x2a,    8 /* Public | MethodCloned */,
       8,    0,   76,    2, 0x08,   10 /* Private */,
       9,    1,   77,    2, 0x08,   11 /* Private */,
      12,    0,   80,    2, 0x08,   13 /* Private */,
      13,    0,   81,    2, 0x08,   14 /* Private */,

 // signals: parameters
    QMetaType::Void, QMetaType::Bool,    3,
    QMetaType::Void, QMetaType::QByteArray,    5,

 // slots: parameters
    QMetaType::Void, QMetaType::QByteArray, QMetaType::Long,    5,    7,
    QMetaType::Void, QMetaType::QByteArray,    5,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 10,   11,
    QMetaType::Void,
    QMetaType::Void,

       0        // eod
};

void TcpIpClient::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<TcpIpClient *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->clientConnection((*reinterpret_cast< bool(*)>(_a[1]))); break;
        case 1: _t->rxData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 2: _t->txData((*reinterpret_cast< QByteArray(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 3: _t->txData((*reinterpret_cast< QByteArray(*)>(_a[1]))); break;
        case 4: _t->socketRxData(); break;
        case 5: _t->socketError((*reinterpret_cast< QAbstractSocket::SocketError(*)>(_a[1]))); break;
        case 6: _t->socketConnected(); break;
        case 7: _t->socketDisconnected(); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 5:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QAbstractSocket::SocketError >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        {
            using _t = void (TcpIpClient::*)(bool );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpIpClient::clientConnection)) {
                *result = 0;
                return;
            }
        }
        {
            using _t = void (TcpIpClient::*)(QByteArray );
            if (*reinterpret_cast<_t *>(_a[1]) == static_cast<_t>(&TcpIpClient::rxData)) {
                *result = 1;
                return;
            }
        }
    }
}

const QMetaObject TcpIpClient::staticMetaObject = { {
    QMetaObject::SuperData::link<QTcpServer::staticMetaObject>(),
    qt_meta_stringdata_TcpIpClient.offsetsAndSize,
    qt_meta_data_TcpIpClient,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_TcpIpClient_t
, QtPrivate::TypeAndForceComplete<TcpIpClient, std::true_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<bool, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>, QtPrivate::TypeAndForceComplete<long, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QByteArray, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QAbstractSocket::SocketError, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>


>,
    nullptr
} };


const QMetaObject *TcpIpClient::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *TcpIpClient::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_TcpIpClient.stringdata0))
        return static_cast<void*>(this);
    return QTcpServer::qt_metacast(_clname);
}

int TcpIpClient::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpServer::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 8)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 8;
    }
    return _id;
}

// SIGNAL 0
void TcpIpClient::clientConnection(bool _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void TcpIpClient::rxData(QByteArray _t1)
{
    void *_a[] = { nullptr, const_cast<void*>(reinterpret_cast<const void*>(std::addressof(_t1))) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
