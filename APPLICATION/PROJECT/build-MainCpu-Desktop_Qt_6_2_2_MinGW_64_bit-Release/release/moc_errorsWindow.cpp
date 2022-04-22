/****************************************************************************
** Meta object code from reading C++ file 'errorsWindow.h'
**
** Created by: The Qt Meta Object Compiler version 68 (Qt 6.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include <memory>
#include "../../../SOURCE/WINDOWS/errorsWindow.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'errorsWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 68
#error "This file was generated using the moc from 6.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
struct qt_meta_stringdata_errorsWindow_t {
    const uint offsetsAndSize[14];
    char stringdata0[77];
};
#define QT_MOC_LITERAL(ofs, len) \
    uint(offsetof(qt_meta_stringdata_errorsWindow_t, stringdata0) + ofs), len 
static const qt_meta_stringdata_errorsWindow_t qt_meta_stringdata_errorsWindow = {
    {
QT_MOC_LITERAL(0, 12), // "errorsWindow"
QT_MOC_LITERAL(13, 18), // "onScrollPushButton"
QT_MOC_LITERAL(32, 0), // ""
QT_MOC_LITERAL(33, 16), // "onExitPushButton"
QT_MOC_LITERAL(50, 10), // "timerEvent"
QT_MOC_LITERAL(61, 12), // "QTimerEvent*"
QT_MOC_LITERAL(74, 2) // "ev"

    },
    "errorsWindow\0onScrollPushButton\0\0"
    "onExitPushButton\0timerEvent\0QTimerEvent*\0"
    "ev"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_errorsWindow[] = {

 // content:
      10,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: name, argc, parameters, tag, flags, initial metatype offsets
       1,    0,   32,    2, 0x08,    1 /* Private */,
       3,    0,   33,    2, 0x08,    2 /* Private */,
       4,    1,   34,    2, 0x08,    3 /* Private */,

 // slots: parameters
    QMetaType::Void,
    QMetaType::Void,
    QMetaType::Void, 0x80000000 | 5,    6,

       0        // eod
};

void errorsWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        auto *_t = static_cast<errorsWindow *>(_o);
        (void)_t;
        switch (_id) {
        case 0: _t->onScrollPushButton(); break;
        case 1: _t->onExitPushButton(); break;
        case 2: _t->timerEvent((*reinterpret_cast< QTimerEvent*(*)>(_a[1]))); break;
        default: ;
        }
    }
}

const QMetaObject errorsWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<windowsClass::staticMetaObject>(),
    qt_meta_stringdata_errorsWindow.offsetsAndSize,
    qt_meta_data_errorsWindow,
    qt_static_metacall,
    nullptr,
qt_incomplete_metaTypeArray<qt_meta_stringdata_errorsWindow_t
, QtPrivate::TypeAndForceComplete<errorsWindow, std::true_type>
, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<void, std::false_type>, QtPrivate::TypeAndForceComplete<QTimerEvent *, std::false_type>


>,
    nullptr
} };


const QMetaObject *errorsWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *errorsWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_meta_stringdata_errorsWindow.stringdata0))
        return static_cast<void*>(this);
    return windowsClass::qt_metacast(_clname);
}

int errorsWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = windowsClass::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 3)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 3;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 3)
            *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType();
        _id -= 3;
    }
    return _id;
}
QT_WARNING_POP
QT_END_MOC_NAMESPACE
